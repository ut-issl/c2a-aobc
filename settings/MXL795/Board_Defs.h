/************************************************************************/
/*																		*/
/*	Board_Defs.h --	Board Customization for Digilent chipKIT Max32		*/
/*																		*/
/************************************************************************/
/*	Author: Gene Apperson												*/
/*	Copyright 2011, Digilent. All rights reserved						*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/* This file contains the board specific declartions and data structure	*/
/* to customize the chipKIT MPIDE for use with the Digilent chipKIT		*/
/* Max32 board.															*/
/*																		*/
/* This code is based on earlier work:									*/
/*		Copyright (c) 2010, 2011 by Mark Sproul							*/
/*		Copyright (c) 2005, 2006 by David A. Mellis						*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	10/06/2011(GeneA): Created											*/
/*	11/28/2011(GeneA): Moved data definitions and configuration			*/
/*		functions to Board_Data.c										*/
/*	11/29/2011(GeneA): Moved int priority definitions to System_Defs.h	*/
/*	03/31/2012(GeneA): added support for second LED on Rev D boards		*/
/*																		*/
/************************************************************************/
//*	This library is free software; you can redistribute it and/or
//*	modify it under the terms of the GNU Lesser General Public
//*	License as published by the Free Software Foundation; either
//*	version 2.1 of the License, or (at your option) any later version.
//*	
//*	This library is distributed in the hope that it will be useful,
//*	but WITHOUT ANY WARRANTY; without even the implied warranty of
//*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//*	Lesser General Public License for more details.
//*	
//*	You should have received a copy of the GNU Lesser General
//*	Public License along with this library; if not, write to the
//*	Free Software Foundation, Inc., 59 Temple Place, Suite 330,
//*	Boston, MA  02111-1307  USA
/************************************************************************/

#if !defined(BOARD_DEFS_H)
#define BOARD_DEFS_H

#include <inttypes.h>

/* ------------------------------------------------------------ */
/*				Public Board Declarations						*/
/* ------------------------------------------------------------ */
/* The following define symbols that can be used in a sketch to
** refer to periperhals on the board generically.
*/

#define	_BOARD_NAME_	"PIC32MX795F512L(80MHz)"

/* Define the Microcontroller peripherals available on the board.
*/
#define	NUM_DIGITAL_PINS	101
#define	NUM_ANALOG_PINS		16
#define	NUM_OC_PINS			5
#define	NUM_IC_PINS			5
#define	NUM_TCK_PINS		4
#define	NUM_INT_PINS		5

#define	NUM_SERIAL_PORTS	6
#define	NUM_SPI_PORTS		1
#define	NUM_I2C_PORTS		1

#define	NUM_DSPI_PORTS		4
#define NUM_DTWI_PORTS		5

/* Define I/O devices on the board.
*/
#define	NUM_LED				0
#define NUM_BTN				0
#define	NUM_SWT				0
#define NUM_SERVO			0

/* Define the number of extended i/o pins. These are pins
** that are not native to the microcontroller. This board
** doesn't have any.
*/
#define	NUM_DIGITAL_PINS_EXTENDED	NUM_DIGITAL_PINS
#define	NUM_ANALOG_PINS_EXTENDED	NUM_ANALOG_PINS

/* ------------------------------------------------------------ */
/*						LED Declarations						*/
/* ------------------------------------------------------------ */

/* Define the pin numbers for the LEDs
*/
//#define	PIN_LED1	13
//#define	PIN_LED1	96

/* ------------------------------------------------------------ */
/*					Button Declarations							*/
/* ------------------------------------------------------------ */

/* No buttons on this board.
*/

/* ------------------------------------------------------------ */
/*					Switch Declarations							*/
/* ------------------------------------------------------------ */

/* No switches on this board.
*/

/* ------------------------------------------------------------ */
/*					Servo Pin Declarations						*/
/* ------------------------------------------------------------ */

/* No servo connectors on this board.
*/

/* ------------------------------------------------------------ */
/*					Timer Pin Declarations						*/
/* ------------------------------------------------------------ */

#define PIN_OC1		72
#define	PIN_OC2		76
#define	PIN_OC3		77
#define	PIN_OC4		78
#define	PIN_OC5		81

#define PIN_IC1		68
#define PIN_IC2		69
#define PIN_IC3		70
#define PIN_IC4		71
#define	PIN_IC5		79

