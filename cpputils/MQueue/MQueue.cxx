#include "MQueue.hpp"

class MQueue {
public:
    MQueue(const char* name,
           mode_t mode = 0644,
           long maxMsg = 10,
           long msgSize = 256)
        : _name(name), _msgSize(msgSize)
    {
        struct mq_attr attr {};
        attr.mq_flags = 0;
        attr.mq_maxmsg = maxMsg;
        attr.mq_msgsize = msgSize;
        attr.mq_curmsgs = 0;

        _mq = mq_open(_name, O_RDWR | O_NONBLOCK);
        if (_mq == static_cast<mqd_t>(-1) && errno == ENOENT) {
            _mq = mq_open(_name, O_CREAT | O_RDWR | O_NONBLOCK, mode, &attr);
        }
    }

    ~MQueue() {
        if (_mq != static_cast<mqd_t>(-1)) {
            mq_close(_mq);
        }
        mq_unlink(_name);
    }

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

    bool getAttr(struct mq_attr& attr) const {
        if (_mq == static_cast<mqd_t>(-1)) {
            return false;
        }

        return mq_getattr(_mq, &attr) == 0;
    }

private:
    std::string _name;
    mqd_t _mq = static_cast<mqd_t>(-1);
    size_t _msgSize;
};