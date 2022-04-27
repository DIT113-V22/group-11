
#include <Smartcar.h>
#include <iostream>



const int fSpeed   = 70;  // 70% of the full speed forward
const int bSpeed   = -70; // 70% of the full speed backward
const int lDegrees = -75; // degrees to turn left
const int rDegrees = 75;  // degrees to turn right
const unsigned long transmissionInterval = 100; // In milliseconds

ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor{arduinoRuntime, smartcarlib::pins::v2::leftMotorPins};
BrushedMotor rightMotor{arduinoRuntime, smartcarlib::pins::v2::rightMotorPins};
DifferentialControl control{leftMotor, rightMotor};

// THIS IS FOR THE ODOMETER
/*const unsigned short odometerPin   = 2;
const unsigned long pulsesPerMeter = 400;
DirectionlessOdometer odometer{
    arduinoRuntime, odometerPin, []() { odometer.update(); }, pulsesPerMeter
};*/
const auto pulsesPerMeter = 600;
DirectionlessOdometer leftOdometer{ arduinoRuntime,
                                    smartcarlib::pins::v2::leftOdometerPin,
                                    []() { leftOdometer.update(); },
                                    pulsesPerMeter };
DirectionlessOdometer rightOdometer{ arduinoRuntime,
                                     smartcarlib::pins::v2::rightOdometerPin,
                                     []() { rightOdometer.update(); },
                                     pulsesPerMeter };
// THIS IS FOR THE ODOMETER

SimpleCar car(control);

const int triggerPin           = 6; // D6
const int echoPin              = 7; // D7
const unsigned int maxDistance = 100;
bool insideRangeF = false;
bool insideRangeB = false;
bool insideRangeR = false;
bool insideRangeL = false;
SR04 left(arduinoRuntime, 2, 3, maxDistance); // trigger and echo pin respectively
SR04 right(arduinoRuntime, 4, 5, maxDistance);
SR04 front(arduinoRuntime, 6, 7, maxDistance);
SR04 back(arduinoRuntime, 16, 17, maxDistance);

auto currentSpeed = 0;

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

//THIS IS FOR THE ODOMETER

Serial.print(String(leftOdometer.getDistance()/100) + " m");
    Serial.print("\t\t");
    Serial.println(String(rightOdometer.getDistance()/100) + " m");

    Serial.println(String(leftOdometer.getSpeed()) + " m/s");
    Serial.print("\t\t");
    Serial.println(String(rightOdometer.getSpeed()) + " m/s");

    /*
    Serial.println(odometer.getDistance());
        delay(100);*/
//THIS IS FOR THE ODOMETER


//THIS IS TO PRINT THE ULTRASONIC SENSORS
     /* static auto previousTransmission = 0UL;
      const auto currentTime = millis();
      // ================= 2
      if (currentTime > previousTransmission + transmissionInterval) {
        previousTransmission = currentTime;
        Serial.println("===");
        Serial.println("Left: " + String(left.getDistance()));
        Serial.println("Right: " + String(right.getDistance()));
        Serial.println("Front: " + String(front.getDistance()));
        Serial.println("Back: " + String(back.getDistance()));}*/
//THIS IS TO PRINT THE ULTRASONIC SENSORS



if (fdistance > 0 && fdistance < 100) {
   car.setSpeed(0);
   insideRangeF = true;

}



    if (bdistance > 0 && bdistance < 100) {
        car.setSpeed(0);
        insideRangeB = true;


    }

     if (rdistance > 0 && rdistance < 60) {
            car.setSpeed(0);
            insideRangeR = true;
        }

       if (ldistance > 0 && ldistance < 60) {
                    car.setSpeed(0);
                    insideRangeL = true;

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
        if(insideRangeL==true || insideRangeF==true){
                car.setSpeed(0);
                Serial.println("Too close to obstacle!");
            } else{
                car.setSpeed(fSpeed);
                car.setAngle(lDegrees);
                currentSpeed=fSpeed;
                if(insideRangeR==true || insideRangeB==true){
                delay(1000);
                insideRangeR=false;
                insideRangeB=false;
                }
            }
            break;
        case 'd': // turn clock-wise
        if(insideRangeR==true || insideRangeF==true){
                car.setSpeed(0);
                Serial.println("Too close to obstacle!");
            } else {
                car.setSpeed(fSpeed);
                car.setAngle(rDegrees);
                currentSpeed=fSpeed;
                if(insideRangeL==true || insideRangeL==true){
                delay(1000);
                insideRangeL=false;
                insideRangeB=false;
                }
            }
            break;
        case 'w': // go ahead
        if(insideRangeF==true || insideRangeR==true || insideRangeL==true){
                car.setSpeed(0);
                Serial.println("Too close to obstacle, you need to reverse!");
            } else {
                car.setSpeed(fSpeed);
                car.setAngle(0);
                currentSpeed=fSpeed;
                if (insideRangeB==true) {
                delay(1000);
                insideRangeB=false;
                }
            }
            break;
        case 's': // go back
        if(insideRangeB==true){
                car.setSpeed(0);
                Serial.println("Too close to obstacle, you need to drive forward!");
            } else {
                car.setSpeed(bSpeed);
                car.setAngle(0);
                currentSpeed=bSpeed;
                if (insideRangeF==true || insideRangeF==true || insideRangeF==true ){
                delay(1000);
                insideRangeF=false;
                insideRangeR=false;
                insideRangeL=false;
                }
            }


            break;
        default: // if you receive something that you don't know, just stop
            car.setSpeed(0);
            car.setAngle(0);
        }
    }
}