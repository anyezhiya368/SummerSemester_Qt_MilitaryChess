#include "connectionconfirm.h"
#include "ui_connectionconfirm.h"

ConnectionConfirm::ConnectionConfirm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionConfirm)
{
    ui->setupUi(this);
}

ConnectionConfirm::~ConnectionConfirm()
{
    delete ui;
}

void ConnectionConfirm::on_buttonBox_rejected()
{
    this->close();
    emit rejectsignal();
}


void ConnectionConfirm::on_buttonBox_accepted()
{
    this->close();
    emit acceptsignal();
}

