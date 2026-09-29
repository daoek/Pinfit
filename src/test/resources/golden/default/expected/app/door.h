/*@Pinfit(file:state-machine-header:door.state-machine.yaml)*/
/*@Pinfit(skeleton-hash:5c8305f0c2bcfd46)*/
/**
 * @file door.h
 * @brief Door controller
 */

#ifndef DOOR_H_
#define DOOR_H_

#include <stdint.h>

/*@Pinfit usercode+ state-machine.header.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(state-enum:door)*/
/** @brief States of door */
typedef enum
{
    DOOR_STATE_CLOSED,
    DOOR_STATE_OPEN,
    DOOR_STATE_FAULT
} door_state_t;

/*@Pinfit(context:door)*/
typedef struct
{
    door_state_t state;
    uint32_t open_count;
} door_context_t;

/*@Pinfit(function:door_init)*/
void door_init(door_context_t *context);

/*@Pinfit(function:door_tick)*/
void door_tick(door_context_t *context);

/*@Pinfit(function:door_go_to_state)*/
void door_go_to_state(door_context_t *context, door_state_t state);

/*@Pinfit(function:door_on_OPEN_REQUEST)*/
/**
 * @brief Request to open
 */
void door_on_OPEN_REQUEST(door_context_t *context);

/*@Pinfit(function:door_on_CLOSE_REQUEST)*/
/**
 * @brief door_on_CLOSE_REQUEST
 */
void door_on_CLOSE_REQUEST(door_context_t *context);

/*@Pinfit(function:door_on_MOTOR_FAULT)*/
/**
 * @brief door_on_MOTOR_FAULT
 * @param code code
 */
void door_on_MOTOR_FAULT(door_context_t *context, uint32_t code);

/*@Pinfit usercode+ state-machine.header.footer*/
/*@Pinfit usercode-*/

#endif /* DOOR_H_ */
