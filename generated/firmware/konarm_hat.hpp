// Auto-generated header for konarm_hat module
// Do not edit manually

#pragma once

#include <cstdint>
#include <string>
#include <cstddef>
#include <tuple>
#include "mc_firmware/super_types.hpp"

#include "konarm_hat_types.hpp"

namespace mcan
{

    namespace konarm_hat
    {

        // Hardware information
        struct Hardware_t
        {
            static constexpr const char *k_name = "konarm_hat";
            static constexpr std::uint32_t k_time_stamp = 1768431600;
            static constexpr std::uint32_t k_hw_revision = 1;
            static constexpr std::uint32_t k_fw_revision = 1;
            static constexpr std::uint64_t k_unique_id = 0x6767676767676767;
            static constexpr const char *k_description = "HAT module used in konarm";
        };

        // Message definitions

        namespace commands
        {

            /**
             * @brief Set position
             * @details Command to set position
             * @permission rw
             */
            struct SetPosition
            {
                using Type = Position_t;
                static constexpr const char *k_name = "set_position";
                static constexpr const char *k_group = "commands";
                static constexpr std::uint32_t k_base_address = 0x010;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = true;

                Position_t value;
            };

            /**
             * @brief Set torque
             * @details Command to set torque
             * @permission rw
             */
            struct SetTorque
            {
                using Type = float;
                static constexpr const char *k_name = "set_torque";
                static constexpr const char *k_group = "commands";
                static constexpr std::uint32_t k_base_address = 0x011;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = true;

                float value;
            };

            /**
             * @brief Clear errors
             * @details Command to clear errors
             * @permission rw
             */
            struct ClearErrors
            {
                using Type = bool;
                static constexpr const char *k_name = "clear_errors";
                static constexpr const char *k_group = "commands";
                static constexpr std::uint32_t k_base_address = 0x012;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = true;

                bool value;
            };

            /**
             * @brief Set control mode
             * @details Command to set control mode
             * @permission rw
             */
            struct SetControlMode
            {
                using Type = ControlMode_t;
                static constexpr const char *k_name = "set_control_mode";
                static constexpr const char *k_group = "commands";
                static constexpr std::uint32_t k_base_address = 0x013;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = true;

                ControlMode_t value;
            };

            /**
             * @brief set_and_reset
             * @details set_and_reset
             * @permission rw
             */
            struct SetAndReset
            {
                using Type = bool;
                static constexpr const char *k_name = "set_and_reset";
                static constexpr const char *k_group = "commands";
                static constexpr std::uint32_t k_base_address = 0x014;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = true;

                bool value;
            };

        } // namespace commands

        namespace states
        {

            /**
             * @details Current status of the joint
             * @permission r
             */
            struct JointStatus
            {
                using Type = JointStatusEnum_t;
                static constexpr const char *k_name = "joint_status";
                static constexpr const char *k_group = "states";
                static constexpr std::uint32_t k_base_address = 0x015;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = false;

                JointStatusEnum_t value;
            };

            /**
             * @details Current position of the joint
             * @permission r
             */
            struct CurrentPosition
            {
                using Type = Position_t;
                static constexpr const char *k_name = "current_position";
                static constexpr const char *k_group = "states";
                static constexpr std::uint32_t k_base_address = 0x016;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = false;

                Position_t value;
            };

            /**
             * @details Current torque
             * @units Nm
             * @permission r
             */
            struct Torque
            {
                using Type = float;
                static constexpr const char *k_name = "torque";
                static constexpr const char *k_group = "states";
                static constexpr std::uint32_t k_base_address = 0x017;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = false;

                float value;
            };

            /**
             * @details IDK
             * @permission r
             */
            struct Errors
            {
                using Type = ErrorsStatus_t;
                static constexpr const char *k_name = "errors";
                static constexpr const char *k_group = "states";
                static constexpr std::uint32_t k_base_address = 0x018;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = false;

                ErrorsStatus_t value;
            };

        } // namespace states

        namespace configs
        {

            /**
             * @brief Discover Devices
             * @details This will trigger all devices on the bus to respond with their uniq ID that defines the hardware
             * @permission r
             */
            struct DiscoverDevices
            {
                using Type = uint64_t;
                static constexpr const char *k_name = "discover_devices";
                static constexpr const char *k_group = "configs";
                static constexpr std::uint32_t k_base_address = 0x001;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = false;

                uint64_t value;
            };

            /**
             * @brief Set Device Node ID
             * @details Set the device node ID of the module
             * @permission w
             */
            struct SetDeviceNodeId
            {
                using Type = uint8_t;
                static constexpr const char *k_name = "set_device_node_id";
                static constexpr const char *k_group = "configs";
                static constexpr std::uint32_t k_base_address = 0x002;
                static constexpr bool k_allow_read = false;
                static constexpr bool k_allow_write = true;

                uint8_t value;
            };

            /**
             * @brief Enter Configuration Mode
             * @details Command to enter configuration mode on all connected modules
             * @default 0
             * @permission w
             */
            struct EnterConfigurationMode
            {
                using Type = uint8_t;
                static constexpr const char *k_name = "enter_configuration_mode";
                static constexpr const char *k_group = "configs";
                static constexpr std::uint32_t k_base_address = 0x003;
                static constexpr bool k_allow_read = false;
                static constexpr bool k_allow_write = true;

