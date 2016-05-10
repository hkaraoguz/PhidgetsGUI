#ifndef PHIDGETSMOTORCONTROL_H
#define PHIDGETSMOTORCONTROL_H

#include <QObject>
#include "phidget21.h"
#include <stdio.h>

struct MotorControlData
{
    int speed;
    double current;
    double voltage;
    int encoder;

};



class PhidgetsMotorControl : public QObject
{
    Q_OBJECT
public:
    explicit PhidgetsMotorControl(QObject *parent = 0, int serialNo=-1);
    ~PhidgetsMotorControl();

    bool initMotorCard();

    void setMotorVelocityandAcceleration(int vel, int acc);

    int getSerialNo();

    void resetEncoder();

    MotorControlData data;
private:
    //Declare a motor control handle
    CPhidgetMotorControlHandle motoControl;
    int serialno;




signals:
    void motorControlValues(MotorControlData);
    void motorControlDetached(int serialno);


public slots:
};




#endif // PHIDGETSMOTORCONTROL_H
