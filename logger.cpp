#include <time.h>
#include <iostream>

#include "logger.hpp"

thread_local int indent = 0;

const std::string currentDateTime() {
    time_t     now = time(0);
    std::tm    tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void Logger::setOptions(std::string filename, std::size_t queue_size) 
{
    std::unique_lock lock(m_mutex);
    m_filename = filename;
    m_queue_size = queue_size;
}

Logger& Logger::get()
{
    static Logger m_instance(m_filename, m_queue_size);
    return m_instance;
}

Logger::~Logger() 
{
    std::unique_lock lock(m_mutex);
    alive = false;
    lock.unlock();
    m_cv.notify_one();
    m_thread.join();
}

void Logger::log(const std::string &message) 
{
    std::unique_lock lock(m_mutex);
    while (m_messages.size() >= m_queue_size)                                       
    {                                   
        m_cv.wait(lock);                                    
    }                                             
    m_messages.push_back(message);                                  
    std::cout << message << std::endl;
    lock.unlock();
    m_cv.notify_all();
}

Logger::Logger(const std::string& filename, std::size_t queue_size) 
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
        lock.unlock();
        m_cv.notify_all();  
    }
}

std::string Logger::m_filename = "log.txt";
std::size_t Logger::m_queue_size = 10;
