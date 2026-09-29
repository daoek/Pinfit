/*@Pinfit(file:command-table-source:uart_cmd.command-table.yaml)*/
/*@Pinfit(skeleton-hash:316a05d76d3e2eaa)*/
/**
 * @file uart_cmd.c
 * @brief UART command table
 */

#include "uart_cmd.h"

/*@Pinfit usercode+ command-table.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(private-function:uartCmdHandlePING)*/
static void uartCmdHandlePING(uart_cmd_context_t *context, const uint8_t *payload, uint32_t length)
{
  (void)context;
  (void)payload;
  (void)length;

  /*@Pinfit usercode+ command.PING.body*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:uartCmdHandleRESET)*/
static void uartCmdHandleRESET(uart_cmd_context_t *context, const uint8_t *payload, uint32_t length)
{
  (void)context;
  (void)payload;
  (void)length;

  /*@Pinfit usercode+ command.RESET.body*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(function:uartCmdDispatch)*/
void uartCmdDispatch(uart_cmd_context_t *context, uart_cmd_command_t command, const uint8_t *payload, uint32_t length)
{
  switch (command)
  {
    case UART_CMD_CMD_PING:
      uartCmdHandlePING(context, payload, length);
      break;

    case UART_CMD_CMD_RESET:
      uartCmdHandleRESET(context, payload, length);
      break;

    default:
    {
      /*@Pinfit usercode+ command.unknown*/
      /*@Pinfit usercode-*/
      break;
    }
  }
}

/*@Pinfit usercode+ command-table.source.footer*/
/*@Pinfit usercode-*/
