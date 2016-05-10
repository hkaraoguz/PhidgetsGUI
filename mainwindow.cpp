#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "motorcontroldialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new IRSensorGraphicsScene;

    ui->graphicsViewIR->setScene(scene);

    // Disable the widgets
  //  ui->tabWidget->setEnabled(false);
    ui->groupBoxMotor1->setEnabled(false);
    ui->groupBoxMotor2->setEnabled(false);

    // This is the main device manager
    deviceManager = new PhidgetsDeviceManager(this);

    // Connect the main device (this is obsolete)
    //connect(deviceManager,SIGNAL(motorCard(int)),this,SLOT(handleMotorCard(int)));

    // Init devices-> This is already using threads so we don't need to generate a thread for this
    deviceManager->initDevices();

    // Waiting for device initializations to be completed
    QTimer::singleShot(5000,this,SLOT(handleDeviceManagerInit()));


    // QFuture<int> future = QtConcurrent::run(deviceManager,&PhidgetsDeviceManager::initDevices);

    //  fwatcherDevices.setFuture(future);

    //  connect(&fwatcherDevices, SIGNAL(finished()), this, SLOT(handleDeviceManagerInit()));


}
// Now the devices are initialized
void MainWindow::handleDeviceManagerInit()
{


    qDebug()<<"Devices are initialized";

    QList<int> motorcardserials = this->deviceManager->getMotorControlCardSerials();
    int interfaceserial = this->deviceManager->getInterfaceKitSerial();

    motorcontroldialog = new MotorControlDialog(this);

    motorcontroldialog->show();

    foreach (int serial, motorcardserials)
    {
        PhidgetsMotorControl* motorControl = new PhidgetsMotorControl(this,serial);

      // connect(motorControl,SIGNAL(motorControlDetached(int)), this, SLOT(handleMotorControlDetached(int)));

      //  connect(motorControl,SIGNAL(motorControlValues(MotorControlData)),this,SLOT(handleMotorControlValues(MotorControlData)));

        this->motorControls.push_back(motorControl);

        if(motorControl->initMotorCard())
        {

            connect(motorControl,SIGNAL(motorControlDetached(int)), this->motorcontroldialog, SLOT(handleMotorControlDetached(int)));

            connect(motorControl,SIGNAL(motorControlValues(MotorControlData)),this->motorcontroldialog,SLOT(handleMotorControlValues(MotorControlData)));

            this->motorcontroldialog->appendToMotorControls(motorControl);
            this->motorcontroldialog->initView();
           // handleMotorControlInit();
        }

        //  QFuture<bool> future = QtConcurrent::run(motorControl,&PhidgetsMotorControl::initMotorCard);

        //  fwatcher.setFuture(future);

        //  connect(&fwatcher, SIGNAL(finished()), this, SLOT(handleMotorControlInit()));

    }



    if(interfaceserial > 0)
    {
        interfacekit = new PhidgetsInterfaceKit(this, interfaceserial);
        if(interfacekit->initKit())
        {
            connect(interfacekit,SIGNAL(sensorAndInputReadings(QList<QList< QVariant > >)),this,SLOT(handleInterfaceSensorandInputReadings(QList<QList< QVariant > >)));
        }


    }

    this->deviceManager->deleteLater();

    // I don't need this thing anymore


    /* else
    {
        QMessageBox::critical(this,"Error","Error initializing Phidgets Devices");

    }*/


}
void MainWindow::handleInterfaceSensorandInputReadings(QList< QList<QVariant> > readings)
{
    QList<QVariant> distanceReadings = readings[0];


    QList<double> doubledistances;

    foreach (QVariant dist, distanceReadings) {

       // qDebug()<<"I am here"<<dist;

        doubledistances.append(dist.toDouble());
        this->scene->drawDistanceSensors(doubledistances);
        ui->graphicsViewIR->update();

    }



}
void MainWindow::handleMotorControlValues(MotorControlData data)
{
    PhidgetsMotorControl* obj = qobject_cast<PhidgetsMotorControl*>(sender());

    int count = 0;

    QMutableListIterator<PhidgetsMotorControl*> iterator(this->motorControls);

    while(iterator.hasNext()) {

        PhidgetsMotorControl* iobj = iterator.next();

        if(iobj->getSerialNo() == obj->getSerialNo()){

            if(count == 0)
            {
                ui->labCurrent1->setText(QString::number(data.current));
                ui->labSpeed1->setText(QString::number(data.speed));
                ui->labEncoder1->setText(QString::number(data.encoder));
                ui->labVoltage1->setText(QString::number(data.voltage));

            }
            else
            {
                ui->labCurrent2->setText(QString::number(data.current));
                ui->labSpeed2->setText(QString::number(data.speed));
                ui->labEncoder2->setText(QString::number(data.encoder));
                ui->labVoltage2->setText(QString::number(data.voltage));

            }
        }

        count++;
    }




}
void MainWindow::handleMotorControlDetached(int serialno)
{
    int count = 0;

    QMutableListIterator<PhidgetsMotorControl*> iterator(this->motorControls);

    while(iterator.hasNext()) {

        PhidgetsMotorControl* obj = iterator.next();

        if(obj->getSerialNo() == serialno)
        {

            if(count == 0)
            {
                ui->groupBoxMotor1->setDisabled(true);


            }
            else
            {
                ui->groupBoxMotor2->setDisabled(true);

            }


            iterator.remove();

            obj->deleteLater();
        }

        count++;

    }

}
/*void MainWindow::handleMotorCard(int serialno)
{
    PhidgetsMotorControl* motorControl = new PhidgetsMotorControl(this,serialno);

    this->motorControls.push_back(motorControl);

    motorControl->initMotorCard();
    this->handleMotorControlInit();

    //    QFuture<bool> future = QtConcurrent::run(motorControl,&PhidgetsMotorControl::initMotorCard);

    //    fwatcher.setFuture(future);

    //    connect(&fwatcher, SIGNAL(finished()), this, SLOT(handleMotorControlInit()));


}*/

