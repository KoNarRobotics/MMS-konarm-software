#include "fram_i2c.hpp"
#include "memory_fram.hpp"
#include "module.hpp"
#include "i2c.hpp"
#include "main.hpp"
#include "main_prog.hpp"
#include "status.hpp"
#include "stmepic.hpp"
#include "konarm_hat_types.hpp"
#include "can_messages.h"
#include "module_callbacks.hpp"

se::SimpleTask task_module_check_for_errors;

se::motor::VescMotorSettings settings_motor1;
se::motor::VescMotorSettings settings_motor2;
se::motor::VescMotorSettings settings_motor3;
se::motor::VescMotorSettings settings_motor4;
se::motor::VescMotorSettings settings_motor5;
se::motor::VescMotorSettings settings_motor6;

joint_can_config config_joint1;
joint_can_config config_joint2;
joint_can_config config_joint3;
joint_can_config config_joint4;
joint_can_config config_joint5;
joint_can_config config_joint6;

std::shared_ptr<se::memory::FramI2C> fram;
std::vector<joint> joints_arr;
joint joint1;
joint joint2;
joint joint3;
magic_number magic_number_eeprom;
board_id id;
ErrorBoard error_board;

void init_vesc_motor_settings()
{
  settings_motor1.base_address = VESC_MOTOR_1_ID;
  settings_motor2.base_address = VESC_MOTOR_2_ID;
  settings_motor3.base_address = VESC_MOTOR_3_ID;
  settings_motor4.base_address = VESC_MOTOR_4_ID;
  settings_motor5.base_address = VESC_MOTOR_5_ID;
  settings_motor6.base_address = VESC_MOTOR_6_ID;

  settings_motor1.gear_ratio = VESC_MOTOR_1_RATIO;
  settings_motor2.gear_ratio = VESC_MOTOR_2_RATIO;
  settings_motor3.gear_ratio = VESC_MOTOR_3_RATIO;
  settings_motor4.gear_ratio = VESC_MOTOR_4_RATIO;
  settings_motor5.gear_ratio = VESC_MOTOR_5_RATIO;
  settings_motor6.gear_ratio = VESC_MOTOR_6_RATIO;

  settings_motor1.current_to_torque = VESC_MOTOR_1_CRT;
  settings_motor2.current_to_torque = VESC_MOTOR_2_CRT;
  settings_motor3.current_to_torque = VESC_MOTOR_3_CRT;
  settings_motor4.current_to_torque = VESC_MOTOR_4_CRT;
  settings_motor5.current_to_torque = VESC_MOTOR_5_CRT;
  settings_motor6.current_to_torque = VESC_MOTOR_6_CRT;

  settings_motor1.polar_pairs = VESC_MOTOR_1_PAIRS;
  settings_motor2.polar_pairs = VESC_MOTOR_2_PAIRS;
  settings_motor3.polar_pairs = VESC_MOTOR_3_PAIRS;
  settings_motor4.polar_pairs = VESC_MOTOR_4_PAIRS;
  settings_motor5.polar_pairs = VESC_MOTOR_5_PAIRS;
  settings_motor6.polar_pairs = VESC_MOTOR_6_PAIRS;
}

