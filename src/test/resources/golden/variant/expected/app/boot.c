/*@Pinfit(file:module-source:boot.module.yaml)*/
/*@Pinfit(skeleton-hash:e056ca7096856f69)*/
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

/*@Pinfit(function:bootInstance)*/
static bool boot_singleton_initialized = false;

void bootInstance(void)
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
