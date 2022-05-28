package com.example.littledrivers;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.example.joystickjhr.JoystickJhr;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class Theme4Activity extends AppCompatActivity {
    private static final String TAG = "SmartcarMqttController";
    private static final String LOCALHOST = "10.0.2.2";
    private static final String MQTT_SERVER = "tcp://" + LOCALHOST + ":1883";
    private static final String THROTTLE_CONTROL = "/LittleDrivers/control/throttle";
    private static final String STEERING_CONTROL = "/LittleDrivers/control/steering";
    private static int MOVEMENT_SPEED = 50;
    private static final int IDLE_SPEED = 0;
    private static final int STRAIGHT_ANGLE = 0;
    private static final int STEERING_ANGLE = 50;
    private static final int QOS = 1;
    private static final int IMAGE_WIDTH = 320;
    private static final int IMAGE_HEIGHT = 240;
    private static final int DIAGONAL = 20;
    int lastDirection = 0;
    boolean insideRangeF = false;
    boolean insideRangeB = false;
    boolean insideRangeR = false;
    boolean insideRangeL = false;
    String speed;
    String distance;

    private MqttClient mMqttClient;
    private boolean isConnected = false;
    private ImageView mCameraView;
    private TextView warningMessage;
    private Switch safeDrive;
    private TextView speedTextView;
    private TextView distanceTextView;

    ImageButton button;
    ImageButton speedUp;
    ImageButton speedDown;
    private MediaPlayer mediaPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_theme4);

        button = findViewById(R.id.home4);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Theme4Activity.this, HomeActivity.class);
                startActivity(intent);
            }
        });



        mMqttClient = new MqttClient(getApplicationContext(), MQTT_SERVER, TAG);
        mCameraView = findViewById(R.id.imageView);
        warningMessage = findViewById(R.id.WarningMessage);
        warningMessage.setVisibility(View.INVISIBLE);
        speedTextView = findViewById(R.id.speedView);
        distanceTextView = findViewById(R.id.distanceView);
        safeDrive = (Switch) findViewById(R.id.safeDrive);
        mediaPlayer = MediaPlayer.create(this, R.raw.beep);

        speedUp = findViewById(R.id.speedUp);
        speedUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mMqttClient.publish("/LittleDrivers/speed/speedUp", "true", QOS, null);
                MOVEMENT_SPEED+=10;
            }
        });
        speedDown = findViewById(R.id.speedDown);
        speedDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mMqttClient.publish("/LittleDrivers/speed/speedDown", "true", QOS, null);
                MOVEMENT_SPEED-=10;

            }
        });
        connectToMqttBroker();

        final JoystickJhr joystick = findViewById(R.id.joystick);

        safeDrive.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (safeDrive.isChecked()) {
                    mMqttClient.publish("/LittleDrivers/obstacleAvoidance/toggle", "true", QOS, null);
                } else if (!safeDrive.isChecked()) {
                    mMqttClient.publish("/LittleDrivers/obstacleAvoidance/toggle", "false", QOS, null);
                }
            }
        });





        joystick.setOnTouchListener((view, motionEvent) -> {
                    if(insideRangeR || insideRangeB || insideRangeL || insideRangeF){
                        warningMessage.setVisibility(View.VISIBLE);
                        mediaPlayer.start();
                        mediaPlayer.setLooping(true);
                    } else {
                        warningMessage.setVisibility(View.INVISIBLE);
                        mediaPlayer.setLooping(false);
                    }
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
                            if(insideRangeF) {
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeF){
                                drive(MOVEMENT_SPEED, STRAIGHT_ANGLE, "Moving Forward");
                            }
                            if(insideRangeB || insideRangeL || insideRangeR){
                                insideRangeB=false;
                                insideRangeL=false;
                                insideRangeR=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/back","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/left","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/right","false", QOS, null );
                            }


                        } else if (direction == JoystickJhr.STICK_UPRIGHT) {
                            if(insideRangeF || insideRangeR){
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeF && !insideRangeR){
                                drive(MOVEMENT_SPEED, DIAGONAL, "Moving Forward");
                            }
                            if(insideRangeB || insideRangeL){
                                insideRangeB=false;
                                insideRangeL=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/back","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/left","false", QOS, null );
                            }


                        } else if (direction == JoystickJhr.STICK_RIGHT) {
                            if(insideRangeR){
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeR){
                                drive(MOVEMENT_SPEED, STEERING_ANGLE, "Moving Forward");
                            }
                            if(insideRangeL || insideRangeB){
                                insideRangeL=false;
                                insideRangeB=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/left","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/back","false", QOS, null );
                            }


                        } else if (direction == JoystickJhr.STICK_DOWNRIGHT) {

                            if(insideRangeB){
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeB){
                                drive(-MOVEMENT_SPEED, DIAGONAL, "Moving Forward");
                            }
                            if(insideRangeF || insideRangeR || insideRangeL){
                                insideRangeF=false;
                                insideRangeR=false;
                                insideRangeL=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/front","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/right","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/left","false", QOS, null );
                            }


                        } else if (direction == JoystickJhr.STICK_DOWN) {
                            if(insideRangeB){
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeB){
                                drive(-MOVEMENT_SPEED, STRAIGHT_ANGLE, "Moving Backward");
                            }
                            if(insideRangeF || insideRangeL || insideRangeR){
                                insideRangeF=false;
                                insideRangeL=false;
                                insideRangeR=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/front","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/left","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/right","false", QOS, null );
                            }


                        } else if (direction == JoystickJhr.STICK_DOWNLEFT) {
                            if(insideRangeB){
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeB){
                                drive(-MOVEMENT_SPEED, -DIAGONAL, "Moving Forward");
                            }
                            if(insideRangeF || insideRangeR || insideRangeL){
                                insideRangeF=false;
                                insideRangeR=false;
                                insideRangeL=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/front","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/right","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/left","false", QOS, null );
                            }


                        } else if (direction == JoystickJhr.STICK_LEFT) {
                            if(insideRangeL){
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeL){
                                drive(MOVEMENT_SPEED, -STEERING_ANGLE, "Moving Forward");
                            }
                            if(insideRangeR || insideRangeB){
                                insideRangeR=false;
                                insideRangeB=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/right","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/back","false", QOS, null );
                            }




                        } else if (direction == JoystickJhr.STICK_UPLEFT) {
                            if(insideRangeF || insideRangeL){
                                drive(IDLE_SPEED, STRAIGHT_ANGLE, "Obstacle nearby");
                            } else if (!insideRangeF && !insideRangeL){
                                drive(MOVEMENT_SPEED, -DIAGONAL, "Moving ");
                            }
                            if(insideRangeB || insideRangeR){
                                insideRangeB=false;
                                insideRangeR=false;
                                mMqttClient.publish("/LittleDrivers/insiderange/back","false", QOS, null );
                                mMqttClient.publish("/LittleDrivers/insiderange/right","false", QOS, null );
                            }


                        } else if (direction == JoystickJhr.STICK_NONE) {
                            drive(IDLE_SPEED, STRAIGHT_ANGLE, "not moving the joystick");
                        }

                    }
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

                    mMqttClient.subscribe("/LittleDrivers/camera", QOS, null);
                    mMqttClient.subscribe("/LittleDrivers/insiderange/#", QOS, null);
                    mMqttClient.subscribe("/LittleDrivers/Odometer/speed", QOS, null);
                    mMqttClient.subscribe("/LittleDrivers/odometer/distance", QOS, null);
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
                        for (int ci = 0; ci < colors.length; ++ ci) {
                            final int r = payload[3 * ci] & 0xFF;
                            final int g = payload[3 * ci + 1] & 0xFF;
                            final int b = payload[3 * ci + 2] & 0xFF;
                            colors[ci] = Color.rgb(r, g, b);
                        }
                        bm.setPixels(colors, 0, IMAGE_WIDTH, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
                        mCameraView.setImageBitmap(bm);
                    } else if (topic.equals("/LittleDrivers/insiderange/front") && message.toString().equals("true")) {
                        insideRangeF = true;
                    } else if (topic.equals("/LittleDrivers/insiderange/back") && message.toString().equals("true")) {
                        insideRangeB = true;
                    } else if (topic.equals("/LittleDrivers/insiderange/left") && message.toString().equals("true")) {
                        insideRangeL = true;
                    } else if (topic.equals("/LittleDrivers/insiderange/right") && message.toString().equals("true")) {
                        insideRangeR = true;
                    } else if (topic.equals("/LittleDrivers/Odometer/speed")){
                        speed = message.toString();
                        speedTextView.setText(speed);
                    } else if (topic.equals("/LittleDrivers/odometer/distance")) {
                        distance = message.toString();
                        distanceTextView.setText(distance);
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