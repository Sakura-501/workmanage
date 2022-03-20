#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QStringList>
#include<QDebug>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QStandardItem>
#include<iomanip>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("职工管理系统");//设置窗口名称
    this->init_worker();//一创建窗口就读入数据初始化职工数组
    ui->stackedWidget->setCurrentIndex(0);//设置登录成功的默认欢迎页面
    //当点击不同按钮是跳转到不同的功能页面：
    connect(ui->addbutton,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(1);//增加职工页面
    });
    connect(ui->selectbutton,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(2);//查询职工页面
    });
    connect(ui->modifybutton,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(3);//修改职工页面
    });
    connect(ui->deletebutton,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(4);//删除职工页面
    });
    connect(ui->sortbutton,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(5);//排序职工页面
    });


    //第一个功能：点击退出按钮，退出系统
    this->quit();

    //第二个功能：增加职工：
    this->Add_Worker();

    //第三个功能：查询职工
    this->query();


    //第四个功能：根据职工名称修改值
    this->modify();


    //第五个功能：根据职工名称或工号删除
    this->delete_worker();

    //第六个功能：根据工号或者年龄进行排序打表：
    this->sortworker();

}

//获取当前文本职工人数
int MainWindow::Get_WorkerNumber()
{
    ifstream in(FILENAME,ios::in);
    int sum=0;
    string idd,agee,namee,sexx,deptnamee,salaryy,phonenumberr;
    in>>idd>>namee>>sexx>>agee>>deptnamee>>salaryy;//过滤文本中第一行的表头
    while(in>>idd&&in>>namee&&in>>sexx&&in>>agee&&
          in>>phonenumberr&&in>>deptnamee&&in>>salaryy)
    {
        sum++;
    }
    in.close();
    return sum;
}

//初始化职工数组
void MainWindow::init_worker()
{
    this->WorkerArray=NULL;
    this->WorkerNumber=this->Get_WorkerNumber();
    this->WorkerArray=new Worker*[this->WorkerNumber];
    ifstream in(FILENAME,ios::in);
    string idd,agee,namee,sexx,phonenumberr,deptnamee,salaryy;
    //先过滤掉第一行的表头
    in>>idd>>namee>>sexx>>agee>>phonenumberr>>deptnamee>>salaryy;
    int index=0;
    for(int i=0;i<this->WorkerNumber;i++)
    {
        in>>idd>>namee>>sexx>>agee>>phonenumberr>>deptnamee>>salaryy;
        Worker *worker=NULL;
        //根据不同部门来创建对象
        if(deptnamee=="员工")
        {
            worker=new Employee(idd,namee,sexx,agee,phonenumberr);
        }
        else if(deptnamee=="经理")
        {
            worker=new Manager(idd,namee,sexx,agee,phonenumberr);
        }
        else
        {
            worker=new Boss(idd,namee,sexx,agee,phonenumberr);
        }
        this->WorkerArray[index]=worker;
        index++;
    }
}
//退出系统操作
void MainWindow::quit()
{
    connect(ui->returnbutton,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"警告","是否退出系统！",
                                                     QMessageBox::Yes|QMessageBox::No))
        {
            this->close();
        }
    });
}
//添加职工操作页面
void  MainWindow::Add_Worker()
{
    this->init_worker();//初始化职工数组

    QStringList strlist;
    strlist<<"员工"<<"经理"<<"老板";
    ui->deptselect->addItems(strlist);//设置下拉框三个选项（其实可以直接在设计界面直接加入）
    ui->sexman->setChecked(true);//设置性别按钮默认选中男
    //清空按钮操作：
    connect(ui->clear,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"警告","是否清空！",QMessageBox::Yes|QMessageBox::No))
        {
            ui->idinput->clear();
            ui->nameinput->clear();
            ui->sexman->setChecked(true);
            ui->ageselect->setCurrentIndex(0);
            ui->deptselect->setCurrentIndex(0);
        }
    });
    //按下确认按钮进行增加职工操作：
    connect(ui->makesuresave,&QPushButton::clicked,[=](){
        string idd,agee,namee,sexx,phonenumberr,deptnamee;
        //获取各项填写的内容值：
        idd=ui->idinput->text().toStdString();
        namee=ui->nameinput->text().toStdString();
        phonenumberr=ui->phonuminput->text().toStdString();
        if(ui->sexman->isChecked()) sexx="男";
        else sexx="女";
        agee=ui->ageselect->currentText().toStdString();
        deptnamee=ui->deptselect->currentText().toStdString();
        //接下来重置workerarray,为保存到文本文件中做准备：
        //首先复制以前的员工：
        int newsize=this->WorkerNumber+1;
        Worker **newspace =new Worker*[newsize];
        if(this->WorkerArray!=NULL)
        {
            for(int i=0;i<this->WorkerNumber;i++)
            {
                newspace[i]=this->WorkerArray[i];
            }
        }
        //加入新职工：
        Worker *worker=NULL;
        //根据不同部门来创建对象
        if(deptnamee=="员工")
        {
            worker=new Employee(idd,namee,sexx,agee,phonenumberr);
        }
        else if(deptnamee=="经理")
        {
            worker=new Manager(idd,namee,sexx,agee,phonenumberr);
        }
        else
        {
            worker=new Boss(idd,namee,sexx,agee,phonenumberr);
        }
        newspace[this->WorkerNumber]=worker;//保存到新空间中
        delete []this->WorkerArray;
        this->WorkerArray=newspace;
        this->WorkerNumber=newsize;
        this->save();//职工数组更改，因此需要保存到文本中
        if(QMessageBox::Yes == QMessageBox::question(this,"提示","添加职工成功，是否需要清空？",QMessageBox::Yes|QMessageBox::No))
        {
            ui->idinput->clear();
            ui->nameinput->clear();
            ui->phonuminput->clear();
            ui->sexman->setChecked(true);
            ui->ageselect->setCurrentIndex(0);
            ui->deptselect->setCurrentIndex(0);
        }
    });

}
void MainWindow::save()
{
    ofstream out(FILENAME,ios::out);
    //先打表头
    out<<left<<setw(25)<<"工号"<<setw(25)<<"姓名"<<setw(25)<<"性别"<<setw(25)<<"年龄"
    <<setw(35)<<"手机号码"<<setw(25)<<"部门"<<setw(25)<<"工资"<<endl;
    for(int i=0;i<this->WorkerNumber;i++)
    {
        out<<left<<setw(25)<<this->WorkerArray[i]->Id<<setw(25)
        <<this->WorkerArray[i]->Name<<setw(25)
        <<this->WorkerArray[i]->Sex<<setw(25)
        <<this->WorkerArray[i]->Age<<setw(25)
        <<this->WorkerArray[i]->PhoneNumber<<setw(25)
        <<this->WorkerArray[i]->DeptName<<setw(25)
        <<this->WorkerArray[i]->Salary<<endl;
    }
    out.close();
}

