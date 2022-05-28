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
## STEP 1: Download and installation 

Android studio: https://developer.android.com/studio

SMCE emulator for windows: https://github.com/ItJustWorksTM/smce-gd/wiki/Windows-setup

SMCE emulator for MacOS: https://github.com/ItJustWorksTM/smce-gd/wiki/MacOS-setup

Mosquitto(MQTT): https://mosquitto.org/download/ 

## STEP 2: Setting up mosquitto broker 

To start the mosquitto broker you first have to run `command promt` as an administrator, then change the directory to mosquitto and type the command `mosquitto -v` to get mosquitto running. 


![Skärmbild 2022-05-28 110618](https://user-images.githubusercontent.com/102026776/170818866-55a6d62a-07f7-4e75-9745-9200d22465a3.png)

## STEP 3: Downloading the application 

Clone the project from our homepage: https://github.com/DIT113-V22/group-11

![Skärmbild 2022-05-28 111600](https://user-images.githubusercontent.com/102026776/170819106-fc97fadf-1671-46de-a606-abccdd271e07.png)






Afterwards you have to open Android studios and click on `Get from VCS`, paste the cloned project link there and press `clone`. 

![Skärmbild 2022-05-28 112507](https://user-images.githubusercontent.com/102026776/170819436-cb44b0b3-a00c-4e91-897d-be1f5f54aed8.png)


## STEP 4: Getting started with the emulator

After you have succesfully installed the SMCE emulator, start up the emulator and press on `start fresh`. Then press on the `+` icon and after that on `add new`.

![Skärmbild 2022-05-28 114222](https://user-images.githubusercontent.com/102026776/170820016-8d17cae3-993e-4873-a50c-5f5e04f3101d.png)

Then you have to find the smartcar.ino file that's located under `arduino` in the file that you cloned. 

![Skärmbild 2022-05-28 114557](https://user-images.githubusercontent.com/102026776/170820103-3c351346-b1af-4608-8112-32b6852c3d08.png)

Then you simply press open and select the sketch you want to run. 

![Skärmbild 2022-05-28 150955](https://user-images.githubusercontent.com/102026776/170827080-9d97a95e-d3bb-448f-ada9-1c534c3811cc.png)

Compile the sketch and then press start to get started. 

![Skärmbild 2022-05-28 151125](https://user-images.githubusercontent.com/102026776/170827265-775da634-cc58-4c60-b9bd-5d2b37233b1d.png)

# GET STARTED
To play the game, you have to have both the emulator and android studios running. You also have to have a device that you can run the application on, here is a good guide for that: https://developers.foxit.com/developer-hub/document/create-an-emulator-for-testing-in-android-studio/#:~:text=In%20Android%20Studio%20go%20to,on%20the%20%E2%80%9CNext%E2%80%9D%20button

The mosquitto broker should be running in the background so what you have to do is simply have the smartcar sketch running on the emulator and then on android studios you press the run button after you have created your device. 

![Skärmbild 2022-05-28 133641](https://user-images.githubusercontent.com/102026776/170823817-9bf95fe6-cab5-47bc-be8b-db7230075c49.png)

You will be greeted with a home screen where you can choose which one of the four themes you would like to play on. 

![Skärmbild 2022-05-28 133810](https://user-images.githubusercontent.com/102026776/170823856-c90d9d50-3b9c-4c17-9158-a0a1f881d438.png)

After choosing your theme a new screen will pop up and there will be where a message that says that you are connected to mqtt and you will also be able to see a first person view of the emulators enviroment through the application. 

![Skärmbild 2022-05-28 134135](https://user-images.githubusercontent.com/102026776/170823980-376f2b77-3c9c-41d8-9759-06f4a89d5238.png)

After this you are ready to play the game. 

For more information abou thow to play the game, please visit our wiki: https://github.com/DIT113-V22/group-11/wiki/Instructions

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
