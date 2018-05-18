/****************************************************************************
* Title                 :   Flash 2 HAL
* Filename              :   flash_2_hal.h
* Author                :   CAL
* Origin Date           :   03/21/2016
* Notes                 :   None
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials       Description
*  03/21/16       1.0.0             CAL        Module Created.
*
*****************************************************************************/
/**
 * @file flash_2_hal.h
 * @brief <h2> HAL layer </h2>
 *
 * @par
 * HAL layer for
 * <a href="http://www.mikroe.com">MikroElektronika's</a> Flash 2 click
 * board.
 */
#ifndef FLASH_2_HAL_H_
#define FLASH_2_HAL_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/
        
/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief <h3> CS High </h3>
 *
 * @par
 *  Used by HW layer functions to set CS PIN high ( deselect )
 */
void flash_2_hal_cs_high( void );

/**
 * @brief <h3> CS Low </h3>
 *
 * @par
 *  Used by HW layer functions to set CS PIN low ( selecet )
 */
void flash_2_hal_cs_low( void );


/**
 * @brief <h3> WP Low </h3>
 *
 * @par
 *  Used by HW layer functions to set WP pin to Low
 */
void flash_2_hal_wp_low( void );

/**
 * @brief <h3> WP High </h3>
 *
 * @par
 *  Used by HW layer functions to set WP pin to High
 */
void flash_2_hal_wp_high( void );

/**
 * @brief <h3> Write Command </h3>
 *
 * @par
 *  Used by HAL layer functions to sends commands through SPI
 */
void flash_2_hal_command(uint8_t command);

/**
 * @brief <h3> HAL Initialization </h3>
 *
 * @par
 *  Hal layer initialization. Must be called before any other function.
 */
uint8_t flash_2_hal_init( void );

/**
 * @brief <h3> HAL Write </h3>
 *
 * @par
 *  Writes data through SPI bus
 *
 * @note Function have no affect to the CS PIN state - chip select is
 * controled directly from HW layer.
 *
 * @param[in] buffer - Buffer with data to write through spi.
 * @param[in] count - Amount of bytes to write through spi.
 */
void flash_2_hal_write( uint8_t *buffer,
                          uint16_t count );

/**
 * @brief <h3> HAL Write Address </h3>
 *
 * @par
 * Writes address through SPI bus
 *
 *
 * @param[in] address - Address to be written to spi.
 *
 */
void flash_2_hal_write_address( uint32_t address );

/**
 * @brief <h3> HAL Read </h3>
 *
 * @par
 * Reads data from SPI bus
 *
 * @note Function have no affect to the CS PIN state - chip select is
 * controled directly from HW layer
 *
 * @param[out] buffer - Buffer for data to be read into.
 * @param[in] count - Amount of bytes to read.
 *
 */
void flash_2_hal_read( uint8_t *buffer,
                         uint16_t count );
                         
/**
 * @brief <h3> HAL Read Byte </h3>
 *
 * @par
 * Reads one byte from SPI bus
 *
 * @note Function have no affect to the CS PIN state - chip select is
 * controled directly from HW layer
 *
 * @param[out] buffer - Dummy Byte.
 *
 */
uint8_t flash_2_hal_read_byte( uint8_t buffer );

#ifdef __cplusplus
} // extern "C"
#endif

#endif

/*** End of File **************************************************************/
