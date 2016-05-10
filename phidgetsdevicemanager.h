#ifndef PHIDGETSDEVICEMANAGER_H
#define PHIDGETSDEVICEMANAGER_H

#include <QObject>
#include <phidget21.h>

class PhidgetsDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit PhidgetsDeviceManager(QObject *parent = 0);
    ~PhidgetsDeviceManager();

    int initDevices();

    QList<int> getMotorControlCardSerials();

    int getInterfaceKitSerial();

    void pushToMotorControlCardSerials(int serial);

    void setInterfaceKitSerial(int serial);

private:
    CPhidgetManagerHandle manager;
    QList<int> motorControlCardSerials;
    int interfaceKitSerial;


signals:
    void motorCard(int serialno);

public slots:
};

#endif // PHIDGETSDEVICEMANAGER_H
