// Auto-generated types for konarm_hat module
//
// DO NOT EDIT MANUALLY
//

#pragma once

#include <cstdint>
#include <string>
#include <cstddef>
#include "mc_firmware/super_types.hpp"

#include "mcan_basic_module_types.hpp"
#include "mcan_ros_types_types.hpp"

namespace mcan {

namespace konarm_hat {

// Using declarations for included namespaces
using namespace mcan::mcan_basic_module;
using namespace mcan::mcan_ros_types;

// Forward declarations
enum class JointStatusEnum_t : std::uint8_t;
struct Position_t;
enum class ErrorStatus_t : std::uint8_t;
struct ErrorsStatus_t;
enum class ControlMode_t : std::uint8_t;

// Type definitions

/**
 * @brief Joint Status Enum
 * Possible states of the joints
 */
enum class JointStatusEnum_t : std::uint8_t {
    OK = 0,
    FAULT = 1,
    OVERHEAT = 2,
    EMERGENCY_STOP = 3,
};

/**
 * @brief Position of joint
 * Position of joint: position, and velocity
 */
struct Position_t {

    /**
     * @brief ?
     * @type float
     */
    float position;

    /**
     * @brief ?
     * @type float
     */
    float velocity;
};

/**
 * @brief Error Status
 */
enum class ErrorStatus_t : std::uint8_t {
    OK = 0,
    FAULT = 1,
};

/**
 * @brief Struct error statuses
 * Struct of diffrent error statuses
 */
struct ErrorsStatus_t {

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t temp_engine_overheating;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t temp_driver_overheating;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t temp_board_overheating;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t temp_engine_sensor_disconnect;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t temp_driver_sensor_disconnect;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t temp_board_sensor_disconnect;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t encoder_arm_disconnect;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t board_overvoltage;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t board_undervoltage;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t can_disconnected;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t can_error;

    /**
     * @type ErrorStatus_t
     */
    ErrorStatus_t controler_motor_limit_position;
};

/**
 * @brief Control Mode
 */
enum class ControlMode_t : std::uint8_t {
    VELOCITY_CONTROL = 0,
    POSITION_CONTROL = 1,
    TORQUE_CONTROL = 2,
};

}  // namespace konarm_hat

}  // namespace mcan