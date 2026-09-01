#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

const double deltaTime = 1.00/60.00;
const double airDensity = 1.225;
//const double frictionCoefficient = 1.03;

struct DriverInput
{
    bool gasPressed = false;
    double throttle = 0.0;
    bool right = false;
    bool left = false;
    bool brakePressed = false;
    double brake = 0.0;
    double steer = 0.0;
};

struct VehicleState
{
   double positionX = 0.0;
   double positionY = 0.0;
   double heading = 0.0;
   double longitudinalVelocity = 0.0;
   double lateralVelocity = 0.0;
   double yawRate = 0.0;
   double dragForce = 0.0;
   double downForce = 0.0;
};

struct VehicleParameters
{
    double mass = 0.0;
    double wheelbase = 0.0;
    //double frontAxleDistance = 0.0;
    //double rearAxleDistance = 0.0;
    double yawInertia = 0.0;
    double downForceCoefficient = 0.0;
    double dragCoefficient = 0.0;
    //double liftCoefficient = 0.0;
    double frontalArea = 0.0;
    double liftSurfaceArea = 0.0;
    double frontTrackWidth = 0.0;
    double rearTrackWidth = 0.0;
    double gravityCenterOffset = 0.0;  //positive assumes center of gravity is ahead of the center of the car
};

double cubicSplineInterpolator(double dataArray[10][10], int dataAmount, double entry)   //The 10's in the array parameters are simply temporary placeholders.
{
    /*This function will take data such as tire cornering stiffness at different slip angles and log it into a data array. The data array will store corresponding x values and y values along with the slope values based off of neighboring points.
    The function will use the 3 columns of info to interpolate cubic splines in between points. While this function is initially meant for tire values, the hope is to make it general enough for other interpolation needs such as torque at different RPMs.
    */
    
    
    double M[dataAmount] = {0};  //this will be dataAmount one a dataAmount can be read from an external txt file.
    dataArray[0][2] = 0;
    double output;
    
    //To figure out the constants for the slope equations
    for (int i=1; i<dataAmount; ++i)
        M[i] = 6 * ((dataArray[i + 1][1] - dataArray[i][1]) - (dataArray[i][1] - dataArray[i-1][1])); 
    
    
    int M1coefficient = 1;
    int constant =  M[1];
    int M1Prev = 0;
    int constantPrev = 0;
    
    
    //To find M1
    for (int i=1; i<dataAmount; ++i)
    {
        M1coefficient += (M1coefficient * -4) + M1Prev;
        constant += (constant * -4) + constantPrev;
        M1Prev = M1coefficient;
        constantPrev = constant;
    }
    double M1 = constant/M1coefficient;
    
    dataArray[0][2] = 0;
    dataArray[1][2] = M1;
    
    //To figure out M values
    
    for (int i=2; i<dataAmount; ++i)
        dataArray[i][2] = M[i] - ((4*dataArray[i-1][2]) + dataArray[i-2][2]); 
    
    
    for (int i=0; i<dataAmount; ++i)
    {
        if (dataArray[entry][0] == dataArray[i][0])
            output = dataArray[entry][1];
        else
        {
            if (dataArray[entry][0] > dataArray[i][0] && dataArray[entry][0] < dataArray[i+1][0])
                {
                    double a,b,c,d;
                    d = dataArray[entry][1];
                    a = (dataArray[entry+1][2] - dataArray[entry][2])/6;
                    b = dataArray[entry][2]/2;
                    c = (dataArray[entry+1][1]-dataArray[entry][1]) - (((2*dataArray[entry][2]) + dataArray[entry+1][2])/6);
          
                 output = (a*pow(entry,3) + (b*pow(entry,2)) + (c*entry) + d);    
                }
        }
    }
    return output;    
}