//查询职工操作：
void MainWindow::query()
{
    connect(ui->query,&QPushButton::clicked,[=](){
        this->init_worker();//获取最新文本初始化职工数组
        ui->tablewidget->setColumnCount(8);
        ui->tablewidget->setRowCount(this->WorkerNumber);
        ui->tablewidget->clearContents();//清空内容
        string kindselect=ui->kindselect->currentText().toStdString();//查询方式选择
        string find=ui->selectedit->text().toStdString();//获取查询内容
        bool flag=false;
        int index=0;
        if(kindselect=="默认查询")
        {
            flag=true;
            for(int i=0;i<this->WorkerNumber;i++)
            {
                int col=0;
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->Name)));
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->Id)));
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->Sex)));
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->Age)));
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->PhoneNumber)));
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->DeptName)));
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->Salary)));
                ui->tablewidget->setItem(i,col++,new QTableWidgetItem
                                         (QString::fromStdString(this->WorkerArray[i]->GetTask())));

            }
        }
        else if(kindselect=="按姓名查询")
        {
            for(int i=0;i<this->WorkerNumber;i++)
            {
                if(this->WorkerArray[i]->Name==find)
                {
                    flag=true;
                    int col=0;
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->Name)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->Id)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->Sex)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->Age)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->PhoneNumber)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->DeptName)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->Salary)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem
                                             (QString::fromStdString(this->WorkerArray[i]->GetTask())));
                    index++;
                }
            }
        }
        else if(kindselect=="按工号查询")
        {
            for(int i=0;i<this->WorkerNumber;i++)
            {
                if(this->WorkerArray[i]->Id==find)
                {
                    flag=true;
                    int col=0;
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Name)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Id)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Sex)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Age)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->PhoneNumber)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->DeptName)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Salary)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->GetTask())));
                    index++;
                }
            }
        }
        else if(kindselect=="按部门查询")
        {
            for(int i=0;i<this->WorkerNumber;i++)
            {
                if(this->WorkerArray[i]->DeptName==find)
                {
                    flag=true;
                    int col=0;
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Name)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Id)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Sex)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Age)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->PhoneNumber)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->DeptName)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Salary)));
                    ui->tablewidget->setItem(index,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->GetTask())));
                    index++;
                }
            }
        }
        if(flag==false)
        {
            QMessageBox::information(this,"提示","抱歉，查无此人！");
        }
    });
}

