/*
 * RdkafkaConsumer.h
 *
 *  Created on: 2016年9月20日
 *      Author: wong
 */

#ifndef RDKAFKACONSUMER_H_
#define RDKAFKACONSUMER_H_

#include <string>
#include <map>

#include "rdkafka.h"

typedef void (*PFN_CONSUME)(const std::string& key, void* playload, size_t len, void* userdata);

class RdkafkaConsumer
{
public:
    RdkafkaConsumer(const std::string& version, const std::string& brokers);
    virtual ~RdkafkaConsumer();

public:
    int Consume(const std::string& topic, PFN_CONSUME cb, void* userdata);

private:
    static void* Monitor(void* data);
    static void ConsumeCB(rd_kafka_message_t* rkmsg, void* opaque);

private:
    PFN_CONSUME m_cb;
    void* m_userdata;
    rd_kafka_t* m_rk;
    rd_kafka_topic_t* m_rkt;
    bool m_running;
    pthread_t m_monitorThread;
};

#endif /* RDKAFKACONSUMER_H_ */
