/*@Pinfit(file:observer-source:button_events.observer.yaml)*/
/*@Pinfit(skeleton-hash:0f220a0a41063ed5)*/
/**
 * @file button_events.c
 * @brief Button event fan-out
 */

#include "button_events.h"
#include <stddef.h>

/*@Pinfit usercode+ observer.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(function:button_events_init)*/
void button_events_init(button_events_context_t *context)
{
    context->count = 0U;
}

/*@Pinfit(function:button_events_subscribe)*/
bool button_events_subscribe(button_events_context_t *context, const button_listener_interface_t *subscriber)
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

/*@Pinfit(function:button_events_unsubscribe)*/
bool button_events_unsubscribe(button_events_context_t *context, const button_listener_interface_t *subscriber)
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

/*@Pinfit(function:button_events_publish_pressed)*/
void button_events_publish_pressed(button_events_context_t *context, uint8_t button_id)
{
    uint32_t index;

    for (index = 0U; index < context->count; index++)
    {
        button_listener_pressed(context->subscribers[index], button_id);
    }
}

/*@Pinfit(function:button_events_publish_released)*/
void button_events_publish_released(button_events_context_t *context, uint8_t button_id, uint32_t held_ms)
{
    uint32_t index;

    for (index = 0U; index < context->count; index++)
    {
        button_listener_released(context->subscribers[index], button_id, held_ms);
    }
}

/*@Pinfit usercode+ observer.source.footer*/
/*@Pinfit usercode-*/
