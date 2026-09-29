/*@Pinfit(file:adapter-source:bus_adapter.adapter.yaml)*/
/*@Pinfit(skeleton-hash:068d0c4f4400b48c)*/
/**
 * @file bus_adapter.c
 * @brief Adapts bus to bus_hal
 */

#include "bus_adapter.h"

/*@Pinfit usercode+ adapter.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(function:bus_adapter_set_target)*/
void bus_adapter_set_target(bus_adapter_context_t *context, const bus_hal_interface_t *target)
{
    context->target = target;
}

/*@Pinfit(private-function:bus_adapter_bus_write)*/
static int32_t bus_adapter_bus_write(void *context, const uint8_t *data, uint32_t length)
{
    bus_adapter_context_t *adapter = (bus_adapter_context_t *)context;
    int32_t pinfit_result = -1;

    pinfit_result = bus_hal_send(adapter->target, data, length);
    return pinfit_result;
}

/*@Pinfit(private-function:bus_adapter_bus_flush)*/
static int32_t bus_adapter_bus_flush(void *context)
{
    bus_adapter_context_t *adapter = (bus_adapter_context_t *)context;
    int32_t pinfit_result = -1;
    (void)adapter;

    /*@Pinfit usercode+ function.bus.flush.body*/
    /*@Pinfit usercode-*/
    return pinfit_result;
}

/*@Pinfit(bind-function:bus_adapter_bind_bus)*/
void bus_adapter_bind_bus(bus_interface_t *interface, bus_adapter_context_t *context)
{
    if (interface != NULL)
    {
        interface->context = context;
        interface->write = bus_adapter_bus_write;
        interface->flush = bus_adapter_bus_flush;
    }
}

/*@Pinfit usercode+ adapter.source.footer*/
/*@Pinfit usercode-*/
