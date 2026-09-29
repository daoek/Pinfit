/*@Pinfit(file:command-table-source:uart_cmd.command-table.yaml)*/
/*@Pinfit(skeleton-hash:88df68f0e4b75d78)*/
/**
 * @file uart_cmd.c
 * @brief UART command table
 */

#include "uart_cmd.h"

/*@Pinfit usercode+ command-table.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(private-function:uart_cmd_handle_PING)*/
static void uart_cmd_handle_PING(uart_cmd_context_t *context, const uint8_t *payload, uint32_t length)
{
    (void)context;
    (void)payload;
    (void)length;

    /*@Pinfit usercode+ command.PING.body*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:uart_cmd_handle_RESET)*/
static void uart_cmd_handle_RESET(uart_cmd_context_t *context, const uint8_t *payload, uint32_t length)
{
    (void)context;
    (void)payload;
    (void)length;

    /*@Pinfit usercode+ command.RESET.body*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(function:uart_cmd_dispatch)*/
void uart_cmd_dispatch(uart_cmd_context_t *context, uart_cmd_command_t command, const uint8_t *payload, uint32_t length)
{
    switch (command)
    {
        case UART_CMD_CMD_PING:
            uart_cmd_handle_PING(context, payload, length);
            break;

        case UART_CMD_CMD_RESET:
            uart_cmd_handle_RESET(context, payload, length);
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
