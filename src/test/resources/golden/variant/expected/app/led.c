/*@Pinfit(file:module-source:led.module.yaml)*/
/*@Pinfit(skeleton-hash:49ec93433588d0ad)*/
/**
 * @file led.c
 * @brief led module
 */

#include "led.h"
#include <stdbool.h>

/*@Pinfit usercode+ module.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ module.source.variables*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ module.source.prototypes*/
/*@Pinfit usercode-*/

/*@Pinfit(function:ledInstance)*/
static led_context_t led_singleton_context;
static bool led_singleton_initialized = false;

led_context_t *ledInstance(void)
{
  if (!led_singleton_initialized)
  {
    led_singleton_initialized = true;
    /*@Pinfit usercode+ singleton.init*/
    /*@Pinfit usercode-*/
  }
  else
  {
    /*@Pinfit usercode+ singleton.else*/
    /*@Pinfit usercode-*/
  }
  return &led_singleton_context;
}

/*@Pinfit usercode+ module.source.footer*/
/*@Pinfit usercode-*/
