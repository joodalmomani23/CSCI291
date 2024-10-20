#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <stdio.h>

#define TIME_STEP 64
#define MAX_VELOCITY 6.28

int main(int argc, char **argv) {
  wb_robot_init();

  // Initialize motors
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");

  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  // Initialize proximity sensors
  WbDeviceTag sensors[8];
  for (int i = 0; i < 8; ++i) {
    char sensor_id[5];
    snprintf(sensor_id, sizeof(sensor_id), "ps%d", i);
    sensors[i] = wb_robot_get_device(sensor_id);
    wb_distance_sensor_enable(sensors[i], TIME_STEP);
  }

  double speeds[2] = {MAX_VELOCITY, MAX_VELOCITY};

  while (wb_robot_step(TIME_STEP) != -1) {
    bool walls[3] = {
      wb_distance_sensor_get_value(sensors[5]) > 80,  // Left wall
      wb_distance_sensor_get_value(sensors[6]) > 80,  // Left corner
      wb_distance_sensor_get_value(sensors[7]) > 80   // Front wall
    };

    // Handle wall in front
    if (walls[2]) {
      speeds[0] = MAX_VELOCITY;
      speeds[1] = -MAX_VELOCITY;
    } else {
      // Wall on left, go straight
      if (walls[0]) {
        speeds[0] = MAX_VELOCITY;
        speeds[1] = MAX_VELOCITY;
      }
      // No wall on left, turn left to find wall
      else {
        speeds[0] = MAX_VELOCITY / 8;
        speeds[1] = MAX_VELOCITY;
      }

      // Avoid getting too close to left corner
      if (walls[1]) {
        speeds[0] = MAX_VELOCITY;
        speeds[1] = MAX_VELOCITY / 8;
      }
    }

    // Set motor speeds
    wb_motor_set_velocity(left_motor, speeds[0]);
    wb_motor_set_velocity(right_motor, speeds[1]);
  }

  wb_robot_cleanup();
  return 0;
}
