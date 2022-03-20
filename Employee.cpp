#include "Employee.h"
//员工的构造函数
Employee::Employee(string id,string name,string sex,string age,string phonenumber)
{
    this->Id=id;
    this->Name=name;
    this->Sex=sex;
    this->Age=age;
    this->PhoneNumber=phonenumber;
    this->DeptName="员工";
    this->Salary="5000";
}
string Employee::GetTask()//返回员工的工作职责
{
    return string("努力打工！");
}
