/****************************************************************************
* Title                 :   Flash 2 CLICK
* Filename              :   flash_2_hw.c
* Author                :   CAL
* Origin Date           :   21/03/2016
* Notes                 :   None
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials   Description
*  03/21/16    XXXXXXXXXXX         CAL      Interface Created.
*
*****************************************************************************/
/**
 * @file flash_2_hw.h
 * @brief <h3> Hardware Layer </h3>
 *
 * @par
 * Low level functions for
 * <a href="http://www.mikroe.com">MikroElektronika's</a> Flash 2 click
 * board.
 */

/**
 * @page LIB_INFO Library Info
 * @date 21 March 2016
 * @author Corey Lakey
 * @copyright GNU Public License
 * @version 1.0.0 - Initial testing and verification
 */

/**
 * @page TEST_CFG Test Configurations
 * <h3> Test configuration 1 : </h3>
 * @par
 * <ul>
 * <li><b>MCU</b> :             STM32F107VC</li>
 * <li><b>Dev. Board</b> :      EasyMx Pro v7</li>
 * <li><b>Oscillator</b> :      72 Mhz internal</li>
 * <li><b>Ext. Modules</b> :    Flash 2 Click</li>
 * <li><b>SW</b> :              MikroC PRO for ARM 4.7.1</li>
 * </ul>
 *
 * <h3> Test configuration 2 : </h3>
 * @par
 * <ul>
 * <li><b>MCU</b> :             PIC32MX795F512L</li>
 * <li><b>Dev. Board</b> :      EasyPic Fusion v7</li>
 * <li><b>Oscillator</b> :      80 Mhz internal</li>
 * <li><b>Ext. Modules</b> :    Flash 2 Click</li>
 * <li><b>SW</b> :              MikroC PRO for PIC 6.6.2</li>
 * </ul>
 *
 * <h3> Test configuration 3 : </h3>
 * @par
 * <ul>
 * <li><b>MCU</b> :             FT900Q</li>
 * <li><b>Dev. Board</b> :      EasyFT90x v7</li>
 * <li><b>Oscillator</b> :      100 Mhz internal</li>
 * <li><b>Ext. Modules</b> :    Flash 2 Click</li>
 * <li><b>SW</b> :              MikroC PRO for FT90x 1.2.1</li>
 * </ul>
 */

/**
 * @mainpage
 * <h3> General Description </h3>
 * @par
 *
 *
 * <h3> Features </h3>
 * @par
 *
 *
 */
#ifndef FLASH_2_HW_H
#define FLASH_2_HW_H
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>

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
 * @brief <h3> HW Layer Initialization </h3>
 *
 * @par
 * Prepares library for usage and sets needed values to default.
 *
 * @note
 * This function must be called first.
 *
 * @retval 0 - Success
 * @retval 1 - Failure
 *
 * @endcode
 */
uint8_t flash_2_init( void );

/**
 * @brief <h3> Flash 2 Busy </h3>
 *
 * @par
 *  Checks if click is busy reading/writing
 *
 * @note Used to check when click is ready to be written to / read from.
 *
 * @retval 0 - Success
 * @retval Other than 0 - Failure
 *
 */
uint8_t flash_2_busy( void );

/**
 * @brief <h3> Flash 2 Get Status Register </h3>
 *
 * @par
 *  The Read-Status Register command outputs the contents
 * of the Status register.
 *
 * @retval Status Register
 *
 */
uint8_t flash_2_get_status_reg( void );

/**
 * @brief <h3> Flash 2 Erase Status </h3>
 *
 * @par
 *  Checks if click erase is suspended
 *
 * @retval 0 - erase not suspended
 * @retval 1 - erase suspended
 *
 */
uint8_t flash_2_erase_status( void );

/**
 * @brief <h3> Flash 2 Write Status </h3>
 *
 * @par
 *  Checks if click write is suspended
 *
 * @retval 0 - write not suspended
 * @retval 1 - write suspended
 *
 */
uint8_t flash_2_write_status( void );

/**
 * @brief <h3> Flash 2 Program Status </h3>
 *
 * @par
 *  Checks if click write-program is suspended
 *
 * @retval 0 - write program not suspended
 * @retval 1 - write program suspended
 *
 */
uint8_t flash_2_program_status( void );

/**
 * @brief <h3> Flash 2 Protect Status </h3>
 *
 * @par
 *  Checks if click write-protect lock-down is suspended
 *
 * @retval 0 - write-protect not suspended
 * @retval 1 - write protect suspended
 *
 */
