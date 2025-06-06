#include "MQueue.hpp"

class MQueue {
public:
    MQueue(const std::string& name,
       mode_t mode = 0644,
       long maxMsg = 10,
       long msgSize = 256)
    : _name(name), _msgSize(msgSize)
    {
        struct mq_attr attr {};
        attr.mq_flags = 0; // ignored by mq_open
        attr.mq_maxmsg = maxMsg;
        attr.mq_msgsize = msgSize;
        attr.mq_curmsgs = 0;

        _mq = mq_open(_name.c_str(), O_RDWR | O_NONBLOCK);
        if (_mq == static_cast<mqd_t>(-1) && errno == ENOENT) {
            // Doesn't exist — create it
            _mq = mq_open(_name.c_str(), O_CREAT | O_RDWR | O_NONBLOCK, mode, &attr);
        }

        if (_mq == static_cast<mqd_t>(-1)) {
            perror("mq_open");
        }
    }

    ~MQueue() {
        if (_mq != static_cast<mqd_t>(-1)) {
            if (mq_close(_mq) == -1)
                perror("mq_close");
        }
        if (mq_unlink(_name.c_str()) == -1)
            perror("mq_unlink");
    }

    template<typename T>
    MqResult MQueue::send(const T& data, unsigned int priority) const {
        if (_mq == static_cast<mqd_t>(-1)) {
            perror("send: queue not open");
            return MqResult::NotOpen;
        }

        if (sizeof(T) > _msgSize) {
            errno = EMSGSIZE;
            perror("send: message too large");
            return MqResult::MessageTooLarge;
        }

        if (mq_send(_mq, reinterpret_cast<const char*>(&data), sizeof(T), priority) == -1) {
            if (errno == EAGAIN) {
                perror("mq_send: queue full (EAGAIN)");
                return MqResult::QueueFull;
            }
            perror("mq_send");
            return MqResult::SysError;
        }

        return MqResult::Success;
    }


    template<typename T>
    MqResult MQueue::receive(T& outData, unsigned int* priority = nullptr) const {
        if (_mq == static_cast<mqd_t>(-1)) {
            perror("receive: queue not open");
            return MqResult::NotOpen;
        }

        ssize_t bytes = mq_receive(_mq, reinterpret_cast<char*>(&outData), sizeof(T), priority);
        if (bytes < 0) {
            if (errno == EAGAIN) {
                return MqResult::QueueEmpty;
            }
            perror("mq_receive");
            return MqResult::SysError;
        }

        return MqResult::Success;
    }


    template<typename T>
    MqResult MQueue::receiveMostRecent(T& outData) const {
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
            perror("getAttr: queue not open");
            return false;
        }

        if (mq_getattr(_mq, &attr) == -1) {
            perror("mq_getattr");
            return false;
        }

        return true;
    }

private:
    std::string _name;
    mqd_t _mq = static_cast<mqd_t>(-1);
    size_t _msgSize;
};