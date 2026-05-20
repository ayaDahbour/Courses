package com.example.project_1200308_1201738;

public class Pizza {
    // Attributes
    private int Id;
    private String type;

    //constructor
    public Pizza() {}
    public Pizza(int id, String type) {
        this.Id = id;
        this.type = type;
    }
    //Setters and Getters
    public int getId() {
        return Id;
    }
    public void setId(int id) {
        Id = id;
    }
    public String getType() {
        return type;
    }
    public void setType(String type) {
        this.type = type;
    }
    // To String Method
    @Override
    public String toString() {
        return "Pizza{" +
                "Id=" + Id +
                ", type='" + type + '\'' +
                '}';
    }
}
