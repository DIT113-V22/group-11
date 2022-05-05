#include <vector>

#include <MQTT.h>
#include <WiFi.h>
#ifdef __SMCE__
#include <OV767X.h>
#endif

#include <Smartcar.h>

MQTTClient mqtt;
WiFiClient net;

const char ssid[] = "***";
const char pass[] = "****";


const int fSpeed   = 70;  // 70% of the full speed forward
const int bSpeed   = -70; // 70% of the full speed backward
const int lDegrees = -75; // degrees to turn left
const int rDegrees = 75;  // degrees to turn right
const unsigned long transmissionInterval = 100; // In milliseconds
const int FRONT_RIGHT = 50;
const int FRONT = 10;

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

const auto oneSecond = 1000UL;
#ifdef __SMCE__
const int triggerPin           = 6; // D6
const int echoPin              = 7; // D7
const auto mqttBrokerUrl = "127.0.0.1";
#else
const auto triggerPin = 33;
const auto echoPin = 32;
const auto mqttBrokerUrl = "192.168.0.40";
#endif
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

std::vector<char> frameBuffer;

void setup()
{
// Move the car with 50% of its full speed
     car.setSpeed(0);

       Serial.begin(9600);

       #ifdef __SMCE__
         Camera.begin(QVGA, RGB888, 15);
         frameBuffer.resize(Camera.width() * Camera.height() * Camera.bytesPerPixel());
       #endif

         WiFi.begin(ssid, pass);
         mqtt.begin(mqttBrokerUrl, 1883, net);
Serial.println("Connecting to WiFi...");
  auto wifiStatus = WiFi.status();
  while (wifiStatus != WL_CONNECTED && wifiStatus != WL_NO_SHIELD) {
    Serial.println(wifiStatus);
    Serial.print(".");
    delay(1000);
    wifiStatus = WiFi.status();
  }


  Serial.println("Connecting to MQTT broker");
  while (!mqtt.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }
  
 String MAIN_TOPIC ="/littledrivers";
const String THROTTLE_CONTROL = MAIN_TOPIC + "/Control/Direction";
const String SPEED = MAIN_TOPIC + "/Control/Speed";

  mqtt.subscribe("//Control/#", 1);
  mqtt.onMessage([](String topic, String message) { //hifehioferh i hiefihj eujhiceieju
    if (topic == "/littledrivers/control/THROTTLE_CONTROL") {
        auto input = message.toInt(); // read everything that has been received so far and log down                                
        switch (input) {
        case 1 : // rotate counter-clockwise going forward
        if(insideRangeL==true || insideRangeF==true){
                car.setSpeed(0);
                Serial.println("Too close to obstacle!");
            } else{
                car.setSpeed(50);
                delay(1000);
                car.setAngle(FRONT_RIGHT);
                
                currentSpeed=fSpeed;
                if(insideRangeR==true || insideRangeB==true){
                insideRangeR=false;
                insideRangeB=false;
                }
            }
            break;
       
        }

    } else if (topic == "/littledrivers/control/steering") {
      car.setAngle(message.toInt());
    } else {
      Serial.println(topic + " " + message);
    }
  });

}



void loop()
{

const auto fdistance = front.getDistance();
       const auto bdistance = back.getDistance();
       const auto rdistance = right.getDistance();
       const auto ldistance = left.getDistance();

if (mqtt.connected()) {
    mqtt.loop();
    const auto currentTime = millis();
#ifdef __SMCE__
    static auto previousFrame = 0UL;
    if (currentTime - previousFrame >= 65) {
      previousFrame = currentTime;
      Camera.readFrame(frameBuffer.data());
      mqtt.publish("/smartcar/camera", frameBuffer.data(), frameBuffer.size(),
                   false, 0);
    }
#endif
 static auto previousTransmission = 0UL;
    if (currentTime - previousTransmission >= oneSecond) {
      previousTransmission = currentTime;
      const auto distance = String(front.getDistance());
            mqtt.publish("/smartcar/ultrasound/front", distance);
          }
        }



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


 

#ifdef __SMCE__
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);

#endif


}
void handleInput(){

}
