#include "ipvalidation.h"
#include "ui_ipvalidation.h"

IPValidation::IPValidation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IPValidation)
{
    ui->setupUi(this);

    this->setWindowTitle("Create the connection");
    flag = false;
}

IPValidation::~IPValidation()
{
    delete ui;
}

void IPValidation::setsentip()
{
    this->flag = true;
}

void IPValidation::setip(QString val)
{
    this->ip = val;
}

void IPValidation::showip()
{
    QString hostname = QHostInfo::localHostName();
    QHostInfo hostinfo = QHostInfo::fromName(hostname);
    auto IPList = hostinfo.addresses();
    foreach(auto ipaddress, IPList)
    {
        if((ipaddress.protocol() == QAbstractSocket::IPv4Protocol) && (ipaddress != QHostAddress::LocalHost))
        {
            HostIP = "Host IP: ";
            HostIP += ipaddress.toString();
            break;
        }
    }
    QLabel* label = new QLabel(this);
    label->setText(HostIP);
    label->move(126, 100);
    this->show();
}

void IPValidation::on_buttonBox_rejected()
{
    this->close();
}


void IPValidation::on_buttonBox_accepted()
{
    emit IPValSignal();
    this->close();
}


