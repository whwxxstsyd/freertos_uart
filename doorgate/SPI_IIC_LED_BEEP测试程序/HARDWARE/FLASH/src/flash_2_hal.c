/*******************************************************************************
* Title                 :   Flash 2 HAL
* Filename              :   flash_2_hal.c
* Author                :   CAL
* Origin Date           :   03/21/2016
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials       Description
*  03/21/16       1.0.0             CAL        Module Created.
*
*******************************************************************************/
/**
 * @file flash_2_hal.c
 * @brief <h2> HAL layer </h2>
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "flash_2_hal.h"
#include "stdint.h"
#include "flash.h"
#include "spi.h"	




/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

static void         ( *write_spi_p )            ( unsigned int data_out );
static unsigned int ( *read_spi_p )             ( unsigned int buffer );
	

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void flash_2_hal_cs_high()
{
        FLASH_2_CS = 1;
}

void flash_2_hal_cs_low()
{
        FLASH_2_CS = 0;
}


void flash_2_hal_wp_low()
{
        FLASH_2_WP = 0;
}

void flash_2_hal_wp_high()
{
        FLASH_2_WP = 1;
}


uint8_t flash_2_hal_init( void )
{
	write_spi_p             = SPI_Wr_Ptr;
	read_spi_p              = SPI_Rd_Ptr;


	flash_2_hal_cs_high();
	flash_2_hal_wp_low();

	return 0;
}

void flash_2_hal_command( uint8_t command )
{
    write_spi_p( command );

    return;
}

void flash_2_hal_write( uint8_t *buffer,
                          uint16_t count )
{

    while( count-- )
    {
        write_spi_p( *( buffer++ ) );          
    }

    return;
}

uint8_t flash_2_hal_read_byte( uint8_t buffer )
{
    buffer = read_spi_p( 0 );

    return buffer;

}

void flash_2_hal_read( uint8_t *buffer,
                         uint16_t count )
{
    
    while( count-- )
    {   
        *( buffer++ ) = read_spi_p( 0x00 );
    }
    
    return;
}

void flash_2_hal_write_address( uint32_t address )
{    
    uint8_t addr_hi = address >> 16;
    uint8_t addr_mi = ( address & 0x00FF00 ) >> 8;
    uint8_t addr_lo = ( address & 0x0000FF );

    write_spi_p( addr_hi );
    write_spi_p( addr_mi );
    write_spi_p( addr_lo );
    
    return;
}



/*************** END OF FUNCTIONS ***************************************************************************/