void leftTurnSlipAngles (const VehicleParameters& currentCar, VehicleState& car)
{
    double velocityFrontLeftX = car.longitudinalVelocity - (car.yawRate * (currentCar.frontTrackWidth/2));
    double velocityFrontLeftY = car.lateralVelocity + (car.yawRate * ((currentCar.wheelbase / 2) - currentCar.gravityCenterOffset));
    
    double velocityFrontRightX = car.longitudinalVelocity + (car.yawRate * (currentCar.frontTrackWidth/2));
    double velocityFrontRightY = car.lateralVelocity + (car.yawRate * ((currentCar.wheelbase / 2) - currentCar.gravityCenterOffset));
    
    double velocityRearLeftX = car.longitudinalVelocity - (car.yawRate * (currentCar.rearTrackWidth/2));
    double velocityRearLeftY = car.lateralVelocity - (car.yawRate * ((currentCar.wheelbase / 2) + currentCar.gravityCenterOffset));
    
    double velocityRearRightX = car.longitudinalVelocity + (car.yawRate * (currentCar.rearTrackWidth/2));
    double velocityRearRightY = car.lateralVelocity - (car.yawRate * ((currentCar.wheelbase / 2) + currentCar.gravityCenterOffset));
}

/*
void rightTurnSlipAngles (const VehicleParameters& currentCar, VehicleState& car)
{
    double velocityFrontLeftX = car.longitudinalVelocity + (car.yawRate * (currentCar.frontTrackWidth/2));
    double velocityFrontLeftY = car.lateralVelocity + (car.yawRate * ((currentCar.wheelbase / 2) - currentCar.gravityCenterOffset));
    
    double velocityFrontRightX = car.longitudinalVelocity - (car.yawRate * (currentCar.frontTrackWidth/2));
    double velocityFrontRightY = car.lateralVelocity + (car.yawRate * ((currentCar.wheelbase / 2) - currentCar.gravityCenterOffset));
    
    double velocityRearLeftX = car.longitudinalVelocity + (car.yawRate * (currentCar.rearTrackWidth/2));
    double velocityRearLeftY = car.lateralVelocity - (car.yawRate * ((currentCar.wheelbase / 2) + currentCar.gravityCenterOffset));
    
    double velocityRearRightX = car.longitudinalVelocity - (car.yawRate * (currentCar.rearTrackWidth/2));
    double velocityRearRightY = car.lateralVelocity - (car.yawRate * ((currentCar.wheelbase / 2) + currentCar.gravityCenterOffset));
}
*/

void aerodynamicEffect (const VehicleParameters& currentCar, VehicleState& car)
{
    car.dragForce = 0.5 * airDensity * currentCar.dragCoefficient * currentCar.frontalArea * pow(sqrt((pow(car.lateralVelocity , 2)) + (pow(car.longitudinalVelocity , 2))) , 2);
    car.downForce = 0.5 * airDensity * currentCar.downForceCoefficient * currentCar.liftSurfaceArea * pow(sqrt((pow(car.lateralVelocity , 2)) + (pow(car.longitudinalVelocity , 2))) , 2);
    
}

void calculateThrottle (DriverInput& control)    //Gradual keyboard throttle application
{
    const double throttleApplicationRate = 2.0;
    const double throttleReleaseRate = 3.0;
    //double throttle = 0;

    if (control.gasPressed)
        control.throttle += throttleApplicationRate * deltaTime;
    
    else
       control.throttle -= throttleReleaseRate * deltaTime; 
    /*   
    if (control.throttle > 1.0)
        control.throttle = 1.0;

    if (control.throttle < 0.0)
        control.throttle = 0.0;
    */
      control.throttle = std::clamp(control.throttle, 0.0, 1.0);
      
}

void accelerate(DriverInput& control, VehicleState& car)
{
      const double accelerationRate = 5.0;
      car.lateralVelocity += accelerationRate * control.throttle * deltaTime;
}

void updatePostition(VehicleState& car)
{
    car.positionX += car.lateralVelocity * deltaTime;
}

/*
void brake()
{
    while ()
    {
        
    }
}
*/
void steer(DriverInput& control, VehicleState& car)
{
    double steeringApplicationRate = 2.0;
    double steeringAngle;
    /*
    if (control.left && (steeringAngle <= 90.0))
    {
        steeringAngle += 
    }
    */
}


int main()
{

    DriverInput control;
    VehicleState car;
    VehicleParameters currentCar;
    
    bool running = true;
    /*
    while (running)
    {
        //readInput(control)
        
        calculateThrottle(control);
        
        accelerate(control, car);
        
        //updatePostition(car);
        
        //drawVehicle(car);
        
        
    }
    */
    for (int i = 0; i < 60; i++)
    {
        control.gasPressed = true;
    
        calculateThrottle(control);
        accelerate(control, car);
    }

    cout << "Throttle: " << control.throttle << endl;
    cout << "Velocity: " << car.lateralVelocity << endl;
    
    return 0;
}