uint8_t flash_2_protect_status( void );

/**
 * @brief <h3> Flash 2 Security Status </h3>
 *
 * @par
 *  Checks if click security ID is locked
 *
 * @retval 0 - security ID not locked
 * @retval 1 - security ID locked
 *
 */
uint8_t flash_2_security_status( void );

/**
 * @brief <h3> Flash 2 Write Protect Enable </h3>
 *
 * @par
 *  Enables Write Protect on Flash 2 Click
 *
 *
 */
void flash_2_write_protect_enable( void );

/**
 * @brief <h3> Flash 2 Write Protect Disable </h3>
 *
 * @par
 *  Disables Write Protect on Flash 2 Click
 *
 *
 */
void flash_2_write_protect_disable( void );

/**
 * @brief <h3> Flash 2 Hold Enable </h3>
 *
 * @par
 *  Enables Hold on Flash 2 Click
 *
 *
 */
void flash_2_hold_enable( void );

/**
 * @brief <h3> Flash 2 Hold Disable </h3>
 *
 * @par
 *  Enables Hold on Flash 2 Click
 *
 *
 */
void flash_2_hold_disable( void );

/**
 * @brief <h3> Flash 2 Read </h3>
 *
 * @par
 *  The Read instruction, 03H, is supported in SPI bus pro-
 * tocol only with clock frequencies up to 40 MHz. This
 * command is not supported in SQI bus protocol. The
 * device outputs the data starting from the specified
 * address location, theand Configuration n continuously streams the data
 * output through all addresses until terminated by a low-
 * to-high transition on CE#. The internal address pointer
 * will automatically increment until the highest memory
 * address is reached. Once the highest memory address
 * is reached, the address pointer will automatically return
 * to the beginning (wrap-around) of the address space.
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read to.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_read( uint32_t address, uint8_t *buffer,
                        uint32_t data_count );

/**
 * @brief <h3> Flash 2 Write </h3>
 *
 * @par
 *  The Page-Program instruction programs up to 256
 * Bytes of data in the memory, and supports both SPI
 * and SQI protocols. The data for the selected page
 * address must be in the erased state (FFH) before initi-
 * ating the Page-Program operation. A Page-Program
 * applied to a protected memory area will be ignored.
 *
 * @param[in] address - Address to start write at.
 * @param[in] buffer - Buffer with data to write.
 * @param[in] data_count - Amount of bytes to write.
 *
 */
void flash_2_write( uint32_t address, uint8_t *buffer,
                        uint32_t data_count );

/**
 * @brief <h3> Flash 2 Quad Enable </h3>
 *
 * @par
 * The Enable Quad I/O (EQIO) instruction, 38H, enables
 * the flash device for SQI bus operation. Upon comple-
 * tion of the instruction, all instructions thereafter are
 * expected to be 4-bit multiplexed input/output (SQI
 * mode) until a power cycle or a “Reset Quad I/O instruc-
 * tion” is executed.
 *
 *
 */
void flash_2_quad_enable( void );

/**
 * @brief <h3> Flash 2 Quad Reset </h3>
 *
 * @par
 *  The Reset Quad I/O instruction, FFH, resets the device
 * to 1-bit SPI protocol operation or exits the Set Mode
 * configuration during a read sequence. This command
 * allows the flash device to return to the default I/O state
 * (SPI) without a power cycle, and executes in either 1-
 * bit or 4-bit mode. If the device is in the Set Mode con-
 * figuration, while in SQI High-Speed Read mode, the
 * RSTQIO command will only return the device to a state
 * where it can accept new command instruction. An addi-
 * tional RSTQIO is required to reset the device to SPI
 * mode.
 *
 *
 */
void flash_2_quad_reset( void );