#define	PIN_TCK1	74 
#define	PIN_TCK2	6	
#define	PIN_TCK3	7
#define	PIN_TCK4	8
#define	PIN_TCK5	9

/* ------------------------------------------------------------ */
/*					Interrupt Pin Declarations					*/
/* ------------------------------------------------------------ */

#define	PIN_INT0	72
#define	PIN_INT1	18
#define PIN_INT2	19
#define	PIN_INT3	66
#define	PIN_INT4	67

/* ------------------------------------------------------------ */
/*					SPI Pin Declarations						*/
/* ------------------------------------------------------------ */
/* These symbols are defined for compatibility with the original
** SPI library and the original pins_arduino.h
*/
/* ikari edit to suit with AOBC */
const static uint8_t SS   = 69;		// PIC32 SS1A
const static uint8_t MOSI = 72;		// PIC32 SDO1A
const static uint8_t MISO = 9;		// PIC32 SDI1A
const static uint8_t SCK  = 70;		// PIC32 SCK1A

/* The Digilent DSPI library uses these ports.
*/
#define	PIN_DSPI0_SS	69		//SPI1
#define	PIN_DSPI1_SS	96		//SPI2
#define	PIN_DSPI2_SS	97		//SPI3
#define	PIN_DSPI3_SS	95		//SPI4

/* ------------------------------------------------------------ */
/*					Analog Pins									*/
/* ------------------------------------------------------------ */
/* Define symbols for accessing the analog pins. This table is
** used to map an analog pin number to the corresponding digital
** pin number.
*/
#define	A0		25
#define	A1		24
#define	A2		23
#define	A3		22
#define	A4		21
#define	A5		20
#define	A6		26
#define	A7		27
#define	A8		32
#define	A9		33
#define	A10		34
#define	A11		35
#define	A12		41
#define	A13		42
#define	A14		43
#define	A15		44

/* ------------------------------------------------------------ */
/*					Change Notice Pins							*/
/* ------------------------------------------------------------ */
/* These define the pin numbers for the various change notice
** pins.
*/
#define	PIN_CN0		74 
#define	PIN_CN1		73 
#define	PIN_CN2		25
#define	PIN_CN3		24
#define	PIN_CN4		23
#define	PIN_CN5		22
#define	PIN_CN6		21
#define	PIN_CN7		20
#define	PIN_CN8		10
#define	PIN_CN9		11
#define	PIN_CN10	12
#define	PIN_CN11	14
#define	PIN_CN12	44
#define	PIN_CN13	81
#define	PIN_CN14	82
#define	PIN_CN15	83
#define	PIN_CN16	84
#define	PIN_CN17	49
#define	PIN_CN18	50
#define	PIN_CN19	80
#define	PIN_CN20	47
#define	PIN_CN21	48

/* ------------------------------------------------------------ */
/*					Pin Mapping Macros							*/
/* ------------------------------------------------------------ */
/*
 This section contains the definitions for pin mapping macros that
 are being redefined for this board variant.
*/

#undef digitalPinToAnalog
#define	digitalPinToAnalog(P) ( (P) < 16 ? (P) : ((P) >= 54) && ((P) < 70) ? (P)-54 : NOT_ANALOG_PIN )

/* ------------------------------------------------------------ */
/*					Data Definitions							*/
/* ------------------------------------------------------------ */

/* The following declare externals to access the pin mapping
 tables. These tables are defined in Board_Data.c.
*/

#if !defined(OPT_BOARD_DATA)

extern const uint32_t	port_to_tris_PGM[];
extern const uint8_t	digital_pin_to_port_PGM[];
extern const uint16_t	digital_pin_to_bit_mask_PGM[];
extern const uint16_t	digital_pin_to_timer_PGM[];

#endif

/* ------------------------------------------------------------ */
/*				Internal Declarations							*/
/* ------------------------------------------------------------ */
/* The following declarations are used to map peripherals for	*/
/* the core and libraries and to provide configuration options	*/
/* for the core. They are not normally needed by a user sketch.	*/
/* ------------------------------------------------------------ */

#if defined(OPT_BOARD_INTERNAL)

