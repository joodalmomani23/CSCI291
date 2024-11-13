#include <webots/robot.h>         // Import the Webots robot library
#include <webots/motor.h>         // Import the Webots motor control library
#include <webots/distance_sensor.h> // Import the Webots distance sensor library
#include <webots/light_sensor.h>  // Import the Webots light sensor library
#include <stdio.h>                // Standard library for input and output
#include <math.h>                 // Standard math library

#define TIME_STEP 64              // Simulation time step (in ms)
#define MAX_SPEED 6.28            // Max speed for motors
#define SAMPLE_INTERVAL 2000      // Interval for light sampling (2 seconds in ms)
#define MAX_SAMPLES 130           // Max number of light samples to store
#define MAX_LIGHT_SENSORS 8       // Number of light sensors on the robot

// Structure to store x and y position
typedef struct {
  double x;
  double y;
} Position;

int main(int argc, char **argv) {
  wb_robot_init();  // Initialize the Webots simulation

  //EXPLORATION PHASE

  // Get and set up left and right motor devices
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");

  wb_motor_set_position(left_motor, INFINITY);  // Set left motor to infinite rotation
  wb_motor_set_position(right_motor, INFINITY); // Set right motor to infinite rotation
  wb_motor_set_velocity(left_motor, 0.0);       // Initially stop the left motor
  wb_motor_set_velocity(right_motor, 0.0);      // Initially stop the right motor

  // Get and enable proximity sensors
  WbDeviceTag prox_sensors[8];
  char prox_sensor_name[50];
  for (int i = 0; i < 8; ++i) {
    sprintf(prox_sensor_name, "ps%d", i);      // Name each sensor from ps0 to ps7
    prox_sensors[i] = wb_robot_get_device(prox_sensor_name);
    wb_distance_sensor_enable(prox_sensors[i], TIME_STEP); // Enable each sensor
  }

  // Get and enable light sensors
  WbDeviceTag light_sensors[MAX_LIGHT_SENSORS];
  char light_sensor_name[50];
  for (int i = 0; i < MAX_LIGHT_SENSORS; ++i) {
    sprintf(light_sensor_name, "ls%d", i);    // Name each light sensor from ls0 to ls7
    light_sensors[i] = wb_robot_get_device(light_sensor_name);
    wb_light_sensor_enable(light_sensors[i], TIME_STEP);   // Enable each sensor
  }

  double left_speed = MAX_SPEED; // Initial speed for left motor
  double right_speed = MAX_SPEED; // Initial speed for right motor

  Position light_positions[MAX_SAMPLES]; // Array to store positions where light samples are taken
  int sample_count = 0;                   // Counter for number of samples
  double min_light_value = INFINITY;      // Track minimum light value seen
  int min_light_index = -1;               // Index of minimum light sample

  int elapsed_time = 0;                   // Track time for sampling interval
  Position current_position = {0.0, 0.0}; // Initialize robot's position
  double distance_per_step = 0.0;         // Distance traveled per time step

  bool left_wall, left_corner, front_wall; // Variables to track obstacles

  while (wb_robot_step(TIME_STEP) != -1) {
    // Update elapsed time
    elapsed_time += TIME_STEP;

    // Check light values at each sampling interval
    if (elapsed_time >= SAMPLE_INTERVAL && sample_count < MAX_SAMPLES) {
      double light_sum = 0.0;

      // Calculate average light reading from all sensors
      for (int i = 0; i < MAX_LIGHT_SENSORS; i++) {
        light_sum += wb_light_sensor_get_value(light_sensors[i]);
      }
      double avg_light_value = light_sum / MAX_LIGHT_SENSORS; // Calculate average
      light_positions[sample_count] = current_position;       // Record current position

      // Print light sample data
      printf("Sample %d - Average Light Value: %f at Position: (%f, %f) \n", 
             sample_count, avg_light_value, current_position.x, current_position.y);

      // Check if this sample has the minimum light value seen so far
      if (avg_light_value < min_light_value) {
        min_light_value = avg_light_value;
        min_light_index = sample_count;
      }

      sample_count++;          // Increment sample counter
      elapsed_time = 0;        // Reset elapsed time for next interval
    }

    // Read distance sensor values for wall-following
    left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 80;
    left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 80;
    front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 80;

    // Wall-following logic to avoid obstacles
    if (front_wall) {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED; // Turn right if a wall is detected in front
    } else if (left_wall) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED;  // Move forward if there is a wall on the left
    } else {
      left_speed = MAX_SPEED / 8;
      right_speed = MAX_SPEED;  // Slow turn right if no wall is on the left
    }

    // Adjust if left corner detected
    if (left_corner) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED / 8; // Slight left turn at a corner
    }

    // Set motor speeds
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);

    // Update robot position (approximation, assuming straight movement)
    distance_per_step = (left_speed + right_speed) / 2 * (TIME_STEP / 1000.0);
    current_position.x += distance_per_step * cos(0);
    current_position.y += distance_per_step * sin(0);

    // Exit loop when enough samples are collected
    if (sample_count >= MAX_SAMPLES) {
      break;
    }
  }

  //FINDING PHASE

  // Stop motors after sampling
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  // Print result for minimum light sample index
  printf("Min Light Index: %d\n", min_light_index);

  // Begin navigation to find area with minimum light value
  printf("Navigating to find minimum light value...\n");
  while (true) {
    // Read current light value
    float lights = 0;
    for (int i = 0; i < MAX_LIGHT_SENSORS; i++) {
      lights += wb_light_sensor_get_value(light_sensors[i]);
    }
    double avg_light_value = lights / MAX_LIGHT_SENSORS;

    // Stop if light value is close to minimum
    if (avg_light_value <= min_light_value + 2) {
      printf("Current light value matches minimum light value within tolerance: %f. Stopping.\n", avg_light_value);
      wb_motor_set_velocity(left_motor, 0.0);
      wb_motor_set_velocity(right_motor, 0.0);
      break; // Exit loop
    }

    // Wall-following logic for navigation
    left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 80;
    left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 80;
    front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 80;

    if (front_wall) {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED; // Turn right
    } else if (left_wall) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED;  // Move forward
    } else {
      left_speed = MAX_SPEED / 8;
      right_speed = MAX_SPEED;  // Turn right slightly
    }

    if (left_corner) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED / 8; // Turn left at corner
    }

    // Set motor speeds
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);

    // Print current light value for debugging
    printf("Current light value: %f\n", avg_light_value);

    wb_robot_step(TIME_STEP); // Advance simulation by one time step
  }

  // Final stop after reaching target
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  // Clean up Webots simulation
  wb_robot_cleanup();

  return 0;
}
