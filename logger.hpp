#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <fstream>
#include <thread>
#include <mutex>
#include <memory>
#include <deque>
#include <sstream>
#include <condition_variable>

extern thread_local int indent;

const std::string currentDateTime();

#define LOG(param) Logger::Message message(__FILE__, __FUNCTION__, __LINE__, param)
#define INCREASE_INDENT (++indent)

class Logger
{
public:
    static void init(const char* filename);
    static Logger& get();
    static void destroy();
    void log(const std::string &message);

    class Message
    {
    public:
        template<typename T>
        Message(const char *file, const char *function, int line, const T& param)
        {
            std::ostringstream stream;
            stream  << currentDateTime() <<  " tid [" << std::hex << std::this_thread::get_id() << "] [" << file << ']' 
                << std::string(indent, '\t') << '[' << function << ':' << std::dec << line << "] " << param;
            Logger::get().log(stream.str());
        }
    };
private:
    Logger(const std::string& filename, std::size_t queue_size);
    void write();
    static std::unique_ptr<Logger> m_instance;
    static std::thread m_thread;
    static std::condition_variable m_cv;
    static bool alive;
    std::deque<std::string> m_messages;
    std::size_t m_queue_size;
    std::mutex m_mutex;
    std::ofstream m_out;
};

#endif//_LOGGER_HPP_