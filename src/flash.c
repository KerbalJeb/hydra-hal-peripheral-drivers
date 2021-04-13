/** @file flash.c
 *   @brief Created on 2021-03-22 by Ben
 */
/**********************************************************************
 * Includes
 **********************************************************************/
#include "flash.h"
#include "stm32f0xx.h"
#include <stddef.h>

/**********************************************************************
 * Module Preprocessor Constants
 **********************************************************************/

/**********************************************************************
 * Module Preprocessor Macros
 **********************************************************************/

/**********************************************************************
 * Module Typedefs
 **********************************************************************/

/**********************************************************************
 * Function Prototypes
 **********************************************************************/

void flash_unlock ();
void flash_wait ();

/**********************************************************************
 * Function Definitions
 **********************************************************************/

void
flash_unlock ()
{
  flash_wait ();
  if (FLASH->CR & FLASH_CR_LOCK)
    {
      FLASH->KEYR = 0x45670123;
      FLASH->KEYR = 0xCDEF89AB;
    }
}

void
flash_erase (uintptr_t address)
{
  flash_unlock ();
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = address;
  FLASH->CR |= FLASH_CR_STRT;
  flash_wait ();
  if (FLASH->SR & FLASH_SR_EOP)
    {
      FLASH->SR = FLASH_SR_EOP;
    }
  FLASH->CR &= ~FLASH_CR_PER;
}

void
flash_write (uintptr_t address, uint16_t value)
{
  flash_unlock ();
  FLASH->CR |= FLASH_CR_PG;
  *(__IO uint16_t *)(address) = value;
  while (FLASH->SR & FLASH_SR_BSY)
    {
    }
  if (FLASH->SR & FLASH_SR_EOP)
    {
      FLASH->SR = FLASH_SR_EOP;
    }
  FLASH->CR &= ~FLASH_CR_PG;
}

uint16_t
flash_read (uintptr_t address)
{
  return *(volatile uint16_t *)(address);
}

void
flash_wait ()
{
  while (FLASH->SR & FLASH_SR_BSY)
    {
    }
}
