package com.example.project_1200308_1201738;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

public class SignUp extends AppCompatActivity {

    private EditText emailEditText, phoneEditText, firstNameEditText, lastNameEditText, passwordEditText, confirmPasswordEditText;
    private Spinner genderSpinner;
    private Button registerButton;
    private SharedPrefManager sharedPrefManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_up);

        sharedPrefManager = SharedPrefManager.getInstance(this);

        // Generate and store AES key
        String aesKey;
        try {
            aesKey = AESUtil.generateKey();
            sharedPrefManager.writeString("aesKey", aesKey);
        } catch (Exception e) {
            throw new RuntimeException("Failed to generate AES key", e);
        }

        String[] options = {"Male", "Female"};
        genderSpinner = findViewById(R.id.genderSpinner);
        ArrayAdapter<String> objGenderArr = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, options);
        genderSpinner.setAdapter(objGenderArr);

        emailEditText = findViewById(R.id.emailEditText);
        phoneEditText = findViewById(R.id.phoneEditText);
        firstNameEditText = findViewById(R.id.firstNameEditText);
        lastNameEditText = findViewById(R.id.lastNameEditText);
        passwordEditText = findViewById(R.id.passwordEditText);
        confirmPasswordEditText = findViewById(R.id.confirmPasswordEditText);
        registerButton = findViewById(R.id.registerButton);

        registerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (validateInputs()) {
                    String email = emailEditText.getText().toString().trim();
                    String phone = phoneEditText.getText().toString().trim();
                    String firstName = firstNameEditText.getText().toString().trim();
                    String lastName = lastNameEditText.getText().toString().trim();
                    String gender = genderSpinner.getSelectedItem().toString();
                    String password = passwordEditText.getText().toString();

                    // Encrypt the password
                    String encryptedPassword;
                    try {
                        encryptedPassword = AESUtil.encrypt(sharedPrefManager.readString("aesKey", ""), password);
                    } catch (Exception e) {
                        Toast.makeText(SignUp.this, "Failed to encrypt password", Toast.LENGTH_SHORT).show();
                        return;
                    }

                    User newUser = new User(email, phone, firstName, lastName, gender, encryptedPassword);

                    DataBaseHelper dbHelper = new DataBaseHelper(SignUp.this);

                    if (dbHelper.checkUserExists(email)) {
                        Toast.makeText(SignUp.this, "User already exists. Please login.", Toast.LENGTH_SHORT).show();
                    } else {
                        if (dbHelper.addUser(newUser)) {
                            Toast.makeText(SignUp.this, "Registration successful. Please login.", Toast.LENGTH_SHORT).show();
                            Intent intent = new Intent(SignUp.this, LogIn.class);
                            startActivity(intent);
                            finish();
                        } else {
                            Toast.makeText(SignUp.this, "Registration failed. Please try again.", Toast.LENGTH_SHORT).show();
                        }
                    }
                }
            }
        });
    }

    private boolean validateInputs() {
        String email = emailEditText.getText().toString().trim();
        String phone = phoneEditText.getText().toString().trim();
        String firstName = firstNameEditText.getText().toString().trim();
        String lastName = lastNameEditText.getText().toString().trim();
        String password = passwordEditText.getText().toString();
        String confirmPassword = confirmPasswordEditText.getText().toString();

        boolean isValid = true;

        if (TextUtils.isEmpty(email) || !android.util.Patterns.EMAIL_ADDRESS.matcher(email).matches()) {
            Toast.makeText(SignUp.this, "Invalid Email input,Review it pls?! ", Toast.LENGTH_SHORT).show();
            isValid = false;
        }

        if (TextUtils.isEmpty(phone) || !phone.matches("05\\d{8}")) {
            Toast.makeText(SignUp.this, "Phone number must be exactly 10 digits and start with 05. ", Toast.LENGTH_SHORT).show();
            isValid = false;
        }

        if (TextUtils.isEmpty(firstName) || firstName.length() < 3) {
            Toast.makeText(SignUp.this, "First name must be at least 3 characters.", Toast.LENGTH_SHORT).show();
            isValid = false;
        }

        if (TextUtils.isEmpty(lastName) || lastName.length() < 3) {
            Toast.makeText(SignUp.this, "Last name must be at least 3 characters.", Toast.LENGTH_SHORT).show();
            isValid = false;
        }

        if (TextUtils.isEmpty(password) || password.length() < 8 || !password.matches(".*\\d.*") || !password.matches(".*[a-zA-Z].*")) {
            Toast.makeText(SignUp.this, "Password must be at least 8 characters and include at least one letter and one number.", Toast.LENGTH_SHORT).show();
            isValid = false;
        }

        if (!password.equals(confirmPassword)) {
            Toast.makeText(SignUp.this, " Passwords do not match.", Toast.LENGTH_SHORT).show();
            isValid = false;
        }

        return isValid;
    }
}
