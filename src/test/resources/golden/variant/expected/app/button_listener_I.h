/*@Pinfit(file:interface:button_listener.interface.yaml)*/
/*@Pinfit(skeleton-hash:04a489aa73dc6ae0)*/
/**
 * @file button_listener_I.h
 * @brief button_listener interface
 */

#ifndef BUTTON_LISTENER_I_H_
#define BUTTON_LISTENER_I_H_

#include <stddef.h>
#include <stdint.h>

/*@Pinfit usercode+ interface.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ interface.declarations*/
/*@Pinfit usercode-*/

/*@Pinfit(interface-table:button_listener)*/
typedef struct
{
  void *context;
  void (*pressed)(void *context, uint8_t button_id);
  void (*released)(void *context, uint8_t button_id, uint32_t held_ms);
} button_listener_interface_t;

/*@Pinfit(function:pressed)*/
/**
 * @brief pressed
 * @param button_id button_id
 */
static inline void buttonListenerPressed(const button_listener_interface_t * const interface, uint8_t button_id)
{
  if (interface != NULL)
  {
    if ((interface->context != NULL) && (interface->pressed != NULL))
    {
      interface->pressed(interface->context, button_id);
    }
  }
}

/*@Pinfit(function:released)*/
/**
 * @brief released
 * @param button_id button_id
 * @param held_ms held_ms
 */
static inline void buttonListenerReleased(const button_listener_interface_t * const interface, uint8_t button_id, uint32_t held_ms)
{
  if (interface != NULL)
  {
    if ((interface->context != NULL) && (interface->released != NULL))
    {
      interface->released(interface->context, button_id, held_ms);
    }
  }
}

/*@Pinfit usercode+ interface.footer*/
/*@Pinfit usercode-*/

#endif /* BUTTON_LISTENER_I_H_ */
