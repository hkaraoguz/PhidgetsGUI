#include "phidgetsdevicemanager.h"
#include <stdio.h>


// This error handler can handle any CPhidget function that returns an int
static int LocalErrorCatcher (int errorCode) {

    const char *errorDescription;

    // If the error code is 0, everything is okay
    if (errorCode != 0) {

        // Otherwise, you can print specific messages or perform actions by error value.
        switch (errorCode) {
           default:
               printf("Error: An error occurred with code %d.\n", errorCode);

               LocalErrorCatcher(
                   CPhidget_getErrorDescription (errorCode, &errorDescription));
               printf("The description for this error is: %s\n", errorDescription);
               break;
       }
    }
    return 0;
}




// -------------------- Event Functions ---------------------------------------

static int CCONV AttachHandler (CPhidgetHandle device, void *userptr) {

    int serialNumber;
    const char *name;

    LocalErrorCatcher(
        CPhidget_getDeviceName(device, &name));
    LocalErrorCatcher(
        CPhidget_getSerialNumber(device, &serialNumber));

    PhidgetsDeviceManager* obj = (PhidgetsDeviceManager*)userptr;

    QString devname(name);


    // If the device name contains motor control in its name then emit a motor control card signal
    if(devname.contains("Motor Control"))
    {
        obj->pushToMotorControlCardSerials(serialNumber);
       // emit obj->motorCard(serialNumber);

    }
    else if(devname.contains("InterfaceKit"))
    {
        obj->setInterfaceKitSerial(serialNumber);
    }



    printf("Hello Device %s, Serial Number: %d\n", name, serialNumber);

    return 0;
}

static int CCONV DetachHandler (CPhidgetHandle device, void *userptr) {

    int serialNumber;
    const char *name;

    LocalErrorCatcher(
        CPhidget_getDeviceName(device, &name));
    LocalErrorCatcher(
        CPhidget_getSerialNumber(device, &serialNumber));

    //printf("Goodbye Device %s, Serial Number: %d\n", name, serialNumber);

    return 0;
}

// When using an error handler with the manager, it takes a
// CPhidgetManagerHandle, when using an individual object, the
// object serves as its own handle.
static int CCONV LibraryErrorHandler (CPhidgetManagerHandle device, void *usrptr,
                    int errorCode, const char *errorDescription) {
  //  printf("Error Event: %d - %s\n", errorCode, errorDescription);
    return 0;
}





PhidgetsDeviceManager::PhidgetsDeviceManager(QObject *parent) : QObject(parent)
{
    manager = 0;

}
PhidgetsDeviceManager::~PhidgetsDeviceManager()
{
    CPhidgetManager_close(manager);
    CPhidgetManager_delete(manager);

}
void PhidgetsDeviceManager::setInterfaceKitSerial(int serial)
{
    this->interfaceKitSerial = serial;
}
int PhidgetsDeviceManager::getInterfaceKitSerial()
{
    return this->interfaceKitSerial;
}
QList<int> PhidgetsDeviceManager::getMotorControlCardSerials()
{
    qSort(this->motorControlCardSerials);

    return motorControlCardSerials;

}
void PhidgetsDeviceManager::pushToMotorControlCardSerials(int serial)
{
    this->motorControlCardSerials.push_back(serial);

}
int PhidgetsDeviceManager::initDevices()
{

      int res = LocalErrorCatcher(CPhidgetManager_create(&manager));

      if(res != 0) return res;


     res =  LocalErrorCatcher(
           CPhidgetManager_set_OnAttach_Handler((CPhidgetManagerHandle) manager,
                                            AttachHandler, this));

     if(res != 0) return res;

     res =  LocalErrorCatcher(
           CPhidgetManager_set_OnDetach_Handler((CPhidgetManagerHandle ) manager,
                                            DetachHandler, this));

     if(res != 0) return res;

     res =  LocalErrorCatcher(
           CPhidgetManager_set_OnError_Handler((CPhidgetManagerHandle) manager,
                                           LibraryErrorHandler, this));

     if(res != 0) return res;

       // Most opening and closing would be via a cast to
       // (CPhidgetHandle), however, this manager has its
       // own handle struct to cast to.
     res=  LocalErrorCatcher(
           CPhidgetManager_open((CPhidgetManagerHandle) manager));



     return res;

}
