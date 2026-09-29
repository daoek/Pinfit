/*@Pinfit(file:module-source:ra_iic.module.yaml)*/
/*@Pinfit(skeleton-hash:aea5924c8a261653)*/
/**
 * @file ra_iic.c
 * @brief RA-family I2C implementation
 */

#include "ra_iic.h"

/*@Pinfit usercode+ module.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ module.source.variables*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ module.source.prototypes*/
/*@Pinfit usercode-*/

/*@Pinfit(function-prototypes:ra_iic)*/
static uint8_t raIicChecksum(const uint8_t *data, uint32_t length);

/*@Pinfit(private-variable:transfer_count)*/
/** @brief transfer_count */
static uint32_t transfer_count;

/*@Pinfit(private-variable:error_count)*/
/** @brief error_count */
static uint32_t error_count;

/*@Pinfit(private-variable:latch)*/
/** @brief latch */
static uint8_t latch;

/*@Pinfit(private-variable:busy)*/
/** @brief busy */
static bool busy;

/*@Pinfit(private-variable:buffer[16])*/
/** @brief buffer[16] */
static uint8_t buffer[16];

/*@Pinfit(private-variable:ticks)*/
/** @brief Uptime */
static uint32_t ticks = 0U;

/*@Pinfit(public-accessor:transfer_count)*/
uint32_t getTransferCount(void)
{
  /*@Pinfit usercode+ variable.transfer_count.get*/
  return transfer_count;
  /*@Pinfit usercode-*/
}

void setTransferCount(uint32_t value)
{
  /*@Pinfit usercode+ variable.transfer_count.set*/
  transfer_count = value;
  /*@Pinfit usercode-*/
}

/*@Pinfit(public-accessor:error_count)*/
uint32_t getErrorCount(void)
{
  /*@Pinfit usercode+ variable.error_count.get*/
  return error_count;
  /*@Pinfit usercode-*/
}

/*@Pinfit(public-accessor:latch)*/
void setLatch(uint8_t value)
{
  /*@Pinfit usercode+ variable.latch.set*/
  latch = value;
  /*@Pinfit usercode-*/
}

/*@Pinfit(public-accessor:ticks)*/
uint32_t getTicks(void)
{
  /*@Pinfit usercode+ variable.ticks.get*/
  return ticks;
  /*@Pinfit usercode-*/
}

void setTicks(uint32_t value)
{
  /*@Pinfit usercode+ variable.ticks.set*/
  ticks = value;
  /*@Pinfit usercode-*/
}

/*@Pinfit(private-function:raIicCommonIicWrite)*/
static common_iic_status_t raIicCommonIicWrite(void *context, uint32_t slave_address, const uint8_t *data, uint32_t length)
{
  ra_iic_context_t *module = (ra_iic_context_t *)context;
  common_iic_status_t pinfit_result = COMMON_IIC_INVALID_PARAM;
  (void)module;
  (void)slave_address;
  (void)data;
  (void)length;

  /*@Pinfit usercode+ function.common_iic.write.body*/
  /*@Pinfit usercode-*/
  return pinfit_result;
}

/*@Pinfit(private-function:raIicCommonIicConfigure)*/
static void raIicCommonIicConfigure(void *context, const common_iic_options_t *options)
{
  ra_iic_context_t *module = (ra_iic_context_t *)context;
  (void)module;
  (void)options;

  /*@Pinfit usercode+ function.common_iic.configure.body*/
  /*@Pinfit usercode-*/
}

/*@Pinfit(bind-function:raIicBindCommonIic)*/
void raIicBindCommonIic(common_iic_interface_t *interface, ra_iic_context_t *context)
{
  if (interface != NULL)
  {
    interface->context = context;
    interface->write = raIicCommonIicWrite;
    interface->configure = raIicCommonIicConfigure;
  }
}

/*@Pinfit(function:raIicInitialize)*/
bool raIicInitialize(void)
{
  bool pinfit_result = false;

  /*@Pinfit usercode+ function.ra_iic_initialize.body*/
  /*@Pinfit usercode-*/
  return pinfit_result;
}

/*@Pinfit(function:raIicChecksum)*/
static uint8_t raIicChecksum(const uint8_t *data, uint32_t length)
{
  uint8_t pinfit_result = 0U;
  (void)data;
  (void)length;

  /*@Pinfit usercode+ function.ra_iic_checksum.body*/
  /*@Pinfit usercode-*/
  return pinfit_result;
}

/*@Pinfit usercode+ module.source.footer*/
/*@Pinfit usercode-*/
