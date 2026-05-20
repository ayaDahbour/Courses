package com.example.todo4.controller;

import com.example.todo4.model.Patient;
import com.example.todo4.repository.PatientRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/patients")
public class PatientController {

    @Autowired
    private PatientRepository patientRepository;

    @GetMapping
    public List<Patient> getAllPatients() {
        return patientRepository.findAll();
    }

    @GetMapping("/{id}")
    public Patient getPatientById(@PathVariable Long id) {
        return patientRepository.findById(id).orElse(null);
    }

    @PostMapping
    public boolean addPatient(@RequestBody Patient patient) {
        patientRepository.save(patient);
        return true;
    }

    @DeleteMapping("/{id}")
    public boolean deletePatient(@PathVariable Long id) {
        patientRepository.deleteById(id);
        return true;
    }

    @PutMapping("/release/{patientId}")
    public boolean releasePatient(@PathVariable Long patientId) {
        Patient patient = patientRepository.findById(patientId).orElse(null);

        if (patient != null) {
            patient.setCured(true);
            patient.getDoctors().clear();
            patientRepository.save(patient);
            return true;
        }
        return false;
    }
}
