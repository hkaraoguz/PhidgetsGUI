#ifndef PHIDGETSINTERFACEKIT_H
#define PHIDGETSINTERFACEKIT_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QTimer>
#include <phidget21.h>
#include <stdio.h>

class PhidgetsInterfaceKit : public QObject
{
    Q_OBJECT
public:
    explicit PhidgetsInterfaceKit(QObject *parent = 0, int serialNo = -1);
    ~PhidgetsInterfaceKit();
    bool initKit();
    static double calculateAnalogtoDistance(int analogvalue);
private:
    CPhidgetInterfaceKitHandle ifKit;
    int serialno;
    QTimer* dataRefreshTimer;
    int numSensors;
    int numInputs;



signals:
    void ifkitDetached();
    void sensorAndInputReadings(QList<QList<QVariant> > readings);
    //void digitalInputChanged(QList<int> indexandvalue);

public slots:

private slots:
    void handleDataRefreshTimeout();
};

#endif // PHIDGETSINTERFACEKIT_H