//通过姓名找寻职工在数组中的下标
int MainWindow::find_worker(string temp)
{
    for(int i=0;i<this->WorkerNumber;i++)
    {
        if(this->WorkerArray[i]->Name==temp)
        {
            return i;
        }
    }
    return -1;
}

//修改职工的数据
void MainWindow::modify()
{
    this->init_worker();//初始化职工数组
    ui->modifman->setChecked(true);//设置性别按钮默认选中男
    //修改之前可以把需要修改的职工信息查询出来进行确认
    //先设置查询展示的表
    ui->modifytable->setRowCount(1);
    connect(ui->modifyquery,&QPushButton::clicked,[=](){
        ui->modifytable->clear();
        this->init_worker();//每次操作必先初始化数组
        string namee;
        int index=-1;
        namee=ui->modifyname->text().toStdString();
        index=this->find_worker(namee);
        if(index!=-1)
        {
            int col=0;
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Name)));
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Id)));
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Sex)));
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Age)));
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->PhoneNumber)));
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->DeptName)));
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Salary)));
            ui->modifytable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->GetTask())));
        }
        else
        {
            QMessageBox::information(this,"提示","抱歉，查无此人！");
        }
    });
    connect(ui->modifyclear,&QPushButton::clicked,[=](){//清空按钮操作：
        if(QMessageBox::Yes == QMessageBox::question(this,"警告","是否清空！",
                                                     QMessageBox::Yes|QMessageBox::No))
        {
            ui->modifyid->clear();
            ui->modifyname->clear();
            ui->modifyphonum->clear();
            ui->sexman->setChecked(true);
            ui->modifyage->setCurrentIndex(0);
            ui->modifydept->setCurrentIndex(0);
        }
    });

    connect(ui->modifysure,&QPushButton::clicked,[=](){
        bool flag=false;
        string idd,agee,namee,sexx,deptnamee,phonenumberr;
        //获取各项填写的内容值：
        idd=ui->modifyid->text().toStdString();
        namee=ui->modifyname->text().toStdString();
        phonenumberr=ui->modifyphonum->text().toStdString();
        if(ui->modifman->isChecked()) sexx="男";
        else sexx="女";
        agee=ui->modifyage->currentText().toStdString();
        deptnamee=ui->modifydept->currentText().toStdString();
        int index=this->find_worker(namee);
        if(index!=-1)
        {
            Worker *worker=NULL;
            //根据不同部门来创建对象
            if(deptnamee=="员工")
            {
                worker=new Employee(idd,namee,sexx,agee,phonenumberr);
            }
            else if(deptnamee=="经理")
            {
                worker=new Manager(idd,namee,sexx,agee,phonenumberr);
            }
            else
            {
                worker=new Boss(idd,namee,sexx,agee,phonenumberr);
            }
            this->WorkerArray[index]=worker;
            flag=true;
            this->save();//一旦数据修改就要保存到文本文件
        }
        if(flag==true)
        {
            if(QMessageBox::Yes == QMessageBox::question(this,"提示","修改职工成功，是否需要清空？",
                                                         QMessageBox::Yes|QMessageBox::No))
            {
                ui->modifyname->clear();
                ui->modifyid->clear();
                ui->modifyphonum->clear();
                ui->modifman->setChecked(true);
                ui->modifyage->setCurrentIndex(0);
                ui->modifydept->setCurrentIndex(0);
            }
        }
        else
        {
            QMessageBox::information(this,"提示","抱歉，查无此人,无法进行修改！");
        }
    });
}

//通过id来找寻职工在数组中的下标位置
int MainWindow::find_workerbyid(string temp)
{
    for(int i=0;i<this->WorkerNumber;i++)
    {
        if(this->WorkerArray[i]->Id==temp)
        {
            return i;
        }
    }
    return -1;
}

