/*@Pinfit(file:module-header:ra_iic.module.yaml)*/
/*@Pinfit(skeleton-hash:bec853cb8e5fa5dc)*/
/**
 * @file ra_iic.h
 * @brief RA-family I2C implementation
 */

#ifndef RA_IIC_H_
#define RA_IIC_H_

#include "../Interface/common_iic_I.h"
#include <stdbool.h>

/*@Pinfit usercode+ module.header.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(enum:ra_iic_mode_t)*/
/** @brief ra_iic_mode_t */
typedef enum
{
    RA_IIC_MODE_OFF = 0,
    RA_IIC_MODE_ON
} ra_iic_mode_t;

/*@Pinfit(public-variable:transfer_count)*/
/** @brief transfer_count */
extern uint32_t transfer_count;

/*@Pinfit(public-variable:ticks)*/
/** @brief Uptime */
extern uint32_t ticks;

/*@Pinfit(context:ra_iic)*/
typedef struct
{
    void *hardware;
    ra_iic_mode_t mode;
} ra_iic_context_t;

/*@Pinfit(bind-function:ra_iic_bind_common_iic)*/
void ra_iic_bind_common_iic(common_iic_interface_t *interface, ra_iic_context_t *context);

/*@Pinfit(function:ra_iic_initialize)*/
/**
 * @brief One-time module initialization
 * @return bool result.
 */
bool ra_iic_initialize(void);

/*@Pinfit usercode+ module.header.footer*/
/*@Pinfit usercode-*/

#endif /* RA_IIC_H_ */
