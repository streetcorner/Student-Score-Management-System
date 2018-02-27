#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QLineEdit>
#include <QWidget>
#include <QRect>
#include <QPoint>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowOpacity(0.97);
    setAttribute(Qt::WA_TranslucentBackground,true); //背景透明
    model=new QSqlTableModel();
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    on_comboBox_2_activated(0); //初始化窗口
    ui->lineEdit->setPlaceholderText(QStringLiteral("studentID/name/dormitory"));
    //编辑框里的隐形字
    //ui->lineEdit->setStyleSheet(");

}

MainWindow::~MainWindow()
{
    delete ui;
}


//右上角关闭按钮
void MainWindow::on_pushButton_5_clicked()
{
    close();
}

//解决去掉标题栏导致的窗口移动问题
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    m_Drag = true;
    last = e->globalPos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    m_Drag = true;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

//显示数据库列表
void MainWindow::on_comboBox_2_activated(int index)
{

    int index1= ui->comboBox_2->currentIndex();
    //qDebug()<<index1;
    if(index1==0) //课程列表
    {
        tablename="course";
        ui->comboBox->hide();
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->pushButton->hide();
        ui->pushButton_4->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->pushButton_3->show();
        ui->pushButton_6->show();
    }
    else if(index1==1) //学生列表
    {
        tablename="student";
        ui->comboBox->hide();
        ui->lineEdit->show();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->pushButton->show();
        ui->pushButton_4->hide();
        ui->pushButton_3->show();
        ui->pushButton_6->show();
        ui->label_2->hide();
        ui->label_3->hide();
    }
    else if(index1==2) //成绩列表。注意没有0的情况
    {
        tablename="grade";
        ui->comboBox->show();
        ui->lineEdit->hide();
        ui->lineEdit_2->show();
        ui->lineEdit_3->show();
        ui->pushButton->hide();
        ui->pushButton_4->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->pushButton_3->show();
        ui->pushButton_6->show();
    }
    model->setTable(tablename);
   //qDebug()<<model->tableName();
    //model->setFilter("courese_credit>2");
    model->select();
    ui->tableView->setModel(model);
}

//录入
void MainWindow::on_pushButton_3_clicked()
{
    QSqlRecord record = model->record();
    model->insertRecord(model->rowCount(),record);
}

//删除
void MainWindow::on_pushButton_6_clicked()
{
    int curRow = ui->tableView->currentIndex().row(); //获取选中的行
    QItemSelectionModel *selections = ui->tableView->selectionModel(); //返回当前的选择模式
    int ok = QMessageBox::warning(this->ui->tableView,tr("删除当前行！"),tr("确定要删除当前行吗？"),
               QMessageBox::Yes,QMessageBox::No); //tr转汉字
    model->database().transaction();//开始事务操作
    if(ok == QMessageBox::No)
       {
          model->revertAll(); //如不删除，则撤销
       }
    else
    {
        model->submitAll();
        model->removeRow(curRow); //删除该行

    }
    model->database().commit();
    on_comboBox_2_activated(0); //初始化窗口
}

