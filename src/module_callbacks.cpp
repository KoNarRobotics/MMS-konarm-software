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

void mcan::konarm_hat::McCanSlaveInterface_t::callback_write_set_position(commands::SetPosition &variable)
{
}

void mcan::konarm_hat::McCanSlaveInterface_t::callback_write_set_torque(commands::SetTorque &variable)
{
}

void mcan::konarm_hat::McCanSlaveInterface_t::callback_write_set_control_mode(commands::SetControlMode &variable)
{
}

void mcan::konarm_hat::McCanSlaveInterface_t::callback_write_config(configs::Config &variable)
{
}

void mcan::konarm_hat::McCanSlaveInterface_t::callback_write_clear_errors(commands::ClearErrors &variable)
{
}

void mcan::konarm_hat::McCanSlaveInterface_t::callback_write_set_and_reset(commands::SetAndReset &variable)
{
}