MainWindow::~MainWindow()
{
    // this->motorControl->setMotorVelocityandAcceleration(0,0);
    delete ui;
}
void MainWindow::handleMotorControlInit()
{

    qDebug()<<"Motor Control Card Initialized";

    initView(this->motorControls.size());

}

void MainWindow::on_horSliderVel_valueChanged(int value)
{
    this->motorControls[0]->setMotorVelocityandAcceleration(value,ui->horSliderAcc->value());
    ui->lEditVel->setText(QString::number(ui->horSliderVel->value()));

}

void MainWindow::on_horSliderAcc_valueChanged(int value)
{
    this->motorControls[0]->setMotorVelocityandAcceleration(ui->horSliderVel->value(),value);
    ui->lEditAcc->setText(QString::number(ui->horSliderAcc->value()));

}
void MainWindow::initView(int count)
{
    ui->tabWidget->setEnabled(true);


    if(count == 1)
    {
        ui->horSliderAcc->setMaximum(100);
        ui->horSliderAcc->setMinimum(0);
        ui->horSliderAcc->setValue(50);

        ui->horSliderVel->setMaximum(100);
        ui->horSliderVel->setMinimum(0);
        ui->horSliderVel->setValue(0);

        ui->lEditAcc->setText(QString::number(ui->horSliderAcc->value()));
        ui->lEditVel->setText(QString::number(ui->horSliderVel->value()));
        ui->groupBoxMotor1->setEnabled(true);

        int serialno = this->motorControls[0]->getSerialNo();
        ui->labSerialNo1->setText(QString::number(serialno));

    }
    else if(count == 2){

        ui->horSliderAcc2->setMaximum(100);
        ui->horSliderAcc2->setMinimum(0);
        ui->horSliderAcc2->setValue(50);

        ui->horSliderVel2->setMaximum(100);
        ui->horSliderVel2->setMinimum(0);
        ui->horSliderVel2->setValue(0);



        ui->lEditAcc2->setText(QString::number(ui->horSliderAcc2->value()));
        ui->lEditVel2->setText(QString::number(ui->horSliderVel2->value()));

        int serialno = this->motorControls[1]->getSerialNo();
        ui->labSerialNo2->setText(QString::number(serialno));


        ui->groupBoxMotor2->setEnabled(true);


    }
}

void MainWindow::on_pushButtonStop1_clicked()
{

    this->motorControls[0]->setMotorVelocityandAcceleration(0,0);
    ui->horSliderAcc->setValue(50);
    ui->horSliderVel->setValue(0);

}

void MainWindow::on_horSliderVel2_valueChanged(int value)
{

    this->motorControls[1]->setMotorVelocityandAcceleration(value,ui->horSliderAcc2->value());
    ui->lEditVel2->setText(QString::number(ui->horSliderVel2->value()));

}

void MainWindow::on_horSliderAcc2_valueChanged(int value)
{

    this->motorControls[1]->setMotorVelocityandAcceleration(ui->horSliderVel2->value(),value);
    ui->lEditAcc2->setText(QString::number(ui->horSliderAcc2->value()));

}

void MainWindow::on_pushButtonStop2_clicked()
{

    this->motorControls[1]->setMotorVelocityandAcceleration(0,0);
    ui->horSliderAcc->setValue(50);
    ui->horSliderVel->setValue(0);


}

void MainWindow::on_pushButtoResetEnc2_clicked()
{
    this->motorControls[1]->resetEncoder();

}

void MainWindow::on_pushButtonResetEnc1_clicked()
{
    this->motorControls[0]->resetEncoder();

}

void MainWindow::on_actionMotors_triggered()
{
  if(this->motorcontroldialog){
    if(!this->motorcontroldialog->isVisible())
    {

        this->motorcontroldialog->show();

    }
  }
}