                uint8_t value = {0};
            };

            /**
             * @details Device Identifier message
             * @permission r
             */
            struct GetHardwareType
            {
                using Type = HardwareType_t;
                static constexpr const char *k_name = "get_hardware_type";
                static constexpr const char *k_group = "configs";
                static constexpr std::uint32_t k_base_address = 0x004;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = false;

                HardwareType_t value;
            };

            /**
             * @brief Ping Module
             * @details Ping the module to check if it's alive, the counter will increment on each ping
             * @default 0
             * @permission w
             */
            struct PingModule
            {
                using Type = uint8_t;
                static constexpr const char *k_name = "ping_module";
                static constexpr const char *k_group = "configs";
                static constexpr std::uint32_t k_base_address = 0x005;
                static constexpr bool k_allow_read = false;
                static constexpr bool k_allow_write = true;

                uint8_t value = {0};
            };

            /**
             * @brief Flash Indicator LED
             * @details Command to flash the indicator LED on the module, useful for locating specific hardware
             * @default 0
             * @permission w
             */
            struct FlashIndicatorLed
            {
                using Type = bool;
                static constexpr const char *k_name = "flash_indicator_led";
                static constexpr const char *k_group = "configs";
                static constexpr std::uint32_t k_base_address = 0x006;
                static constexpr bool k_allow_read = false;
                static constexpr bool k_allow_write = true;

                bool value = {0};
            };

            /**
             * @details Get config
             * @permission rw
             */
            struct Config
            {
                using Type = float;
                static constexpr const char *k_name = "config";
                static constexpr const char *k_group = "configs";
                static constexpr std::uint32_t k_base_address = 0x019;
                static constexpr bool k_allow_read = true;
                static constexpr bool k_allow_write = true;

                float value;
            };

        } // namespace configs

        // CAN MC Slave Interface
        class McCanSlaveInterface_t
        {
        public:
            commands::SetPosition set_position;
            commands::SetTorque set_torque;
            commands::ClearErrors clear_errors;
            commands::SetControlMode set_control_mode;
            commands::SetAndReset set_and_reset;
            states::JointStatus joint_status;
            states::CurrentPosition current_position;
            states::Torque torque;
            states::Errors errors;
            configs::Config config;

            // Write callbacks
            void callback_write_set_position(commands::SetPosition &variable);
            void callback_write_set_torque(commands::SetTorque &variable);
            void callback_write_clear_errors(commands::ClearErrors &variable);
            void callback_write_set_control_mode(commands::SetControlMode &variable);
            void callback_write_set_and_reset(commands::SetAndReset &variable);
            void callback_write_config(configs::Config &variable);

            auto get_write_callbacks()
            {
                return std::make_tuple(
                    std::make_pair(&McCanSlaveInterface_t::callback_write_set_position, &McCanSlaveInterface_t::set_position),
                    std::make_pair(&McCanSlaveInterface_t::callback_write_set_torque, &McCanSlaveInterface_t::set_torque),
                    std::make_pair(&McCanSlaveInterface_t::callback_write_clear_errors, &McCanSlaveInterface_t::clear_errors),
                    std::make_pair(&McCanSlaveInterface_t::callback_write_set_control_mode, &McCanSlaveInterface_t::set_control_mode),
                    std::make_pair(&McCanSlaveInterface_t::callback_write_set_and_reset, &McCanSlaveInterface_t::set_and_reset),
                    std::make_pair(&McCanSlaveInterface_t::callback_write_config, &McCanSlaveInterface_t::config));
            }

            auto get_read_variables()
            {
                return std::make_tuple(
                    &McCanSlaveInterface_t::set_position,
                    &McCanSlaveInterface_t::set_torque,
                    &McCanSlaveInterface_t::clear_errors,
                    &McCanSlaveInterface_t::set_control_mode,
                    &McCanSlaveInterface_t::set_and_reset,
                    &McCanSlaveInterface_t::joint_status,
                    &McCanSlaveInterface_t::current_position,
                    &McCanSlaveInterface_t::torque,
                    &McCanSlaveInterface_t::errors,
                    &McCanSlaveInterface_t::config);
            }

            auto get_state_variables()
            {
                return std::make_tuple(
                    &McCanSlaveInterface_t::joint_status,
                    &McCanSlaveInterface_t::current_position,
                    &McCanSlaveInterface_t::torque,
                    &McCanSlaveInterface_t::errors);
            }

            auto get_command_variables()
            {
                return std::make_tuple(
                    &McCanSlaveInterface_t::set_position,
                    &McCanSlaveInterface_t::set_torque,
                    &McCanSlaveInterface_t::clear_errors,
                    &McCanSlaveInterface_t::set_control_mode,
                    &McCanSlaveInterface_t::set_and_reset);
            }

            auto get_config_variables()
            {
                return std::make_tuple(
                    &McCanSlaveInterface_t::config);
            }
        };
    } // namespace konarm_hat

} // namespace mcan