void init_joint_can_config()
{
  config_joint1 = {0xff0,
                   0x000,
                   0x610,
                   0x000,

                   CAN_KONARM_1_STATUS_FRAME_ID,
                   CAN_KONARM_1_SET_POS_FRAME_ID,
                   CAN_KONARM_1_GET_POS_FRAME_ID,
                   CAN_KONARM_1_CLEAR_ERRORS_FRAME_ID,
                   CAN_KONARM_1_GET_ERRORS_FRAME_ID,
                   CAN_KONARM_1_SET_CONTROL_MODE_FRAME_ID,
                   CAN_KONARM_1_SET_EFFECTOR_POSITION_FRAME_ID,
                   CAN_KONARM_1_GET_TORQUE_FRAME_ID,
                   CAN_KONARM_1_GET_CONFIG_FRAME_ID,
                   CAN_KONARM_1_SEND_CONFIG_FRAME_ID,
                   CAN_KONARM_1_SET_AND_RESET_FRAME_ID,
                   CAN_KONARM_1_SET_TORQUE_FRAME_ID};

  config_joint2 = {0xff0,
                   0x000,
                   0x620,
                   0x000,

                   CAN_KONARM_2_STATUS_FRAME_ID,
                   CAN_KONARM_2_SET_POS_FRAME_ID,
                   CAN_KONARM_2_GET_POS_FRAME_ID,
                   CAN_KONARM_2_CLEAR_ERRORS_FRAME_ID,
                   CAN_KONARM_2_GET_ERRORS_FRAME_ID,
                   CAN_KONARM_2_SET_CONTROL_MODE_FRAME_ID,
                   CAN_KONARM_2_SET_EFFECTOR_POSITION_FRAME_ID,
                   CAN_KONARM_2_GET_TORQUE_FRAME_ID,
                   CAN_KONARM_2_GET_CONFIG_FRAME_ID,
                   CAN_KONARM_2_SEND_CONFIG_FRAME_ID,
                   CAN_KONARM_2_SET_AND_RESET_FRAME_ID,
                   CAN_KONARM_2_SET_TORQUE_FRAME_ID};

  config_joint3 = {0xff0,
                   0x000,
                   0x630,
                   0x000,

                   CAN_KONARM_3_STATUS_FRAME_ID,
                   CAN_KONARM_3_SET_POS_FRAME_ID,
                   CAN_KONARM_3_GET_POS_FRAME_ID,
                   CAN_KONARM_3_CLEAR_ERRORS_FRAME_ID,
                   CAN_KONARM_3_GET_ERRORS_FRAME_ID,
                   CAN_KONARM_3_SET_CONTROL_MODE_FRAME_ID,
                   CAN_KONARM_3_SET_EFFECTOR_POSITION_FRAME_ID,
                   CAN_KONARM_3_GET_TORQUE_FRAME_ID,
                   CAN_KONARM_3_GET_CONFIG_FRAME_ID,
                   CAN_KONARM_3_SEND_CONFIG_FRAME_ID,
                   CAN_KONARM_3_SET_AND_RESET_FRAME_ID,
                   CAN_KONARM_3_SET_TORQUE_FRAME_ID};

  config_joint4 = {0xff0,
                   0x000,
                   0x640,
                   0x000,

                   CAN_KONARM_4_STATUS_FRAME_ID,
                   CAN_KONARM_4_SET_POS_FRAME_ID,
                   CAN_KONARM_4_GET_POS_FRAME_ID,
                   CAN_KONARM_4_CLEAR_ERRORS_FRAME_ID,
                   CAN_KONARM_4_GET_ERRORS_FRAME_ID,
                   CAN_KONARM_4_SET_CONTROL_MODE_FRAME_ID,
                   CAN_KONARM_4_SET_EFFECTOR_POSITION_FRAME_ID,
                   CAN_KONARM_4_GET_TORQUE_FRAME_ID,
                   CAN_KONARM_4_GET_CONFIG_FRAME_ID,
                   CAN_KONARM_4_SEND_CONFIG_FRAME_ID,
                   CAN_KONARM_4_SET_AND_RESET_FRAME_ID,
                   CAN_KONARM_4_SET_TORQUE_FRAME_ID};

  config_joint5 = {0xff0,
                   0x000,
                   0x650,
                   0x000,

                   CAN_KONARM_5_STATUS_FRAME_ID,
                   CAN_KONARM_5_SET_POS_FRAME_ID,
                   CAN_KONARM_5_GET_POS_FRAME_ID,
                   CAN_KONARM_5_CLEAR_ERRORS_FRAME_ID,
                   CAN_KONARM_5_GET_ERRORS_FRAME_ID,
                   CAN_KONARM_5_SET_CONTROL_MODE_FRAME_ID,
                   CAN_KONARM_5_SET_EFFECTOR_POSITION_FRAME_ID,
                   CAN_KONARM_5_GET_TORQUE_FRAME_ID,
                   CAN_KONARM_5_GET_CONFIG_FRAME_ID,
                   CAN_KONARM_5_SEND_CONFIG_FRAME_ID,
                   CAN_KONARM_5_SET_AND_RESET_FRAME_ID,
                   CAN_KONARM_5_SET_TORQUE_FRAME_ID};

  config_joint6 = {0xff0,
                   0x000,
                   0x660,
                   0x000,

                   CAN_KONARM_6_STATUS_FRAME_ID,
                   CAN_KONARM_6_SET_POS_FRAME_ID,
                   CAN_KONARM_6_GET_POS_FRAME_ID,
                   CAN_KONARM_6_CLEAR_ERRORS_FRAME_ID,
                   CAN_KONARM_6_GET_ERRORS_FRAME_ID,
                   CAN_KONARM_6_SET_CONTROL_MODE_FRAME_ID,
                   CAN_KONARM_6_SET_EFFECTOR_POSITION_FRAME_ID,
                   CAN_KONARM_6_GET_TORQUE_FRAME_ID,
                   CAN_KONARM_6_GET_CONFIG_FRAME_ID,
                   CAN_KONARM_6_SEND_CONFIG_FRAME_ID,
                   CAN_KONARM_6_SET_AND_RESET_FRAME_ID,
                   CAN_KONARM_6_SET_TORQUE_FRAME_ID};
}

