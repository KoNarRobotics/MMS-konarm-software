#include "Timing.hpp"
// #include "can2.0.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "main_prog.hpp"
#include "simple_task.hpp"
#include "konarm_hat.hpp"
#include "module.hpp"

namespace se = stmepic;

// TODO : Uzupełnić callbacki

void JointInterface::callback_write_set_position(mcan::konarm_hat::commands::SetPosition &variable)
{
}

void JointInterface::callback_write_set_torque(mcan::konarm_hat::commands::SetTorque &variable)
{
}

void JointInterface::callback_write_set_control_mode(mcan::konarm_hat::commands::SetControlMode &variable)
{
}

void JointInterface::callback_write_config(mcan::konarm_hat::configs::Config &variable)
{
}

void JointInterface::callback_write_clear_errors(mcan::konarm_hat::commands::ClearErrors &variable)
{
}

void JointInterface::callback_write_set_and_reset(mcan::konarm_hat::commands::SetAndReset &variable)
{
}