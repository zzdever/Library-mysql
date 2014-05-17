
#include "mainwindow.h"
//if you want to #include,please add into mainwindow.h

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mysql_init(&mMySql);

    ui->pwEdit->setEchoMode(QLineEdit::Password);

    if(mysql_real_connect(&mMySql,
                          "localhost",
                          "root",
                          "",
                          "proj",//change database
                          3306,NULL,0))
    {
        mysql_query(&mMySql,"set names 'utf8'");  //set encoding
    }
    else
        getErr(&mMySql);
}

MainWindow::~MainWindow()
{
    delete ui;
    mysql_close(&mMySql);
}

void MainWindow::on_loginButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("未登录!!!"))
    {
        string strToQuery=(QString("select * from admin where id='")+ui->userEdit->text()+QString("' and passwd='")+
                           ui->pwEdit->text()+QString("';")).toStdString();
        if(mysql_query(&mMySql,strToQuery.c_str()))
            getErr(&mMySql);
        MYSQL_RES *result=NULL;
        result=mysql_store_result(&mMySql);

        if(mysql_num_rows(result)>0)ui->logInfoLabel->setText("已登录!!!");
        else showHintBox("用户名或密码填错啦==");

    }
}

void MainWindow::on_logoutButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("未登录!!!"))
    {
        showHintBox("本来就没登录好不好");
    }
    else
    {
        ui->logInfoLabel->setText("未登录!!!");
        showHintBox("下次再来～");
    }
}

void MainWindow::on_singleInsertButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        //need to modify something maybe
        if(ui->bnoEdit->text()==NULL)showHintBox("书号是必须嗒～");
        else
        {
            string strToQuery=(QString("insert into book values('")+
                               (ui->bnoEdit->text().isEmpty()?QString("NULL"):ui->bnoEdit->text())+QString("','")+
                               (ui->categoryEdit->text().isEmpty()?QString("NULL"):ui->categoryEdit->text())+QString("','")+
                               (ui->titleEdit->text().isEmpty()?QString("NULL"):ui->titleEdit->text())+QString("','")+
                               (ui->pressEdit->text().isEmpty()?QString("NULL"):ui->pressEdit->text())+QString("','")+
                               (ui->yearEdit->text().isEmpty()?QString("NULL"):ui->yearEdit->text())+QString("','")+
                               (ui->authorEdit->text().isEmpty()?QString("NULL"):ui->authorEdit->text())+QString("',")+
                               (ui->priceEdit->text().isEmpty()?QString("NULL"):ui->priceEdit->text())+QString(",")+
                               (ui->totalEdit->text().isEmpty()?QString("NULL"):ui->totalEdit->text())+QString(",")+
                               (ui->totalEdit->text().isEmpty()?QString("NULL"):ui->totalEdit->text())+QString(");")
                               ).toStdString();
            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);
            else
                showSuccessBox();
        }
    }
    else
        showNotLogInBox();
}

void MainWindow::on_muliInsertButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        QString path= QFileDialog::getOpenFileName();

        if(!path.isEmpty())
        {
            string bno,category,title,press,year,author,price,total,stock;
            ifstream fin(path.toStdString().c_str());
            while(fin>>bno>>category>>title>>press>>year>>author>>price>>total>>stock)
            {
                //you need to modify something maybe
                string strToQuery=(string("insert into book values('")+
                                   bno+string("','")+category+string("','")+
                                   title+string("','")+press+string("','")+
                                   year+string("','")+author+string("',")+
                                   price+string(",")+total+string(",")+
                                   stock+string(");")
                                   );
                if(mysql_query(&mMySql,strToQuery.c_str()))
                    getErr(&mMySql);
            }
            fin.close();
            showSuccessBox();
        }
    }
    else
        showNotLogInBox();
}

