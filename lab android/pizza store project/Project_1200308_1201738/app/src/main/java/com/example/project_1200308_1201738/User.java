package com.example.project_1200308_1201738;

public class User {
    // Attributes
    private String email;
    private String phoneNumber;
    private String firstName;
    private String lastName;
    private String Gender;
    private String Password;

    // Constructors
    public User() {}
    public User(String email, String phoneNumber, String firstName, String lastName, String Gender, String Password) {
        this.email = email;
        this.phoneNumber=phoneNumber;
        this.firstName=firstName;
        this.lastName=lastName;
        this.Gender=Gender;
        this.Password=Password;
    }
    // Setters and Getters
    public String getEmail() {
        return email;
    }
    public void setEmail(String email) {
        this.email = email;
    }
    public String getPhoneNumber() {
        return phoneNumber;
    }
    public void setPhoneNumber(String phoneNumber) {
        this.phoneNumber = phoneNumber;
    }
    public String getFirstName() {
        return firstName;
    }
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }
    public String getLastName() {
        return lastName;
    }
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }
    public String getGender() {
        return Gender;
    }
    public void setGender(String gender) {
        Gender = gender;
    }
    public String getPassword() {
        return Password;
    }
    public void setPassword(String password) {
        Password = password;
    }
    // To String Method
    @Override
    public String toString() {
        return "User{" +
                "email='" + email + '\'' +
                ", phoneNumber='" + phoneNumber + '\'' +
                ", firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", Gender='" + Gender + '\'' +
                ", Password='" + Password + '\'' +
                '}';
    }
}
