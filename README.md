# Little Drivers (Group 11) 🏎
Vroom...Vroom... ![image](https://user-images.githubusercontent.com/102043923/170706113-ac743769-d8b5-4416-8803-87ca39d3b394.png)

WARNING: DRIVERS WANTED!

Do you have a little one who loves cars and can't wait to drive a car of their own? Well, this is the application for them. Not only do they experience the joy and thrill of driving in first person, but they also get to drive in a virtual environment. This application is for children of age 5 and above.

# WHAT IS LITTLE DRIVERS? 

Little Drivers is an application created for entertainment. 

![image](https://user-images.githubusercontent.com/102043923/170705267-33e4965b-bd2d-47d5-ae48-6cc78299a647.png)

Little Drivers allows the players to drive a virtual car freely around a virtual environment using a joystick. Therefore, players can use our application whenever and wherever they want and start playing. The application has four different themes that the players can choose to play in. The themes are based on Disney Princesses, Finding Nemo, Big Hero 6 and Cars 😉. Our application includes a safe drive mode, that allows the players to drive the car without hitting obsctacles up ahead.

# WHY DID WE CREATE LITTLE DRIVERS?
The purpose of this application is to provide entertainment to little kids. 

We were inspired by our childhood memories. When you were a child, did you not want to be behind the steering wheel and drive the car? Did you not want to sit in the front seat and view the car as the driver views it? Well, we hope to make our childhood dreams come true through you. 

With different themes our application is inclusive to children with different interests. We therefore capture a bigger audience and hopefully we have something for you. 

# HOW DID WE CREATE LITTLE DRIVERS?
ARDUINO is used to create the sketches for the car that will be uploaded on the emulator. 

ANDROID STUDIO to build and design the app.

MQTT to connect the app to the emulator.

SMCE is the emulator in which the car is driven.

ESPRESSO is going to be used to test the application.

# SETUP

# GET STARTED

# FEATURES
- Different themes: 
    - The application provides the user with four different application displays to drive in. These displays are based on disney movies.  

- Safe drive: 
  -  The theme screens contain a safe drive toggle button, when its on, if the car is close to an obstacle an emergency break kicks in. In addition to          that there will be warning messages that will pop on the screen and a beeping sound will play to inform the player that they are too close to an            obstacle.

- Joystick:
  - Each theme display will have their own customized joystick that the player can use to move the car. The joystick allows the car to move in eight           directions.

- Camera feed:
  - The player will view the virtual environment in first person view from the car's perspective through a camera feed shown on the theme screens.

- Display speed and distance:
  - The application will display the speed the car is driven in and the total distance travelled on all of the theme screens. 

- Acceleration/Deceleration:
  - There will be two buttons on each theme screen that allows the players to control the speed of the car. The speed is changed in increments of 10 with the minimum speed being 0 and maximum speed being 100 m/s.  

# SOFTWARE ARCHITECTURE

## MQTT

## smartcar.ino

This is an Arduino Sketch that uses the programming language C++ and contains all the functionalities of the car. The car is equipped with four ultrasonic sensors, two odometers and a camera in first person POV. The ultrasonic sensors are used to detect obstacles all around the car and determine when the car should be stopped. The odometers are used to retrieve the total distance traveled and its current speed. The file also implements the MQTT protocol to communicate with the Android application. To handle these messages received through the broker the method mqtt.onMessage() is used. The subscriptions include:
- /LittleDrivers/insiderange/# (which includes front, back, left, right): to trigger the obstacle avoidance at the right times
- /LittleDrivers/control/#: to steer the car in the right direction and with the right speed
- /LittleDrivers/obstacleAvoidance/toggle: to know when the ‘Safe drive’ switch is on or off

Furthermore, smartcar.ino publishes to:
- /LittleDrivers/insiderange/# (which includes front, back, left, right): to make the joystick “aware” of where it can steer with the obstacle avoidance enabled
- /LittleDrivers/camera: to send frame data to the application which keeps the camera feed updated
- /LittleDrivers/odometer/speed: to send data of the current speed 
- /LittleDrivers/odometer/distance: to send data of the total distance traveled

## HomeActivity.java

## NavigationTest.kt

This kotlin class is responsible for the navigational tests between the screens. All the tests were made using espresso, a testing framework to make easy and reliable UI (user interface) tests. The tests ensure that the buttons for moving from one page to the other work properly and don’t go to an unwanted page. For example, the home button on every theme of our application is made so that the user can easily access the home page and the 4 of these test makes sure that if the home button is clicked it goes to the homepage. The test is done by first launching the screen which is done by recognizing the screen’s id given in the xml file, then the button to be tested is pressed using the onClick method. The test then checks if the correct screen is displayed by using the method isDisplayed and putting the screen’s id in its parameter.

The CI for instrumented tests in GitHub makes it easy to test as we don’t need to test manually by running them in android studios, instead, they are tested when the code is committed and pushed.
