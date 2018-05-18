/****************************************************************************
* Title                 :   Flash 2 CLICK
* Filename              :   flash_2_hw.h
* Author                :   CAL
* Origin Date           :   21/03/2016
* Notes                 :   Hardware layer
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials   Description
*  03/21/16        1.0              CAL     Interface Created.
*
*****************************************************************************/
/**
 * @file flash_2_hw.c
 * @brief <h3> Hardware Layer </h3>
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "flash_2_hw.h"
#include "flash_2_hal.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define STATUS_REG
#define STATUS_WEL  0x02 /**< Write-enable latch status 1 = device is write-enabled
                            0 = device is not write-enabled */
#define STATUS_WSE  0x04 /**< Write suspend-erase status 1 = erase suspended 0 =
                            erase is not suspended */
#define STATUS_WSP  0x08 /**< Write suspend-program status 1 = program suspended
                            0 = program is not suspended */
#define STATUS_WPLD 0x10 /**< Write protections lock-down status 1 = write
                            protection lock-down enabled 0 = write protection
                            lock-down disabled */
#define STATUS_SEC  0x20 /**< Security ID status 1 = security ID space locked
                            0 = security ID space not locked */
#define STATUS_RES  0x40 /**< Reserved for future use */
#define STATUS_BUSY 0x80 /**< Write operation status 1 = write in progress 0 =
                            no operation in progress */



#define CONFIGURATION_REG
#define CONFIGURATION_RES   0x01 /**< Reserved */
#define CONFIGURATION_IOC   0x02 /**< I/O configuration for SPI mode 1 = WP and
                                    HOLD pins disabled 0 = WP and HOLD enabled*/
#define CONFIGURATION_BPNV  0x08 /**< Block-protection volatility state 1 = no
                                   memory block permanently locked 0 = any block
                                    has been permanently locked */

#define CONFIGURATION_WPEN  0x80 /**< Write-protection pin (WP) enable 1 = enabled
                                    0 = disabled */

#define INSTRUCTIONS
#define NOP         0x00 /**< No Operation */
#define RSTEN       0x66 /**< Reset Enable */
#define RST         0x99 /**< Reset Memory */
#define EQIO        0x38 /**< Enable Quad I/O */
#define RSTQIO      0xFF /**< Reset Quad I/O */
#define RDSR        0x05 /**< Read Status Register */
#define WRSR        0x01 /**< Write Status Register */
#define RDCR        0x35 /**< Read Configuration Register */
#define READ        0x03 /**< Read Memory */
#define HS_READ     0x0B /**< Read Memory at Higher Speed */
#define SQOR        0x6B /**< SPI Quad Output Read */
#define SQIOR       0xEB /**< SPI Quad I/O Read */
#define SDOR        0x3B /**< SPI Dual Output Read */
#define SDIOR       0xBB /**< SPI Dual I/O Read */
#define SB          0xC0 /**< Set Burst Length */
#define RBSQI       0x0C /**< SQI Read Burst with Wrap */
#define RBSPI       0xEC /**< SPI Read Burst with Wrap */
#define JEDECID     0x9F /**< JEDEC-ID Read */
#define QUAD_JID    0xAF /**< Quad I/O J-ID read */
#define SFDP        0x5A /**< Serial Flash Discoverable Parameters */
#define WREN        0x06 /**< Write Enable */
#define WRDI        0x04 /**< Write Disable */
#define SE          0x20 /**< Erase 4 KBytes of Memory Array */
#define BE          0xD8 /**< Erase 64, 32 or 8 KBytes of Memory Array */
#define CE          0xC7 /**< Erase Full Array */
#define PP          0x02 /**< Page Program */
#define SPI_QUAD    0x32 /**< SQI Quad Page Program */
#define WRSU        0xB0 /**< Suspends Program / Erase */
#define WRRE        0x30 /**< Resumes Program / Erase */
#define RBPR        0x72 /**< Read Block-Protection Register */
#define WBPR        0x42 /**< Write Block-Protection Register */
#define LBPR        0x8D /**< Lock Down Block-Protection Register */
#define NVWLDR      0xE8 /**< Non-Volatile Write Lock-Down Register */
#define ULBPR       0x98 /**< Global Block Protection Unlock */
#define RSID        0x88 /**< Read Security ID */
#define PSID        0xA5 /**< Program User Security ID Area */
#define LSID        0x85 /**< Lockout Security ID Programming */

