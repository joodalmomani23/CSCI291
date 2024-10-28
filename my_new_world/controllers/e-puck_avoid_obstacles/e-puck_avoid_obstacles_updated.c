 #include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/light_sensor.h>  // Include for light sensor
#include <stdio.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28

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
  for (int ind = 0; ind < 8; ++ind) {
    sprintf(prox_sensor_name, "ps%d", ind);
    prox_sensors[ind] = wb_robot_get_device(prox_sensor_name);
    wb_distance_sensor_enable(prox_sensors[ind], TIME_STEP);
  }

  // Initialize the light sensor
  WbDeviceTag light_sensor = wb_robot_get_device("lts"); 
  wb_light_sensor_enable(light_sensor, TIME_STEP);

  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;

  // Define an array to store light sensor and distance sensor values
  double values[30][2];  // Assuming max 30 readings
  int value_count = 0;   // Counter for readings

  while (wb_robot_step(TIME_STEP) != -1) {
    // Sensor readings
    bool left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 80;
    bool left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 80;
    bool front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 80;
    bool right_wall = wb_distance_sensor_get_value(prox_sensors[2]) > 80;

  

    // Light sensor reading
    double light_value = wb_light_sensor_get_value(light_sensor);
    printf("Light Sensor Value: %f\n", light_value);  // Display light sensor value

    // If walls are detected on the left, right, and front, store the light and distance sensor values
    if (right_wall && left_wall && front_wall && value_count < 30) {
      values[value_count][0] = light_value;                              // Store light sensor value
      values[value_count][1] = wb_distance_sensor_get_value(prox_sensors[7]);  // Store front wall distance sensor value (example)
      printf("Storing light and distance sensor values at count %d\n", value_count);
      value_count++;
    }

    // Robot's wall-following logic
    if (front_wall) {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED;
    } else if (left_wall) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED;
    } else {
      left_speed = MAX_SPEED / 8;
      right_speed = MAX_SPEED;
    }

    // Slow down if the left corner is detected
    if (left_corner) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED / 8;
    }

    // Set motor speeds
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
  }

  // Cleanup the Webots API
  wb_robot_cleanup();

  return 0;
}