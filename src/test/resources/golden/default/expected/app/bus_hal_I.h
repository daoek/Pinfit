/*@Pinfit(file:interface:bus_hal.interface.yaml)*/
/*@Pinfit(skeleton-hash:2f406718a4345080)*/
/**
 * @file bus_hal_I.h
 * @brief bus_hal interface
 */

#ifndef BUS_HAL_I_H_
#define BUS_HAL_I_H_

#include <stddef.h>
#include <stdint.h>

/*@Pinfit usercode+ interface.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ interface.declarations*/
/*@Pinfit usercode-*/

/*@Pinfit(interface-table:bus_hal)*/
typedef struct
{
    void *context;
    int32_t (*send)(void *context, const uint8_t *data, uint32_t length);
} bus_hal_interface_t;

/*@Pinfit(function:send)*/
/**
 * @brief send
 * @param data data
 * @param length length
 * @return int32_t result.
 */
static inline int32_t bus_hal_send(const bus_hal_interface_t * const interface, const uint8_t *data, uint32_t length)
{
    int32_t pinfit_result = -1;

    if (interface != NULL)
    {
        if ((interface->context != NULL) && (interface->send != NULL))
        {
            pinfit_result = interface->send(interface->context, data, length);
        }
        else
        {
            pinfit_result = -2;
        }
    }

    return pinfit_result;
}

/*@Pinfit usercode+ interface.footer*/
/*@Pinfit usercode-*/

#endif /* BUS_HAL_I_H_ */
