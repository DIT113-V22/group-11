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
mqtt.subscribe("/LittleDrivers/speed/#");


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
    } else if(topic=="/LittleDrivers/speed/speedUp") {
            Serial.println("+10");
    } else if(topic=="/LittleDrivers/speed/speedDown") {
          Serial.println("-10");
    }else {
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
String speed = "";
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
 static auto previousTransmission = 0UL;
    if (currentTime - previousTransmission >= oneSecond) {
      previousTransmission = currentTime;
      const auto distance = String(front.getDistance());
            mqtt.publish("/LittleDrivers/ultrasound/front", distance);
          }
        }

//------- To show speed and total distance travelled on app -------
 speed = String(smartCar.getSpeed());
 mqtt.publish("/LittleDrivers/Odometer/speed",speed);
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

handleInput();

#ifdef __SMCE__
  // Avoid over-using the CPU if we are running in the emulator
  delay(1);

#endif

}

//------- Controls car through serial -------
void handleInput()
{ // handle serial input if there is any
    if (Serial.available())
    {
        char input = Serial.read(); // read everything that has been received so far and log down
                                    // the last entry

        switch (input)
        {
         case 'u': // change speed to 30
                  smartCar.setSpeed(30);
                  Serial.println("The speed of the car has been set to 30!");
                break;
                case 'i': // change speed to 50
                  smartCar.setSpeed(50);
                  Serial.println("The speed of the car has been set to 50!");
                break;
                case 'o': // change speed to 70
                  smartCar.setSpeed(70);
                  Serial.println("The speed of the car has been set to 70!");
                break;
        case 'a': // rotate counter-clockwise going forward
        if(insideRangeL==true || insideRangeF==true){
                smartCar.setSpeed(0);
                Serial.println("Too close to obstacle!");
            } else{
                smartCar.setSpeed(fSpeed);
                smartCar.setAngle(lDegrees);
                if(insideRangeR==true || insideRangeB==true){
                delay(1000);
                insideRangeR=false;
                insideRangeB=false;
                }
            }
            break;
        case 'd': // turn clock-wise
        if(insideRangeR==true || insideRangeF==true){
                smartCar.setSpeed(0);
                Serial.println("Too close to obstacle!");
            } else {
                smartCar.setSpeed(fSpeed);
                smartCar.setAngle(rDegrees);
                if(insideRangeL==true || insideRangeL==true){
                delay(1000);
                insideRangeL=false;
                insideRangeB=false;
                }
            }
            break;
        case 'w': // go ahead
        if(insideRangeF==true || insideRangeR==true || insideRangeL==true){
                smartCar.setSpeed(0);
                Serial.println("Too close to obstacle, you need to reverse!");
            } else {
                smartCar.setSpeed(fSpeed);
                smartCar.setAngle(0);
                if (insideRangeB==true) {
                delay(1000);
                insideRangeB=false;
                }
            }
            break;
        case 's': // go back
        if(insideRangeB==true){
                smartCar.setSpeed(0);
                Serial.println("Too close to obstacle, you need to drive forward!");
            } else {
                smartCar.setSpeed(bSpeed);
                smartCar.setAngle(0);
                if (insideRangeF==true || insideRangeF==true || insideRangeF==true ){
                delay(1000);
                insideRangeF=false;
                insideRangeR=false;
                insideRangeL=false;
                }
            }
            break;
        default: // if you receive something that you don't know, just stop
            smartCar.setSpeed(0);
            smartCar.setAngle(0);
        }
    }
}