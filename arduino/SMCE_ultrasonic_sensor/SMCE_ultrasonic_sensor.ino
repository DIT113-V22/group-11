#include <Smartcar.h>
#include <iostream>



const int fSpeed   = 70;  // 70% of the full speed forward
const int bSpeed   = -70; // 70% of the full speed backward
const int lDegrees = -75; // degrees to turn left
const int rDegrees = 75;  // degrees to turn right

ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor{arduinoRuntime, smartcarlib::pins::v2::leftMotorPins};
BrushedMotor rightMotor{arduinoRuntime, smartcarlib::pins::v2::rightMotorPins};
DifferentialControl control{leftMotor, rightMotor};

SimpleCar car(control);

const int triggerPin           = 6; // D6
const int echoPin              = 7; // D7
const unsigned int maxDistance = 100;

SR04 left(arduinoRuntime, 2, 3, maxDistance); // trigger and echo pin respectively
SR04 right(arduinoRuntime, 4, 5, maxDistance);
SR04 front(arduinoRuntime, 6, 7, maxDistance);
SR04 back(arduinoRuntime, 16, 17, maxDistance);


void setup()
{
  // Move the car with 50% of its full speed
  car.setSpeed(0);
  
    Serial.begin(9600);

}



void loop()
{
  const auto fdistance = front.getDistance();
  const auto bdistance = back.getDistance();
  const auto rdistance = right.getDistance();
  const auto ldistance = left.getDistance();
  // When distance is `0` it means there's no obstacle detected


    
/*if (distance > 0 && distance < 150) {
  insideRange=true;
  while(insideRange){
    car.setSpeed(bSpeed);
    car.setAngle(0);
    while (distance > 149) {
        insideRange=false;
      }
  }
       car.setSpeed(0);
    }*/


if (fdistance > 0 && fdistance < 150) {

    car.setSpeed(-100);
    delay(1000);
   car.setSpeed(0);

}

    if (bdistance > 0 && bdistance < 150) {

        car.setSpeed(100);
        delay(1000);
        car.setSpeed(0);

    }

handleInput();
  
    

 

#ifdef __SMCE__
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);

#endif


}

void handleInput()
{ // handle serial input if there is any
    if (Serial.available())
    {
        char input = Serial.read(); // read everything that has been received so far and log down
                                    // the last entry
                                    
        switch (input)
        {
        case 'a': // rotate counter-clockwise going forward
            car.setSpeed(fSpeed);
            car.setAngle(lDegrees);
            break;
        case 'd': // turn clock-wise
            car.setSpeed(fSpeed);
            car.setAngle(rDegrees);
            break;
        case 'w': // go ahead
            car.setSpeed(fSpeed);
            car.setAngle(0);
            break;
        case 's': // go back
        
            car.setSpeed(bSpeed);
            car.setAngle(0);
        
            break;
        default: // if you receive something that you don't know, just stop
            car.setSpeed(0);
            car.setAngle(0);
        }
    }
}