/* ------------------------------------------------------------ */
/*				Core Configuration Declarations					*/
/* ------------------------------------------------------------ */
/*																*/
/* These are conditional compilation switches that control the	*/
/* board core configuration functions. These functions provide	*/
/* hooks that can call from some of the core functions into		*/
/* functions defined below that can be used to extend or		*/
/* replace the default behavior of the core function. To use	*/
/* this, enter the appropriate code into the appropriate		*/
/* function skeleton below and then set the appropriate switch	*/
/* value to 1. This will cause the configuration function to be	*/
/* compiled into the build and will cause the code to call the	*/
/* hook function to be compiled into the core function.			*/
/*																*/
/* This board requires some additional initialization, so it	*/
/* sets OPT_BOARD_INIT to 1 to cause the extra init function	*/
/* to be included.												*/
/*																*/
/* ------------------------------------------------------------ */

#define	OPT_BOARD_INIT			0	//board needs custom init code
#define	OPT_BOARD_DIGITAL_IO	0	//board does not extend digital i/o functions
#define	OPT_BOARD_ANALOG_READ	0	//board does not extend analogRead
#define	OPT_BOARD_ANALOG_WRITE	0	//board does not extend analogWrite

/* ------------------------------------------------------------ */
/*					Serial Port Declarations					*/
/* ------------------------------------------------------------ */

/* Serial port 0 uses UART1
*/
#define	_SER0_BASE		_UART1_BASE_ADDRESS
#define	_SER0_IRQ		_UART1_ERR_IRQ
#define	_SER0_VECTOR	_UART_1_VECTOR
#define _SER0_IPL_ISR	IPL2SOFT
#define	_SER0_IPL		2
#define	_SER0_SPL		0

/* Serial port 1 uses UART2
*/
#define	_SER1_BASE		_UART2_BASE_ADDRESS
#define	_SER1_IRQ		_UART2_ERR_IRQ
#define	_SER1_VECTOR	_UART_2_VECTOR
#define _SER1_IPL_ISR	IPL2SOFT
#define	_SER1_IPL		2
#define	_SER1_SPL		0

/* Serial port 2 uses UART3 
*/
#define	_SER2_BASE		_UART3_BASE_ADDRESS
#define	_SER2_IRQ		_UART3_ERR_IRQ
#define	_SER2_VECTOR	_UART_3_VECTOR
#define _SER2_IPL_ISR	IPL2SOFT
#define	_SER2_IPL		2
#define	_SER2_SPL		0

/* Serial port 3 uses UART4 
*/
#define	_SER3_BASE		_UART4_BASE_ADDRESS
#define	_SER3_IRQ		_UART4_ERR_IRQ
#define	_SER3_VECTOR	_UART_4_VECTOR
#define _SER3_IPL_ISR	IPL2SOFT
#define	_SER3_IPL		2
#define	_SER3_SPL		0

/* Serial port 4 uses UART5 
*/
#define	_SER4_BASE		_UART5_BASE_ADDRESS
#define	_SER4_IRQ		_UART5_ERR_IRQ
#define	_SER4_VECTOR	_UART_5_VECTOR
#define _SER4_IPL_ISR	IPL2SOFT
#define	_SER4_IPL		2
#define	_SER4_SPL		0

/* Serial port 5 uses UART6
*/
#define	_SER5_BASE		_UART6_BASE_ADDRESS
#define	_SER5_IRQ		_UART6_ERR_IRQ
#define	_SER5_VECTOR	_UART_6_VECTOR
#define _SER5_IPL_ISR	IPL2SOFT
#define	_SER5_IPL		2
#define	_SER5_SPL		0


/* ------------------------------------------------------------ */
/*					SPI Port Declarations						*/
/* ------------------------------------------------------------ */

/* The standard SPI port uses SPI1.
*/

#define	_SPI_BASE			_SPI1_BASE_ADDRESS
#define _SPI_ERR_IRQ		_SPI1_ERR_IRQ
#define	_SPI_RX_IRQ			_SPI1_RX_IRQ
#define	_SPI_TX_IRQ			_SPI1_TX_IRQ
#define	_SPI_VECTOR			_SPI_1_VECTOR
#define	_SPI_IPL_ISR		_SPI1_IPL_ISR
#define	_SPI_IPL			_SPI1_IPL_IPC
#define	_SPI_SPL			_SPI1_SPL_IPC

