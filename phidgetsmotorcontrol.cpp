#include "phidgetsmotorcontrol.h"



static int CCONV AttachHandler(CPhidgetHandle MC, void *userptr);
static int CCONV DetachHandler(CPhidgetHandle MC, void *userptr);
static int CCONV ErrorHandler(CPhidgetHandle MC, void *userptr, int ErrorCode, const char *Description);
static int CCONV InputChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int State);
static int CCONV VelocityChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value);
static int CCONV CurrentChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value);
static int CCONV PositionChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int Time, int positionChange);
static int CCONV  PositionUpdateHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int positionChange);
static int display_properties(CPhidgetMotorControlHandle phid);


PhidgetsMotorControl::PhidgetsMotorControl(QObject *parent, int serialNo) : QObject(parent)
{

    //Declare a motor control handle
    motoControl = 0;
    this->serialno = serialNo;



}
int PhidgetsMotorControl::getSerialNo()
{
    return this->serialno;

}
PhidgetsMotorControl::~PhidgetsMotorControl()
{

    this->setMotorVelocityandAcceleration(0,0);
    CPhidget_close((CPhidgetHandle)motoControl);
    CPhidget_delete((CPhidgetHandle)motoControl);

}

bool PhidgetsMotorControl::initMotorCard()
{
    int result;
    const char *err;



    //create the motor control object
    CPhidgetMotorControl_create(&motoControl);

    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)motoControl, AttachHandler, this);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)motoControl, DetachHandler, this);
    CPhidget_set_OnError_Handler((CPhidgetHandle)motoControl, ErrorHandler, this);

    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnInputChange_Handler (motoControl, InputChangeHandler, this);

    //Registers a callback that will run if a motor changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnVelocityChange_Handler (motoControl, VelocityChangeHandler, this);

    //Registers a callback that will run if the current draw changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnCurrentChange_Handler (motoControl, CurrentChangeHandler, this);


  //  CPhidgetMotorControl_set_OnEncoderPositionChange_Handler(motoControl,PositionChangeHandler,this);

    CPhidgetMotorControl_set_OnEncoderPositionUpdate_Handler(motoControl,PositionUpdateHandler,this);

    //open the motor control for device connections
    CPhidget_open((CPhidgetHandle)motoControl, this->serialno);

    //get the program to wait for a motor control device to be attached
   // printf("Waiting for MotorControl to be attached....");
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)motoControl, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
        return false;
        //return 0;
    }

    //Display the properties of the attached motor control device
    display_properties(motoControl);

    return true;

}
void PhidgetsMotorControl::setMotorVelocityandAcceleration(int vel, int acc)
{
    CPhidgetMotorControl_setAcceleration (motoControl, 0, (double)acc);
    CPhidgetMotorControl_setVelocity (motoControl, 0, (double)vel);

}
void PhidgetsMotorControl::resetEncoder()
{
    CPhidgetMotorControl_setEncoderPosition(this->motoControl,0,0);

}


static int CCONV AttachHandler(CPhidgetHandle MC, void *userptr)
{
    int serialNo;
    const char *name;

    CPhidget_getDeviceName (MC, &name);
    CPhidget_getSerialNumber(MC, &serialNo);

    printf("%s %10d attached!\n", name, serialNo);



    return 0;
}

static int CCONV DetachHandler(CPhidgetHandle MC, void *userptr)
{
    int serialNo;
    const char *name;

    CPhidget_getDeviceName (MC, &name);
    CPhidget_getSerialNumber(MC, &serialNo);
    printf("%s %10d detached!\n", name, serialNo);

    PhidgetsMotorControl*obj = (PhidgetsMotorControl*)userptr;

    emit obj->motorControlDetached(obj->getSerialNo());

    return 0;
}

static int CCONV ErrorHandler(CPhidgetHandle MC, void *userptr, int ErrorCode, const char *Description)
{
    PhidgetsMotorControl* obj = (PhidgetsMotorControl*)userptr;

    printf("Error handled for motor card with serial No: %d. %d - %s\n", obj->getSerialNo(),ErrorCode, Description);
    return 0;
}


static int CCONV InputChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int State)
{
    printf("Input %d > State: %d\n", Index, State);
    return 0;
}

static int CCONV VelocityChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
   // printf("Motor %d > Current Speed: %f\n", Index, Value);

    PhidgetsMotorControl* obj = (PhidgetsMotorControl*)usrptr;


    obj->data.speed = Value;


    return 0;
}

int CCONV CurrentChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
    PhidgetsMotorControl* obj = (PhidgetsMotorControl*)usrptr;

    obj->data.current = Value;

    emit obj->motorControlValues(obj->data);

  //  printf("Motor: %d > Current Draw: %f\n", Index, Value);
    return 0;
}
int CCONV PositionChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int Time, int positionChange)
{
    int position;
    CPhidgetMotorControl_getEncoderPosition(MC,0,&position);

    return 0;

   // printf("Encoder %d %d %d\n",Index,Time,position);

}

int CCONV  PositionUpdateHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int positionChange)
{
    int position;
    double voltage;
    CPhidgetMotorControl_getEncoderPosition(MC,0,&position);
    CPhidgetMotorControl_getSupplyVoltage(MC,&voltage);

    PhidgetsMotorControl* obj = (PhidgetsMotorControl*)usrptr;

    obj->data.encoder = position;
    obj->data.voltage = voltage;

    emit obj->motorControlValues(obj->data);

    return 0;

   // printf("Encoder %d %d \n",Index,position);

}

static int display_properties(CPhidgetMotorControlHandle phid)
{
    int serialNo, version, numInputs, numMotors;
    const char* ptr;

    CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
    CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
    CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

    CPhidgetMotorControl_getInputCount(phid, &numInputs);
    CPhidgetMotorControl_getMotorCount(phid, &numMotors);

    printf("%s\n", ptr);
    printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
    printf("# Inputs: %d\n# Motors: %d\n", numInputs, numMotors);

    return 0;
}

