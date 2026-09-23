/* generated common source file - do not edit */
#include "common_data.h"
/** IOPORT interface configuration for event link **/
ioport_event_group_output_t g_port_group_output_cfg[] =
{
{ .pin_select = (uint8_t) (IOPORT_EVENT_PIN_SELECTION_NONE), .operation = IOPORT_EVENT_OUTPUT_OPERATION_LOW },

  { .pin_select = (uint8_t) (IOPORT_EVENT_PIN_SELECTION_NONE), .operation = IOPORT_EVENT_OUTPUT_OPERATION_LOW }, };
ioport_event_group_input_t g_port_group_input_cfg[] =
{
{ .event_control = IOPORT_EVENT_CONTROL_DISABLE,
  .pin_select = (uint8_t) (IOPORT_EVENT_PIN_SELECTION_NONE),
  .edge_detection = IOPORT_EVENT_DETECTION_RISING_EDGE,
  .overwrite_control = IOPORT_EVENT_CONTROL_DISABLE,
  .buffer_init_value = IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 7U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 6U
          | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 5U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 4U
          | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 3U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 2U
          | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 1U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW },

  { .event_control = IOPORT_EVENT_CONTROL_DISABLE,
    .pin_select = (uint8_t) (IOPORT_EVENT_PIN_SELECTION_NONE),
    .edge_detection = IOPORT_EVENT_DETECTION_RISING_EDGE,
    .overwrite_control = IOPORT_EVENT_CONTROL_DISABLE,
    .buffer_init_value = IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 7U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 6U
            | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 5U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 4U
            | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 3U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 2U
            | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW << 1U | IOPORT_EVENT_INITIAL_BUFFER_VALUE_LOW }, };
ioport_event_single_t g_single_port_cfg[] =
{
{ .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction = IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
          (uint16_t) BSP_IO_PORT_06_PIN_00,
  .operation = IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection = IOPORT_EVENT_DETECTION_RISING_EDGE },

  { .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction = IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
            (uint16_t) BSP_IO_PORT_06_PIN_00,
    .operation = IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection = IOPORT_EVENT_DETECTION_RISING_EDGE },

  { .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction = IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
            (uint16_t) BSP_IO_PORT_06_PIN_00,
    .operation = IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection = IOPORT_EVENT_DETECTION_RISING_EDGE },

  { .event_control = IOPORT_EVENT_CONTROL_DISABLE, .direction = IOPORT_EVENT_DIRECTION_OUTPUT, .port_num =
            (uint16_t) BSP_IO_PORT_06_PIN_00,
    .operation = IOPORT_EVENT_OUTPUT_OPERATION_LOW, .edge_detection = IOPORT_EVENT_DETECTION_RISING_EDGE } };

const ioport_extend_cfg_t g_ioport_cfg_extend =
{ .p_port_group_output_cfg = &g_port_group_output_cfg[0],
  .p_port_group_input_cfg = &g_port_group_input_cfg[0],
  .p_single_port_cfg = &g_single_port_cfg[0] };

ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_ctrl = &g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };
void g_common_init(void)
{
}
