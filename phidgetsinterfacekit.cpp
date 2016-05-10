#include "phidgetsinterfacekit.h"


// - InterfaceKit simple -
// This simple example simply creates an InterfaceKit handle, hooks the event handlers and opens it.  It then waits
// for an InterfaceKit to be attached and waits for events to be fired. We progress through three steps, 1. Normal settings,
// 2. Setting analog sensor sensitivity to 100, 3. Toggling Ratiometric, waiting for user input to proceed to next step to allow
// data to be read.
//
// Copyright 2008 Phidgets Inc.  All rights reserved.
// This work is licensed under the Creative Commons Attribution 2.5 Canada License.
// view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/

#define IR_SENSOR_MIN_ANALOG_VALUE 70
#define IR_SENSOR_MAX_ANALOG_VALUE 1000
#define MAX_CMS_VAL 35.0



static int CCONV AttachHandler(CPhidgetHandle IFK, void *userptr)
{
    int serialNo;
    const char *name;

    CPhidget_getDeviceName(IFK, &name);
    CPhidget_getSerialNumber(IFK, &serialNo);

    printf("%s %10d attached!\n", name, serialNo);

    return 0;
}

static int CCONV DetachHandler(CPhidgetHandle IFK, void *userptr)
{
    int serialNo;
    const char *name;

    CPhidget_getDeviceName (IFK, &name);
    CPhidget_getSerialNumber(IFK, &serialNo);

    printf("%s %10d detached!\n", name, serialNo);

    PhidgetsInterfaceKit* obj = (PhidgetsInterfaceKit*)userptr;

    // The device is detached. Should emit signal
    emit obj->ifkitDetached();

    return 0;
}

static int CCONV ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
    printf("Error handled. %d - %s", ErrorCode, unknown);
    return 0;
}

//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
static int CCONV InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
    printf("Digital Input: %d > State: %d\n", Index, State);
    return 0;
}

//callback that will run if an output changes.
//Index - Index of the output that generated the event, State - boolean (0 or 1) representing the output state (on or off)
static int CCONV OutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
    printf("Digital Output: %d > State: %d\n", Index, State);
    return 0;
}

//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
static int CCONV SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
    printf("Sensor: %d > Value: %d\n", Index, Value);
    double cms = (double)2076/(Value - 11) + 4;
    printf("Sensor: %d > CMS: %f\n", Index, cms);
    return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
//Will also display the number of inputs, outputs, and analog inputs on the interface kit as well as the state of the ratiometric flag
//and the current analog sensor sensitivity.
static int display_properties(CPhidgetInterfaceKitHandle phid)
{
    int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
    const char* ptr;

    CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
    CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
    CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

    CPhidgetInterfaceKit_getInputCount(phid, &numInputs);
    CPhidgetInterfaceKit_getOutputCount(phid, &numOutputs);
    CPhidgetInterfaceKit_getSensorCount(phid, &numSensors);
    CPhidgetInterfaceKit_getRatiometric(phid, &ratiometric);

    printf("%s\n", ptr);
    printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
    printf("# Digital Inputs: %d\n# Digital Outputs: %d\n", numInputs, numOutputs);
    printf("# Sensors: %d\n", numSensors);
    printf("Ratiometric: %d\n", ratiometric);

    for(i = 0; i < numSensors; i++)
    {
        CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);

        printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
    }

    return 0;
}


bool PhidgetsInterfaceKit::initKit()
{
    int result,  i;
    const char *err;

    //Declare an InterfaceKit handle
   // ifKit = 0;

    //create the InterfaceKit object
    CPhidgetInterfaceKit_create(&ifKit);

    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, AttachHandler, this);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, DetachHandler, this);
    CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, ErrorHandler, this);

    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
  //  CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, InputChangeHandler, this);

    //Registers a callback that will run if the sensor value changes by more than the OnSensorChange trig-ger.
    //Requires the handle for the IntefaceKit, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
   // CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifKit, SensorChangeHandler, this);

    //Registers a callback that will run if an output changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    //CPhidgetInterfaceKit_set_OnOutputChange_Handler (ifKit, OutputChangeHandler, this);

    //open the interfacekit for device connections
    CPhidget_open((CPhidgetHandle)ifKit, this->serialno);

    //get the program to wait for an interface kit device to be attached
    printf("Waiting for interface kit to be attached....");
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
        return false;
    }

    //CPhidgetInterfaceKit_setOutputState(ifKit,0,0);



    //Display the properties of the attached interface kit device
    display_properties(ifKit);



    //get the number of sensors available
    CPhidgetInterfaceKit_getSensorCount(ifKit, &numSensors);

    CPhidgetInterfaceKit_getInputCount(ifKit, &numInputs);

    //Change the sensitivity trigger of the sensors
 /*   for(i = 0; i < numSensors; i++)
    {
        CPhidgetInterfaceKit_setSensorChangeTrigger(ifKit, i, 50);  //we'll just use 10 for fun



    }*/


    dataRefreshTimer = new QTimer(this);
    dataRefreshTimer->setInterval(10);
    connect(dataRefreshTimer,SIGNAL(timeout()),this,SLOT(handleDataRefreshTimeout()));
    dataRefreshTimer->start();

    //all done, exit
    return true;
}
void PhidgetsInterfaceKit::handleDataRefreshTimeout()
{
    QList<QVariant> distanceSensors;

    QList<QVariant> inputValues;


    for(int i = 0 ; i < numSensors; i++)
    {
        int sensorValue;
        CPhidgetInterfaceKit_getSensorValue(ifKit,i,&sensorValue);

        distanceSensors.append(QVariant::fromValue(this->calculateAnalogtoDistance(sensorValue)));
    }

    for(int i = 0; i < numInputs ; i++ )
    {
        int inputValue;
        CPhidgetInterfaceKit_getInputState(ifKit,i,&inputValue);
        inputValues.append(QVariant::fromValue(inputValue));

    }

    QList<QList<QVariant> > result;

    result.append(distanceSensors);
    result.append(inputValues);

    emit sensorAndInputReadings(result);


}

double PhidgetsInterfaceKit::calculateAnalogtoDistance(int analogvalue)
{
   double cms = MAX_CMS_VAL;

   if(analogvalue > IR_SENSOR_MIN_ANALOG_VALUE && analogvalue < IR_SENSOR_MAX_ANALOG_VALUE)
   {
       cms = (double)2076/(analogvalue - 11) + 4;

   }

   return cms;

}
QList<int> PhidgetsInterfaceKit::getDigitalOutStates()
{
    QList<int> result;

    for(int i = 0; i < this->numSensors; i++)
    {
        int state;
        CPhidgetInterfaceKit_getOutputState(this->ifKit,i,&state);
        result.append(state);
    }

    return result;
}



PhidgetsInterfaceKit::PhidgetsInterfaceKit(QObject *parent, int serialNo) : QObject(parent)
{
   ifKit = 0;
   numSensors = 0;
   numInputs = 0;
   serialno = serialNo;

}
PhidgetsInterfaceKit::~PhidgetsInterfaceKit()
{
    CPhidget_close((CPhidgetHandle)ifKit);
    CPhidget_delete((CPhidgetHandle)ifKit);
}

void PhidgetsInterfaceKit::handleDigitalOutputToggle(QList<int> values)
{
    CPhidgetInterfaceKit_setOutputState(this->ifKit,values[0],values[1]);

}
