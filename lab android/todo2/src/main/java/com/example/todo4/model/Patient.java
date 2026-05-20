package com.example.todo4.model;

import com.fasterxml.jackson.annotation.JsonBackReference;

import jakarta.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
public class Patient {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String name;
    private double illnessExperienceRequirement;
    private boolean isCured = false;

    @ManyToMany
    @JoinTable(
            name = "patient_doctor",
            joinColumns = @JoinColumn(name = "patient_id"),
            inverseJoinColumns = @JoinColumn(name = "doctor_id")
    )
    @JsonBackReference
    private List<Doctor> doctors = new ArrayList<>();

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

    public double getIllnessExperienceRequirement() {
        return illnessExperienceRequirement;
    }

    public void setIllnessExperienceRequirement(double illnessExperienceRequirement) {
        this.illnessExperienceRequirement = illnessExperienceRequirement;
    }

    public boolean isCured() {
        return isCured;
    }

    public void setCured(boolean cured) {
        isCured = cured;
    }

    public List<Doctor> getDoctors() {
        return doctors;
    }

    public void setDoctors(List<Doctor> doctors) {
        this.doctors = doctors;
    }
}
