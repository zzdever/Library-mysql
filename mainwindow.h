#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <QString>
#include <fstream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();

    void on_logoutButton_clicked();

    void on_singleInsertButton_clicked();

    void on_selectButton_clicked();

    void on_borrowCardButton_clicked();

    void on_borrowButton_clicked();

    void on_returnCardButton_clicked();

    void on_returnButton_clicked();

    void on_cardInsertButton_clicked();

    void on_cardDeleteButton_clicked();

    void on_muliInsertButton_clicked();

private:
    Ui::MainWindow *ui;
    MYSQL mMySql;

    void showNotLogInBox()
    {
        QMessageBox msgBox;
        msgBox.setText(QString("还米有登录吧 0_0"));
        msgBox.exec();
    }

    void showSuccessBox()
    {
        QMessageBox msgBox;
        msgBox.setText(QString("操作成功!"));
        msgBox.exec();
    }

    void showHintBox(string str)
    {
        QMessageBox msgBox;
        msgBox.setText(QString(str.c_str()));
        msgBox.exec();
    }

    void setTable(MYSQL_RES *result,QTableWidget *table)
    {
        my_ulonglong fieldCount=mysql_num_fields(result);
        my_ulonglong rowCount=mysql_num_rows(result);
        table->setRowCount(rowCount);
        MYSQL_ROW row;
        row=mysql_fetch_row(result);
        for(my_ulonglong i=0;i<rowCount;++i)
        {
            for(my_ulonglong j=0;j<fieldCount;++j)
            {
                table->setItem(i,j,new QTableWidgetItem(QString(row[j])));

            }
            row=mysql_fetch_row(result);
        }
        table->show();
    }

    void getErr(MYSQL*mysql)
    {
        const char* err=mysql_error(mysql);
        QMessageBox msgBox;
        msgBox.setText(QString(err));
        msgBox.exec();
    }
};

#endif // MAINWINDOW_H
