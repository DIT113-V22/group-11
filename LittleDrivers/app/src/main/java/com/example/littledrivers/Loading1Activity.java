package com.example.littledrivers;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;

public class Loading1Activity extends AppCompatActivity {

    ProgressBar progressBar;
    TextView textView;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_loading1);

        progressBar = findViewById(R.id.pro1);
        textView = findViewById(R.id.txt1);

        progressBar.setMax(100);
        progressBar.setScaleY(2f);

        progressAnimation();

        Drawable progressDrawable = progressBar.getProgressDrawable().mutate();
        progressDrawable.setColorFilter(Color.LTGRAY, android.graphics.PorterDuff.Mode.SRC_IN);
        progressBar.setProgressDrawable(progressDrawable);
    }

    private void progressAnimation() {

        ProgressAnimation1 animation = new ProgressAnimation1(this,textView,progressBar,0f,100f);
        animation.setDuration(5000);
        progressBar.setAnimation(animation);

    }
}