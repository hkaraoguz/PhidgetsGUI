#ifndef DIGITALIODIALOG_H
#define DIGITALIODIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVariant>

namespace Ui {
class DigitalIODialog;
}

class DigitalIODialog : public QDialog
{
    Q_OBJECT

public:
    explicit DigitalIODialog(QWidget *parent = 0);
    ~DigitalIODialog();
public slots:
    void handleInterfaceSensorandInputReadings(QList<QList<QVariant> >readings);

private slots:
    void on_comboBoxDigitalOut1_currentIndexChanged(int index);

    void on_comboBoxDigitalOut2_currentIndexChanged(int index);

    void on_comboBoxDigitalOut3_currentIndexChanged(int index);

    void on_comboBoxDigitalOut4_currentIndexChanged(int index);

    void on_comboBoxDigitalOut5_currentIndexChanged(int index);

    void on_comboBoxDigitalOut6_currentIndexChanged(int index);

    void on_comboBoxDigitalOut7_currentIndexChanged(int index);

    void on_comboBoxDigitalOut8_currentIndexChanged(int index);

private:
    Ui::DigitalIODialog *ui;
    QList<QLabel*> inputlabels;
signals:
    void digitalInputToggle(QList<int> values);
};

#endif // DIGITALIODIALOG_H
