#include "motorcontroldialog.h"
#include "ui_motorcontroldialog.h"

MotorControlDialog::MotorControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotorControlDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Motor Control Dialog");
}

MotorControlDialog::~MotorControlDialog()
{
    delete ui;
}
void MotorControlDialog::appendToMotorControls(PhidgetsMotorControl *motorControl)
{
    this->motorControls.append(motorControl);

}

void MotorControlDialog::on_horSliderVel_valueChanged(int value)
{
    this->motorControls[0]->setMotorVelocityandAcceleration(value,ui->horSliderAcc->value());
    ui->lEditVel->setText(QString::number(ui->horSliderVel->value()));

    if(ui->checkBoxMotorsSync->isChecked() && ui->groupBoxMotor2->isEnabled())
    {
        this->motorControls[1]->setMotorVelocityandAcceleration(value,ui->horSliderAcc->value());
        ui->lEditVel2->setText(QString::number(ui->horSliderVel->value()));
        ui->horSliderVel2->setValue(ui->horSliderVel->value());

    }


}

void MotorControlDialog::on_horSliderAcc_valueChanged(int value)
{

    this->motorControls[0]->setMotorVelocityandAcceleration(ui->horSliderVel->value(),value);
    ui->lEditAcc->setText(QString::number(ui->horSliderAcc->value()));

    if(ui->checkBoxMotorsSync->isChecked() && ui->groupBoxMotor2->isEnabled())
    {
        this->motorControls[1]->setMotorVelocityandAcceleration(ui->horSliderVel->value(),value);
        ui->lEditAcc2->setText(QString::number(ui->horSliderAcc->value()));
        ui->horSliderAcc2->setValue(ui->horSliderAcc->value());
    }

}

void MotorControlDialog::on_pushButtonResetEnc1_clicked()
{

    this->motorControls[0]->resetEncoder();

}

void MotorControlDialog::on_pushButtonStop1_clicked()
{

    this->motorControls[0]->setMotorVelocityandAcceleration(0,0);
    ui->horSliderAcc->setValue(50);
    ui->horSliderVel->setValue(0);

    if(ui->checkBoxMotorsSync->isChecked() && ui->groupBoxMotor2->isEnabled())
    {
        this->motorControls[1]->setMotorVelocityandAcceleration(0,0);
        ui->horSliderAcc2->setValue(50);
        ui->horSliderVel2->setValue(0);
    }

}

void MotorControlDialog::on_pushButtoResetEnc2_clicked()
{

    this->motorControls[1]->resetEncoder();


}

void MotorControlDialog::on_pushButtonStop2_clicked()
{
    this->motorControls[1]->setMotorVelocityandAcceleration(0,0);
    ui->horSliderAcc2->setValue(50);
    ui->horSliderVel2->setValue(0);

    if(ui->checkBoxMotorsSync->isChecked() && ui->groupBoxMotor1->isEnabled())
    {
        this->motorControls[0]->setMotorVelocityandAcceleration(0,0);
        ui->horSliderAcc->setValue(50);
        ui->horSliderVel->setValue(0);

    }

}

void MotorControlDialog::on_horSliderVel2_valueChanged(int value)
{
    this->motorControls[1]->setMotorVelocityandAcceleration(value,ui->horSliderAcc2->value());
    ui->lEditVel2->setText(QString::number(ui->horSliderVel2->value()));

    if(ui->checkBoxMotorsSync->isChecked() && ui->groupBoxMotor1->isEnabled())
    {
        this->motorControls[0]->setMotorVelocityandAcceleration(value,ui->horSliderAcc2->value());
        ui->lEditVel->setText(QString::number(ui->horSliderVel2->value()));
        ui->horSliderVel->setValue(ui->horSliderVel2->value());
    }

}

void MotorControlDialog::on_horSliderAcc2_valueChanged(int value)
{

    this->motorControls[1]->setMotorVelocityandAcceleration(ui->horSliderVel2->value(),value);
    ui->lEditAcc2->setText(QString::number(ui->horSliderAcc2->value()));

    if(ui->checkBoxMotorsSync->isChecked() && ui->groupBoxMotor1->isEnabled())
    {

        this->motorControls[0]->setMotorVelocityandAcceleration(ui->horSliderVel2->value(),value);
        ui->lEditAcc->setText(QString::number(ui->horSliderAcc2->value()));
        ui->horSliderAcc->setValue(ui->horSliderAcc2->value());
    }

}
void MotorControlDialog::handleMotorControlValues(MotorControlData data)
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
void MotorControlDialog::handleMotorControlDetached(int serialno)
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

void MotorControlDialog::initView()
{
    ui->tabWidget->setEnabled(true);


    if(this->motorControls.size() == 1)
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
    else if(this->motorControls.size() == 2){

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
