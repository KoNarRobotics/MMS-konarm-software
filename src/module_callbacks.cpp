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
        return; // unpack error
    this_joint->motor->set_velocity(signals.velocity);
    this_joint->motor->set_position(signals.position);
    log_debug("Set position:" + std::to_string(signals.position) + "velocity:" + std::to_string(signals.velocity));
}

void can_callback_set_torque(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
   (void) can;
    joint *this_joint=static_cast<joint*>(args);

    can_konarm_1_set_torque_t signals;
    if(can_konarm_1_set_torque_unpack(&signals, received_msg.data, received_msg.data_size))
        return; // unpack error
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
}

void can_callback_clear_errors(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}

void can_callback_get_errors(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}

void can_callback_default(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) args;
    (void) received_msg;
}

void can_callback_set_control_mode(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}

void can_callback_set_effector_position(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}

void can_callback_get_config(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}

void can_callback_send_config(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}

void can_callback_set_and_reset(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}