/**
 * @brief <h3> Flash 2 High Speed Read </h3>
 *
 * @par
 *  The High-Speed Read instruction, 0BH, is supported in
 * both SPI bus protocol and SQI protocol. This instruction
 * supports frequencies of up to 104 MHz from 2.7-3.6V
 * and up to 80 MHz from 2.3-3.6V.On power-up, the
 * device is set to use SPI.
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data to.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_highspeed_read( uint32_t address, uint8_t *buffer,
                        uint32_t data_count );

/**
 * @brief <h3> Flash 2 Quad Output Read </h3>
 *
 * @par
 *  The SPI Quad-Output Read instruction supports fre-
 * quencies of up to 104 MHz from 2.7-3.6V and up to 80
 * MHz from 2.3-3.6V. SST26VF064B requires the IOC bit
 * in the configuration register to be set to ‘1’ prior to exe-
 * cuting the command.
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data to.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_quad_out_read(uint32_t address, uint8_t *buffer,
                            uint32_t data_count);

/**
 * @brief <h3> Flash 2 Quad I/O Read </h3>
 *
 * @par
 *  The SPI Quad I/O Read (SQIOR) instruction supports
 * frequencies of up to 104 MHz from 2.7-3.6V and up to
 * 80 MHz from 2.3-3.6V. SST26VF064B requires the
 * IOC bit in the configuration register to be set to ‘1’ prior
 * to executing the command.
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data to.
 * @param[in] data_count - Amount of bytes to read.
 * @param[in] mode - Mode to put Quad I/O in.
 *
 */
void flash_2_quad_io_read(  uint32_t address, uint8_t mode,
                            uint8_t *buffer, uint32_t data_count);

/**
 * @brief <h3> Flash 2 Set Burst </h3>
 *
 * @par
 *  The Set Burst command specifies the number of bytes
 * to be output during a Read Burst command before the
 * device wraps around. It supports both SPI and SQI pro-
 * tocols.
 *
 * @brief
 *      - length = 0x00 = 8 bytes
 *      - length = 0x01 = 16 bytes
 *      - length = 0x02 = 32 bytes
 *      - length - 0x03 = 64 bytes
 *
 * @param[in] length
 *
 */
void flash_2_set_burst( uint8_t length );

/**
 * @brief <h3> Flash 2 Read Burst with Wrap through SQI </h3>
 *
 * @par
 *  During RBSQI, the internal address pointer automati-
 * cally increments until the last byte of the burst is
 * reached, then it wraps around to the first byte of the
 * burst. For example, if the burst length is eight Bytes,
 * and the start address is 06h, the burst sequence would be:
 * 06h, 07h, 00h, 01h, 02h, 03h, 04h, 05h, 06h, etc. The pattern
 * repeats until the command is terminated by a low-to-high
 * transition on CE#.
 *  During this operation, blocks that are Read-locked will
 * output data 00H.
 *
 * 8 Bytes 00-07H, 08-0FH, 10-17H, 18-1FH...
 * 16 Bytes 00-0FH, 10-1FH, 20-2FH, 30-3FH...
 * 32 Bytes 00-1FH, 20-3FH, 40-5FH, 60-7FH...
 * 64 Bytes 00-3FH, 40-7FH, 80-BFH, C0-FFH
 *
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data to.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_read_sqi_burst_wrap( uint32_t address, uint8_t *buffer,
                                uint32_t data_count);

/**
 * @brief <h3> Flash 2 Read Burst with Wrap through SPI </h3>
 *
 * @par
 *  During RBSPI, the internal address pointer automati-
 * cally increments until the last byte of the burst is
 * reached, then it wraps around to the first byte of the
 * burst. For example, if the burst length is eight Bytes,
 * and the start address is 06h, the burst sequence would be:
 * 06h, 07h, 00h, 01h, 02h, 03h, 04h, 05h, 06h, etc. The pattern
 * repeats until the command is terminated by a low-to-high
 * transition on CE#.
 *  During this operation, blocks that are Read-locked will
 * output data 00H.
 *
 * 8 Bytes 00-07H, 08-0FH, 10-17H, 18-1FH...
 * 16 Bytes 00-0FH, 10-1FH, 20-2FH, 30-3FH...
 * 32 Bytes 00-1FH, 20-3FH, 40-5FH, 60-7FH...
 * 64 Bytes 00-3FH, 40-7FH, 80-BFH, C0-FFH
 *
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data to.
 * @param[in] data_count - Amount of bytes to read.
 *
 *
 */
void flash_2_read_spi_burst_wrap( uint32_t address, uint8_t *buffer,
                                uint32_t data_count);

/**
 * @brief <h3> Flash 2 Read Dual Output </h3>
 *
 * @par
 *  The SPI Dual-Output Read instruction supports fre-
 * quencies of up to 104 MHz from 2.7-3.6V and up to 80
 * MHz from 2.3-3.6V.
 *
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data to.
 * @param[in] data_count - Amount of bytes to read.
 *
 *
 */
void flash_2_read_dual_output( uint32_t address, uint8_t *buffer,
                                uint32_t data_count);

