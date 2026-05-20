#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <time.h>   

// define user-defined values
#define ultrasonicThreshold 2.5    
#define cameraThreshold 0.7        
#define handEyeThreshold 0.75     
#define maxIdentifiedIntruders 50  
#define maxMisidentifiedIntruders 5 
#define resetTime 30              

// library functions 
float capture_image(int camera_number) {
    printf("Camera %d: Capturing image...\n", camera_number);
    return 1.0; // successful capture
}

float call_algorithm(int camera_number, int algorithm_number) {
    printf("Camera %d: Processing image with Algorithm %d...\n", camera_number, algorithm_number);
    return (float)(rand() % 100) / 100; // prob between 0 and 1
}

bool activate_light(int light_number) {
    printf("Light %d activated.\n", light_number);
    return 1; // light activated
}

bool deactivate_light(int light_number) {
    printf("Light %d deactivated.\n", light_number);
    return 1; // light deactivated
}

bool open_gate(int gate_number) {
    printf("Gate %d opened.\n", gate_number);
    return 1; // gate opened
}

bool close_gate(int gate_number) {
    printf("Gate %d closed.\n", gate_number);
    return 1; // gate closed
}

// counters for system state
int identified_intruders = 0;
int misidentified_intruders = 0;

// thread function for a single camera
void *process_camera(void *arg) {
    int camera_number = *(int *)arg;
    float probabilities[4];
    float average_probability = 0.0;

    capture_image(camera_number);

    // 4 algorithms
    for (int i = 0; i < 4; i++) {
        probabilities[i] = call_algorithm(camera_number, i + 1);
        average_probability += probabilities[i];
    }

    // compute avg prob
    average_probability /= 4.0;
    printf("Camera %d: Average Probability = %.2f\n", camera_number,average_probability);

    // update the counters based on prob
    if (average_probability >= cameraThreshold) {
        identified_intruders++;
    } else {
        misidentified_intruders++;
    }
    return NULL;
}

int main() {
    // random number generator 
    srand(time(NULL));

    pthread_t threads[3];
    int camera_ids[3] = {1, 2, 3};

    // activate lights and open gate at the start
    open_gate(1);
    activate_light(1);
    activate_light(2);

    // start threads for each camera
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, process_camera, &camera_ids[i]);
    }

    // wait the threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // deactivate lights and close gate after processing
    deactivate_light(1);
    deactivate_light(2);
    close_gate(1);

    // check system state
    printf("Intruders Identified: %d\n", identified_intruders);
    printf("Intruders Misidentified: %d\n", misidentified_intruders);

    if (identified_intruders > maxIdentifiedIntruders) {
        printf("System permanently disabled: Too many identified intruders.\n");
        return 1; // exit with error
    }

    if (misidentified_intruders > maxMisidentifiedIntruders) {
        printf("System permanently disabled: Too many misidentified intruders.\n");
        return 1; // exit with error
    }

    // reset after alert
    printf("Resetting system in %d seconds...\n", resetTime);
    sleep(resetTime);
    printf("System reset complete. Ready to monitor again.\n");

    return 0;
}
