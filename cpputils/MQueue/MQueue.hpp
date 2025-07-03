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


class MQueue {
    public:
        MQueue(const char* name, const MQueue_Settings& settings);


        ~MQueue();

       template<typename T>
        MqResult send(const T& data, unsigned int priority = 0) const {
            if (_mq == static_cast<mqd_t>(-1)) {
                return MqResult::NotOpen;
            }

            if (sizeof(T) != _msgSize) {
                return MqResult::InvalidMessageSize;
            }

            if (mq_send(_mq, reinterpret_cast<const char*>(&data), sizeof(T), priority) == -1) {
                if (errno == EAGAIN) {
                    return MqResult::QueueFull;
                }
                return MqResult::SysError;
            }

            return MqResult::Success;
        }

        template<typename T>
        MqResult receive(T& outData, unsigned int* priority = nullptr) const {
            if (_mq == static_cast<mqd_t>(-1)) {
                return MqResult::NotOpen;
            }

            ssize_t bytes = mq_receive(_mq, reinterpret_cast<char*>(&outData), sizeof(T), priority);
            if (bytes < 0) {
                if (errno == EAGAIN) {
                    return MqResult::QueueEmpty;
                }
                return MqResult::SysError;
            }

            return MqResult::Success;
        }

        template<typename T>
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


        bool getAttr(struct mq_attr& attr) const;

    private:
        const char* _name;
        mqd_t _mq = (mqd_t)-1;
        size_t _msgSize;
};