/**
 * @brief <h3> Flash 2 Read Dual I/O </h3>
 *
 * @par
 *  Following the Set Mode configuration bits, the
 * SST26VF064B/064BA outputs data from the specified
 * address location. The device continually streams data
 * output through all addresses until terminated by a low-
 * to-high transition on CE#.
 *
 *  The Set Mode Configuration bit M[7:0] indicates if the
 * next instruction cycle is another SPI Dual I/O Read
 * command. When M[7:0] = AXH, the device expects the
 * next continuous instruction to be another SDIOR com-
 * mand, BBH, and does not require the op-code to be
 * entered again. The host may set the next SDIOR cycle
 * by driving CE# low, then sending the two-bit wide input
 * for address A[23:0], followed by the Set Mode configu-
 * ration bits M[7:0]. After the Set Mode configuration bits,
 * the device outputs the data starting from the specified
 * address location. There are no restrictions on address
 * location access.
 *
 *  When M[7:0] is any value other than AXH, the device
 * expects the next instruction initiated to be a command
 * instruction. To reset/exit the Set Mode configuration,
 * To reset/exit the Set Mode configuration, execute the
 * Reset Quad I/O command.
 *
 * @todo Mode Selection must be implemented
 *
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data to.
 * @param[in] data_count - Amount of bytes to read.
 *
 *
 */
void flash_2_read_dual_io( uint32_t address, uint8_t *buffer,
                          uint32_t data_count);

/**
 * @brief <h3> Flash 2 Sector Erase </h3>
 *
 * @par
 *  The Sector-Erase instruction clears all bits in the
 * selected 4 KByte sector to ‘1,’ but it does not change a
 * protected memory area.
 *
 * @param[in] address - Address to start sector erase from.
 */
void flash_2_sector_erase( uint32_t address );

/**
 * @brief <h3> Flash 2 Block Erase </h3>
 *
 * @par
 *  The Block-Erase instruction clears all bits in the
 * selected block to ‘1’. Block sizes can be 8 KByte, 32
 * KByte or 64 KByte depending on address.
 * A Block-Erase instruction applied to a protected memory
 * area will be ignored.
 *
 * @param[in] address - Address to start block erase from.
 *
 */
void flash_2_block_erase( uint32_t address );

/**
 * @brief <h3> Flash 2 Chip Erase </h3>
 *
 * @par
 *  The Chip-Erase instruction clears all bits in the device
 * to ‘1.’ The Chip-Erase instruction is ignored if any of the
 * memory area is protected.
 *
 *
 */
void flash_2_chip_erase( void );

/**
 * @brief <h3> Flash 2 Quad Write </h3>
 *
 * @par
 *  The SPI Quad Page-Program instruction programs up
 * to 256 Bytes of data in the memory. The data for the
 * selected page address must be in the erased state
 * (FFH) before initiating the SPI Quad Page-Program
 * operation. A SPI Quad Page-Program applied to a pro-
 * tected memory area will be ignored. SST26VF064B
 * requires the ICO bit in the configuration register to be
 * set to ‘1’ prior to executing the command.
 *
 * @param[in] address - Address to start write at.
 * @param[in] buffer - Buffer with data to write.
 * @param[in] data_count - Amount of bytes to write.
 */
void flash_2_quad_write( uint32_t address, uint8_t *buffer,
                        uint32_t data_count );

/**
 * @brief <h3> Flash 2 get Serial Flash Discoverable \
 *  Parameters</h3>
 *
 * @par
 *  The Serial Flash Discoverable Parameters (SFDP)
 * contain information describing the characteristics of the
 * device. This allows device-independent, JEDEC ID-
 * independent, and forward/backward compatible soft-
 * ware support for all future Serial Flash device families.
 *
 * @param[in] address - Address to start reading from.
 * @param[out] buffer - Buffer to read data into.
 * @param[in] data_count - Amount of bytes to read.
 */
void flash_2_get_sfdp_params( uint32_t address, uint8_t *buffer,
                          uint32_t data_count);

/**
 * @brief <h3> Flash 2 Write Suspend </h3>
 *
 * @par
 *  Write-Suspend allows the interruption of Sector-Erase,
 * Block-Erase, SPI Quad Page-Program, or Page-Pro-
 * gram operations in order to erase, program, or read
 * data in another portion of memory. The original opera-
 * tion can be continued with the Write-Resume com-
 * mand. This operation is supported in both SQI and SPI
 * protocols.
 *  Only one write operation can be suspended at a time;
 * if an operation is already suspended, the device will
 * ignore the Write-Suspend command. Write-Suspend
 * during Chip-Erase is ignored; Chip-Erase is not a valid
 * command while a write is suspended. The Write-
 * Resume command is ignored until any write operation
 * (Program or Erase) initiated during the Write-Suspend
 * is complete. The device requires a minimum of 500 μs
 * between each Write-Suspend command.
 *
 *
 */
