#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "phidgetsmotorcontrol.h"
#include "phidgetsdevicemanager.h"
#include "phidgetsinterfacekit.h"
#include "irsensorgraphicsscene.h"

#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QList>
#include <QMessageBox>
#include <QTimer>
#include <QListIterator>

class MotorControlDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QFutureWatcher<int> fwatcherDevices;
    QFutureWatcher<int> fwatcherMotors;

    QList<PhidgetsMotorControl*> motorControls;

    MotorControlDialog* motorcontroldialog;

    PhidgetsInterfaceKit* interfacekit;

   // PhidgetsMotorControl* motorControl;

    PhidgetsDeviceManager* deviceManager;

    IRSensorGraphicsScene* scene;

    void initView(int count);
private slots:
    void handleMotorControlInit();
    void handleDeviceManagerInit();
    void handleMotorControlDetached(int serialno);
    void handleMotorControlValues(MotorControlData data);
    void handleInterfaceSensorandInputReadings(QList<QList<QVariant> >readings);
   // void handleMotorCard(int serialno);
    void on_horSliderVel_valueChanged(int value);
    void on_horSliderAcc_valueChanged(int value);
    void on_pushButtonStop1_clicked();
    void on_horSliderVel2_valueChanged(int value);
    void on_horSliderAcc2_valueChanged(int value);
    void on_pushButtonStop2_clicked();
    void on_pushButtoResetEnc2_clicked();
    void on_pushButtonResetEnc1_clicked();
    void on_actionMotors_triggered();
};

#endif // MAINWINDOW_H
