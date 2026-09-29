/*@Pinfit(file:module-header:led.module.yaml)*/
/*@Pinfit(skeleton-hash:f7a9584e3a89c201)*/
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

/*@Pinfit(function:led_instance)*/
led_context_t *led_instance(void);

/*@Pinfit usercode+ module.header.footer*/
/*@Pinfit usercode-*/

#endif /* LED_H_ */
