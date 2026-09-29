/*@Pinfit(file:state-machine-source:door.state-machine.yaml)*/
/*@Pinfit(skeleton-hash:767666da8e10a09d)*/
/**
 * @file door.c
 * @brief Door controller
 */

#include "door.h"
#include <stdbool.h>

/*@Pinfit usercode+ state-machine.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit(private-function:door_enter_CLOSED)*/
static void door_enter_CLOSED(door_context_t *context)
{
    (void)context;

    /*@Pinfit usercode+ state.CLOSED.entry*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:door_exit_CLOSED)*/
static void door_exit_CLOSED(door_context_t *context)
{
    (void)context;

    /*@Pinfit usercode+ state.CLOSED.exit*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:door_enter_OPEN)*/
static void door_enter_OPEN(door_context_t *context)
{
    (void)context;

    /*@Pinfit usercode+ state.OPEN.entry*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:door_exit_OPEN)*/
static void door_exit_OPEN(door_context_t *context)
{
    (void)context;

    /*@Pinfit usercode+ state.OPEN.exit*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:door_enter_FAULT)*/
static void door_enter_FAULT(door_context_t *context)
{
    (void)context;

    /*@Pinfit usercode+ state.FAULT.entry*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:door_exit_FAULT)*/
static void door_exit_FAULT(door_context_t *context)
{
    (void)context;

    /*@Pinfit usercode+ state.FAULT.exit*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(function:door_init)*/
void door_init(door_context_t *context)
{
    context->state = DOOR_STATE_CLOSED;
    door_enter_CLOSED(context);
}

/*@Pinfit(function:door_tick)*/
void door_tick(door_context_t *context)
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

/*@Pinfit(function:door_go_to_state)*/
void door_go_to_state(door_context_t *context, door_state_t state)
{
    switch (context->state)
    {
        case DOOR_STATE_CLOSED:
            door_exit_CLOSED(context);
            break;

        case DOOR_STATE_OPEN:
            door_exit_OPEN(context);
            break;

        case DOOR_STATE_FAULT:
            door_exit_FAULT(context);
            break;

        default:
            break;
    }

    context->state = state;

    switch (state)
    {
        case DOOR_STATE_CLOSED:
            door_enter_CLOSED(context);
            break;

        case DOOR_STATE_OPEN:
            door_enter_OPEN(context);
            break;

        case DOOR_STATE_FAULT:
            door_enter_FAULT(context);
            break;

        default:
            break;
    }
}

/*@Pinfit(function:door_on_OPEN_REQUEST)*/
void door_on_OPEN_REQUEST(door_context_t *context)
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
                door_exit_CLOSED(context);
                /*@Pinfit usercode+ transition.CLOSED.OPEN_REQUEST.action*/
                /*@Pinfit usercode-*/
                context->state = DOOR_STATE_OPEN;
                door_enter_OPEN(context);
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

/*@Pinfit(function:door_on_CLOSE_REQUEST)*/
void door_on_CLOSE_REQUEST(door_context_t *context)
{
    bool pinfit_transitioned = false;

    switch (context->state)
    {
        case DOOR_STATE_OPEN:
        {
            door_exit_OPEN(context);
            /*@Pinfit usercode+ transition.OPEN.CLOSE_REQUEST.action*/
            /*@Pinfit usercode-*/
            context->state = DOOR_STATE_CLOSED;
            door_enter_CLOSED(context);
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

/*@Pinfit(function:door_on_MOTOR_FAULT)*/
void door_on_MOTOR_FAULT(door_context_t *context, uint32_t code)
{
    bool pinfit_transitioned = false;
    (void)code;

    switch (context->state)
    {
        case DOOR_STATE_OPEN:
        {
            door_exit_OPEN(context);
            /*@Pinfit usercode+ transition.OPEN.MOTOR_FAULT.action*/
            /*@Pinfit usercode-*/
            context->state = DOOR_STATE_FAULT;
            door_enter_FAULT(context);
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
