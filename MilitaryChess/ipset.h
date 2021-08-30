#ifndef IPSET_H
#define IPSET_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class IPSet;
}

class IPSet : public QWidget
{
    Q_OBJECT

public:
    explicit IPSet(QWidget *parent = nullptr);
    ~IPSet();

signals:
    void MySignal(QString ip);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();


private:
    Ui::IPSet *ui;
    QString ip;
};

#endif // IPSET_H