#define START_PAGE_ADDRESS      0x010000
#define END_PAGE_ADDRESS        0x7FFFFF
#define FLASH_PAGE_SIZE         256
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/******************************************************************************
* Public Function Definitions
*******************************************************************************/
uint8_t flash_2_init()
{
    if( !flash_2_hal_init() )
    {
        return 0;
    }
    else
        return -1;
}

uint8_t flash_2_busy()
{
    uint8_t temp;

    flash_2_hal_cs_low();
    flash_2_hal_command( RDSR );
    temp = flash_2_hal_read_byte( 0 );
    flash_2_hal_cs_high();
    temp &= STATUS_BUSY;
    return temp;
}

uint8_t flash_2_get_status_reg( void )
{
    uint8_t temp;

    //Status Reg is only instruction that doesn't need to wait for Busy bit
    flash_2_hal_cs_low();
    flash_2_hal_command( RDSR );
    temp = flash_2_hal_read_byte( 0 );
    flash_2_hal_cs_high();

    return temp;
}

uint8_t flash_2_erase_status( void )
{
    uint8_t temp;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RDSR );
    flash_2_hal_read( &temp, 1);
    flash_2_hal_cs_high();
    temp &= STATUS_WSE;

    return temp;
}

uint8_t flash_2_write_status( void )
{
    uint8_t temp;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RDSR );
    flash_2_hal_read( &temp, 1);
    flash_2_hal_cs_high();
    temp &= STATUS_WEL;

    return temp;
}

uint8_t flash_2_program_status( void )
{
    uint8_t temp;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RDSR );
    flash_2_hal_read( &temp, 1);
    flash_2_hal_cs_high();
    temp &= STATUS_WSP;

    return temp;
}

uint8_t flash_2_protect_status( void )
{
    uint8_t temp;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RDSR );
    flash_2_hal_read( &temp, 1);
    flash_2_hal_cs_high();
    temp &= STATUS_WPLD;

    return temp;
}

void flash_2_lock_security_id( void )
{
    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( LSID );
    flash_2_hal_cs_high();

    return;

}

uint8_t flash_2_security_status( void )
{
    uint8_t temp;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RDSR );
    flash_2_hal_read( &temp, 1);
    flash_2_hal_cs_high();
    temp &= STATUS_SEC;

    return temp;
}

void flash_2_write_protect_enable( void )
{
    flash_2_hal_wp_high();
}

void flash_2_write_protect_disable( void )
{
    flash_2_hal_wp_low();
}



void flash_2_write_suspend( void )
{
    flash_2_hal_cs_low();
    flash_2_hal_command( WRSU );
    flash_2_hal_cs_high();
}

void flash_2_write_resume( void )
{
    flash_2_hal_cs_low();
    flash_2_hal_command( WRRE );
    flash_2_hal_cs_high();
}


void flash_2_spi_get_security_id( uint8_t *buffer, uint32_t data_count)
{

    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( RSID );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;

}

void flash_2_sqi_get_security_id( uint8_t *buffer, uint32_t data_count)
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( RSID );
    flash_2_hal_write( 0x00, 1 );
    flash_2_hal_write( 0x00, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;

}

void flash_2_set_security_id( uint8_t *buffer,
                             uint32_t data_count )
{
    uint8_t addressL = 0;
    uint8_t addressH = 7;

    while( flash_2_busy() );
    
    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( PSID );
    flash_2_hal_write( &addressL, 1);
    flash_2_hal_write( &addressH, 1);
    flash_2_hal_write( buffer, data_count );
    flash_2_hal_cs_high();

    return;

}

void flash_2_write_disable( void )
{
    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( WRDI );
    flash_2_hal_cs_high();

    return;

}

void flash_2_spi_get_bpr(  uint8_t *buffer,
                           uint32_t data_count )
{
    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RBPR );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;
}

void flash_2_sqi_get_bpr( uint8_t *buffer,
                          uint32_t data_count  )
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RBPR );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;
}

