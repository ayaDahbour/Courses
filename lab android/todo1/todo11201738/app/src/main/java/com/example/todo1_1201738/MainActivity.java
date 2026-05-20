package com.example.todo1_1201738;

import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    TextView scoreTextView, timerTextView, questionTextView, nameTextView;
    Button option1, option2, option3, option4;
    int score = 0, questionIndex = 0;
    CountDownTimer countDownTimer;
    String[] questions = {"Q1: What Is The Capital Of Palestine", "Q2: What is the date of the Al-Aqsa Flood?", "Q3: What is the total number of martyrs in Gaza in the current war?", "Q4: What is the area of Palestine?", "Q5: What is the oldest city in Palestine?"};
    String[][] options = {{"Ramallah", "Jerusalem", "Nablus", "Jericho"}, {"7-Oct", "8-Oct", "9-Oct", "10-Oct"}, {"40K+", "27K+", "20K+", "31K+"}, {"7000km", "6025km", "27027km", "30000km"}, {"Jericho", "Nablus", "Tulkarm", "Bethlehem"}};
    int[] answers = {1, 0, 3, 2, 0}; // Correct answers index from the choices

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        scoreTextView = findViewById(R.id.scoreTextView);
        timerTextView = findViewById(R.id.timerTextView);
        questionTextView = findViewById(R.id.questionTextView);
        nameTextView = findViewById(R.id.nameTextView);
        option1 = findViewById(R.id.option1);
        option2 = findViewById(R.id.option2);
        option3 = findViewById(R.id.option3);
        option4 = findViewById(R.id.option4);

        nameTextView.setText("Aya Dahbour 1201738");
        updateQuestion();

        View.OnClickListener answerButtonClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (v.getTag().toString().equals(String.valueOf(answers[questionIndex]))) {
                    score++;
                    scoreTextView.setText("Score: " + score);
                }
                moveToNext();
            }
        };
        option1.setOnClickListener(answerButtonClickListener);
        option2.setOnClickListener(answerButtonClickListener);
        option3.setOnClickListener(answerButtonClickListener);
        option4.setOnClickListener(answerButtonClickListener);
    }
    private void updateQuestion() {
        if (questionIndex < questions.length) {
            questionTextView.setText(questions[questionIndex]);
            option1.setText(options[questionIndex][0]);
            option2.setText(options[questionIndex][1]);
            option3.setText(options[questionIndex][2]);
            option4.setText(options[questionIndex][3]);
            startTimer();
        } else {
            finishQuiz();
        }
    }
    private void startTimer() {
        countDownTimer = new CountDownTimer(10000, 1000) {
            public void onTick(long millisUntilFinished) {
                timerTextView.setText("Time: " + millisUntilFinished / 1000);
            }
            public void onFinish() {
                moveToNext();
            }
        }.start();
    }

    private void moveToNext() {
        questionIndex++;
        countDownTimer.cancel();
        updateQuestion();
    }

    private void finishQuiz() {
        Intent intent = new Intent(MainActivity.this, ResultActivity.class);
        intent.putExtra("SCORE", score);
        startActivity(intent);
        finish();
    }
}
