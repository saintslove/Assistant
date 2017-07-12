/*
 * RdkafkaProducer.h
 *
 *  Created on: 2016年9月20日
 *      Author: wong
 */

#ifndef RDKAFKAPRODUCER_H_
#define RDKAFKAPRODUCER_H_

#include <string>
#include <map>

#include "rdkafka.h"

class RdkafkaProducer
{
public:
    RdkafkaProducer(const std::string& version, const std::string& brokers);
    virtual ~RdkafkaProducer();

public:
    int Produce(const std::string& topic, const std::string& key,
        char* buf, size_t len);

private:
    static void* Monitor(void* data);
    static void DeliveryReport(rd_kafka_t* rk,
        const rd_kafka_message_t* rkmessage, void* opaque);
    rd_kafka_topic_t* GetOrCreateTopic(const std::string& topic);

private:
    typedef std::map<std::string, rd_kafka_topic_t*> TopicMap;
    rd_kafka_t* m_rk;
    TopicMap m_topicMap;
};

#endif /* RDKAFKAPRODUCER_H_ */
