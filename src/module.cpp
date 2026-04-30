#include "fram_i2c.hpp"
#include "memory_fram.hpp"
#include "module.hpp"
#include "i2c.hpp"
#include "main.hpp"
#include "main_prog.hpp"
#include "status.hpp"
#include "stmepic.hpp"

se::SimpleTask task_module_control_loop;
se::SimpleTask task_module_update_values_for_can;

se::motor::VescMotorSettings settings_motor1;
se::motor::VescMotorSettings settings_motor2;
se::motor::VescMotorSettings settings_motor3;
se::motor::VescMotorSettings settings_motor4;
se::motor::VescMotorSettings settings_motor5;
se::motor::VescMotorSettings settings_motor6;

std::shared_ptr<se::memory::FramI2C> fram;
std::vector<joint> joints_arr;
joint joint1;
joint joint2;
joint joint3;
magic_number magic_number_eeprom;
board_id id;

// Default settings for VESC motors
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

// MODULES FUNCTIONS
void write_board_id()
{
  STMEPIC_NONE_OR_HRESET(fram->writeStruct<magic_number>(0, magic_number_eeprom));
  id.id = 0x0;
  STMEPIC_NONE_OR_HRESET(fram->writeStruct<board_id>(2, id));
  // TODO : Dodać jakiś indykator typu uart/led, że skończył się zapis
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
    auto maybe_module0 = ModuleType::Make(nullptr, get_unique_id());
    if (!maybe_module0.ok())
    {
      return se::Status::Invalid(maybe_module0.status().to_string().c_str());
    }
    joint1.module = maybe_module0.valueOrDie();
    joint1.joint_can_interface.joint_idx = 0;

    joint2.module = nullptr;
    joint3.module = nullptr;

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
      break;
    case BOARD_ID_1:
      joint1.motor->device_set_settings(settings_motor2);
      joint1.motor->set_max_velocity(VESC_MOTOR_2_VEL);
      break;
    case BOARD_ID_2:
      joint1.motor->device_set_settings(settings_motor3);
      joint1.motor->set_max_velocity(VESC_MOTOR_3_VEL);
      break;
    }

    joint1.motor->device_start();
    joint2.encoder->device_start();

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
    auto maybe_module0 = ModuleType::Make(nullptr, get_unique_id());
    if (!maybe_module0.ok())
    {
      return se::Status::Invalid(maybe_module0.status().to_string().c_str());
    }
    joint1.module = maybe_module0.valueOrDie();
    joint1.joint_can_interface.joint_idx = 0;

    auto maybe_module1 = ModuleType::Make(nullptr, get_unique_id());
    if (!maybe_module1.ok())
    {
      return se::Status::Invalid(maybe_module1.status().to_string().c_str());
    }
    joint2.module = maybe_module1.valueOrDie();
    joint2.joint_can_interface.joint_idx = 1;

    auto maybe_module2 = ModuleType::Make(nullptr, get_unique_id());
    if (!maybe_module2.ok())
    {
      return se::Status::Invalid(maybe_module2.status().to_string().c_str());
    }
    joint3.module = maybe_module2.valueOrDie();
    joint3.joint_can_interface.joint_idx = 2;

    auto maybe_encoder0 = se::encoders::EncoderAbsoluteMagneticMT6701::Make(i2c);
    if (!maybe_encoder0.ok())
    {
      return se::Status::Invalid(maybe_encoder0.status().to_string().c_str());
    }
    joint1.encoder = maybe_encoder0.valueOrDie();
    joint1.encoder->device_start();

    auto maybe_encoder1 = se::encoders::EncoderAbsoluteMagneticMT6701::Make(i2c);
    if (!maybe_encoder1.ok())
    {
      return se::Status::Invalid(maybe_encoder1.status().to_string().c_str());
    }
    joint2.encoder = maybe_encoder1.valueOrDie();
    joint2.encoder->device_start();

    auto maybe_encoder2 = se::encoders::EncoderAbsoluteMagneticMT6701::Make(i2c);
    if (!maybe_encoder2.ok())
    {
      return se::Status::Invalid(maybe_encoder2.status().to_string().c_str());
    }
    joint3.encoder = maybe_encoder2.valueOrDie();
    joint3.encoder->device_start();

    auto maybe_motor0 = se::motor::VescMotor::Make(can2);
    if (!maybe_motor0.ok())
    {
      return se::Status::Invalid(maybe_motor0.status().to_string().c_str());
    }
    joint1.motor = maybe_motor0.valueOrDie();
    joint1.motor->device_set_settings(settings_motor4);
    joint1.motor->set_max_velocity(VESC_MOTOR_4_VEL);
    joint1.motor->device_start();

    auto maybe_motor1 = se::motor::VescMotor::Make(can2);
    if (!maybe_module1.ok())
    {
      return se::Status::Invalid(maybe_motor1.status().to_string().c_str());
    }
    joint2.motor = maybe_motor1.valueOrDie();
    joint2.motor->device_set_settings(settings_motor5);
    joint2.motor->set_max_velocity(VESC_MOTOR_5_VEL);
    joint2.motor->device_start();

    auto maybe_motor2 = se::motor::VescMotor::Make(can2);
    if (!maybe_motor2.ok())
    {
      return se::Status::Invalid(maybe_motor2.status().to_string().c_str());
    }
    joint3.motor = maybe_motor2.valueOrDie();
    joint3.motor->device_set_settings(settings_motor6);
    joint3.motor->set_max_velocity(VESC_MOTOR_6_VEL);
    joint3.motor->device_start();

    joints_arr.reserve(3);

    joints_arr.push_back(joint1);
    joints_arr.push_back(joint2);
    joints_arr.push_back(joint3);
  }
  return se::Status::OK();
}

se::Status module_task(se::SimpleTask &, void *args)
{
  (void)args;
  for (auto &joint : joints_arr)
    joint.module->control_loop();
  return se::Status::OK();
}

se::Status update_values_for_can(se::SimpleTask &, void *args)
{
  (void)args;
  // TODO : dodać zczytywanie wartości
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
  STMEPIC_RETURN_ON_ERROR(read_board_id());
  STMEPIC_RETURN_ON_ERROR(init_joints_arr());

  task_module_control_loop.task_init(module_task, nullptr, 1, nullptr, 1048,
                                     tskIDLE_PRIORITY + 1, "ModuleControlLoop");
  task_module_control_loop.task_run();

  for (auto &joint : joints_arr)
  {
    if (joint.module != nullptr)
    {
      joint.module->start_driver();
    }
  }

  task_module_update_values_for_can.task_init(update_values_for_can, nullptr, 1,
                                              nullptr, 1048, tskIDLE_PRIORITY, "UpdateValuesForCan");
  task_module_update_values_for_can.task_run();

  return se::Status::OK();
#endif
}