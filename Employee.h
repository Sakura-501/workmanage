#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include<Worker.h>
//员工类
class Employee:public Worker//公有继承职工抽象类
{
public:
    Employee(string ,string ,string ,string ,string );
    string GetTask();//返回员工的职责
};

#endif // EMPLOYEE_H