void flash_2_write_suspend( void );

/**
 * @brief <h3> Flash 2 Write Resume </h3>
 *
 * @par
 *  Write-Suspend allows the interruption of Sector-Erase,
 * Block-Erase, SPI Quad Page-Program, or Page-Pro-
 * gram operations in order to erase, program, or read
 * data in another portion of memory. The original opera-
 * tion can be continued with the Write-Resume com-
 * mand. This operation is supported in both SQI and SPI
 * protocols.
 *  Only one write operation can be suspended at a time;
 * if an operation is already suspended, the device will
 * ignore the Write-Suspend command. Write-Suspend
 * during Chip-Erase is ignored; Chip-Erase is not a valid
 * command while a write is suspended. The Write-
 * Resume command is ignored until any write operation
 * (Program or Erase) initiated during the Write-Suspend
 * is complete. The device requires a minimum of 500 μs
 * between each Write-Suspend command.
 *
 */
void flash_2_write_resume( void );

/**
 * @brief <h3> Flash 2 Get Security ID SPI </h3>
 *
 * @par
 *  Reads the Unique ID Pre-Programmed at factory
 *
 * @param[out] buffer - Buffer to read data into.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_spi_get_security_id( uint8_t *buffer,
                               uint32_t data_count);

/**
 * @brief <h3> Flash 2 Get Security ID SQI </h3>
 *
 * @par
 *  Reads the Unique ID Pre-Programmed at factory
 *
 * @param[out] buffer - Buffer to read data into.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_sqi_get_security_id( uint8_t *buffer,
                               uint32_t data_count);

/**
 * @brief <h3> Flash 2 Set Security ID </h3>
 *
 * @par
 *  The Program Security ID instruction programs one to
 * 2040 Bytes of data in the user-programmable, Security
 * ID space. This Security ID space is one-time program-
 * mable (OTP). The device ignores a Program Security
 * ID instruction pointing to an invalid or protected
 * address
 *
 * @param[in] buffer - Buffer to write into.
 * @param[in] data_count - Amount of bytes to write.
 *
 */
void flash_2_set_security_id( uint8_t *buffer,
                              uint32_t data_count );

/**
 * @brief <h3> Flash 2 Get Block Protection Register SPI </h3>
 *
 * @par
 *  The Read Block-Protection Register instruction outputs
 * the Block-Protection register data which determines
 * the protection status.
 *
 * @param[out] buffer - Buffer to read data into.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_spi_get_bpr( uint8_t *buffer , uint32_t data_count);

/**
 * @brief <h3> Flash 2 Get Block Protection Register SQI </h3>
 *
 * @par
 *  The Read Block-Protection Register instruction outputs
 * the Block-Protection register data which determines
 * the protection status.
 *
 * @param[out] buffer - Buffer to read data into.
 * @param[in] data_count - Amount of bytes to read.
 *
 */
void flash_2_sqi_get_bpr( uint8_t *buffer , uint32_t data_count);

/**
 * @brief <h3> Flash 2 Set Block Protection Register </h3>
 *
 * @par
 *  The Write Block-Protection Register command changes
 * the Block-Protection register data to indicate the protection status.
 *
 * @param[in] buffer - Buffer with new BPR register values to write.
 *
 */
void flash_2_set_bpr( uint8_t *buffer);

/**
 * @brief <h3> Flash 2 Lock Block Protection Register </h3>
 *
 * @par
 *  The Lock-Down Block-Protection Register instruction
 * prevents changes to the Block-Protection register
 * during device operation. Lock-Down resets after power
 * cycling; this allows the Block-Protection register to be
 * changed.
 *
 *
 */
void flash_2_lock_bpr( void );

/**
 * @brief <h3> Flash 2 Non-Volatile Write-Lock </h3>
 *
 * @par
 *  The Non-Volatile Write-Lock Lock-Down Register
 * (nVWLDR) instruction controls the ability to change the
 * Write-Lock bits in the Block-Protection register.
 *
 * @param[in] buffer - Buffer with new values for BPR register.
 *
 */
