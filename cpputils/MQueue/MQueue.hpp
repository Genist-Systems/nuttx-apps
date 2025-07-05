#pragma once

#include <mqueue.h>
#include <string>
#include <vector>
#include <cstring>
#include <cerrno>
#include <cstdio>

struct MQueue_Settings
{
   int flags = O_CREAT | O_RDWR | O_NONBLOCK;
   mode_t mode = 0644;
   long maxMsg = 8;
   long msgSize = 32;


};


enum class MqResult {
    Success,
    NotOpen,
    InvalidMessageSize,
    QueueFull,
    QueueEmpty,
    SysError
};


template<typename T>
class MQueue {
public:
    MQueue(const char* name, const MQueue_Settings& base_settings)
        : _name(name), _msgSize(sizeof(T))
    {
        struct mq_attr attr {};
        attr.mq_flags   = 0;
        attr.mq_maxmsg  = base_settings.maxMsg;
        attr.mq_msgsize = sizeof(T);
        attr.mq_curmsgs = 0;

        _mq = mq_open(_name, base_settings.flags, base_settings.mode, &attr);

        if (_mq == static_cast<mqd_t>(-1)) {
            printf("MQueue ERROR: mq_open failed for %s (errno=%d)\n", _name, errno);
        }
    }

    ~MQueue() {
        if (_mq != static_cast<mqd_t>(-1)) {
            mq_close(_mq);
        }
        mq_unlink(_name);
    }

    MqResult send(const T& data, unsigned int priority = 0) const {
        if (_mq == static_cast<mqd_t>(-1)) return MqResult::NotOpen;
        if (sizeof(T) != _msgSize) return MqResult::InvalidMessageSize;

        if (mq_send(_mq, reinterpret_cast<const char*>(&data), sizeof(T), priority) == -1) {
            if (errno == EAGAIN) return MqResult::QueueFull;
            return MqResult::SysError;
        }

        return MqResult::Success;
    }

    MqResult receive(T& outData, unsigned int* priority = nullptr) const {
        if (_mq == static_cast<mqd_t>(-1)) return MqResult::NotOpen;

        ssize_t bytes = mq_receive(_mq, reinterpret_cast<char*>(&outData), sizeof(T), priority);
        if (bytes < 0) {
            if (errno == EAGAIN) return MqResult::QueueEmpty;
            return MqResult::SysError;
        }

        return MqResult::Success;
    }

    MqResult receiveMostRecent(T& outData) const {
        T temp;
        bool gotAny = false;

        while (true) {
            MqResult result = receive(temp);
            if (result == MqResult::QueueEmpty) break;
            if (result != MqResult::Success) return result;

            outData = temp;
            gotAny = true;
        }

        return gotAny ? MqResult::Success : MqResult::QueueEmpty;
    }

    bool getAttr(struct mq_attr& attr) const {
        if (_mq == static_cast<mqd_t>(-1)) return false;
        return mq_getattr(_mq, &attr) == 0;
    }

private:
    const char* _name;
    mqd_t _mq = (mqd_t)-1;
    size_t _msgSize;
};

