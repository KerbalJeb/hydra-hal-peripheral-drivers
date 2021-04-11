/** @file gpio_config.c
*   @brief Created on 2021-03-10 by Ben
*/
/**********************************************************************
* Includes
**********************************************************************/
#include "gpio_config.h"

/**********************************************************************
* Module Preprocessor Constants
**********************************************************************/

/**********************************************************************
* Module Preprocessor Macros
**********************************************************************/

/**********************************************************************
* Module Typedefs
**********************************************************************/

/*********************************************************************
* Module Variable Definitions
**********************************************************************/
const struct GpioConfig GpioConfigTable[] =
{
{SS_RELAY_PIN, GPIO_AF_MODE, GPIO_PULL_NONE, ALTERNATE_FUNCTION_A1, GPIO_LOW , PUSH_PULL_OUTPUT  }, /** SS-Relay */
{SPI_SCK_PIN , GPIO_AF_MODE, GPIO_PULL_NONE, ALTERNATE_FUNCTION_A0, GPIO_LOW , PUSH_PULL_OUTPUT  }, /** SPI1 SCK */
{SPI_MISO_PIN, GPIO_AF_MODE, GPIO_PULL_NONE, ALTERNATE_FUNCTION_A0, GPIO_LOW , PUSH_PULL_OUTPUT  }, /** SPI1 MISO */
{SPI_CS_PIN  , GPIO_OUTPUT , GPIO_PULL_NONE, ALTERNATE_FUNCTION_A0, GPIO_HIGH, PUSH_PULL_OUTPUT  }, /** CS */
{I2C_SDA     , GPIO_AF_MODE, GPIO_PULL_UP  , ALTERNATE_FUNCTION_A4, GPIO_LOW , OPEN_DRAIN_OUTPUT }, /** SDA */
{I2C_SCK     , GPIO_AF_MODE, GPIO_PULL_UP  , ALTERNATE_FUNCTION_A4, GPIO_LOW , OPEN_DRAIN_OUTPUT }, /** SCK */
{PIN_B1      , GPIO_INPUT  , GPIO_PULL_UP  , ALTERNATE_FUNCTION_A0, GPIO_LOW , PUSH_PULL_OUTPUT  }, /** B1 */
{PIN_B3      , GPIO_INPUT  , GPIO_PULL_UP  , ALTERNATE_FUNCTION_A0, GPIO_LOW , PUSH_PULL_OUTPUT  }, /** B2 */
{PIN_B4      , GPIO_INPUT  , GPIO_PULL_UP  , ALTERNATE_FUNCTION_A0, GPIO_LOW , PUSH_PULL_OUTPUT  }, /** B3 */
{GPIO_PIN_END, 0, 0, 0, 0} /** End of table */,
};
/**********************************************************************
* Function Prototypes
**********************************************************************/

/**********************************************************************
* Function Definitions
**********************************************************************/

/**
 * @brief Used to access the GPIO configuration table
 * @return The configuration table
 */
const struct GpioConfig *gpio_get_config(void)
{
    return &GpioConfigTable[0];
}
