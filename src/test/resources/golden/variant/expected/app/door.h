/*@Pinfit(file:state-machine-header:door.state-machine.yaml)*/
/*@Pinfit(skeleton-hash:2357c6e20fea3ec3)*/
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

/*@Pinfit(function:doorInit)*/
void doorInit(door_context_t *context);

/*@Pinfit(function:doorTick)*/
void doorTick(door_context_t *context);

/*@Pinfit(function:doorGoToState)*/
void doorGoToState(door_context_t *context, door_state_t state);

/*@Pinfit(function:doorOnOPENREQUEST)*/
/**
 * @brief Request to open
 */
void doorOnOPENREQUEST(door_context_t *context);

/*@Pinfit(function:doorOnCLOSEREQUEST)*/
/**
 * @brief doorOnCLOSEREQUEST
 */
void doorOnCLOSEREQUEST(door_context_t *context);

/*@Pinfit(function:doorOnMOTORFAULT)*/
/**
 * @brief doorOnMOTORFAULT
 * @param code code
 */
void doorOnMOTORFAULT(door_context_t *context, uint32_t code);

/*@Pinfit usercode+ state-machine.header.footer*/
/*@Pinfit usercode-*/

#endif /* DOOR_H_ */
