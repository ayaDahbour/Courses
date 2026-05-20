package com.example.todo4.controller;

import com.example.todo4.dto.DoctorDTO;
import com.example.todo4.model.Doctor;
import com.example.todo4.model.Patient;
import com.example.todo4.repository.DoctorRepository;
import com.example.todo4.repository.PatientRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/doctors")
public class DoctorController {

    @Autowired
    private DoctorRepository doctorRepository;

    @Autowired
    private PatientRepository patientRepository;

    @GetMapping
    public List<DoctorDTO> getAllDoctors() {
        return doctorRepository.findAll().stream().map(this::convertToDto).collect(Collectors.toList());
    }

    @GetMapping("/{id}")
    public DoctorDTO getDoctorById(@PathVariable Long id) {
        Optional<Doctor> doctor = doctorRepository.findById(id);
        return doctor.map(this::convertToDto).orElse(null);
    }

    @PostMapping
    public boolean addDoctor(@RequestBody Doctor doctor) {
        doctorRepository.save(doctor);
        return true;
    }

    @DeleteMapping("/{id}")
    public boolean deleteDoctor(@PathVariable Long id) {
        doctorRepository.deleteById(id);
        return true;
    }

    @PostMapping("/assign/{doctorId}")
    public boolean assignDoctor(@PathVariable Long doctorId, @RequestBody Long patientId) {
        Optional<Doctor> doctorOptional = doctorRepository.findById(doctorId);
        Optional<Patient> patientOptional = patientRepository.findById(patientId);

        if (doctorOptional.isPresent() && patientOptional.isPresent()) {
            Doctor doctor = doctorOptional.get();
            Patient patient = patientOptional.get();

            if (doctor.getExperience() >= patient.getIllnessExperienceRequirement() &&
                    doctor.getCurrentPatients() < doctor.getMaxPatients()) {

                doctor.setCurrentPatients(doctor.getCurrentPatients() + 1);
                patient.getDoctors().add(doctor);
                doctorRepository.save(doctor);
                patientRepository.save(patient);
                return true;
            }
        }
        return false;
    }

    @PutMapping("/release/{patientId}")
    public boolean releasePatient(@PathVariable Long patientId) {
        Optional<Patient> patientOptional = patientRepository.findById(patientId);

        if (patientOptional.isPresent()) {
            Patient patient = patientOptional.get();
            patient.setCured(true);
            patient.getDoctors().clear();
            patientRepository.save(patient);
            return true;
        }
        return false;
    }

    private DoctorDTO convertToDto(Doctor doctor) {
        DoctorDTO dto = new DoctorDTO();
        dto.setId(doctor.getId());
        dto.setName(doctor.getName());
        dto.setMaxPatients(doctor.getMaxPatients());
        dto.setCurrentPatients(doctor.getCurrentPatients());
        dto.setExperience(doctor.getExperience());
        return dto;
    }
}
