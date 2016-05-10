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
class DigitalIODialog;

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

    QList<QLabel*> irlabels;

    MotorControlDialog* motorcontroldialog;

    DigitalIODialog* digitaliodialog;

    PhidgetsInterfaceKit* interfacekit;

   // PhidgetsMotorControl* motorControl;

    PhidgetsDeviceManager* deviceManager;

    IRSensorGraphicsScene* scene;

    void initView(int count);


private slots:
    void handleMotorControlInit();
    void handleDeviceManagerInit();
   // void handleMotorControlDetached(int serialno);
    void handleMotorControlValues(MotorControlData data);
    void handleInterfaceSensorandInputReadings(QList<QList<QVariant> >readings);
   // void handleMotorCard(int serialno);

    void on_actionMotors_triggered();
    void on_actionDigital_I_O_triggered();
};

#endif // MAINWINDOW_H
