#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/light_sensor.h>
#include <stdio.h>
#include <math.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28
#define SAMPLE_INTERVAL 2000 // 2 seconds in milliseconds
#define MAX_SAMPLES 130 // Maximum number of light samples to store
#define MAX_LIGHT_SENSORS 8 // Define the number of light sensors

// Structure to hold position data
typedef struct {
  double x;
  double y;
} Position;

int main(int argc, char **argv) {
  wb_robot_init();  // Initialize the Webots API

  // Initialize motors
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");

  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  // Initialize proximity sensors
  WbDeviceTag prox_sensors[8];
  char prox_sensor_name[50];
  for (int i = 0; i < 8; ++i) {
    sprintf(prox_sensor_name, "ps%d", i);
    prox_sensors[i] = wb_robot_get_device(prox_sensor_name);
    wb_distance_sensor_enable(prox_sensors[i], TIME_STEP);
  }

  // Initialize light sensors
  WbDeviceTag light_sensors[MAX_LIGHT_SENSORS]; // Updated to hold 8 light sensors
  char light_sensor_name[50];
  for (int i = 0; i < MAX_LIGHT_SENSORS; ++i) {
    sprintf(light_sensor_name, "ls%d", i); // Light sensors are named ls0, ls1, ..., ls7
    light_sensors[i] = wb_robot_get_device(light_sensor_name);
    wb_light_sensor_enable(light_sensors[i], TIME_STEP);
  }

  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;

  // Array to store average light sensor values over time
  double light_values[MAX_SAMPLES];
  Position light_positions[MAX_SAMPLES]; // To store positions of light samples
  int sample_count = 0;
  double max_light_value = 0.0;
  int max_light_index = -1;

  int elapsed_time = 0; // Time counter to manage sampling interval
  Position current_position = {0.0, 0.0}; // Initial robot position
  double distance_per_step = 0.0; // Distance traveled per time step

  while (wb_robot_step(TIME_STEP) != -1) {
    // Update elapsed time
    elapsed_time += TIME_STEP;

    // Calculate the average light value from all light sensors every SAMPLE_INTERVAL
    if (elapsed_time >= SAMPLE_INTERVAL && sample_count < MAX_SAMPLES) {
      double light_sum = 0.0;

      // Calculate the average light value from all light sensors
      for (int i = 0; i < MAX_LIGHT_SENSORS; i++) {
        light_sum += wb_light_sensor_get_value(light_sensors[i]);
      }
      double avg_light_value = light_sum / MAX_LIGHT_SENSORS; // Update to use 8 sensors
      light_values[sample_count] = avg_light_value;
      light_positions[sample_count] = current_position; // Store current position

      // Print average light value and corresponding position
      printf("Sample %d - Average Light Value: %f at Position: (%f, %f) \n", 
             sample_count, avg_light_value, current_position.x, current_position.y);

      // Check if this is the maximum light value
      if (avg_light_value > max_light_value) {
        max_light_value = avg_light_value;
        max_light_index = sample_count;
      }

      sample_count++;
      elapsed_time = 0; // Reset the elapsed time for the next sample
    }

    // Sensor readings for wall-following logic
    bool left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 80;
    bool left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 80;
    bool front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 80;
    bool right_wall = wb_distance_sensor_get_value(prox_sensors[2]) > 80;

    // Robot's wall-following logic 
    if (front_wall) {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED; // Turn right
    } else if (left_wall) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED; // Move forward
    } else {
      left_speed = MAX_SPEED / 8;
      right_speed = MAX_SPEED; // Move forward
    }

    // Slow down if the left corner is detected
    if (left_corner) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED / 8; // Turn left
    }

    // Set motor speeds
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);

    // Calculate current position based on speed
    distance_per_step = (left_speed + right_speed) / 2 * (TIME_STEP / 1000.0); // Average speed
    current_position.x += distance_per_step * cos(0); // Assuming straight movement for simplicity
    current_position.y += distance_per_step * sin(0); // Assuming straight movement for simplicity

    // Check if maze traversal is complete
    if (sample_count >= MAX_SAMPLES) {
      break;
    }
  }

  // Stop the robot
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  // Debugging output for maximum light index
  printf("Max Light Index: %d\n", max_light_index);
  
  // Navigate back to the position with the maximum light value
  if (max_light_index != -1) {
    Position target_position = light_positions[max_light_index];
    printf("Returning to brightest location with average light value of: %f at position: (%f,%f) \n", max_light_value, target_position.x, target_position.y);

    // Navigate to the target position
    while (fabs(current_position.x - target_position.x) > 0.1 || fabs(current_position.y - target_position.y) > 0.1) {
      // Calculate the angle to the target position
      double angle_to_target = atan2(target_position.y - current_position.y, target_position.x - current_position.x);

      // Determine motor speeds based on angle to target
      left_speed = MAX_SPEED * (1 - fabs(angle_to_target / M_PI)); // Adjust left speed
      right_speed = MAX_SPEED * (1 + fabs(angle_to_target / M_PI)); // Adjust right speed

      // Clamp the speeds to ensure they do not exceed MAX_SPEED
      if (left_speed > MAX_SPEED) left_speed = MAX_SPEED;
      if (right_speed > MAX_SPEED) right_speed = MAX_SPEED;

      // Adjust speeds for proximity sensors to avoid obstacles
      bool front_obstacle = wb_distance_sensor_get_value(prox_sensors[7]) > 80; // Check for a front wall
      if (front_obstacle) {
        left_speed = 0;
        right_speed = 0; // Stop if an obstacle is detected in front
      }

      // Set motor speeds
      wb_motor_set_velocity(left_motor, left_speed);
      wb_motor_set_velocity(right_motor, right_speed);
      
      // Update current position based on the average speed
      distance_per_step = (left_speed + right_speed) / 2 * (TIME_STEP / 1000.0);
      current_position.x += distance_per_step * cos(0); // Assuming straight movement for simplicity
      current_position.y += distance_per_step * sin(0); // Assuming straight movement for simplicity
      
      // Step the robot
      wb_robot_step(TIME_STEP);
    }
  } else {
    printf("No valid maximum light index found. Navigation to target position will not proceed.\n");
  }

  // Cleanup the Webots API
  wb_robot_cleanup();

  return 0;
}
