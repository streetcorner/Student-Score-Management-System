#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMouseEvent> //解决窗口拖动问题
#include <QMainWindow>
#include <QString>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlQuery>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void region(const QPoint &cursorGlobalPoint);
    private slots:
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    //void on_comboBox_2_currentIndexChanged(int index);
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    //void on_lineEdit_returnPressed();
    void on_pushButton_4_clicked();
    void on_comboBox_activated(int index);
    void on_comboBox_2_activated(int index);

     //改善窗口移动问题
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;
    QPoint last; //改善鼠标移动问题
    QString tablename;
    QSqlTableModel *model;
    QSqlQuery query;  
    bool winEvent(MSG *message, long *result);
    bool m_Drag;
};

#endif // MAINWINDOW_H
