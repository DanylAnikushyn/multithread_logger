#include <thread>

#include "logger.hpp"

void myFunc3() 
{
    INCREASE_INDENT;
    LOG(indent);
    DECREASE_INDENT;
}
void myFunc2()
{
    INCREASE_INDENT;
    LOG(indent);
    DECREASE_INDENT;
}
void myFunc1()
{
    INCREASE_INDENT;
    LOG(indent);
    myFunc2();
    LOG(indent);
    DECREASE_INDENT;
}

void myFunc4()
{
    INCREASE_INDENT;
    LOG(indent);
    DECREASE_INDENT;
}

void myFunc5()
{
    INCREASE_INDENT;
    LOG(indent);
    myFunc4();
    LOG(indent);
    DECREASE_INDENT;
}

void myFunc6()
{
    INCREASE_INDENT;
    LOG(indent);
    myFunc5();
    LOG(indent);
    DECREASE_INDENT;
}

int main()
{
    Logger::get().setOptions("log.txt", 1);
    LOG("Hello from main!");
    std::thread t1(myFunc1);
    std::thread t2(myFunc6);
    std::thread t3(myFunc3);
    std::thread t4(myFunc6);
    std::thread t5(myFunc6);
    std::thread t6(myFunc6);
    std::thread t7(myFunc6);
    std::thread t8(myFunc6);
    std::thread t9(myFunc6);
    std::thread t10(myFunc6);
    t1.join();
    t2.join(); 
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
    return 0;
}