void MainWindow::on_selectButton_clicked()
{
    if(1)//(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        /*
         * modify "select ...".
         * PS : get info from edits :
         *      ui->categorySelectEdit->text() ...
         *      ui->yearFromSpin->value() ...
         * PS : change type :
         *      QString.toStdString() ...
         *      QString.toInt() ...
         */
        QString condition;
        string strToQuery;

        condition=QString("select * from book where");
        condition+=QString(" year between '")+ui->yearFromSpin->text()+QString("-01-01' and '")+ui->yearToSpin->text()+QString("-12-31'");
        condition+=QString(" and price between ")+ui->priceFromSpin->text()+QString(" and ")+ui->priceToSpin->text();
        if(!ui->categorySelectEdit->text().isEmpty())condition+=QString(" and category like '%")+ui->categorySelectEdit->text()+QString("%'");
        if(!ui->titleSelectEdit->text().isEmpty())condition+=QString(" and title like '%")+ui->titleSelectEdit->text()+QString("%'");
        if(!ui->pressSelectEdit->text().isEmpty())condition+=QString(" and press like '%")+ui->pressSelectEdit->text()+QString("%'");
        if(!ui->authorSelectEdit->text().isEmpty())condition+=QString(" and author like '%")+ui->authorSelectEdit->text()+QString("%'");
        if(!ui->isbnSelectEdit->text().isEmpty())condition+=QString(" and isbn like '%")+ui->isbnSelectEdit->text()+QString("%'");
        condition+=QString(";");

        strToQuery=condition.toStdString();
        if(mysql_query(&mMySql,strToQuery.c_str()))
            getErr(&mMySql);
        //else
            //showSuccessBox();

        MYSQL_RES *result=NULL;
        result=mysql_store_result(&mMySql);

        setTable(result,ui->bookTable);

        mysql_free_result(result);
    }
    else
        showNotLogInBox();
}

void MainWindow::on_borrowCardButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        if(!ui->borrowCardEdit->text().isEmpty())
        {
            string strToQuery=(QString("select * from card where cno='")+ui->borrowCardEdit->text()+QString("';")).toStdString();
            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);
            MYSQL_RES *result=NULL;
            result=mysql_store_result(&mMySql);

            if(mysql_num_rows(result)>0)
            {
                //similar with before,modify "select ..."
                //get card info from ui->borrowCardEdit->text()
                strToQuery=(QString("select * from borrow where cno='")+ui->borrowCardEdit->text()+QString("';")).toStdString();

                if(mysql_query(&mMySql,strToQuery.c_str()))
                    getErr(&mMySql);
                //else
                  //  showSuccessBox();

                result=mysql_store_result(&mMySql);

                setTable(result,ui->borrowCardTable);
            }
            else
            {
                showHintBox("查无此证！你逗我");
            }

            mysql_free_result(result);
        }
    }
    else
        showNotLogInBox();
}

void MainWindow::on_borrowButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        if(!ui->borrowCardEdit->text().isEmpty() && !ui->borrowEdit->text().isEmpty())
        {
            int stock=0;
            string strToQuery=(QString("select stock from book where isbn='")+
                               ui->borrowEdit->text()+QString("';")).toStdString();
            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);

            MYSQL_RES *result=NULL;
            MYSQL_ROW row;
            result=mysql_store_result(&mMySql);

            if(mysql_num_rows(result)>0)
            {
                row=mysql_fetch_row(result);
                stock=atoi(row[0]);
                //showHintBox(row[0]);
            }


            if(stock>0)
            {
                strToQuery=(QString("select name from admin where id='")+ui->userEdit->text()+QString("';")).toStdString();
                if(mysql_query(&mMySql,strToQuery.c_str()))
                    getErr(&mMySql);
                else
                {
                    result=mysql_store_result(&mMySql);
                    row=mysql_fetch_row(result);
                }

                QDateTime time;
                time=QDateTime::currentDateTime();
                QString time_str=time.toString("yyyy-MM-dd");
                strToQuery=(QString("insert into borrow values('")+
                                   ui->borrowCardEdit->text()+QString("','")+ui->borrowEdit->text()+QString("','")+
                                   time_str+QString("',NULL,'','")+QString(row[0])+QString("');")).toStdString();

                if(mysql_query(&mMySql,strToQuery.c_str()))
                    getErr(&mMySql);
                else
                {
                    //calculate the deadline
                    time=time.QDateTime::addDays(40);
                    showHintBox((QString("给你！拿去！期限似四死天  (死线是")+time.toString("yyyy-MM-dd")+QString(")")).toStdString());
                }

                strToQuery=(QString("update book set stock=stock-1 where isbn='")+
                                   ui->borrowEdit->text()+QString("';")).toStdString();
                if(mysql_query(&mMySql,strToQuery.c_str()))
                    getErr(&mMySql);

                on_borrowCardButton_clicked();
            }
            else
            {
                mysql_num_rows(result)>0 ? showHintBox("米有了嘛！人家94不给") : showHintBox("查无此书！");
            }

            mysql_free_result(result);

        }
        else
        {
            showHintBox("你猜猜缺什么信息");
        }
    }
    else
        showNotLogInBox();
}

