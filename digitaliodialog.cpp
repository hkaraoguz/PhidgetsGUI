#include "digitaliodialog.h"
#include "ui_digitaliodialog.h"

DigitalIODialog::DigitalIODialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DigitalIODialog)
{
    ui->setupUi(this);

    this->inputlabels.append(ui->labelDigitalIn1);
    this->inputlabels.append(ui->labelDigitalIn2);
    this->inputlabels.append(ui->labelDigitalIn3);
    this->inputlabels.append(ui->labelDigitalIn4);
    this->inputlabels.append(ui->labelDigitalIn5);
    this->inputlabels.append(ui->labelDigitalIn6);
    this->inputlabels.append(ui->labelDigitalIn7);
    this->inputlabels.append(ui->labelDigitalIn8);

    ui->comboBoxDigitalOut1->insertItem(0,"0");
    ui->comboBoxDigitalOut1->insertItem(1,"1");

    ui->comboBoxDigitalOut2->insertItem(0,"0");
    ui->comboBoxDigitalOut2->insertItem(1,"1");

    ui->comboBoxDigitalOut3->insertItem(0,"0");
    ui->comboBoxDigitalOut3->insertItem(1,"1");

    ui->comboBoxDigitalOut4->insertItem(0,"0");
    ui->comboBoxDigitalOut4->insertItem(1,"1");

    ui->comboBoxDigitalOut5->insertItem(0,"0");
    ui->comboBoxDigitalOut5->insertItem(1,"1");

    ui->comboBoxDigitalOut6->insertItem(0,"0");
    ui->comboBoxDigitalOut6->insertItem(1,"1");

    ui->comboBoxDigitalOut7->insertItem(0,"0");
    ui->comboBoxDigitalOut7->insertItem(1,"1");

    ui->comboBoxDigitalOut8->insertItem(0,"0");
    ui->comboBoxDigitalOut8->insertItem(1,"1");
}

DigitalIODialog::~DigitalIODialog()
{
    delete ui;
}
void DigitalIODialog::handleInterfaceSensorandInputReadings(QList<QList<QVariant> > readings)
{

    QList<QVariant> digitalin = readings[1];

    int count = 0;
    foreach (QVariant adigitalin, digitalin)
    {
        inputlabels[count]->setText(QString::number(adigitalin.toInt()));
        count +=1;

    }


}

void DigitalIODialog::on_comboBoxDigitalOut1_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(0);
    values.append(output);

    emit digitalInputToggle(values);

}

void DigitalIODialog::on_comboBoxDigitalOut2_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(1);
    values.append(output);

    emit digitalInputToggle(values);

}

void DigitalIODialog::on_comboBoxDigitalOut3_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(2);
    values.append(output);

    emit digitalInputToggle(values);

}

void DigitalIODialog::on_comboBoxDigitalOut4_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(3);
    values.append(output);

    emit digitalInputToggle(values);

}

void DigitalIODialog::on_comboBoxDigitalOut5_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(4);
    values.append(output);

    emit digitalInputToggle(values);

}

void DigitalIODialog::on_comboBoxDigitalOut6_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(5);
    values.append(output);

    emit digitalInputToggle(values);

}

void DigitalIODialog::on_comboBoxDigitalOut7_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(6);
    values.append(output);

    emit digitalInputToggle(values);

}

void DigitalIODialog::on_comboBoxDigitalOut8_currentIndexChanged(int index)
{
    QList<int> values;
    int output = 0;

    if(index == 1)
        output=1;

    values.append(7);
    values.append(output);

    emit digitalInputToggle(values);

}
