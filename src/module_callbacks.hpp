#pragma once
#include "Timing.hpp"
#include "can2.0.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "main_prog.hpp"
#include "simple_task.hpp"
#include "konarm_hat.hpp"
#include "module.hpp"

void can_callback_set_pos(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_set_torque(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_get_pos(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_status(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_clear_errors(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_get_errors(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_default(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_set_control_mode(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_set_effector_position(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_get_torque(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_get_config(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_send_config(se::CanBase &can, se::CanDataFrame &received_msg, void *args);

void can_callback_set_and_reset(se::CanBase &can, se::CanDataFrame &received_msg, void *args);
