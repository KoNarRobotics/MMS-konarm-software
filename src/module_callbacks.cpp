#include "Timing.hpp"
#include "can2.0.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "main_prog.hpp"
#include "simple_task.hpp"
#include "konarm_hat.hpp"
#include "module.hpp"
#include "module_callbacks.hpp"
//to do: can timeout??
void can_callback_set_pos(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
 
}

void can_callback_set_torque(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
   (void) can;
    joint *this_joint=static_cast<joint>(*args);

    can_konarm_1_set_torque_t signals;
    if(can_konarm_1_set_torque_unpack(&signals, received_msg.data, received_msg.data_size))
        return; // unpack error
    this_joint->motor->set_torque(signals);
    log_debug("Set torque:" + std::to_string(signals.torque));
}

void can_callback_get_pos(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
}

void can_callback_get_torque(se::CanBase &can, se::CanDataFrame &received_msg, void *args)
{
    (void) can;
    (void) received_msg;
    joint *this_joint=static_cast<joint>(*args);

    can_konarm_1_get_torque_t src_p;
    se::CanDataFrame send_msg;
    src_p.torque=this_joint->motor->get_torque(); //float;
    send_msg.frame_id    = config.can_konarm_get_torque_frame_id;
    send_msg.data_size   = CAN_KONARM_1_GET_TORQUE_LENGTH;
    send_msg.fdcan_frame = false;
    send_msg.extended_id = CAN_KONARM_1_GET_TORQUE_IS_EXTENDED;
    can_konarm_1_get_torque_pack(send_msg.data, &src_p, send_msg.data_size);
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
