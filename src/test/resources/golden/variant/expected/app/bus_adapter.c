/*@Pinfit(file:adapter-source:bus_adapter.adapter.yaml)*/
/*@Pinfit(skeleton-hash:f3cf849ecfa738ac)*/
/**
 * @file bus_adapter.c
 * @brief Adapts bus to bus_hal
 */

#include "bus_adapter.h"

/*@Pinfit usercode+ adapter.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(function:busAdapterSetTarget)*/
void busAdapterSetTarget(bus_adapter_context_t *context, const bus_hal_interface_t *target)
{
  context->target = target;
}

/*@Pinfit(private-function:busAdapterBusWrite)*/
static int32_t busAdapterBusWrite(void *context, const uint8_t *data, uint32_t length)
{
  bus_adapter_context_t *adapter = (bus_adapter_context_t *)context;
  int32_t pinfit_result = -1;

  pinfit_result = busHalSend(adapter->target, data, length);
  return pinfit_result;
}

/*@Pinfit(private-function:busAdapterBusFlush)*/
static int32_t busAdapterBusFlush(void *context)
{
  bus_adapter_context_t *adapter = (bus_adapter_context_t *)context;
  int32_t pinfit_result = -1;
  (void)adapter;

  /*@Pinfit usercode+ function.bus.flush.body*/
  /*@Pinfit usercode-*/
  return pinfit_result;
}

/*@Pinfit(bind-function:busAdapterBindBus)*/
void busAdapterBindBus(bus_interface_t *interface, bus_adapter_context_t *context)
{
  if (interface != NULL)
  {
    interface->context = context;
    interface->write = busAdapterBusWrite;
    interface->flush = busAdapterBusFlush;
  }
}

/*@Pinfit usercode+ adapter.source.footer*/
/*@Pinfit usercode-*/