void MainWindow::on_returnCardButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        if(!ui->returnCardEdit->text().isEmpty())
        {            
            string strToQuery=(QString("select * from card where cno='")+ui->returnCardEdit->text()+QString("';")).toStdString();
            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);
            MYSQL_RES *result=NULL;
            result=mysql_store_result(&mMySql);

            if(mysql_num_rows(result)>0)
            {
                strToQuery=(QString("select * from borrow where cno='")+ui->returnCardEdit->text()+QString("';")).toStdString();

                if(mysql_query(&mMySql,strToQuery.c_str()))
                    getErr(&mMySql);
                //else
                  //  showSuccessBox();

                result=mysql_store_result(&mMySql);

                setTable(result,ui->returnCardTable);
            }
            else
            {
                showHintBox("查无此证！你逗我");
            }

            mysql_free_result(result);
        }
    }
    else
        showNotLogInBox();
}

void MainWindow::on_returnButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        if(!ui->returnCardEdit->text().isEmpty() && !ui->returnEdit->text().isEmpty())
        {
            QDate time, time_borrow;
            QString time_str;
            time=QDate::currentDate();
            string strToQuery=(QString("select borrow_date from borrow where cno='")+
                               ui->returnCardEdit->text()+QString("' and bno='")+
                               ui->returnEdit->text()+QString("';")).toStdString();
            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);

            MYSQL_RES *result=NULL;
            MYSQL_ROW row;
            result=mysql_store_result(&mMySql);
            if(mysql_num_rows(result)>0)
            {
                row=mysql_fetch_row(result);
            }
            else
            {
                showHintBox("你没有借这本书！");
                return;
            }

            time_borrow=QDate::fromString(QString(row[0]),QString("yyyy-MM-dd"));

            int delay_days=0;
            delay_days=time_borrow.daysTo(time)-40;
            if(delay_days<=0)
            {
                showHintBox("收到！");
            }
            else
            {
                showHintBox((QString("你来晚了！罚你 ")+QString::number(delay_days*10,10)+QString(" 元！")).toStdString());
            }


            //update return date and penalty
            time_str=time.toString("yyyy-MM-dd");
            strToQuery=(QString("update borrow set return_date='")+time_str+
                               QString("',penalty='")+(delay_days>0 ? QString::number(delay_days*10,10) : 0)+
                               QString("' where cno='")+ui->returnCardEdit->text()+QString("' and bno='")+
                               ui->returnEdit->text()+QString("';")).toStdString();

            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);


            strToQuery=(QString("update book set stock=stock+1 where isbn='")+
                               ui->returnEdit->text()+QString("';")).toStdString();
            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);

            on_returnCardButton_clicked();

            mysql_free_result(result);

        }
        else
        {
            showHintBox("你猜猜缺什么信息");
        }
    }
    else
        showNotLogInBox();

}

void MainWindow::on_cardInsertButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        if(ui->cnoInsertEdit->text()==NULL)showHintBox("你必须告诉人家卡号！");
        else
        {
            string strToQuery=(QString("insert into card values('")+
                               (ui->cnoInsertEdit->text().isEmpty()?QString("NULL"):ui->cnoInsertEdit->text())+QString("','")+
                               (ui->nameInsertEdit->text().isEmpty()?QString("NULL"):ui->nameInsertEdit->text())+QString("','")+
                               (ui->depInsertEdit->text().isEmpty()?QString("NULL"):ui->depInsertEdit->text())+QString("','")+
                               (ui->typeInsertEdit->text().isEmpty()?QString("NULL"):ui->typeInsertEdit->text())+QString("');")
                               ).toStdString();
            if(mysql_query(&mMySql,strToQuery.c_str()))
                getErr(&mMySql);
            else
                showSuccessBox();
        }
    }
    else
        showNotLogInBox();

}

void MainWindow::on_cardDeleteButton_clicked()
{
    if(ui->logInfoLabel->text()==QString("已登录!!!"))
    {
        string strToQuery=(QString("delete from card where cno='")+
                           ui->cnoDeleteEdit->text()+QString("';")
                           ).toStdString();
        if(mysql_query(&mMySql,strToQuery.c_str()))
            getErr(&mMySql);
        else
            showHintBox("呜呜被T了...");
    }
    else
        showNotLogInBox();
}



