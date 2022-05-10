package com.example.littledrivers;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.joystickjhr.JoystickJhr;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "SmartcarMqttController";
    private static final String LOCALHOST = "10.0.2.2";
    private static final String MQTT_SERVER = "tcp://" + LOCALHOST + ":1883";
    private static final String THROTTLE_CONTROL = "/LittleDrivers/control/throttle";
    private static final String STEERING_CONTROL = "/LittleDrivers/control/steering";
    private static final int MOVEMENT_SPEED = 70;
    private static final int IDLE_SPEED = 0;
    private static final int STRAIGHT_ANGLE = 0;
    private static final int STEERING_ANGLE = 50;
    private static final int QOS = 1;
    private static final int IMAGE_WIDTH = 320;
    private static final int IMAGE_HEIGHT = 240;
    private static final int DIAGONAL = 20;
    int lastDirection = 0;

    private MqttClient mMqttClient;
    private boolean isConnected = false;
    private ImageView mCameraView;

    private TextView angleTextView;
    private TextView powerTextView;
    private TextView directionTextView;
    // Importing also other views


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_main);
        mMqttClient = new MqttClient(getApplicationContext(), MQTT_SERVER, TAG);
        mCameraView = findViewById(R.id.imageView);


        connectToMqttBroker();

        final JoystickJhr joystick = findViewById(R.id.joystick);

        joystick.setOnTouchListener((view, motionEvent) -> {

            joystick.move(motionEvent);
            joystick.joyX();
            joystick.joyY();
            joystick.angle();
            joystick.distancia();


                joystick.move(motionEvent);
                joystick.joyX();
                joystick.joyY();
                joystick.angle();
                joystick.distancia();

                int direction = joystick.getDireccion();

                if (lastDirection != direction) {
                    lastDirection = direction;

                    if (direction == JoystickJhr.STICK_UP) {
                        drive(MOVEMENT_SPEED,STRAIGHT_ANGLE,"Moving Forward");
                    } else if (direction == JoystickJhr.STICK_UPRIGHT) {
                        drive(MOVEMENT_SPEED,DIAGONAL,"Moving Forward");
                    } else if (direction == JoystickJhr.STICK_RIGHT) {
                        drive(MOVEMENT_SPEED,STEERING_ANGLE,"Moving Forward");
                    } else if (direction == JoystickJhr.STICK_DOWNRIGHT) {
                        drive(-MOVEMENT_SPEED,DIAGONAL,"Moving Forward");
                    } else if (direction == JoystickJhr.STICK_DOWN) {
                        drive(-MOVEMENT_SPEED,STRAIGHT_ANGLE,"Moving Backward");
                    } else if (direction == JoystickJhr.STICK_DOWNLEFT) {
                        drive(-MOVEMENT_SPEED,-DIAGONAL,"Moving Forward");
                    } else if (direction == JoystickJhr.STICK_LEFT) {
                        drive(MOVEMENT_SPEED,-STEERING_ANGLE,"Moving Forward");
                    } else if (direction == JoystickJhr.STICK_UPLEFT) {
                        drive(MOVEMENT_SPEED,- DIAGONAL,"Moving ");
                    } else if (direction == JoystickJhr.STICK_NONE){
                        drive(IDLE_SPEED, STRAIGHT_ANGLE, "not moving the joystick"); }

                }




       /* angleTextView = (TextView) findViewById(R.id.angleTextView);
        powerTextView = (TextView) findViewById(R.id.powerTextView);
        directionTextView = (TextView) findViewById(R.id.directionTextView);*/
            //Referencing also other views
            //joystick = (JoystickView) findViewById(R.id.joystickView);

            //Event listener that always returns the variation of the angle in degrees, motion power in percentage and direction of movement


            return true;
        }
    );

    }

    @Override
    protected void onResume() {
        super.onResume();

        connectToMqttBroker();
    }

    @Override
    protected void onPause() {
        super.onPause();

        mMqttClient.disconnect(new IMqttActionListener() {
            @Override
            public void onSuccess(IMqttToken asyncActionToken) {
                Log.i(TAG, "Disconnected from broker");
            }

            @Override
            public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
            }
        });
    }

    private void connectToMqttBroker() {
        if (!isConnected) {
            mMqttClient.connect(TAG, "", new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    isConnected = true;

                    final String successfulConnection = "Connected to MQTT broker";
                    Log.i(TAG, successfulConnection);
                    Toast.makeText(getApplicationContext(), successfulConnection, Toast.LENGTH_SHORT).show();

                    mMqttClient.subscribe("/LittleDrivers/ultrasound/front", QOS, null);
                    mMqttClient.subscribe("/LittleDrivers/camera", QOS, null);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    final String failedConnection = "Failed to connect to MQTT broker";
                    Log.e(TAG, failedConnection);
                    Toast.makeText(getApplicationContext(), failedConnection, Toast.LENGTH_SHORT).show();
                }
            }, new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    isConnected = false;

                    final String connectionLost = "Connection to MQTT broker lost";
                    Log.w(TAG, connectionLost);
                    Toast.makeText(getApplicationContext(), connectionLost, Toast.LENGTH_SHORT).show();
                }

                @Override
                public void messageArrived(String topic, MqttMessage message) throws Exception {
                    if (topic.equals("/LittleDrivers/camera")) {
                        final Bitmap bm = Bitmap.createBitmap(IMAGE_WIDTH, IMAGE_HEIGHT, Bitmap.Config.ARGB_8888);

                        final byte[] payload = message.getPayload();
                        final int[] colors = new int[IMAGE_WIDTH * IMAGE_HEIGHT];
                        for (int ci = 0; ci < colors.length; ++ci) {
                            final byte r = payload[3 * ci];
                            final byte g = payload[3 * ci + 1];
                            final byte b = payload[3 * ci + 2];
                            colors[ci] = Color.rgb(r, g, b);
                        }
                        bm.setPixels(colors, 0, IMAGE_WIDTH, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
                        mCameraView.setImageBitmap(bm);
                    } else {
                        Log.i(TAG, "[MQTT] Topic: " + topic + " | Message: " + message.toString());
                    }
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    Log.d(TAG, "Message delivered");
                }
            });
        }
    }

    void drive(int throttleSpeed, int steeringAngle, String actionDescription) {
        if (!isConnected) {
            final String notConnected = "Not connected (yet)";
            Log.e(TAG, notConnected);
            Toast.makeText(getApplicationContext(), notConnected, Toast.LENGTH_SHORT).show();
            return;
        }
        Log.i(TAG, actionDescription);
        mMqttClient.publish(THROTTLE_CONTROL, Integer.toString(throttleSpeed), QOS, null);
        mMqttClient.publish(STEERING_CONTROL, Integer.toString(steeringAngle), QOS, null);
    }


}