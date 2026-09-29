/*@Pinfit(file:module-source:ra_iic.module.yaml)*/
/*@Pinfit(skeleton-hash:492fd921d088149a)*/
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
static uint8_t ra_iic_checksum(const uint8_t *data, uint32_t length);

/*@Pinfit(variable-definition:transfer_count)*/
/** @brief transfer_count */
uint32_t transfer_count;

/*@Pinfit(private-variable:busy)*/
/** @brief busy */
static bool busy;

/*@Pinfit(private-variable:buffer[16])*/
/** @brief buffer[16] */
static uint8_t buffer[16];

/*@Pinfit(variable-definition:ticks)*/
/** @brief Uptime */
uint32_t ticks = 0U;

/*@Pinfit(private-function:ra_iic_common_iic_write)*/
static common_iic_status_t ra_iic_common_iic_write(void *context, uint32_t slave_address, const uint8_t *data, uint32_t length)
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

/*@Pinfit(private-function:ra_iic_common_iic_configure)*/
static void ra_iic_common_iic_configure(void *context, const common_iic_options_t *options)
{
    ra_iic_context_t *module = (ra_iic_context_t *)context;
    (void)module;
    (void)options;

    /*@Pinfit usercode+ function.common_iic.configure.body*/
    /*@Pinfit usercode-*/
}

/*@Pinfit(bind-function:ra_iic_bind_common_iic)*/
void ra_iic_bind_common_iic(common_iic_interface_t *interface, ra_iic_context_t *context)
{
    if (interface != NULL)
    {
        interface->context = context;
        interface->write = ra_iic_common_iic_write;
        interface->configure = ra_iic_common_iic_configure;
    }
}

/*@Pinfit(function:ra_iic_initialize)*/
bool ra_iic_initialize(void)
{
    bool pinfit_result = false;

    /*@Pinfit usercode+ function.ra_iic_initialize.body*/
    /*@Pinfit usercode-*/
    return pinfit_result;
}

/*@Pinfit(function:ra_iic_checksum)*/
static uint8_t ra_iic_checksum(const uint8_t *data, uint32_t length)
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
