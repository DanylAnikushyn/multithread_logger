#include <thread>

#include "logger.hpp"

void myFunc3() 
{
    INCREASE_INDENT;
    LOG(indent);
}
void myFunc2()
{
    INCREASE_INDENT;
    LOG(indent);
}
void myFunc1()
{
    INCREASE_INDENT;
    LOG(indent);
    myFunc2();
}

void myFunc4()
{
    INCREASE_INDENT;
    LOG(indent);
}

void myFunc5()
{
    INCREASE_INDENT;
    LOG(indent);
    myFunc4();
}

void myFunc6()
{
    INCREASE_INDENT;
    LOG(indent);
    myFunc5();
}

int main()
{
    Logger::init("log.txt");
    LOG("Hello from main!");
    std::thread t1(myFunc1);
    std::thread t2(myFunc6);
    std::thread t3(myFunc3);
    t1.join();
    t2.join(); 
    t3.join();
    Logger::destroy();
    return 0;
}