void flash_2_set_bpr(uint8_t *buffer )
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( WBPR );
    flash_2_hal_write( buffer, 18 );
    flash_2_hal_cs_high();

    return;
}

void flash_2_lock_bpr( void )
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( LBPR );
    flash_2_hal_cs_high();

    return;
}

void flash_2_nonvolatile_write_lock( uint8_t *buffer)
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( NVWLDR );
    flash_2_hal_write( buffer, 18 );
    flash_2_hal_cs_high();

    return;
}


void flash_2_global_block_unlock( void )
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( ULBPR );
    flash_2_hal_cs_high();

    return;
}

void flash_2_read( uint32_t address, uint8_t *buffer,
                        uint32_t data_count )
{
    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( READ );
    flash_2_hal_write_address( address );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();
}

void flash_2_highspeed_read( uint32_t address, uint8_t *buffer,
                        uint32_t data_count )
{
    while( flash_2_busy() )
    
    flash_2_hal_cs_low();
    flash_2_hal_command( HS_READ );
    flash_2_hal_write_address( address );
    flash_2_hal_write( buffer, data_count );
    flash_2_hal_cs_high();
}
void flash_2_quad_write( uint32_t address, uint8_t *buffer,
                        uint32_t data_count )
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( SPI_QUAD );
    
    if( data_count <= FLASH_PAGE_SIZE )
    {


        flash_2_hal_write_address( address );
        flash_2_hal_write( buffer, data_count );

    }
    else
    {
        uint8_t i;
        uint8_t repeat = data_count / FLASH_PAGE_SIZE;
        uint8_t repeat_over = data_count % FLASH_PAGE_SIZE;

        for ( i = 0; i < repeat; i++ )
        {


            flash_2_hal_write_address( address + ( i * FLASH_PAGE_SIZE ) );
            flash_2_hal_write( buffer + ( i * FLASH_PAGE_SIZE ),
                                        FLASH_PAGE_SIZE );
        }
        if ( repeat_over )
        {

            flash_2_hal_write_address( address +
                                           ( repeat * FLASH_PAGE_SIZE ) );
            flash_2_hal_write( buffer + ( repeat * FLASH_PAGE_SIZE ),
                                        repeat_over );
        }
    }
    flash_2_hal_cs_high();
    
    return;
}

void flash_2_write( uint32_t address, uint8_t *buffer,
                        uint32_t data_count )
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( PP );
    
    if( data_count <= FLASH_PAGE_SIZE )
    {

        flash_2_hal_write_address( address );
        flash_2_hal_write( buffer, data_count );

    }
    else
    {
        uint8_t i;
        uint8_t repeat = data_count / FLASH_PAGE_SIZE;
        uint8_t repeat_over = data_count % FLASH_PAGE_SIZE;

        for ( i = 0; i < repeat; i++ )
        {

            flash_2_hal_write_address( address + ( i * FLASH_PAGE_SIZE ) );
            flash_2_hal_write( buffer + ( i * FLASH_PAGE_SIZE ),
                                        FLASH_PAGE_SIZE );
        }
        if ( repeat_over )
        {


            flash_2_hal_write_address( address +
                                           ( repeat * FLASH_PAGE_SIZE ) );
            flash_2_hal_write( buffer + ( repeat * FLASH_PAGE_SIZE ),
                                        repeat_over );
        }
    }
    flash_2_hal_cs_high();
    
    return;
    
}

void flash_2_quad_enable( void )
{
    flash_2_hal_command( EQIO );
}

void flash_2_quad_out_read( uint32_t address, uint8_t *buffer,
                            uint32_t data_count)
{

    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_wp_low();
    flash_2_hal_command( SQOR );
    flash_2_hal_write_address( address );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;
}

void flash_2_quad_io_read(  uint32_t address, uint8_t mode,
                            uint8_t *buffer, uint32_t data_count)
{

    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_wp_low();	
    flash_2_hal_command( SQIOR );
    flash_2_hal_write_address( address );
    flash_2_hal_write( &mode, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;
}

void flash_2_quad_reset( void )
{

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RSTQIO );
    flash_2_hal_cs_high();
}

