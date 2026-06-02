#include "Timing.hpp"
#include "can2.0.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "main_prog.hpp"
#include "simple_task.hpp"
#include "konarm_hat.hpp"
#include "module.hpp"
#include "module_callbacks.hpp"
#include "can_messages.h"
//to do: can timeout??
void can_callback_set_pos(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    joint *this_joint=static_cast<joint*>(args);

    can_konarm_1_set_pos_t signals;
    if(can_konarm_1_set_pos_unpack(&signals, received_msg.data, received_msg.data_size))
    {
        this_joint->motor->status = Status::ExecutionError("Failed to unpack set torque message");
        return;
    }
    //this_joint->motor->set_velocity(signals.velocity); // not needed
    this_joint->motor->set_position(signals.position);
    log_debug("Set position:" + std::to_string(signals.position) + "velocity:" + std::to_string(signals.velocity));
}

void can_callback_set_torque(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
   (void) can;
    joint *this_joint=static_cast<joint*>(args);

    can_konarm_1_set_torque_t signals;
    if(can_konarm_1_set_torque_unpack(&signals, received_msg.data, received_msg.data_size))
    {
        this_joint->motor->status = Status::ExecutionError("Failed to unpack set torque message");
        return;
    }
    this_joint->motor->set_torque(signals.torque);
    log_debug("Set torque:" + std::to_string(signals.torque));
}

void can_callback_get_pos(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;
    joint *this_joint=static_cast<joint*>(args);

    can_konarm_1_get_pos_t src_p;
    se::CanDataFrame send_msg;
    src_p.position=this_joint->motor->get_position();
    src_p.velocity=this_joint->motor->get_velocity();
    send_msg.frame_id  = this_joint->config.can_konarm_get_pos_frame_id; 
    send_msg.data_size = CAN_KONARM_1_GET_POS_LENGTH;
    send_msg.fdcan_frame = false;
    send_msg.extended_id = CAN_KONARM_1_GET_POS_IS_EXTENDED;
    (void)can_konarm_1_get_pos_pack(send_msg.data, &src_p, send_msg.data_size);
    (void)can.write(send_msg);
}

void can_callback_get_torque(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;
    joint *this_joint=static_cast<joint*>(args);

    can_konarm_1_get_torque_t src_p;
    se::CanDataFrame send_msg;
    src_p.torque=this_joint->motor->get_torque();
    send_msg.frame_id    = this_joint->config.can_konarm_get_torque_frame_id; 
    send_msg.data_size   = CAN_KONARM_1_GET_TORQUE_LENGTH;
    send_msg.fdcan_frame = false;
    send_msg.extended_id = CAN_KONARM_1_GET_TORQUE_IS_EXTENDED;
    (void)can_konarm_1_get_torque_pack(send_msg.data, &src_p, send_msg.data_size);
    (void)can.write(send_msg);
}
void can_callback_status(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;
    (void) args;
    //status
}

void can_callback_clear_errors(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;

    joint *this_joint=static_cast<joint*>(args);
    this_joint->errors.can_error=false;
    this_joint->errors.can_disconnected=false;
}

void can_callback_get_errors(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) received_msg;
    joint *this_joint=static_cast<joint*>(args);
    se::CanDataFrame send_msg;
    can_konarm_1_get_errors_t src_p;

    // According to module.hpp:
    
    // There are none temp sensors on boards.
    // Temp variables are kept for ROS compatibility
    // Same with voltage

    send_msg.frame_id                    = this_joint->config.can_konarm_get_errors_frame_id;

    src_p.temp_engine_overheating        = this_joint->errors.temp_engine_overheating;
    src_p.temp_driver_overheating        = this_joint->errors.temp_driver_overheating;
    src_p.can_disconnected               = this_joint->errors.can_disconnected;
    src_p.can_error                      = this_joint->errors.can_error;
    src_p.encoder_arm_disconnect         = this_joint->errors.encoder_arm_disconnect;
    src_p.encoder_motor_disconnect       = this_joint->errors.encoder_motor_disconnect;

    src_p.temp_board_overheating         = error_board.temp_board_overheating;
    src_p.temp_engine_sensor_disconnect  = error_board.temp_engine_sensor_disconnect;
    src_p.temp_driver_sensor_disconnect  = error_board.temp_driver_sensor_disconnect;
    src_p.temp_board_sensor_disconnect   = error_board.temp_board_sensor_disconnect;
    src_p.board_overvoltage              = error_board.board_overvoltage;
    src_p.board_undervoltage             = error_board.board_undervoltage;
    src_p.controler_motor_limit_position = error_board.controler_motor_limit_position;
    send_msg.data_size                    = CAN_KONARM_1_GET_ERRORS_LENGTH;
    (void)can_konarm_1_get_errors_pack(send_msg.data, &src_p, send_msg.data_size);
    (void)can.write(send_msg);
}

void can_callback_default(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;

    joint *this_joint=static_cast<joint*>(args);
    this_joint->errors.can_error=true;
}

void can_callback_set_control_mode(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
        (void) can;
    (void) received_msg;
    (void) args;
    //to do,
    //VescMotor already sets control mode in set_position, set_torque and set_velocity functions
    //functionality could be to check if the control mode is correct?
    // furthermore no init_and_set_movement_controler_mode() presetn in module.cpp
}

void can_callback_set_effector_position(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;
    (void) args;
    // no servo?
}

void can_callback_get_config(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;
    (void) args;
    //config
}

void can_callback_send_config(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{

    (void) can;
    (void) received_msg;
    (void) args;
    //config
    //brak getterow w vesc_blcd.cpp dla max_velocity, gear_ratio
}

void can_callback_set_and_reset(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{

    (void) can;
    (void) received_msg;
    (void) args;
    //nothing to save?
    HAL_NVIC_SystemReset();
}
