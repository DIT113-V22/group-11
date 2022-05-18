package com.example.littledrivers;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.ImageView;

public class HomeActivity extends AppCompatActivity {

    ImageButton theme1;
    ImageButton theme2;
    ImageButton theme3;
    ImageButton theme4;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_home);
        theme1 = findViewById(R.id.Theme1);
        theme2 = findViewById(R.id.Theme2);
        theme3 = findViewById(R.id.Theme3);
        theme4 = findViewById(R.id.Theme4);

        theme1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(HomeActivity.this,Theme1Activity.class);
                startActivity(intent);
            }
        });

        theme2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(HomeActivity.this,Theme2Activity.class);
                startActivity(intent);
            }
        });

        theme3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(HomeActivity.this,Theme3Activity.class);
                startActivity(intent);
            }
        });

        theme4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(HomeActivity.this,Theme4Activity.class);
                startActivity(intent);
            }
        });
    }
}