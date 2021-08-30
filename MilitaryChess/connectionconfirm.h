#ifndef CONNECTIONCONFIRM_H
#define CONNECTIONCONFIRM_H

#include <QWidget>
#include <QString>
#include <QLabel>

namespace Ui {
class ConnectionConfirm;
}

class ConnectionConfirm : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionConfirm(QWidget *parent = nullptr);
    ~ConnectionConfirm();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

signals:
    void rejectsignal();
    void acceptsignal();

private:
    Ui::ConnectionConfirm *ui;
};

#endif // CONNECTIONCONFIRM_H
