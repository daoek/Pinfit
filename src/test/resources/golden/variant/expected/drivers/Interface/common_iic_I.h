/*@Pinfit(file:interface:common_iic.interface.yaml)*/
/*@Pinfit(skeleton-hash:f88f8f6628582699)*/
/**
 * @file common_iic_I.h
 * @brief Portable I2C master interface
 */

#ifndef COMMON_IIC_I_H_
#define COMMON_IIC_I_H_

#include <stddef.h>
#include <stdint.h>

/*@Pinfit usercode+ interface.preamble*/
/*@Pinfit usercode-*/

/*@Pinfit(enum:common_iic_status_t)*/
/** @brief Transfer result */
typedef enum
{
  COMMON_IIC_SUCCESS = 0,
  COMMON_IIC_INVALID_PARAM = 1,
  COMMON_IIC_NOT_INITIALIZED
} common_iic_status_t;

/*@Pinfit(struct:common_iic_options_t)*/
/** @brief Bus options */
typedef struct
{
  uint32_t speed;
  uint8_t address_bits;
} common_iic_options_t;

/*@Pinfit usercode+ interface.declarations*/
/*@Pinfit usercode-*/

/*@Pinfit(interface-table:common_iic)*/
typedef struct
{
  void *context;
  common_iic_status_t (*write)(void *context, uint32_t slave_address, const uint8_t *data, uint32_t length);
  void (*configure)(void *context, const common_iic_options_t *options);
} common_iic_interface_t;

/*@Pinfit(function:write)*/
/**
 * @brief Write bytes to a slave
 * @param slave_address slave_address
 * @param data data
 * @param length Byte count
 * @return common_iic_status_t result.
 */
static inline common_iic_status_t commonIicWrite(const common_iic_interface_t * const interface, uint32_t slave_address, const uint8_t *data, uint32_t length)
{
  common_iic_status_t pinfit_result = COMMON_IIC_INVALID_PARAM;

  if (interface != NULL)
  {
    if ((interface->context != NULL) && (interface->write != NULL))
    {
      pinfit_result = interface->write(interface->context, slave_address, data, length);
    }
    else
    {
      pinfit_result = COMMON_IIC_NOT_INITIALIZED;
    }
  }

  return pinfit_result;
}

/*@Pinfit(function:configure)*/
/**
 * @brief configure
 * @param options options
 */
static inline void commonIicConfigure(const common_iic_interface_t * const interface, const common_iic_options_t *options)
{
  if (interface != NULL)
  {
    if ((interface->context != NULL) && (interface->configure != NULL))
    {
      interface->configure(interface->context, options);
    }
  }
}

/*@Pinfit usercode+ interface.footer*/
/*@Pinfit usercode-*/

#endif /* COMMON_IIC_I_H_ */
