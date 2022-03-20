#include "Manager.h"
//经理的构造函数
Manager::Manager(string id,string name,string sex,string age,string phonenumber)
{
    this->Id=id;
    this->Name=name;
    this->Sex=sex;
    this->Age=age;
    this->PhoneNumber=phonenumber;
    this->DeptName="经理";
    this->Salary="8000";
}
string Manager::GetTask()//返回经理的工作职责
{
    return string("帮老板打工！");
}