#define	_DSPI0_BASE			_SPI1_BASE_ADDRESS
#define	_DSPI0_ERR_IRQ		_SPI1_ERR_IRQ
#define	_DSPI0_RX_IRQ		_SPI1_RX_IRQ
#define	_DSPI0_TX_IRQ		_SPI1_TX_IRQ
#define	_DSPI0_VECTOR		_SPI_1_VECTOR
#define	_DSPI0_IPL_ISR		_SPI1_IPL_ISR
#define	_DSPI0_IPL			_SPI1_IPL_IPC
#define	_DSPI0_SPL			_SPI1_SPL_IPC

#define	_DSPI1_BASE			_SPI2_BASE_ADDRESS
#define	_DSPI1_ERR_IRQ		_SPI2_ERR_IRQ
#define	_DSPI1_RX_IRQ		_SPI2_RX_IRQ
#define	_DSPI1_TX_IRQ		_SPI2_TX_IRQ
#define	_DSPI1_VECTOR		_SPI_2_VECTOR
#define	_DSPI1_IPL_ISR		_SPI2_IPL_ISR
#define	_DSPI1_IPL			_SPI2_IPL_IPC
#define	_DSPI1_SPL			_SPI2_SPL_IPC

#define	_SPI3_ERR_IRQ		_SPI1A_ERR_IRQ	//this declaration missing from the
										//Microchip header file
#define	_DSPI2_BASE			_SPI3_BASE_ADDRESS
#define	_DSPI2_ERR_IRQ		_SPI3_ERR_IRQ
#define	_DSPI2_RX_IRQ		_SPI3_RX_IRQ
#define	_DSPI2_TX_IRQ		_SPI3_TX_IRQ
#define	_DSPI2_VECTOR		_SPI_3_VECTOR
#define	_DSPI2_IPL_ISR		_SPI3_IPL_ISR
#define	_DSPI2_IPL			_SPI3_IPL_IPC
#define	_DSPI2_SPL			_SPI3_SPL_IPC

#define	_DSPI3_BASE			_SPI4_BASE_ADDRESS
#define	_DSPI3_ERR_IRQ		_SPI4_ERR_IRQ
#define	_DSPI3_RX_IRQ		_SPI4_RX_IRQ
#define	_DSPI3_TX_IRQ		_SPI4_TX_IRQ
#define	_DSPI3_VECTOR		_SPI_4_VECTOR
#define	_DSPI3_IPL_ISR		_SPI4_IPL_ISR
#define	_DSPI3_IPL			_SPI4_IPL_IPC
#define	_DSPI3_SPL			_SPI4_SPL_IPC

/* ------------------------------------------------------------ */
/*					I2C Port Declarations						*/
/* ------------------------------------------------------------ */

/* The standard I2C port uses I2C1 (SCL1/SDA1). These come to digital
** pins 20 & 21 on the communications connector (J4)
*/


#define	_TWI_BASE		_I2C1_BASE_ADDRESS
#define	_TWI_BUS_IRQ	_I2C1_BUS_IRQ
#define	_TWI_SLV_IRQ	_I2C1_SLAVE_IRQ
#define	_TWI_MST_IRQ	_I2C1_MASTER_IRQ
#define	_TWI_VECTOR		_I2C_1_VECTOR
#define	_TWI_IPL_ISR	_I2C1_IPL_ISR
#define _TWI_IPL		_I2C1_IPL_IPC
#define	_TWI_SPL		_I2C1_SPL_IPC

