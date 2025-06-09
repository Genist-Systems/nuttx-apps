#pragma once

#include <mqueue.h>
#include <string>
#include <vector>
#include <cstring>
#include <cerrno>
#include <cstdio>

enum class MqResult {
    Success,
    NotOpen,
    InvalidMessageSize,
    QueueFull,
    QueueEmpty,
    SysError
};


class MQueue {
public:
    MQueue(const std::string& name,
           int flags = O_CREAT | O_RDWR | O_NONBLOCK,
           mode_t mode = 0644,
           long maxMsg = 10,
           long msgSize = 256);

    ~MQueue();

    template<typename T>
    MqResult send(const T& data, unsigned int priority = 0) const;

    template<typename T>
    MqResult receive(T& outData, unsigned int* priority = nullptr) const;

    template<typename T>
    MqResult receiveMostRecent(T& outData) const;


    bool getAttr(struct mq_attr& attr) const;

private:
    std::string _name;
    mqd_t _mq = (mqd_t)-1;
    size_t _msgSize;
};
