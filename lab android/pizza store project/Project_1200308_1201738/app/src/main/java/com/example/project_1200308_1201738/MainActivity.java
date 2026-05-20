package com.example.project_1200308_1201738;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;


import java.util.List;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        Button get_started = findViewById(R.id.button_getstarted);
        get_started.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ConnectionAsyncTask connectionAsyncTask = new ConnectionAsyncTask(MainActivity.this);
                connectionAsyncTask.setResultListener(new ConnectionAsyncTask.ConnectionResultListener() {
                    @Override
                    public void onConnectionResult(boolean successful) {
                        if (successful) {
                            Toast.makeText(MainActivity.this, "Connection successful", Toast.LENGTH_SHORT).show();
                            Intent intent = new Intent(MainActivity.this, LogIn_SignUpActivity.class);
                            startActivity(intent);
                        } else {
                            Toast.makeText(MainActivity.this, "Connection unsuccessful", Toast.LENGTH_SHORT).show();
                        }
                    }
                });
                connectionAsyncTask.execute("https://18fbea62d74a40eab49f72e12163fe6c.api.mockbin.io/\n");
            }
        });
    }
}