/*
#define	_TWI_BASE		_I2C2_BASE_ADDRESS
#define	_TWI_BUS_IRQ	_I2C2_BUS_IRQ
#define	_TWI_SLV_IRQ	_I2C2_SLAVE_IRQ
#define	_TWI_MST_IRQ	_I2C2_MASTER_IRQ
#define	_TWI_VECTOR		_I2C_2_VECTOR
#define	_TWI_IPL_ISR	_I2C2_IPL_ISR
#define _TWI_IPL		_I2C2_IPL_IPC
#define	_TWI_SPL		_I2C2_SPL_IPC
*/
/* Declarations for Digilent DTWI library.
**		DTWI0:	SDA pin 67, SCL pin 66
**		DTWI1:	SDA pin 59, SCL pin 58
**		DTWI2:	SDA pin 52,  SCL pin 53
**		DTWI3:	SDA pin 11, SCL pin 12
**		DTWI4:	SDA pin 49, SCL pin 50
*/
/*
#define	_DTWI0_BASE		_I2C2_BASE_ADDRESS
#define	_DTWI0_BUS_IRQ	_I2C2_BUS_IRQ
#define	_DTWI0_VECTOR	_I2C_2_VECTOR
#define	_DTWI0_IPL_ISR	_I2C2_IPL_ISR
#define	_DTWI0_IPL		_I2C2_IPL_IPC
#define	_DTWI0_SPL		_I2C2_SPL_IPC
#define _DTWI0_SCL_PIN  58
#define _DTWI0_SDA_PIN  59

#define	_DTWI1_BASE		_I2C1_BASE_ADDRESS
#define	_DTWI1_BUS_IRQ	_I2C1_BUS_IRQ
#define	_DTWI1_VECTOR	_I2C_1_VECTOR
#define	_DTWI1_IPL_ISR	_I2C1_IPL_ISR
#define	_DTWI1_IPL		_I2C1_IPL_IPC
#define	_DTWI1_SPL		_I2C1_SPL_IPC
#define _DTWI1_SCL_PIN  66 
#define _DTWI1_SDA_PIN  67
*/
#define	_DTWI0_BASE		_I2C1_BASE_ADDRESS
#define	_DTWI0_BUS_IRQ	_I2C1_BUS_IRQ
#define	_DTWI0_VECTOR	_I2C_1_VECTOR
#define	_DTWI0_IPL_ISR	_I2C1_IPL_ISR
#define	_DTWI0_IPL		_I2C1_IPL_IPC
#define	_DTWI0_SPL		_I2C1_SPL_IPC
#define _DTWI0_SCL_PIN  66
#define _DTWI0_SDA_PIN  67

#define	_DTWI1_BASE		_I2C2_BASE_ADDRESS
#define	_DTWI1_BUS_IRQ	_I2C2_BUS_IRQ
#define	_DTWI1_VECTOR	_I2C_2_VECTOR
#define	_DTWI1_IPL_ISR	_I2C2_IPL_ISR
#define	_DTWI1_IPL		_I2C2_IPL_IPC
#define	_DTWI1_SPL		_I2C2_SPL_IPC
#define _DTWI1_SCL_PIN  58 
#define _DTWI1_SDA_PIN  59

#define	_DTWI2_BASE		_I2C3_BASE_ADDRESS
#define	_DTWI2_BUS_IRQ	_I2C3_BUS_IRQ
#define	_DTWI2_VECTOR	_I2C_3_VECTOR
#define	_DTWI2_IPL_ISR	_I2C3_IPL_ISR
#define	_DTWI2_IPL		_I2C3_IPL_IPC
#define	_DTWI2_SPL		_I2C3_SPL_IPC
#define _DTWI2_SCL_PIN  53
#define _DTWI2_SDA_PIN  52

#define	_DTWI3_BASE		_I2C4_BASE_ADDRESS
#define	_DTWI3_BUS_IRQ	_I2C4_BUS_IRQ
#define	_DTWI3_VECTOR	_I2C_4_VECTOR
#define	_DTWI3_IPL_ISR	_I2C4_IPL_ISR
#define	_DTWI3_IPL		_I2C4_IPL_IPC
#define	_DTWI3_SPL		_I2C4_SPL_IPC
#define _DTWI3_SCL_PIN  12 
#define _DTWI3_SDA_PIN  11

#define	_DTWI4_BASE		_I2C5_BASE_ADDRESS
#define	_DTWI4_BUS_IRQ	_I2C5_BUS_IRQ
#define	_DTWI4_VECTOR	_I2C_5_VECTOR
#define	_DTWI4_IPL_ISR	_I2C5_IPL_ISR
#define	_DTWI4_IPL		_I2C5_IPL_IPC
#define	_DTWI4_SPL		_I2C5_SPL_IPC
#define _DTWI4_SCL_PIN  50 
#define _DTWI4_SDA_PIN  49


/* ------------------------------------------------------------ */
/*					A/D Converter Declarations					*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */

#endif	//OPT_BOARD_INTERNAL

/* ------------------------------------------------------------ */

#endif	// BOARD_DEFS_H

/************************************************************************/
