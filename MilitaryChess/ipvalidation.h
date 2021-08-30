#ifndef IPVALIDATION_H
#define IPVALIDATION_H

#include <QWidget>
#include <QLabel>
#include <QTcpServer>
#include <QHostInfo>
#include "ipset.h"

namespace Ui {
class IPValidation;
}

class IPValidation : public QWidget
{
    Q_OBJECT

public:
    explicit IPValidation(QWidget *parent = nullptr);
    ~IPValidation();
    void showip();
    void setsentip();
    void setip(QString);
signals:
    void IPValSignal();


private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::IPValidation *ui;
    bool flag;
    QString ip;
    QString HostIP;
};

#endif // IPVALIDATION_H
