#ifndef MANAGER_H
#define MANAGER_H
#include<Worker.h>
//经理类
class Manager:public Worker//公有继承职工抽象类
{
public:
    Manager(string ,string ,string ,string ,string );
    string GetTask();//返回经理的职责
};
#endif // MANAGER_H