//保存
void MainWindow::on_pushButton_8_clicked()
{
    model->database().transaction();//开始事务操作
    if (model->submitAll())
    {
        model->database().commit();//提交
    }
    else {
            model->database().rollback(); //回滚
            QMessageBox::warning(this,tr("tableModel"),
                                tr("数据库错误:%1")
                                .arg(model->lastError().text()));
          }

    //计算综合成绩和学分
    if(model->tableName() == "grade")
    {
        //计算综合成绩
        int curRow = ui->tableView->currentIndex().row(); //获取当前行
        double regularg = model->data(model->index(curRow,2)).toDouble() ;
        double pracg = model->data(model->index(curRow,3)).toDouble();
        double paperg = model->data(model->index(curRow,4)).toDouble();
        double fingrade;
        if(pracg> 0)
        {
           fingrade = 0.15 * regularg + 0.15 * pracg + 0.7 * paperg;
        }
       else
        {
            fingrade = 0.3 * regularg +0.7 * paperg;
        }
        model->setData(model->index(curRow,5),fingrade);


        //计算学分
        QString courID = model->data(model->index(curRow,0)).toString(); //获得courseID
        QString str = "select * from course where courseID = "+courID;
        //转化成string类型，001才得以保存下来
        query.exec(str);  //进入course表，找对应学分那行select * from course where courseID = "+courID
        double courcredit;
       if(query.first())
        {
            courcredit = query.value(2).toDouble(); //获取那行数据集里的课程学分项
        }
       else
       {
            qDebug()<<query.lastError().text();
            //qDebug() << str;
       }
        //qDebug()<< courID;
        //qDebug()<< courcredit;
        double fincredit;
        if(fingrade > 90)
            {fincredit = courcredit;}
        else if(fingrade >80)
            {fincredit = courcredit * 0.8;}
        else if(fingrade >70)
            {fincredit = courcredit * 0.75;}
        else if(fingrade >60)
            {fincredit = courcredit * 0.6;}
        else
            {fincredit = 0;}
        model->setData(model->index(curRow,6),fincredit);
        model->submitAll(); //写入数据库
    }
    on_comboBox_2_activated(0); //初始化窗口

}

//撤销修改
void MainWindow::on_pushButton_7_clicked()
{
    model->revertAll();
}

// 排序
void MainWindow::on_comboBox_activated(int index)
{
    if(model->tableName() == "grade")
    {
        int index2= ui->comboBox->currentIndex();
        //qDebug()<<index1;
        if(index2==0)
        {
            model->setSort(5,Qt::AscendingOrder);
            model->select();
        }
        else if(index2==1)
        {
            model->setSort(5,Qt::DescendingOrder);
            model->select();
        }
        else if(index2==2)
        {
            model->setSort(6,Qt::AscendingOrder);
            model->select();
        }
        else
        {
            model->setSort(6,Qt::DescendingOrder);
            model->select();
        }
    }

}

//lineedit中的提示信息可以在单击后消失


//学生信息查询
void MainWindow::on_pushButton_clicked()
{
    QString input = ui->lineEdit->text();       // 获取编辑框的内容
    int len;
    len = input.length();
    if (input.at(0).isNumber()) //获得编辑框里的内容，取第一个字符判断区分数字和姓名
    {
       //qDebug()<<input;
        if(len > 8)  //区分学号和宿舍号
        {
            //学号
            QString str= "studentID= "+input;
            //qDebug()<<str;
            model->setFilter(str);
            //qDebug()<<model->tableName();
            //qDebug()<<input;
            model->select();
            ui->tableView->setModel(model);
        }//
        else //宿舍号
        {
            model->setFilter(QString("dormitory = %1").arg(input));
        }
    }
    else
    {
        //姓名
        model->setFilter(QString("student_name = '%1'").arg(input));
        //query.exec("select * from studennt where  = "+input1);
    }
    ui->pushButton_3->hide();
    ui->pushButton_6->hide();
}

//成绩查询
void MainWindow::on_pushButton_4_clicked()
{
    QString input = ui->lineEdit_2->text(); //input输入为学生学号
    QString input2 = ui-> lineEdit_3->text(); //input2输入的为课程编号
    if (input2.isEmpty() && !input.isEmpty() )
    {
        //学号
        QString str = "studentID = "+input; //分开写不会报错。。
        //qDebug()<<str;
        model->setFilter(str);
    }
    if(input.isEmpty() && !input2.isEmpty())//课程编号
    {
        QString str2 = "courseID = "+input2;
        //qDebug()<<str2;
        model->setFilter(str2);
        //query.exec("select *from grade where courseID =" +input2);
    }
    if(!input.isEmpty() && !input2.isEmpty())
    {
        QString str = "studentID = "+input+" and courseID = "+input2; //分开写不会报错。。
        //QString str2 = "courseID = "+input2;
        qDebug()<<str;
        model->setFilter(str);
    }
    model->select();
    ui->tableView->setModel(model);
    ui->pushButton_3->hide();
    ui->pushButton_6->hide();
}

