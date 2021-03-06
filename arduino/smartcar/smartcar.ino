#include <vector>

#include <MQTT.h>
#include <WiFi.h>
#ifdef __SMCE__
#include <OV767X.h>
#endif

#include <Smartcar.h>

// ------- Variable declaration -------
MQTTClient mqtt;
WiFiClient net;

const char ssid[] = "***";
const char pass[] = "****";

int fSpeed   = 50;  // 50% of the full speed forward
int bSpeed   = -50; // 50% of the full speed backward
int lDegrees = -50; // degrees to turn left
int rDegrees = 50;  // degrees to turn right
const unsigned long transmissionInterval = 100; // In milliseconds
const auto pulsesPerMeter = 600;

const auto oneSecond = 1000UL;
#ifdef __SMCE__
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
bool toggleOn = false;
int speed = 0;

std::vector<char> frameBuffer;

//------- Car instantiation -------
ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor{arduinoRuntime, smartcarlib::pins::v2::leftMotorPins};
BrushedMotor rightMotor{arduinoRuntime, smartcarlib::pins::v2::rightMotorPins};
DifferentialControl control{leftMotor, rightMotor};
DirectionlessOdometer leftOdometer{arduinoRuntime, smartcarlib::pins::v2::leftOdometerPin, [](){leftOdometer.update();},pulsesPerMeter};
DirectionlessOdometer rightOdometer{arduinoRuntime,smartcarlib::pins::v2::rightOdometerPin, []() {rightOdometer.update();},pulsesPerMeter};

SR04 left(arduinoRuntime, 2, 3, maxDistance);
SR04 right(arduinoRuntime, 4, 5, maxDistance);
SR04 front(arduinoRuntime, 6, 7, maxDistance);
SR04 back(arduinoRuntime, 16, 17, maxDistance);

GY50 gyroscope(arduinoRuntime, 0, 100UL);

SmartCar smartCar(arduinoRuntime, control, gyroscope,leftOdometer, rightOdometer);

void setup()
{
// Move the car with 50% of its full speed
     smartCar.setSpeed(0);
     speed = 0;

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

  // -------MQTT Subscriptions-------
mqtt.subscribe("/LittleDrivers/insiderange/#", 1);
mqtt.subscribe("/LittleDrivers/control/#", 1);
mqtt.subscribe("/LittleDrivers/obstacleAvoidance/toggle", 1);


mqtt.onMessage([](String topic, String message) {

//------- Controls the car through the joystick -------
    if (topic == "/LittleDrivers/control/throttle") {
      smartCar.setSpeed(message.toInt());
      speed = message.toInt();
    } else if (topic == "/LittleDrivers/control/steering") {
      smartCar.setAngle(message.toInt());
    } else if(topic=="/LittleDrivers/insiderange/front"){
           if(message.equals("false")){
                delay(1000);
                insideRangeF=false;
                }
    } else if(topic=="/LittleDrivers/insiderange/back"){
            if(message.equals("false")){
                delay(1000);
                insideRangeB=false;
                }
    } else if(topic=="/LittleDrivers/insiderange/left"){
            if(message.equals("false")){
                delay(1000);
                insideRangeL=false;
                }
    } else if(topic=="/LittleDrivers/insiderange/right"){
            if(message.equals("false")){
                delay(1000);
                insideRangeR=false;
                }
    } else if(topic=="/LittleDrivers/obstacleAvoidance/toggle"){
            if(message.equals("false")){
                toggleOn=false;
            } else if(message.equals("true")){
                toggleOn=true;
            }
    } else if(topic=="/LittleDrivers/obstacleAvoidance/toggle"){
            smartCar.setSpeed(speed);
    } else if(topic=="/LittleDrivers/obstacleAvoidance/toggle"){
            if (speed <=0){
                smartCar.setSpeed(0);
                speed = 0;
            }
    } else {
      Serial.println(topic + " " + message);
    }
  });

}

void loop()
{

//------- Gets distance to obstacle -------
const auto fdistance = front.getDistance();
const auto bdistance = back.getDistance();
const auto rdistance = right.getDistance();
const auto ldistance = left.getDistance();

String distance = "";
String speedString = "";
//------- Camera -------
if (mqtt.connected()) {
    mqtt.loop();
    const auto currentTime = millis();
#ifdef __SMCE__
    static auto previousFrame = 0UL;
    if (currentTime - previousFrame >= 65) {
      previousFrame = currentTime;
      Camera.readFrame(frameBuffer.data());
      mqtt.publish("/LittleDrivers/camera", frameBuffer.data(), frameBuffer.size(),
                   false, 0);
    }
#endif
        }

//------- To show speed and total distance travelled on app -------
 speedString = String(smartCar.getSpeed());
 mqtt.publish("/LittleDrivers/odometer/speed",speedString);
 distance = String (smartCar.getDistance()/100);
 mqtt.publish("/LittleDrivers/odometer/distance", distance);

//------- Obstacle avoidance -------
if(toggleOn){

if (fdistance > 0 && fdistance < 90 && insideRangeF==false) {
   smartCar.setSpeed(0);
   insideRangeF = true;
   mqtt.publish("/LittleDrivers/insiderange/front", "true");
   }

if (bdistance > 0 && bdistance < 90 && insideRangeB==false) {
   smartCar.setSpeed(0);
   insideRangeB = true;
   mqtt.publish("/LittleDrivers/insiderange/back", "true");
   }

if (rdistance > 0 && rdistance < 60 && insideRangeR==false) {
    smartCar.setSpeed(0);
    insideRangeR = true;
    mqtt.publish("/LittleDrivers/insiderange/right", "true");
    }

if (ldistance > 0 && ldistance < 60 && insideRangeL==false) {
    smartCar.setSpeed(0);
    insideRangeL = true;
    mqtt.publish("/LittleDrivers/insiderange/left", "true");
    }
}

#ifdef __SMCE__
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);

#endif

}