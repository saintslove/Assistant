/*
 * RdkafkaProducer.cpp
 *
 *  Created on: 2016年9月20日
 *      Author: wong
 */

#include "RdkafkaProducer.h"

#include <signal.h>

RdkafkaProducer::RdkafkaProducer(const std::string& version,
    const std::string& brokers)
: m_rk(NULL)
{
    rd_kafka_conf_t* conf = rd_kafka_conf_new();
    //rd_kafka_conf_set_log_cb(conf, logger);
    /* Quick termination */
    char tmp[16] = {0};
    snprintf(tmp, sizeof(tmp), "%i", SIGIO);
    rd_kafka_conf_set(conf, "internal.termination.signal", tmp, NULL, 0);
    rd_kafka_conf_set(conf, "broker.version.fallback", version.c_str(), NULL, 0);
    rd_kafka_conf_set_dr_msg_cb(conf, DeliveryReport);

    char errstr[512] = {0};
    m_rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    conf = NULL;

    //rd_kafka_set_log_level(rk, LOG_DEBUG);

    rd_kafka_brokers_add(m_rk, brokers.c_str());

    pthread_t id;
    pthread_create(&id, NULL, Monitor, m_rk);
}

RdkafkaProducer::~RdkafkaProducer()
{
    for (TopicMap::iterator it = m_topicMap.begin();
        it != m_topicMap.end(); it++)
    {
        if (it->second != NULL)
        {
            rd_kafka_topic_destroy(it->second);
        }
    }
    if (m_rk != NULL)
    {
        rd_kafka_destroy(m_rk);
    }
}

void* RdkafkaProducer::Monitor(void* data)
{
    rd_kafka_t* rk = static_cast<rd_kafka_t*>(data);
    if (rk == NULL)
    {
        return (void*)-1;
    }
    while (true)
    {
        rd_kafka_poll(rk, 10);
    }
    return 0;
}

void RdkafkaProducer::DeliveryReport(rd_kafka_t* rk,
    const rd_kafka_message_t* rkmessage, void* opaque)
{
    static int count = 0;
    count++;
    if (count % 100 == 0)
    {
        printf("delivery: %s: offset %ld\n",
            rd_kafka_err2str(rkmessage->err), rkmessage->offset);
    }
    if (rkmessage->err)
    {
        fprintf(stderr, "%% Message delivery failed: %s\n",
            rd_kafka_message_errstr(rkmessage));
    }
/*    else
    {
        fprintf(stderr,
            "%% Message delivered (%zd bytes, offset %ld, "
            "partition %d): %.*s\n",
            rkmessage->len, rkmessage->offset,
            rkmessage->partition,
            (int)rkmessage->len, (const char*)rkmessage->payload);
    }*/
}

rd_kafka_topic_t* RdkafkaProducer::GetOrCreateTopic(const std::string& topic)
{
    TopicMap::iterator it = m_topicMap.find(topic);
    if (it != m_topicMap.end())
    {
        return it->second;
    }
    else
    {
        char errstr[512] = {0};
        rd_kafka_topic_conf_t* topicConf = rd_kafka_topic_conf_new();
        rd_kafka_topic_conf_set(topicConf,
            "produce.offset.report",
            "true", errstr, sizeof(errstr));
        rd_kafka_topic_t* rkt = rd_kafka_topic_new(m_rk, topic.c_str(),
            topicConf);
        topicConf = NULL; /* Now owned by topic */
        m_topicMap[topic] = rkt;
        return rkt;
    }
}

int RdkafkaProducer::Produce(const std::string& topic, const std::string& key,
    char* buf, size_t len)
{
    rd_kafka_topic_t* rkt = GetOrCreateTopic(topic);
    return rd_kafka_produce(rkt, RD_KAFKA_PARTITION_UA, RD_KAFKA_MSG_F_COPY,
            buf, len, key.c_str(), key.length(), NULL);
}

