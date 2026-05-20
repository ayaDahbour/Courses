package com.example.a1201738ayadahbour;

import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.AnimationUtils;
import android.view.animation.RotateAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private ImageView sun, earth, lowerCloud, upperCloud, lightRed, lightOrange, lightGreen, rock, car;
    private Animation sunRotateAnimation, carRotateAnimation, lowerCloudTranslateAnimation, upperCloudTranslateAnimation, rockFallAnimation;
    private final int RED_DURATION = 3000;
    private final int ORANGE_DURATION = 2000;
    private final Handler trafficLightHandler = new Handler();
    private float currentSpeed = 1.0f;  // Assuming initial speed x of the car

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize views
        sun = findViewById(R.id.Sun);
        earth = findViewById(R.id.Earth);
        lowerCloud = findViewById(R.id.LowerCloud);
        upperCloud = findViewById(R.id.UpperCloud);
        lightRed = findViewById(R.id.LightRed);
        lightOrange = findViewById(R.id.LightOrange);
        lightGreen = findViewById(R.id.LightGreen);
        rock = findViewById(R.id.Rock);
        car = findViewById(R.id.Car);

        // Load animations
        sunRotateAnimation = AnimationUtils.loadAnimation(this, R.anim.sun_rotate);
        carRotateAnimation = AnimationUtils.loadAnimation(this, R.anim.car_rotate);

        // Start animations
        sun.startAnimation(sunRotateAnimation);

        // Start traffic light sequence
        trafficLightSequence();

        carRotateAnimation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {
                // Not needed
            }

            @Override
            public void onAnimationEnd(Animation animation) {
                // Restart the traffic light sequence when car completes one round
                trafficLightSequence();
            }

            @Override
            public void onAnimationRepeat(Animation animation) {
                checkCarRockCollision();
            }
        });

        // Rock fall animation with rotation
        rockFallAnimation = new TranslateAnimation(0, -100, 0, 100);
        rockFallAnimation.setDuration(1000);
        rockFallAnimation.setFillAfter(true);
        RotateAnimation rotateAnimation = new RotateAnimation(0, 180, Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.5f);
        rotateAnimation.setDuration(1000);
        rotateAnimation.setFillAfter(true);
        AnimationSet rockAnimationSet = new AnimationSet(true);
        rockAnimationSet.addAnimation(rockFallAnimation);
        rockAnimationSet.addAnimation(rotateAnimation);
    }

    private void checkCarRockCollision() {
        Rect carBounds = new Rect();
        car.getHitRect(carBounds);
        Rect rockBounds = new Rect();
        rock.getHitRect(rockBounds);

        if (Rect.intersects(carBounds, rockBounds)) {
            currentSpeed *= 0.5;
            carRotateAnimation.setDuration((long) (carRotateAnimation.getDuration() * 2 / currentSpeed));
            rock.startAnimation(rockFallAnimation);
        }
    }

    private void trafficLightSequence() {
        lightRed.setVisibility(ImageView.VISIBLE);
        lightOrange.setVisibility(ImageView.INVISIBLE);
        lightGreen.setVisibility(ImageView.INVISIBLE);
        trafficLightHandler.postDelayed(() -> {
            lightRed.setVisibility(ImageView.INVISIBLE);
            lightOrange.setVisibility(ImageView.VISIBLE);
            trafficLightHandler.postDelayed(() -> {
                lightOrange.setVisibility(ImageView.INVISIBLE);
                lightGreen.setVisibility(ImageView.VISIBLE);
                car.startAnimation(carRotateAnimation);
            }, ORANGE_DURATION);
        }, RED_DURATION);
    }
}
