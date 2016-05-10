#ifndef MOTORCONTROLDIALOG_H
#define MOTORCONTROLDIALOG_H

#include <QDialog>
#include <QVariant>
#include "phidgetsmotorcontrol.h"

namespace Ui {
class MotorControlDialog;
}

class MotorControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MotorControlDialog(QWidget *parent = 0);

    void appendToMotorControls(PhidgetsMotorControl* motorControl);
    QList<PhidgetsMotorControl*> getMotorControls();

    ~MotorControlDialog();

    void initView();

private slots:
    void on_horSliderVel_valueChanged(int value);

    void on_horSliderAcc_valueChanged(int value);

    void on_pushButtonResetEnc1_clicked();

    void on_pushButtonStop1_clicked();

    void on_pushButtoResetEnc2_clicked();

    void on_pushButtonStop2_clicked();

    void on_horSliderVel2_valueChanged(int value);

    void on_horSliderAcc2_valueChanged(int value);

private:
    Ui::MotorControlDialog *ui;
    QList<PhidgetsMotorControl*> motorControls;

public slots:

    void handleMotorControlDetached(int serialno);
    void handleMotorControlValues(MotorControlData data);
};

#endif // MOTORCONTROLDIALOG_H
