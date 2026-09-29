/*@Pinfit(file:adapter-header:bus_adapter.adapter.yaml)*/
/*@Pinfit(skeleton-hash:5a45f2aabb561f7c)*/
/**
 * @file bus_adapter.h
 * @brief Adapts bus to bus_hal
 */

#ifndef BUS_ADAPTER_H_
#define BUS_ADAPTER_H_

#include "bus_I.h"
#include "bus_hal_I.h"

/*@Pinfit usercode+ adapter.header.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(context:bus_adapter)*/
typedef struct
{
    const bus_hal_interface_t *target;
} bus_adapter_context_t;

/*@Pinfit(function:bus_adapter_set_target)*/
void bus_adapter_set_target(bus_adapter_context_t *context, const bus_hal_interface_t *target);

/*@Pinfit(bind-function:bus_adapter_bind_bus)*/
void bus_adapter_bind_bus(bus_interface_t *interface, bus_adapter_context_t *context);

/*@Pinfit usercode+ adapter.header.footer*/
/*@Pinfit usercode-*/

#endif /* BUS_ADAPTER_H_ */
