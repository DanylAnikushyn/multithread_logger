#include <time.h>
#include <iostream>

#include "logger.hpp"

thread_local int indent = 0;

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void Logger::init(const char* filename) 
{
    m_instance.reset(new Logger(filename, 10));
}

Logger& Logger::get()
{
    if (!m_instance)
    {
        m_instance.reset(new Logger("log.txt", 10));
    }
    return *m_instance;
}

void Logger::destroy() 
{
    alive = false;
    m_cv.notify_one();
    m_thread.join();
}

void Logger::log(const std::string &message) 
{
    std::unique_lock lock(m_mutex);
    m_messages.push_back(message);
    std::cout << message << std::endl;
    lock.unlock();
    m_cv.notify_one();
}

Logger::Logger(const std::string& filename, std::size_t queue_size) : m_messages(queue_size), m_queue_size(queue_size) 
{
    m_out.open(filename);
    m_thread = std::move(std::thread(&Logger::write, this));
}

void Logger::write() 
{
    while (true) 
    {
        std::unique_lock lock(m_mutex);
        std::string message;
        while (m_messages.empty() && alive)
        {
            m_cv.wait(lock);
        }
        if (m_messages.empty() && !alive) return;
        message = m_messages.front();
        m_messages.pop_front();
        m_out << message << std::endl;
    }
}

std::unique_ptr<Logger> Logger::m_instance = nullptr;
std::thread Logger::m_thread;
std::condition_variable Logger::m_cv;
bool Logger::alive = true;
