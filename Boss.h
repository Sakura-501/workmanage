#ifndef BOSS_H
#define BOSS_H
#include<Worker.h>
//老板类
class Boss:public Worker//公有继承职工抽象类
{
public:
    Boss(string ,string ,string ,string ,string );
    string GetTask();//返回老板的职责
};

#endif // BOSS_H
