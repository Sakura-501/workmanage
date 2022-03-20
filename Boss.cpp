#include "Boss.h"
//老板的构造函数
Boss::Boss(string id,string name,string sex,string age,string phonenumber)
{
    this->Id=id;
    this->Name=name;
    this->Sex=sex;
    this->Age=age;
    this->PhoneNumber=phonenumber;
    this->DeptName="老板";
    this->Salary="10000";
}
string Boss::GetTask()//返回老板的工作职责
{
    return string("喝茶休息！");
}
