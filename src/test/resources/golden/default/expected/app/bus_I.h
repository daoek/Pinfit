/*@Pinfit(file:interface:bus.interface.yaml)*/
/*@Pinfit(skeleton-hash:bec3e3e05b5f8c72)*/
/**
 * @file bus_I.h
 * @brief bus interface
 */

#ifndef BUS_I_H_
#define BUS_I_H_

#include <stddef.h>
#include <stdint.h>

/*@Pinfit usercode+ interface.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ interface.declarations*/
/*@Pinfit usercode-*/

/*@Pinfit(interface-table:bus)*/
typedef struct
{
    void *context;
    int32_t (*write)(void *context, const uint8_t *data, uint32_t length);
    int32_t (*flush)(void *context);
} bus_interface_t;

/*@Pinfit(function:write)*/
/**
 * @brief write
 * @param data data
 * @param length length
 * @return int32_t result.
 */
static inline int32_t bus_write(const bus_interface_t * const interface, const uint8_t *data, uint32_t length)
{
    int32_t pinfit_result = -1;

    if (interface != NULL)
    {
        if ((interface->context != NULL) && (interface->write != NULL))
        {
            pinfit_result = interface->write(interface->context, data, length);
        }
        else
        {
            pinfit_result = -2;
        }
    }

    return pinfit_result;
}

/*@Pinfit(function:flush)*/
/**
 * @brief flush
 * @return int32_t result.
 */
static inline int32_t bus_flush(const bus_interface_t * const interface)
{
    int32_t pinfit_result = -1;

    if (interface != NULL)
    {
        if ((interface->context != NULL) && (interface->flush != NULL))
        {
            pinfit_result = interface->flush(interface->context);
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

#endif /* BUS_I_H_ */
