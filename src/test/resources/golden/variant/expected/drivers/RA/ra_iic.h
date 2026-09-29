/*@Pinfit(file:module-header:ra_iic.module.yaml)*/
/*@Pinfit(skeleton-hash:b6de217e61ffc5b4)*/
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

/*@Pinfit(public-accessor:transfer_count)*/
/** @brief transfer_count */
uint32_t getTransferCount(void);
void setTransferCount(uint32_t value);

/*@Pinfit(public-accessor:error_count)*/
/** @brief error_count */
uint32_t getErrorCount(void);

/*@Pinfit(public-accessor:latch)*/
/** @brief latch */
void setLatch(uint8_t value);

/*@Pinfit(public-accessor:ticks)*/
/** @brief Uptime */
uint32_t getTicks(void);
void setTicks(uint32_t value);

/*@Pinfit(context:ra_iic)*/
typedef struct
{
  void *hardware;
  ra_iic_mode_t mode;
} ra_iic_context_t;

/*@Pinfit(bind-function:raIicBindCommonIic)*/
void raIicBindCommonIic(common_iic_interface_t *interface, ra_iic_context_t *context);

/*@Pinfit(function:raIicInitialize)*/
/**
 * @brief One-time module initialization
 * @return bool result.
 */
bool raIicInitialize(void);

/*@Pinfit usercode+ module.header.footer*/
/*@Pinfit usercode-*/

#endif /* RA_IIC_H_ */
