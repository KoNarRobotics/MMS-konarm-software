#pragma once
#include "encoder_magnetic.hpp"
#include "i2c.hpp"
#include "fram_i2c.hpp"
#include "main.hpp"
#include "main_prog.hpp"
#include "status.hpp"
#include "stmepic.hpp"
#include "uart.hpp"
#include "konarm_hat.hpp"
#include "vesc_bldc.hpp"

#include "MT6701.hpp"
#include "uart.hpp"

#include "can_mc_driver.hpp"
#include "mc_firmware/can_base.hpp"
#include "mc_firmware/mc_common.hpp"

// TODO : pozmieniać odpowiednio te stałe
#define VESC_MOTOR_1_ID 0x00
#define VESC_MOTOR_2_ID 0x01
#define VESC_MOTOR_3_ID 0x02
#define VESC_MOTOR_4_ID 0x03
#define VESC_MOTOR_5_ID 0x04
#define VESC_MOTOR_6_ID 0x05

// Gear ratio
#define VESC_MOTOR_1_RATIO 1.0f
#define VESC_MOTOR_2_RATIO 1.0f
#define VESC_MOTOR_3_RATIO 1.0f
#define VESC_MOTOR_4_RATIO 1.0f
#define VESC_MOTOR_5_RATIO 1.0f
#define VESC_MOTOR_6_RATIO 1.0f

// Current to torque
#define VESC_MOTOR_1_CRT 0.0665f
#define VESC_MOTOR_2_CRT 0.0665f
#define VESC_MOTOR_3_CRT 0.0665f
#define VESC_MOTOR_4_CRT 0.0665f
#define VESC_MOTOR_5_CRT 0.0665f
#define VESC_MOTOR_6_CRT 0.0665f

// Polar pairs
#define VESC_MOTOR_1_PAIRS 7
#define VESC_MOTOR_2_PAIRS 7
#define VESC_MOTOR_3_PAIRS 7
#define VESC_MOTOR_4_PAIRS 7
#define VESC_MOTOR_5_PAIRS 7
#define VESC_MOTOR_6_PAIRS 7

// Velocity
#define VESC_MOTOR_1_VEL 100
#define VESC_MOTOR_2_VEL 100
#define VESC_MOTOR_3_VEL 100
#define VESC_MOTOR_4_VEL 100
#define VESC_MOTOR_5_VEL 100
#define VESC_MOTOR_6_VEL 100

#define BOARD_ID_0 0x00 // 1 joint
#define BOARD_ID_1 0x01 // 1 joint
#define BOARD_ID_2 0x02 // 1 joint
#define BOARD_ID_3 0x03 // 3 joints

#define MAGIC_NUMER_EEPROM 0xDEAD

extern se::motor::VescMotorSettings settings_motor1;
extern se::motor::VescMotorSettings settings_motor2;
extern se::motor::VescMotorSettings settings_motor3;
extern se::motor::VescMotorSettings settings_motor4;
extern se::motor::VescMotorSettings settings_motor5;
extern se::motor::VescMotorSettings settings_motor6;

struct joint_can_config
{
  uint32_t can_filter_mask_high;
  uint32_t can_filter_mask_low;
  uint32_t can_filter_id_high;
  uint32_t can_filter_id_low;

  uint32_t can_konarm_status_frame_id;
  uint32_t can_konarm_set_pos_frame_id;
  uint32_t can_konarm_get_pos_frame_id;
  uint32_t can_konarm_clear_errors_frame_id;
  uint32_t can_konarm_get_errors_frame_id;
  uint32_t can_konarm_set_control_mode_frame_id;
  uint32_t can_konarm_set_effector_position_frame_id;

  uint32_t can_konarm_get_torque_frame_id;
  uint32_t can_konarm_get_config_frame_id;
  uint32_t can_konarm_send_config_frame_id;
  uint32_t can_konarm_set_and_reset_frame_id;
  uint32_t can_konarm_set_torque_frame_id;
};

extern joint_can_config config_joint1;
extern joint_can_config config_joint2;
extern joint_can_config config_joint3;
extern joint_can_config config_joint4;
extern joint_can_config config_joint5;
extern joint_can_config config_joint6;

// There are none temp sensors on boards.
// Temp variables are kept for ROS compatibility
// Same with voltage
struct ErrorJoint
{
  bool encoder_arm_disconnect = false;
  bool encoder_motor_disconnect = false;
  bool can_disconnected = false;
  bool can_error = false;
  bool temp_engine_overheating = false;
  bool temp_driver_overheating = false;
};

struct ErrorBoard
{
  bool temp_board_overheating = false;
  bool temp_engine_sensor_disconnect = false;
  bool temp_driver_sensor_disconnect = false;
  bool temp_board_sensor_disconnect = false;

  // board errors
  bool board_overvoltage = false;
  bool board_undervoltage = false;

  // other errors
  bool controler_motor_limit_position = false;
};

extern ErrorBoard error_board;

inline uint32_t get_unique_id()
{
  uint32_t t1 = HAL_GetUIDw0();
  uint32_t t2 = HAL_GetUIDw1();
  uint32_t t3 = HAL_GetUIDw2();
  return (t1 ^ t2 ^ t3) & 0x1FFFFF; // Return only the lower 21 bits
}

extern se::SimpleTask task_module_check_for_errors;

struct joint
{
  joint_can_config config;
  std::shared_ptr<se::motor::VescMotor> motor;
  std::shared_ptr<se::encoders::EncoderAbsoluteMagneticMT6701> encoder;
  ErrorJoint errors;
};

struct board_id
{
  uint8_t id;
};

struct magic_number
{
  uint16_t num;
};

extern std::shared_ptr<se::memory::FramI2C> fram;
extern std::vector<joint> joints_arr;
extern joint joint1;
extern joint joint2;
extern joint joint3;
extern magic_number magic_number_eeprom;
extern board_id id;

void write_board_id();
se::Status add_callbacks();
se::Status read_board_id();
se::Status init_joints_arr();

void init_vesc_motor_settings();
void init_joint_can_config();

se::Status check_for_errors(se::SimpleTask &, void *args);
