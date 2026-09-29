/*@Pinfit(file:command-table-header:uart_cmd.command-table.yaml)*/
/*@Pinfit(skeleton-hash:39369c86cc3708b1)*/
/**
 * @file uart_cmd.h
 * @brief UART command table
 */

#ifndef UART_CMD_H_
#define UART_CMD_H_

#include <stdint.h>

/*@Pinfit usercode+ command-table.header.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(command-enum:uart_cmd)*/
/** @brief Commands of uart_cmd */
typedef enum
{
  UART_CMD_CMD_PING = 0,
  UART_CMD_CMD_RESET = 16
} uart_cmd_command_t;

/*@Pinfit(context:uart_cmd)*/
typedef struct
{
  unsigned char reserved;
} uart_cmd_context_t;

/*@Pinfit(function:uartCmdDispatch)*/
void uartCmdDispatch(uart_cmd_context_t *context, uart_cmd_command_t command, const uint8_t *payload, uint32_t length);

/*@Pinfit usercode+ command-table.header.footer*/
/*@Pinfit usercode-*/

#endif /* UART_CMD_H_ */
