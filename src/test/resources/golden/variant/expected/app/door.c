/*@Pinfit(file:state-machine-source:door.state-machine.yaml)*/
/*@Pinfit(skeleton-hash:8fece8bc65cbe0ce)*/
/**
 * @file door.c
 * @brief Door controller
 */

#include "door.h"
#include <stdbool.h>

/*@Pinfit usercode+ state-machine.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(private-function:doorEnterCLOSED)*/
static void doorEnterCLOSED(door_context_t *context)
{
  (void)context;

  /*@Pinfit usercode+ state.CLOSED.entry*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:doorExitCLOSED)*/
static void doorExitCLOSED(door_context_t *context)
{
  (void)context;

  /*@Pinfit usercode+ state.CLOSED.exit*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:doorEnterOPEN)*/
static void doorEnterOPEN(door_context_t *context)
{
  (void)context;

  /*@Pinfit usercode+ state.OPEN.entry*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:doorExitOPEN)*/
static void doorExitOPEN(door_context_t *context)
{
  (void)context;

  /*@Pinfit usercode+ state.OPEN.exit*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:doorEnterFAULT)*/
static void doorEnterFAULT(door_context_t *context)
{
  (void)context;

  /*@Pinfit usercode+ state.FAULT.entry*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:doorExitFAULT)*/
static void doorExitFAULT(door_context_t *context)
{
  (void)context;

  /*@Pinfit usercode+ state.FAULT.exit*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(function:doorInit)*/
void doorInit(door_context_t *context)
{
  context->state = DOOR_STATE_CLOSED;
  doorEnterCLOSED(context);
}

/*@Pinfit(function:doorTick)*/
void doorTick(door_context_t *context)
{
  switch (context->state)
  {
    case DOOR_STATE_CLOSED:
    {
      /*@Pinfit usercode+ state.CLOSED.tick*/
      /*@Pinfit usercode-*/
      break;
    }
    case DOOR_STATE_OPEN:
    {
      /*@Pinfit usercode+ state.OPEN.tick*/
      /*@Pinfit usercode-*/
      break;
    }
    case DOOR_STATE_FAULT:
    {
      /*@Pinfit usercode+ state.FAULT.tick*/
      /*@Pinfit usercode-*/
      break;
    }
    default:
      break;
  }
}

/*@Pinfit(function:doorGoToState)*/
void doorGoToState(door_context_t *context, door_state_t state)
{
  switch (context->state)
  {
    case DOOR_STATE_CLOSED:
      doorExitCLOSED(context);
      break;

    case DOOR_STATE_OPEN:
      doorExitOPEN(context);
      break;

    case DOOR_STATE_FAULT:
      doorExitFAULT(context);
      break;

    default:
      break;
  }

  context->state = state;

  switch (state)
  {
    case DOOR_STATE_CLOSED:
      doorEnterCLOSED(context);
      break;

    case DOOR_STATE_OPEN:
      doorEnterOPEN(context);
      break;

    case DOOR_STATE_FAULT:
      doorEnterFAULT(context);
      break;

    default:
      break;
  }
}

/*@Pinfit(function:doorOnOPENREQUEST)*/
void doorOnOPENREQUEST(door_context_t *context)
{
  bool pinfit_transitioned = false;

  switch (context->state)
  {
    case DOOR_STATE_CLOSED:
    {
      bool pinfit_guard = true;

      /*@Pinfit usercode+ transition.CLOSED.OPEN_REQUEST.guard*/
      /*@Pinfit usercode-*/
      if (pinfit_guard)
      {
        doorExitCLOSED(context);
        /*@Pinfit usercode+ transition.CLOSED.OPEN_REQUEST.action*/
        /*@Pinfit usercode-*/
        context->state = DOOR_STATE_OPEN;
        doorEnterOPEN(context);
        pinfit_transitioned = true;
      }
      break;
    }
    default:
      break;
  }

  if (!pinfit_transitioned)
  {
    /*@Pinfit usercode+ event.OPEN_REQUEST.unhandled*/
    /*@Pinfit usercode-*/
  }
}

/*@Pinfit(function:doorOnCLOSEREQUEST)*/
void doorOnCLOSEREQUEST(door_context_t *context)
{
  bool pinfit_transitioned = false;

  switch (context->state)
  {
    case DOOR_STATE_OPEN:
    {
      doorExitOPEN(context);
      /*@Pinfit usercode+ transition.OPEN.CLOSE_REQUEST.action*/
      /*@Pinfit usercode-*/
      context->state = DOOR_STATE_CLOSED;
      doorEnterCLOSED(context);
      pinfit_transitioned = true;
      break;
    }
    default:
      break;
  }

  if (!pinfit_transitioned)
  {
    /*@Pinfit usercode+ event.CLOSE_REQUEST.unhandled*/
    /*@Pinfit usercode-*/
  }
}

/*@Pinfit(function:doorOnMOTORFAULT)*/
void doorOnMOTORFAULT(door_context_t *context, uint32_t code)
{
  bool pinfit_transitioned = false;
  (void)code;

  switch (context->state)
  {
    case DOOR_STATE_OPEN:
    {
      doorExitOPEN(context);
      /*@Pinfit usercode+ transition.OPEN.MOTOR_FAULT.action*/
      /*@Pinfit usercode-*/
      context->state = DOOR_STATE_FAULT;
      doorEnterFAULT(context);
      pinfit_transitioned = true;
      break;
    }
    default:
      break;
  }

  if (!pinfit_transitioned)
  {
    /*@Pinfit usercode+ event.MOTOR_FAULT.unhandled*/
    /*@Pinfit usercode-*/
  }
}

/*@Pinfit usercode+ state-machine.source.footer*/
/*@Pinfit usercode-*/
