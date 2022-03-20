#ifndef WORKER_H
#define WORKER_H
#include<iostream>
#include<cstring>
#include<QString>
using namespace std;
//职工抽象类
class Worker{
public:
    virtual string GetTask()=0;//声明了一个纯虚函数，返回职工的职责
    string Id,Age,Name,Sex,PhoneNumber,DeptName,Salary;//工号，年龄,姓名，性别，手机号码，部门名称,工资，都是string类型
};

#endif // WORKER_H
