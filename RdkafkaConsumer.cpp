/*
 * RdkafkaConsumer.cpp
 *
 *  Created on: 2016年9月20日
 *      Author: wong
 */
#include "RdkafkaConsumer.h"

#include <signal.h>
#include "base/Logging.h"


RdkafkaConsumer::RdkafkaConsumer(const std::string& version,
                                 const std::string& brokers)
    : m_cb(NULL),
      m_userdata(NULL),
      m_rk(NULL),
      m_rkt(NULL),
      m_running(false),
      m_monitorThread(0) {
  rd_kafka_conf_t* conf = rd_kafka_conf_new();
  //rd_kafka_conf_set_log_cb(conf, logger);
  /* Quick termination */
  char tmp[16] = { 0 };
  snprintf(tmp, sizeof(tmp), "%i", SIGIO);
  rd_kafka_conf_set(conf, "internal.termination.signal", tmp, NULL, 0);
  rd_kafka_conf_set(conf, "api.version.request", "false", NULL, 0);
  rd_kafka_conf_set(conf, "broker.version.fallback", version.c_str(), NULL, 0);

  char errstr[512] = { 0 };
  m_rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));
  conf = NULL;

  rd_kafka_brokers_add(m_rk, brokers.c_str());
}

RdkafkaConsumer::~RdkafkaConsumer() {
  m_running = false;
  pthread_join(m_monitorThread, NULL);
  if (m_rkt != NULL) {
    rd_kafka_topic_destroy(m_rkt);
  }
  if (m_rk != NULL) {
    rd_kafka_destroy(m_rk);
  }
}

void* RdkafkaConsumer::Monitor(void* data) {
  RdkafkaConsumer* that = reinterpret_cast<RdkafkaConsumer*>(data);
  if (that == NULL || that->m_rk == NULL) {
    return (void*) -1;
  }

  const struct rd_kafka_metadata *metadata;
  rd_kafka_resp_err_t err = rd_kafka_metadata(that->m_rk, 0, that->m_rkt,
                                              &metadata, 5000);
  if (err != RD_KAFKA_RESP_ERR_NO_ERROR || metadata->topic_cnt != 1) {
    LOG_ERROR<< "Failed to acquire metadata: " << rd_kafka_err2str(err);
    rd_kafka_metadata_destroy(metadata);
    return (void*)-1;
  }
  int cnt = metadata->topics[0].partition_cnt;
  rd_kafka_metadata_destroy(metadata);

  for (int i = 0; i < cnt; ++i) {
    int ret = rd_kafka_consume_start(that->m_rkt, i, RD_KAFKA_OFFSET_END);
    if (ret == -1) {
      LOG_ERROR<< "rd_kafka_consume_start err=" << rd_kafka_last_error() << " " << i;
      return (void*)-1;
    }
  }

  int total = 0;
  while (that->m_running) {
    rd_kafka_poll(that->m_rk, 10);
    for (int i = 0; i < cnt; ++i) {
      int ret = rd_kafka_consume_callback(that->m_rkt, i, 100, ConsumeCB, that);
      if (ret != -1) {
        total += ret;
        LOG_DEBUG << "rd_kafka_consume_callback ret=" << ret << " total="
                  << total << " partition=" << i;
      } else {
        LOG_WARN<< "rd_kafka_consume_callback err=" << rd_kafka_last_error() << " " << i;
      }
    }
  }

  for (int i = 0; i < cnt; ++i) {
    rd_kafka_consume_stop(that->m_rkt, i);
  }
  while (rd_kafka_outq_len(that->m_rk) > 0) {
    rd_kafka_poll(that->m_rk, 10);
  }

  return 0;
}

int RdkafkaConsumer::Consume(const std::string& topic, PFN_CONSUME cb,
                             void* userdata) {
  if (m_monitorThread != 0) {
    LOG_ERROR<< "Consume Reentry! m_monitorThread = " << m_monitorThread;
  }
  m_cb = cb;
  m_userdata = userdata;

  rd_kafka_topic_conf_t* topicConf = rd_kafka_topic_conf_new();
  m_rkt = rd_kafka_topic_new(m_rk, topic.c_str(), topicConf);
  topicConf = NULL; /* Now owned by topic */

  m_running = true;
  pthread_create(&m_monitorThread, NULL, Monitor, this);
  return 0;
}

void RdkafkaConsumer::ConsumeCB(rd_kafka_message_t* rkmsg, void* opaque) {
  RdkafkaConsumer* that = reinterpret_cast<RdkafkaConsumer*>(opaque);
  if (that == NULL) {
    return;
  }
  if (rkmsg->err != 0 || rkmsg->len == 0) {
    LOG_WARN<< "rkmsg->err=" << rkmsg->err << " rkmsg->len=" << rkmsg->len;
    return;
  }
  if (that->m_cb == NULL) {
    LOG_WARN << "that->m_cb == NULL";
    return;
  }
  std::string key = (rkmsg->key_len > 0) ? (char*) rkmsg->key : "";
  that->m_cb(key, rkmsg->payload, rkmsg->len, that->m_userdata);
}

