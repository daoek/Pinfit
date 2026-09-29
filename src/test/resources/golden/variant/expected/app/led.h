/*@Pinfit(file:module-header:led.module.yaml)*/
/*@Pinfit(skeleton-hash:25908e93e46b840e)*/
/**
 * @file led.h
 * @brief led module
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include <stdbool.h>

/*@Pinfit usercode+ module.header.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(context:led)*/
typedef struct
{
  uint8_t pin;
} led_context_t;

/*@Pinfit(function:ledInstance)*/
led_context_t *ledInstance(void);

/*@Pinfit usercode+ module.header.footer*/
/*@Pinfit usercode-*/

#endif /* LED_H_ */
