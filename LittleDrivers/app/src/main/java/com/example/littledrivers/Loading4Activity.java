package com.example.littledrivers;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;

public class Loading4Activity extends AppCompatActivity {

    ProgressBar progressBar;
    TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_loading4);

        progressBar = findViewById(R.id.pro4);
        textView = findViewById(R.id.txt4);

        progressBar.setMax(100);
        progressBar.setScaleY(2f);

        progressAnimation();

        Drawable progressDrawable = progressBar.getProgressDrawable().mutate();
        progressDrawable.setColorFilter(Color.LTGRAY, android.graphics.PorterDuff.Mode.SRC_IN);
        progressBar.setProgressDrawable(progressDrawable);
    }

    private void progressAnimation() {

        ProgressAnimation4 animation = new ProgressAnimation4(this,textView,progressBar,0f,100f);
        animation.setDuration(5000);
        progressBar.setAnimation(animation);

    }
}