void flash_2_nonvolatile_write_lock( uint8_t *buffer);

/**
 * @brief <h3> Flash 2 Global Block Unlock </h3>
 *
 * @par
 *  The Global Block-Protection Unlock instruction clears all
 * write-protection bits in the Block-Protection register,
 * except for those bits that have been locked down with the
 * nVWLDR command.
 *
 *
 */
void flash_2_global_block_unlock( void );

/**
 * @brief <h3> Flash 2 Quad Device Manufacturer </h3>
 *
 * @par
 *  The Read Quad J-ID Read instruction identifies the
 * device as SST26VF064B/064BA and manufacturer as
 * Microchip.
 *
 * @return - Device Manufacturer
 *
 *
 */
uint8_t flash_2_quad_device_manufac( void );

/**
 * @brief <h3> Flash 2 Quad Device Type </h3>
 *
 * @par
 *  The Read Quad J-ID Read instruction identifies the
 * device as SST26VF064B/064BA and manufacturer as
 * Microchip.
 *
 * @return - Device Type
 *
 *
 */
uint8_t flash_2_quad_device_type( void );

/**
 * @brief <h3> Flash 2 Quad Device ID </h3>
 *
 * @par
 *  The Read Quad J-ID Read instruction identifies the
 * device as SST26VF064B/064BA and manufacturer as
 * Microchip.
 *
 * @return - Device ID
 *
 *
 */
uint8_t flash_2_quad_device_id( void );


/**
 * @brief <h3> Flash 2 Device Manufacturer </h3>
 *
 * @par
 *  Using traditional SPI protocol, the JEDEC-ID Read
 * instruction identifies the device as SST26VF064B/
 * 064BA and the manufacturer as Microchip®.
 *
 * @return - Device Manufacturer
 *
 *
 */
uint8_t flash_2_device_manufac( void );

/**
 * @brief <h3> Flash 2 Device Type </h3>
 *
 * @par
 *  Using traditional SPI protocol, the JEDEC-ID Read
 * instruction identifies the device as SST26VF064B/
 * 064BA and the manufacturer as Microchip®.
 *
 * @return - Device Type
 *
 *
 */
uint8_t flash_2_device_type( void );

/**
 * @brief <h3> Flash 2 Device ID </h3>
 *
 * @par
 *  Using traditional SPI protocol, the JEDEC-ID Read
 * instruction identifies the device as SST26VF064B/
 * 064BA and the manufacturer as Microchip®.
 *
 * @return - Device ID
 *
 *
 */
uint8_t flash_2_device_id( void );

/**
 * @brief <h3> Flash 2 Reset </h3>
 *
 * @par
 *  The Reset operation is used as a system (software)
 *  reset that puts the device in normal operating Ready
 *  mode. This operation consists of two commands:
 *  Reset-Enable (RSTEN) followed by Reset (RST).
 *
 */
void flash_2_reset( void );

/**
 * @brief <h3> Flash 2 Write Status Register </h3>
 *
 * @par
 *  The Write-Status Register (WRSR) command writes
 * new values to the Configuration register.
 *
 * @param[in] sReg - New Conifuration Register Values
 */
void flash_2_write_status_reg( uint8_t sReg );

/**
 * @brief <h3> Flash 2 Get Config Register </h3>
 *
 * @par
 *  The Read-Configuration Register command outputs the contents
 * of the Configuration register.
 *
 * @returns Contents of Configuration Register
 *
 */
uint8_t flash_2_get_config_reg( void );

/**
 * @brief <h3> Flash 2 Write Enable </h3>
 *
 * @par
 *  The Write Enable (WREN) instruction sets the Write-
 * Enable-Latch bit in the Status register to ‘1,’ allowing
 * Write operations to occur.
 *
 */
void flash_2_write_enable( void );

/**
 * @brief <h3> Flash 2 Write Disable </h3>
 *
 * @par
 *  The Write Disable instruction sets the Write-
 * Enable-Latch bit in the Status register to ‘0,’ not allowing
 * Write operations to occur.
 *
 */
void flash_2_write_disable( void );

/**
 * @brief flash_2_lock_security_id
 *
 * @par
 *  The Lockout Security ID instruction prevents any future
 * changes to the Security ID, and is supported in both
 * SPI and SQI modes.
 *
 */
void flash_2_lock_security_id( void );

#ifdef __cplusplus
} // extern "C"
#endif

#endif
/*** End of File **************************************************************/
