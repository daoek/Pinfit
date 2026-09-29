/*@Pinfit(file:observer-source:button_events.observer.yaml)*/
/*@Pinfit(skeleton-hash:ad580a0ec160ce7e)*/
/**
 * @file button_events.c
 * @brief Button event fan-out
 */

#include "button_events.h"
#include <stddef.h>

/*@Pinfit usercode+ observer.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(function:buttonEventsInit)*/
void buttonEventsInit(button_events_context_t *context)
{
  context->count = 0U;
}

/*@Pinfit(function:buttonEventsSubscribe)*/
bool buttonEventsSubscribe(button_events_context_t *context, const button_listener_interface_t *subscriber)
{
  bool pinfit_result = false;

  if ((subscriber != NULL) && (context->count < BUTTON_EVENTS_CAPACITY))
  {
    uint32_t index;
    bool already_subscribed = false;

    for (index = 0U; index < context->count; index++)
    {
      if (context->subscribers[index] == subscriber)
      {
        already_subscribed = true;
      }
    }

    if (!already_subscribed)
    {
      context->subscribers[context->count] = subscriber;
      context->count++;
      pinfit_result = true;
    }
  }

  return pinfit_result;
}

/*@Pinfit(function:buttonEventsUnsubscribe)*/
bool buttonEventsUnsubscribe(button_events_context_t *context, const button_listener_interface_t *subscriber)
{
  bool pinfit_result = false;
  uint32_t index;

  for (index = 0U; index < context->count; index++)
  {
    if (context->subscribers[index] == subscriber)
    {
      context->count--;
      context->subscribers[index] = context->subscribers[context->count];
      pinfit_result = true;
      break;
    }
  }

  return pinfit_result;
}

/*@Pinfit(function:buttonEventsPublishPressed)*/
void buttonEventsPublishPressed(button_events_context_t *context, uint8_t button_id)
{
  uint32_t index;

  for (index = 0U; index < context->count; index++)
  {
    buttonListenerPressed(context->subscribers[index], button_id);
  }
}

/*@Pinfit(function:buttonEventsPublishReleased)*/
void buttonEventsPublishReleased(button_events_context_t *context, uint8_t button_id, uint32_t held_ms)
{
  uint32_t index;

  for (index = 0U; index < context->count; index++)
  {
    buttonListenerReleased(context->subscribers[index], button_id, held_ms);
  }
}

/*@Pinfit usercode+ observer.source.footer*/
/*@Pinfit usercode-*/
