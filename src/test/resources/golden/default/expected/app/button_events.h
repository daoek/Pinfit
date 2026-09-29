/*@Pinfit(file:observer-header:button_events.observer.yaml)*/
/*@Pinfit(skeleton-hash:9694c84e5c6622b2)*/
/**
 * @file button_events.h
 * @brief Button event fan-out
 */

#ifndef BUTTON_EVENTS_H_
#define BUTTON_EVENTS_H_

#include <stdbool.h>
#include <stdint.h>
#include "button_listener_I.h"

/*@Pinfit usercode+ observer.header.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(macro:BUTTON_EVENTS_CAPACITY)*/
#define BUTTON_EVENTS_CAPACITY 4u

/*@Pinfit(context:button_events)*/
typedef struct
{
    const button_listener_interface_t *subscribers[BUTTON_EVENTS_CAPACITY];
    uint32_t count;
} button_events_context_t;

/*@Pinfit(function:button_events_init)*/
void button_events_init(button_events_context_t *context);

/*@Pinfit(function:button_events_subscribe)*/
bool button_events_subscribe(button_events_context_t *context, const button_listener_interface_t *subscriber);

/*@Pinfit(function:button_events_unsubscribe)*/
bool button_events_unsubscribe(button_events_context_t *context, const button_listener_interface_t *subscriber);

/*@Pinfit(function:button_events_publish_pressed)*/
/**
 * @brief pressed
 * @param button_id button_id
 */
void button_events_publish_pressed(button_events_context_t *context, uint8_t button_id);

/*@Pinfit(function:button_events_publish_released)*/
/**
 * @brief released
 * @param button_id button_id
 * @param held_ms held_ms
 */
void button_events_publish_released(button_events_context_t *context, uint8_t button_id, uint32_t held_ms);

/*@Pinfit usercode+ observer.header.footer*/
/*@Pinfit usercode-*/

#endif /* BUTTON_EVENTS_H_ */
