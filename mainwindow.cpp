#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "motorcontroldialog.h"
#include "digitaliodialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    // Create the Sensor Graphics scene
    scene = new IRSensorGraphicsScene;

    // Set the scene of graphics View as scene
    ui->graphicsViewIR->setScene(scene);

 /******* Create the IR Labels vector to easily change the label with index ******/
    this->irlabels.push_back(ui->labelIR1Dist);
    this->irlabels.push_back(ui->labelIR2Dist);
    this->irlabels.push_back(ui->labelIR3Dist);
    this->irlabels.push_back(ui->labelIR4Dist);
    this->irlabels.push_back(ui->labelIR5Dist);
    this->irlabels.push_back(ui->labelIR6Dist);
    this->irlabels.push_back(ui->labelIR7Dist);
    this->irlabels.push_back(ui->labelIR8Dist);
  /********************************************************************************/

    // This is the main device manager
    deviceManager = new PhidgetsDeviceManager(this);


    // Init devices-> This is already using threads so we don't need to generate a thread for this
    deviceManager->initDevices();

    // Waiting for device initializations to be completed
    QTimer::singleShot(5000,this,SLOT(handleDeviceManagerInit()));



}
MainWindow::~MainWindow()
{
    // this->motorControl->setMotorVelocityandAcceleration(0,0);
    delete ui;
}
// Now the devices are initialized
void MainWindow::handleDeviceManagerInit()
{


    qDebug()<<"Devices should be initialized";



    /// Get the serial numbers of motor cards -> Currently a maximum of 2 cards are supported
    QList<int> motorcardserials = this->deviceManager->getMotorControlCardSerials();


    motorcontroldialog = new MotorControlDialog(this);

  //  motorcontroldialog->show();

    foreach (int serial, motorcardserials)
    {
        PhidgetsMotorControl* motorControl = new PhidgetsMotorControl(this,serial);


        if(motorControl->initMotorCard())
        {
          //  this->motorControls.push_back(motorControl);

            connect(motorControl,SIGNAL(motorControlDetached(int)), this->motorcontroldialog, SLOT(handleMotorControlDetached(int)));

            connect(motorControl,SIGNAL(motorControlValues(MotorControlData)),this->motorcontroldialog,SLOT(handleMotorControlValues(MotorControlData)));

            this->motorcontroldialog->appendToMotorControls(motorControl);

            this->motorcontroldialog->initView();

        }


    }

    if(this->motorcontroldialog->getMotorControls().size() > 0)
    {
        this->motorcontroldialog->show();
    }


    // Get the serial number of the interface kit
    int interfaceserial = this->deviceManager->getInterfaceKitSerial();


    if(interfaceserial > 0)
    {
        interfacekit = new PhidgetsInterfaceKit(this, interfaceserial);
        digitaliodialog = new DigitalIODialog(this);
        if(interfacekit->initKit())
        {
            connect(interfacekit,SIGNAL(sensorAndInputReadings(QList<QList< QVariant > >)),this,SLOT(handleInterfaceSensorandInputReadings(QList<QList< QVariant > >)));
            connect(interfacekit,SIGNAL(sensorAndInputReadings(QList<QList< QVariant > >)),this->digitaliodialog,SLOT(handleInterfaceSensorandInputReadings(QList<QList< QVariant > >)));

            connect(this->digitaliodialog,SIGNAL(digitalInputToggle(QList<int>)),interfacekit,SLOT(handleDigitalOutputToggle(QList<int>)));

            this->digitaliodialog->show();
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

    int count = 0;

    foreach (QVariant dist, distanceReadings) {

       // qDebug()<<"I am here"<<dist;
        this->irlabels[count]->setText(QString::number(dist.toDouble()));
        count +=1;
        doubledistances.append(dist.toDouble());
        this->scene->drawDistanceSensors(doubledistances);
        ui->graphicsViewIR->update();

    }



}
void MainWindow::handleMotorControlValues(MotorControlData data)
{
 /*   PhidgetsMotorControl* obj = qobject_cast<PhidgetsMotorControl*>(sender());

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


*/

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


void MainWindow::handleMotorControlInit()
{

    qDebug()<<"Motor Control Card Initialized";

  //  initView(this->motorControls.size());

}


void MainWindow::initView(int count)
{
    ui->tabWidget->setEnabled(true);


 /*   if(count == 1)
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


    }*/
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

void MainWindow::on_actionDigital_I_O_triggered()
{
    if(this->digitaliodialog)
    {
        if(!this->digitaliodialog->isVisible())
        {
            this->digitaliodialog->show();
        }
    }

}
