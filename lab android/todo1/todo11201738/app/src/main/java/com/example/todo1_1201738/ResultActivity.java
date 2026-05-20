package com.example.todo1_1201738;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;


public class ResultActivity extends AppCompatActivity {
    TextView resultTextView, feedbackTextView;
    Button resetButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);

        resultTextView = findViewById(R.id.resultTextView);
        feedbackTextView = findViewById(R.id.feedbackTextView);
        resetButton = findViewById(R.id.resetButton);
        resetButton.setBackgroundColor(ContextCompat.getColor(this, android.R.color.holo_red_dark));
        resetButton.setTextColor(ContextCompat.getColor(this, android.R.color.white));


        int score = getIntent().getIntExtra("SCORE", 0);
        resultTextView.setText(score + "/5");
        feedbackTextView.setText(score >= 4 ? "You Won!" : "You Lost!");

        resetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ResultActivity.this, MainActivity.class);
                startActivity(intent);
                finish();
            }
        });
    }
}