//删除职工
void MainWindow::delete_worker()
{
    this->init_worker();//初始化职工数组
    string kindselect=ui->deletekind->currentText().toStdString();
    string find=ui->deleteselect->text().toStdString();
    //删除之前可以先查询
    connect(ui->deletequery,&QPushButton::clicked,[=](){
        ui->deletetable->clearContents();//清空内容
        string kindselect=ui->deletekind->currentText().toStdString();
        string find=ui->deleteselect->text().toStdString();
        int index=-1;
        if(kindselect=="按姓名删除")
        {
                index=this->find_worker(find);
        }
        else
        {
                index=this->find_workerbyid(find);
        }
        if(index!=-1)
        {
            int col=0;
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Name)));
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Id)));
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Sex)));
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Age)));
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->PhoneNumber)));
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->DeptName)));
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->Salary)));
            ui->deletetable->setItem(0,col++,new QTableWidgetItem
                                     (QString::fromStdString(this->WorkerArray[index]->GetTask())));
        }
        else
        {
            QMessageBox::information(this,"提示","抱歉，查无此人！");
        }
    });
    //正式删除：
    connect(ui->deletesure,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"提示","确认是否删除职工？",
                                                     QMessageBox::Yes|QMessageBox::No))
        {
            string kindselect=ui->deletekind->currentText().toStdString();
            string find=ui->deleteselect->text().toStdString();
            int index=-1;
            if(kindselect=="按姓名删除")
            {
                    index=this->find_worker(find);
            }
            else
            {
                    index=this->find_workerbyid(find);
            }
            if(index!=-1)
            {
                for(int i=index;i<this->WorkerNumber;i++)
                {
                    this->WorkerArray[i]=this->WorkerArray[i+1];
                }
                this->WorkerNumber--;
                this->save();
                QMessageBox::information(this,"提示","删除成功！");
            }
            else
            {
                QMessageBox::information(this,"提示","抱歉，查无此人,无法进行删除！");
            }
        }

    });
}
//bool MainWindow::increaseid(Worker *a,Worker *b)
//{
//    return a->Id<b->Id;
//}
//bool MainWindow::decreaseid(Worker *a, Worker *b)
//{
//    return a->Id>b->Id;
//}
//bool MainWindow::increaseage(Worker *a, Worker *b)
//{
//    return a->Age<b->Age;
//}
//bool MainWindow::decreaseage(Worker *a, Worker *b)
//{
//    return a->Age>b->Age;
//}
void MainWindow::sortworker()//职工排序功能
{

    connect(ui->increasesort,&QPushButton::clicked,[=](){
        ui->sorttable->setRowCount(this->WorkerNumber);
        ui->sorttable->clearContents();
        string sortkind=ui->sortkind->currentText().toStdString();
        if(sortkind=="按工号")
        {
            //根据工号升序：
                for(int i=0;i<this->WorkerNumber;i++)
                {
                    for(int j=0;j<this->WorkerNumber-i-1;j++)
                    {
                        if(atoi(this->WorkerArray[j]->Id.c_str())>atoi(this->WorkerArray[j+1]->Id.c_str()))
                        {
                            Worker *temp=WorkerArray[j];
                            WorkerArray[j]=WorkerArray[j+1];
                            WorkerArray[j+1]=temp;
                        }
                    }
                }
        }
        else
        {
            //根据年龄升序：
                for(int i=0;i<this->WorkerNumber;i++)
                {
                    for(int j=0;j<this->WorkerNumber-i-1;j++)
                    {
                        if(atoi(this->WorkerArray[j]->Age.c_str())>atoi(this->WorkerArray[j+1]->Age.c_str()))
                        {
                            Worker *temp=WorkerArray[j];
                            WorkerArray[j]=WorkerArray[j+1];
                            WorkerArray[j+1]=temp;
                        }
                    }
                }
        }
        //升序后打印全体职工信息：
        for(int i=0;i<this->WorkerNumber;i++)
        {
            int col=0;
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Name)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Id)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Sex)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Age)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->PhoneNumber)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->DeptName)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Salary)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->GetTask())));

        }
    });
    connect(ui->decreasesort,&QPushButton::clicked,[=](){
        ui->sorttable->setRowCount(this->WorkerNumber);
        ui->sorttable->clearContents();
        string sortkind=ui->sortkind->currentText().toStdString();
        if(sortkind=="按工号")
        {
            //根据工号降序：
                for(int i=0;i<this->WorkerNumber;i++)
                {
                    for(int j=0;j<this->WorkerNumber-i-1;j++)
                    {
                        if(atoi(this->WorkerArray[j]->Id.c_str())<atoi(this->WorkerArray[j+1]->Id.c_str()))
                        {
                            Worker *temp=WorkerArray[j];
                            WorkerArray[j]=WorkerArray[j+1];
                            WorkerArray[j+1]=temp;
                        }
                    }
                }
        }
        else
        {
            //根据年龄降序：
                for(int i=0;i<this->WorkerNumber;i++)
                {
                    for(int j=0;j<this->WorkerNumber-i-1;j++)
                    {
                        if(atoi(this->WorkerArray[j]->Age.c_str())<atoi(this->WorkerArray[j+1]->Age.c_str()))
                        {
                            Worker *temp=WorkerArray[j];
                            WorkerArray[j]=WorkerArray[j+1];
                            WorkerArray[j+1]=temp;
                        }
                    }
                }
        }
        //降序后打印全体职工信息：
        for(int i=0;i<this->WorkerNumber;i++)
        {
            int col=0;
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Name)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Id)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Sex)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Age)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->PhoneNumber)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->DeptName)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->Salary)));
            ui->sorttable->setItem(i,col++,new QTableWidgetItem(QString::fromStdString(this->WorkerArray[i]->GetTask())));

        }
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}