// MODULES FUNCTIONS
void write_board_id()
{
  STMEPIC_NONE_OR_HRESET(fram->writeStruct<magic_number>(0, magic_number_eeprom));
  id.id = 0x0;
  STMEPIC_NONE_OR_HRESET(fram->writeStruct<board_id>(2, id));
  // TODO : Dodać jakiś indykator typu uart/led, że skończył się zapis
}

se::Status add_callbacks()
{
  for (auto &joint : joints_arr)
  {
    if (joint.motor != nullptr)
    {
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_clear_errors_frame_id, can_callback_clear_errors, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_get_config_frame_id, can_callback_get_config, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_get_errors_frame_id, can_callback_get_errors, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_get_pos_frame_id, can_callback_get_pos, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_get_torque_frame_id, can_callback_get_torque, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_send_config_frame_id, can_callback_send_config, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_set_and_reset_frame_id, can_callback_set_and_reset, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_set_control_mode_frame_id, can_callback_set_control_mode, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_set_effector_position_frame_id, can_callback_set_effector_position, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_set_pos_frame_id, can_callback_set_pos, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_set_torque_frame_id, can_callback_set_torque, (void *)&joint));
      STMEPIC_RETURN_ON_ERROR(
          can1->add_callback(joint.config.can_konarm_status_frame_id, can_callback_status, (void *)&joint));
    }
  }
  return se::Status::OK();
}

se::Status read_board_id()
{
  auto maybe_magic_number = fram->readStruct<magic_number>(0);
  magic_number_eeprom = maybe_magic_number.valueOrDie();
  if (magic_number_eeprom.num == MAGIC_NUMER_EEPROM)
  {
    auto maybe_board_id = fram->readStruct<board_id>(2);
    if (!maybe_board_id.ok())
    {
      return se::Status::Invalid(maybe_board_id.status().to_string().c_str());
    }
    id = maybe_board_id.valueOrDie();
  }
  return se::Status::OK();
}