void flash_2_set_burst( uint8_t length )
{
    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( SB );
    flash_2_hal_write( &length, 1 );
    flash_2_hal_cs_high();

    return;
}

void flash_2_read_sqi_burst_wrap(uint32_t address, uint8_t *buffer,
                              uint32_t data_count)
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RBSQI );
    flash_2_hal_write_address( address );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;

}

void flash_2_read_spi_burst_wrap( uint32_t address, uint8_t *buffer,
                              uint32_t data_count)
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RBSPI );
    flash_2_hal_write_address( address );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;

}

void flash_2_read_dual_output( uint32_t address, uint8_t *buffer,
                                uint32_t data_count)
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( SDOR );
    flash_2_hal_write_address( address );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;

}

void flash_2_read_dual_io (uint32_t address, uint8_t *buffer,
                          uint32_t data_count)
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( SDIOR );
    flash_2_hal_write_address( address );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;
}

void flash_2_sector_erase( uint32_t address )
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( SE );
    flash_2_hal_write_address( address );
    flash_2_hal_cs_high();

}

void flash_2_block_erase( uint32_t address )
{
    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( BE );
    flash_2_hal_write_address( address );
    flash_2_hal_cs_high();

}

void flash_2_chip_erase( void )
{

    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( CE );
    flash_2_hal_cs_high();
    return;
}

void flash_2_get_sfdp_params( uint32_t address, uint8_t *buffer,
                         uint32_t data_count)
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( SFDP );
    flash_2_hal_write_address( address );
    flash_2_hal_write( &dummy_byte, 1 );
    flash_2_hal_read( buffer, data_count );
    flash_2_hal_cs_high();

    return;
}

uint8_t flash_2_quad_device_manufac( void )
{
    uint8_t partialInfo[3];

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( QUAD_JID );
    flash_2_hal_read( partialInfo, 3 );
    flash_2_hal_cs_high();

    return partialInfo[0];
}

uint8_t flash_2_quad_device_type( void )
{
    uint8_t partialInfo[3];

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( QUAD_JID );
    flash_2_hal_read( partialInfo, 3 );
    flash_2_hal_cs_high();

    return partialInfo[1];
}

uint8_t flash_2_quad_device_id( void )
{
    uint8_t partialInfo[3];

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( QUAD_JID );
    flash_2_hal_read( partialInfo, 3 );
    flash_2_hal_cs_high();

    return partialInfo[2];
}

uint8_t flash_2_device_manufac( void )
{
    uint8_t partialInfo[3];

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( JEDECID );
    flash_2_hal_read( partialInfo, 3 );
    flash_2_hal_cs_high();

    return partialInfo[0];
}

uint8_t flash_2_device_type( void )
{
    uint8_t partialInfo[3];

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( JEDECID );
    flash_2_hal_read( partialInfo, 3 );
    flash_2_hal_cs_high();

    return partialInfo[1];
}

uint8_t flash_2_device_id( void )
{
    uint8_t partialInfo[3];

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( JEDECID );
    flash_2_hal_read( partialInfo, 3 );
    flash_2_hal_cs_high();

    return partialInfo[2];
}

void flash_2_reset( void )
{
    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RSTEN );
    flash_2_hal_cs_high();
    flash_2_hal_cs_low();
    flash_2_hal_command( RST );
    flash_2_hal_cs_high();

    return;

}

void flash_2_write_status_reg( uint8_t sReg )
{
    uint8_t dummy_byte = 0x00;

    while( flash_2_busy() );

    flash_2_write_enable();
    flash_2_hal_cs_low();
    flash_2_hal_command( WRSR );
    flash_2_hal_write( &dummy_byte, 1);
    flash_2_hal_write( &sReg, 1);
    flash_2_hal_cs_high();

}

uint8_t flash_2_get_config_reg( void )
{
    uint8_t temp;

    while( flash_2_busy() );

    flash_2_hal_cs_low();
    flash_2_hal_command( RDCR );
    flash_2_hal_read( &temp, 1);
    flash_2_hal_cs_high();

    return temp;
}

void flash_2_write_enable( void )
{
    flash_2_hal_cs_low();
    flash_2_hal_command( WREN );
    flash_2_hal_cs_high();

    return;
}

/*************** END OF FUNCTIONS *********************************************/