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

class JointInterface : public mcan::konarm_hat::McCanSlaveInterface_t
{
public:
  uint8_t joint_idx = 0;

  void callback_write_set_position(mcan::konarm_hat::commands::SetPosition &var);
  void callback_write_set_torque(mcan::konarm_hat::commands::SetTorque &var);
  void callback_write_set_control_mode(mcan::konarm_hat::commands::SetControlMode &var);
  void callback_write_config(mcan::konarm_hat::configs::Config &var);
  void callback_write_clear_errors(mcan::konarm_hat::commands::ClearErrors &var);
  void callback_write_set_and_reset(mcan::konarm_hat::commands::SetAndReset &var);

  auto get_write_callbacks()
  {
    return std::make_tuple(
        std::make_pair(&JointInterface::callback_write_set_position, &JointInterface::set_position),
        std::make_pair(&JointInterface::callback_write_set_torque, &JointInterface::set_torque),
        std::make_pair(&JointInterface::callback_write_set_control_mode, &JointInterface::set_control_mode),
        std::make_pair(&JointInterface::callback_write_config, &JointInterface::config),
        std::make_pair(&JointInterface::callback_write_clear_errors, &JointInterface::clear_errors),
        std::make_pair(&JointInterface::callback_write_set_and_reset, &JointInterface::set_and_reset));
  }
};

inline uint32_t get_unique_id()
{
  uint32_t t1 = HAL_GetUIDw0();
  uint32_t t2 = HAL_GetUIDw1();
  uint32_t t3 = HAL_GetUIDw2();
  return (t1 ^ t2 ^ t3) & 0x1FFFFF; // Return only the lower 21 bits
}

extern se::SimpleTask task_module_control_loop;
extern se::SimpleTask task_module_update_values_for_can;

using ModuleType = mcan::McSlaveDriver<JointInterface,
                                       mcan::konarm_hat::Hardware_t>;

struct joint
{
  JointInterface joint_can_interface;
  std::shared_ptr<ModuleType> module;
  std::shared_ptr<se::motor::VescMotor> motor;
  std::shared_ptr<se::encoders::EncoderAbsoluteMagneticMT6701> encoder;
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
se::Status read_board_id();
se::Status init_joints_arr();
void init_vesc_motor_settings();

se::Status update_values_for_can(se::SimpleTask &, void *args);