se::Status init_joints_arr()
{
  if (id.id == BOARD_ID_0 or id.id == BOARD_ID_1 or id.id == BOARD_ID_2)
  {
    auto maybe_encoder0 = se::encoders::EncoderAbsoluteMagneticMT6701::Make(i2c);
    if (!maybe_encoder0.ok())
    {
      return se::Status::Invalid(maybe_encoder0.status().to_string().c_str());
    }
    joint1.encoder = maybe_encoder0.valueOrDie();

    auto maybe_motor0 = se::motor::VescMotor::Make(can2);
    if (!maybe_motor0.ok())
    {
      return se::Status::Invalid(maybe_motor0.status().to_string().c_str());
    }
    joint1.motor = maybe_motor0.valueOrDie();

    switch (id.id)
    {
    case BOARD_ID_0:
      joint1.motor->device_set_settings(settings_motor1);
      joint1.motor->set_max_velocity(VESC_MOTOR_1_VEL);
      joint1.config = config_joint1;
      break;
    case BOARD_ID_1:
      joint1.motor->device_set_settings(settings_motor2);
      joint1.motor->set_max_velocity(VESC_MOTOR_2_VEL);
      joint1.config = config_joint2;
      break;
    case BOARD_ID_2:
      joint1.motor->device_set_settings(settings_motor3);
      joint1.motor->set_max_velocity(VESC_MOTOR_3_VEL);
      joint1.config = config_joint3;
      break;
    }

    STMEPIC_RETURN_ON_ERROR(joint1.motor->device_start());
    STMEPIC_RETURN_ON_ERROR(joint1.encoder->device_start());

    joint2.encoder = nullptr;
    joint2.motor = nullptr;

    joint3.encoder = nullptr;
    joint3.motor = nullptr;

    joints_arr.reserve(3);

    joints_arr.push_back(joint1);
    joints_arr.push_back(joint2);
    joints_arr.push_back(joint3);
  }
  else if (id.id == BOARD_ID_3)
  {
    auto maybe_encoder0 = se::encoders::EncoderAbsoluteMagneticMT6701::Make(i2c);
    if (!maybe_encoder0.ok())
    {
      return se::Status::Invalid(maybe_encoder0.status().to_string().c_str());
    }
    joint1.encoder = maybe_encoder0.valueOrDie();
    STMEPIC_RETURN_ON_ERROR(joint1.encoder->device_start());

    auto maybe_encoder1 = se::encoders::EncoderAbsoluteMagneticMT6701::Make(i2c);
    if (!maybe_encoder1.ok())
    {
      return se::Status::Invalid(maybe_encoder1.status().to_string().c_str());
    }
    joint2.encoder = maybe_encoder1.valueOrDie();
    STMEPIC_RETURN_ON_ERROR(joint2.encoder->device_start());

    auto maybe_encoder2 = se::encoders::EncoderAbsoluteMagneticMT6701::Make(i2c);
    if (!maybe_encoder2.ok())
    {
      return se::Status::Invalid(maybe_encoder2.status().to_string().c_str());
    }
    joint3.encoder = maybe_encoder2.valueOrDie();
    STMEPIC_RETURN_ON_ERROR(joint3.encoder->device_start());

    auto maybe_motor0 = se::motor::VescMotor::Make(can2);
    if (!maybe_motor0.ok())
    {
      return se::Status::Invalid(maybe_motor0.status().to_string().c_str());
    }
    joint1.motor = maybe_motor0.valueOrDie();
    joint1.motor->device_set_settings(settings_motor4);
    joint1.motor->set_max_velocity(VESC_MOTOR_4_VEL);
    STMEPIC_RETURN_ON_ERROR(joint1.motor->device_start());

    auto maybe_motor1 = se::motor::VescMotor::Make(can2);
    if (!maybe_motor1.ok())
    {
      return se::Status::Invalid(maybe_motor1.status().to_string().c_str());
    }
    joint2.motor = maybe_motor1.valueOrDie();
    joint2.motor->device_set_settings(settings_motor5);
    joint2.motor->set_max_velocity(VESC_MOTOR_5_VEL);
    STMEPIC_RETURN_ON_ERROR(joint2.motor->device_start());

    auto maybe_motor2 = se::motor::VescMotor::Make(can2);
    if (!maybe_motor2.ok())
    {
      return se::Status::Invalid(maybe_motor2.status().to_string().c_str());
    }
    joint3.motor = maybe_motor2.valueOrDie();
    joint3.motor->device_set_settings(settings_motor6);
    joint3.motor->set_max_velocity(VESC_MOTOR_6_VEL);
    STMEPIC_RETURN_ON_ERROR(joint3.motor->device_start());

    joint1.config = config_joint4;
    joint2.config = config_joint5;
    joint3.config = config_joint6;

    joints_arr.reserve(3);

    joints_arr.push_back(joint1);
    joints_arr.push_back(joint2);
    joints_arr.push_back(joint3);
  }
  return se::Status::OK();
}

// TODO : Dodać jakoś sygnalizacje errorów z silnikiem

se::Status check_for_errors(se::SimpleTask &, void *args)
{
  // Joint errors
  for (auto &joint : joints_arr)
  {
    joint.errors.encoder_motor_disconnect = !joint.encoder->device_is_connected().valueOrDie();

    if (!joint.motor->device_is_connected().valueOrDie())
    {
      // do sth
    }
  }

  return se::Status::OK();
}

se::Status init_board()
{
  STMEPIC_ASSING_TO_OR_RETURN(
      i2c,
      se::I2C::Make(hi2c2, pin_i2c_sda, pin_i2c_scl, se::HardwareType::DMA));

  STMEPIC_ASSING_TO_OR_RETURN(uart,
                              se::UART::Make(huart4, se::HardwareType::DMA));

  STMEPIC_ASSING_OR_RETURN(fram, se::memory::FramI2C::Make(i2c, 0x50, 0, 62144))

#ifdef ONETIME_CODE
  write_board_id();
  while (1)
  {
  };
#else
  init_vesc_motor_settings();
  init_joint_can_config();
  STMEPIC_RETURN_ON_ERROR(read_board_id());
  STMEPIC_RETURN_ON_ERROR(init_joints_arr());
  STMEPIC_RETURN_ON_ERROR(add_callbacks());

  task_module_check_for_errors.task_init(check_for_errors, nullptr, 1,
                                         nullptr, 1048, tskIDLE_PRIORITY, "CheckForErrors");
  task_module_check_for_errors.task_run();

  return se::Status::OK();
#endif
}