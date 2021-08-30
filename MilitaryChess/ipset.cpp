#include "ipset.h"
#include "ui_ipset.h"
#include <regex>
#include <string>

IPSet::IPSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IPSet)
{
    ui->setupUi(this);

    this->setWindowTitle("Connect to server");
    ip = "";


}

IPSet::~IPSet()
{
    delete ui;
}

void IPSet::on_pushButton_clicked()
{
    ip += "1";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_2_clicked()
{
    ip += "2";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_3_clicked()
{
    ip += "3";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_4_clicked()
{
    ip += "4";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_5_clicked()
{
    ip += "5";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_6_clicked()
{
    ip += "6";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_7_clicked()
{
    ip += "7";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_8_clicked()
{
    ip += "8";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_9_clicked()
{
    ip += "9";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_11_clicked()
{
    ip += "0";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_12_clicked()
{
    ip += ".";
    ui->lineEdit->setText(ip);
}


void IPSet::on_pushButton_10_clicked()
{
    qsizetype n = ip.size();
    ip.remove(n - 1, 1);
    ui->lineEdit->setText(ip);
}


void IPSet::on_buttonBox_accepted()
{
    std::regex right_ip("\\b((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\b");
    bool ret = std::regex_match(ip.toStdString(), right_ip);
    if(ret)
    {
        emit MySignal(ip);
        ip = "";
        this->close();
    }
    else
    {
        QWidget* wid = new QWidget();
        QLabel* lab = new QLabel(wid);
        lab->setText("Wrong input! Please enter a correct ip.");
        ip = "";
        wid->show();
    }
}


void IPSet::on_buttonBox_rejected()
{
    ip = "";
    this->close();
}

