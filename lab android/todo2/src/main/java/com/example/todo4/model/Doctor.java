package com.example.todo4.model;

import com.fasterxml.jackson.annotation.JsonManagedReference;

import jakarta.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
public class Doctor {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String name;
    private int maxPatients;
    private int currentPatients;
    private double experience;

    @ManyToMany(mappedBy = "doctors")
    @JsonManagedReference
    private List<Patient> patients = new ArrayList<>();

    // Getters and Setters
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getMaxPatients() {
        return maxPatients;
    }

    public void setMaxPatients(int maxPatients) {
        this.maxPatients = maxPatients;
    }

    public int getCurrentPatients() {
        return currentPatients;
    }

    public void setCurrentPatients(int currentPatients) {
        this.currentPatients = currentPatients;
    }

    public double getExperience() {
        return experience;
    }

    public void setExperience(double experience) {
        this.experience = experience;
    }

    public List<Patient> getPatients() {
        return patients;
    }

    public void setPatients(List<Patient> patients) {
        this.patients = patients;
    }
}
