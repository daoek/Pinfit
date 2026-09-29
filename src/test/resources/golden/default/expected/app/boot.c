/*@Pinfit(file:module-source:boot.module.yaml)*/
/*@Pinfit(skeleton-hash:c8f8bd7ee74d49c5)*/
/**
 * @file boot.c
 * @brief boot module
 */

#include "boot.h"
#include <stdbool.h>

/*@Pinfit usercode+ module.source.includes*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ module.source.variables*/
/*@Pinfit usercode-*/

/*@Pinfit usercode+ module.source.prototypes*/
/*@Pinfit usercode-*/

/*@Pinfit(function:boot_instance)*/
static bool boot_singleton_initialized = false;

void boot_instance(void)
{
    if (!boot_singleton_initialized)
    {
        boot_singleton_initialized = true;
        /*@Pinfit usercode+ singleton.init*/
        /*@Pinfit usercode-*/
    }
}

/*@Pinfit usercode+ module.source.footer*/
/*@Pinfit usercode-*/
