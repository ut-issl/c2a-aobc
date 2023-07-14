/************************************************************************/
/*																		*/
/*	Board_Data.c -- Board Customization Data for Digilent chipKIT Max32	*/
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
/*	11/28/2011(GeneA): Created by splitting data out of Board_Defs.h	*/
/*	03/31/2012(GeneA): added support for second LED on Rev D boards		*/
/*	02/12/2013(GeneA): removed dependency on Microchip plib library		*/
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

#if !defined(BOARD_DATA_C)
#define BOARD_DATA_C

#include <inttypes.h>

/* ------------------------------------------------------------ */
/*					Data Tables									*/
/* ------------------------------------------------------------ */
/* The following declarations define data used in pin mapping.	*/
/* ------------------------------------------------------------ */

#if defined(OPT_BOARD_DATA)

/* ------------------------------------------------------------ */
/* This table is used to map from port number to the address of
** the TRIS register for the port. This is used for setting the
** pin direction.
*/
const uint32_t port_to_tris_PGM[] = {
	NOT_A_PORT,				//index value 0 is not used

#if defined(_PORTA)
	(uint32_t)&TRISA,
#else
	NOT_A_PORT,
#endif

#if defined(_PORTB)
	(uint32_t)&TRISB,
#else
	NOT_A_PORT,
#endif

#if defined(_PORTC)
	(uint32_t)&TRISC,
#else
	NOT_A_PORT,
#endif

#if defined(_PORTD)
	(uint32_t)&TRISD,
#else
	NOT_A_PORT,
#endif

#if defined(_PORTE)
	(uint32_t)&TRISE,
#else
	NOT_A_PORT,
#endif

#if defined(_PORTF)
	(uint32_t)&TRISF,
#else
	NOT_A_PORT,
#endif

#if defined(_PORTG)
	(uint32_t)&TRISG,
#else
	NOT_A_PORT,
#endif

	NOT_A_PORT,
};

/* ------------------------------------------------------------ */
/* This table is used to map the digital pin number to the port
** containing that pin.
*/
const uint8_t digital_pin_to_port_PGM[] = {
	NOT_A_PORT,		//0
	_IOPORT_PG,		//1		AERXERR/RG15
	NOT_A_PORT,		//2		VDD
	_IOPORT_PE,		//3		PMD5/RE5
	_IOPORT_PE,		//4		PMD6/RE6
	_IOPORT_PE,		//5		PMD7/RE7
	_IOPORT_PC,		//6		T2CK/RC1
	_IOPORT_PC,		//7		T3CK/AC2TX/RC2
	_IOPORT_PC,		//8		T4CK/AC2RX/RC3
	_IOPORT_PC,		//9		T5CK/SDI1/RC4
	_IOPORT_PG,		//10	ECOL/SCK2/U6TX/U3RTS/PMA5/CN8/RG6
	_IOPORT_PG,		//11	ECRS/SDA4/SDI2/U3RX/PMA4/CN9/RG7
	_IOPORT_PG,		//12	ERXDV/AERXDV/ECRSDV/AECRSDV/SCL4/SDO2/U3TX/PMA3/CN10/RG8
	NOT_A_PORT,		//13	//MCLR
	_IOPORT_PG,		//14	ERXCLK/AERXCLK/EREFCLK/AEREFCLK/SS2/U6RX/U3CTS/PMA2/CN11/RG9
	NOT_A_PORT,		//15	//VSS
	NOT_A_PORT,		//16	//VDD
	_IOPORT_PA,		//17	TMS/RA0
	_IOPORT_PE,		//18	AERXD0/INT1/RE8
	_IOPORT_PE,		//19	AERXD1/INT2/RE9
	_IOPORT_PB,		//20	AN5/C1IN+/VBUSON/CN7/RB5
	_IOPORT_PB,		//21	AN4/C1IN-/CN6/RB4
	_IOPORT_PB,		//22	AN3/C2IN+/CN5/RB3
	_IOPORT_PB,		//23	AN2/C2IN-/CN4/RB2
	_IOPORT_PB,		//24	PGEC1/AN1/CN3/RB1
	_IOPORT_PB,		//25	PGED1/AN0/CN2/RB0
	_IOPORT_PB,		//26	PGEC2/AN6/OCFA/RB6
	_IOPORT_PB,		//27	PGED2/AN7/RB7
	_IOPORT_PA,		//28	VREF+/CVREF+/AERXD3/PMA6/RA10
	_IOPORT_PA,		//29	VREF-/CVREF-/AERXD2/PMA7/RA9
	NOT_A_PORT,		//30	//AVDD
	NOT_A_PORT,		//31	//AVSS
	_IOPORT_PB,		//32	AN8/C1OUT/RB8
	_IOPORT_PB,		//33	AN9/C2OUT/RB9
	_IOPORT_PB,		//34	AN10/CVREFOUT/PMA13/RB10
	_IOPORT_PB,		//35	AN11/ERXERR/AETXERR/PMA12/RB11
	NOT_A_PORT,		//36	//VSS
	NOT_A_PORT,		//37	//VDD
	_IOPORT_PA,		//38	TCK/RA1
	_IOPORT_PF,		//39	AC1TX/SCK4/U5TX/U2RTS/RF13
	_IOPORT_PF,		//40	AC1RX/SS4/U5RX/U2CTS/RF12
	_IOPORT_PB,		//41	AN12/ERXD0/AECRS/PMA11/RB12
	_IOPORT_PB,		//42	AN13/ERXD1/AECOL/PMA10/RB13
	_IOPORT_PB,		//43	AN14/ERXD2/AETXD3/PMALH/PMA1/RB14
	_IOPORT_PB,		//44	AN15/ERXD3/AETXD2/OCFB/PMALL/PMA0/CN12/RB15
	NOT_A_PORT,		//45	//VSS
	NOT_A_PORT,		//46	//VDD
	_IOPORT_PD,		//47	AETXD0/SS3/U4RX/U1CTS/CN20/RD14
	_IOPORT_PD,		//48	AETXD1/SCK3/U4TX/U1RTS/CN21/RD15
	_IOPORT_PF,		//49	SDA5/SDI4/U2RX/PMA9/CN17/RF4
	_IOPORT_PF,		//50	SCL5/SDO4/U2TX/PMA8/CN18/RF5
	_IOPORT_PF,		//51	USBID/RF3
	_IOPORT_PF,		//52	SDA3/SDI3/U1RX/RF2
	_IOPORT_PF,		//53	SCL3/SDO3/U1TX/RF8
	NOT_A_PORT,		//54	//VBUS
	NOT_A_PORT,		//55	//VUSB3V3
	_IOPORT_PG,		//56	D-/RG3
	_IOPORT_PG,		//57	D+/RG2
	_IOPORT_PA,		//58	SCL2/RA2
	_IOPORT_PA,		//59	SDA2/RA3
	_IOPORT_PA,		//60	TDI/RA4
	_IOPORT_PA,		//61	TDO/RA5
	NOT_A_PORT,		//62	//VDD
	NOT_A_PORT,		//63	//OSC1/CLKI/RC12
	NOT_A_PORT,		//64	//OSC2/CLKO/RC15
	NOT_A_PORT,		//65	//VSS
	_IOPORT_PA,		//66	AETXCLK/SCL1/INT3/RA14
	_IOPORT_PA,		//67	AETXEN/SDA1/INT4/RA15
	_IOPORT_PD,		//68	RTCC/EMDIO/AEMDIO/IC1/RD8
	_IOPORT_PD,		//69	SS1/IC2/RD9
	_IOPORT_PD,		//70	SCK1/IC3/PMCS2/PMA15/RD10
	_IOPORT_PD,		//71	EMDC/AEMDC/IC4/PMCS1/PMA14/RD11
	_IOPORT_PD,		//72	SDO1/OC1/INT0/RD0
	_IOPORT_PC,		//73	//SOSCI/CN1/RC13
	_IOPORT_PC,		//74	//SOSCO/T1CK/CN0/RC14
	NOT_A_PORT,		//75	//VSS
	_IOPORT_PD,		//76	OC2/RD1
	_IOPORT_PD,		//77	OC3/RD2
	_IOPORT_PD,		//78	OC4/RD3
	_IOPORT_PD,		//79	ETXD2/IC5/PMD12/RD12
	_IOPORT_PD,		//80	ETXD3/PMD13/CN19/RD13
	_IOPORT_PD,		//81	OC5/PMWR/CN13/RD4
	_IOPORT_PD,		//82	PMRD/CN14/RD5
	_IOPORT_PD,		//83	ETXEN/PMD14/CN15/RD6
	_IOPORT_PD,		//84	ETXCLK/PMD15/CN16/RD7
	NOT_A_PORT,		//85	//VCAP/VDDCORE
	NOT_A_PORT,		//86	//VDD
	_IOPORT_PF,		//87	C1RX/ETXD1/PMD11/RF0
	_IOPORT_PF,		//88	C1TX/ETXD0/PMD10/RF1
	_IOPORT_PG,		//89	C2TX/ETXERR/PMD9/RG1
	_IOPORT_PG,		//90	C2RX/PMD8/RG0
	_IOPORT_PA,		//91	TRCLK/RA6
	_IOPORT_PA,		//92	TRD3/RA7
	_IOPORT_PE,		//93	PMD0/RE0
	_IOPORT_PE,		//94	PMD1/RE1
	_IOPORT_PG,		//95	TRD2/RG14
	_IOPORT_PG,		//96	TRD1/RG12
	_IOPORT_PG,		//97	TRD0/RG13
	_IOPORT_PE,		//98	PMD2/RE2
	_IOPORT_PE,		//99	PMD3/RE3
	_IOPORT_PE,		//100	PMD4/RE4
};

/* ------------------------------------------------------------ */
/* This table is used to map from digital pin number to a bit mask
** for the corresponding bit within the port.
*/
const uint16_t digital_pin_to_bit_mask_PGM[] = {
	NOT_A_PIN,		//0
	_BV( 15 ),		//AERXERR/RG15
	NOT_A_PIN,		//VDD
	_BV( 5 ),		//PMD5/RE5
	_BV( 6 ),		//PMD6/RE6
	_BV( 7 ),		//PMD7/RE7
	_BV( 1 ),		//T2CK/RC1
	_BV( 2 ),		//T3CK/AC2TX/RC2
	_BV( 3 ),		//T4CK/AC2RX/RC3
	_BV( 4 ),		//T5CK/SDI1/RC4
	_BV( 6 ),		//ECOL/SCK2/U6TX/U3RTS/PMA5/CN8/RG6
	_BV( 7 ),		//ECRS/SDA4/SDI2/U3RX/PMA4/CN9/RG7
	_BV( 8 ),		//ERXDV/AERXDV/ECRSDV/AECRSDV/SCL4/SDO2/U3TX/PMA3/CN10/RG8
	NOT_A_PIN,		////MCLR
	_BV( 9 ),		//ERXCLK/AERXCLK/EREFCLK/AEREFCLK/SS2/U6RX/U3CTS/PMA2/CN11/RG9
	NOT_A_PIN,		////VSS
	NOT_A_PIN,		////VDD
	_BV( 0 ),		//TMS/RA0
	_BV( 8 ),		//AERXD0/INT1/RE8
	_BV( 9 ),		//AERXD1/INT2/RE9
	_BV( 5 ),		//AN5/C1IN+/VBUSON/CN7/RB5
	_BV( 4 ),		//AN4/C1IN-/CN6/RB4
	_BV( 3 ),		//AN3/C2IN+/CN5/RB3
	_BV( 2 ),		//AN2/C2IN-/CN4/RB2
	_BV( 1 ),		//PGEC1/AN1/CN3/RB1
	_BV( 0 ),		//PGED1/AN0/CN2/RB0
	_BV( 6 ),		//PGEC2/AN6/OCFA/RB6
	_BV( 7 ),		//PGED2/AN7/RB7
	_BV( 10 ),		//VREF+/CVREF+/AERXD3/PMA6/RA10
	_BV( 9 ),		//VREF-/CVREF-/AERXD2/PMA7/RA9
	NOT_A_PIN,		////AVDD
	NOT_A_PIN,		////AVSS
	_BV( 8 ),		//AN8/C1OUT/RB8
	_BV( 9 ),		//AN9/C2OUT/RB9
	_BV( 10 ),		//AN10/CVREFOUT/PMA13/RB10
	_BV( 11 ),		//AN11/ERXERR/AETXERR/PMA12/RB11
	NOT_A_PIN,		////VSS
	NOT_A_PIN,		////VDD
	_BV( 1 ),		//TCK/RA1
	_BV( 13 ),		//AC1TX/SCK4/U5TX/U2RTS/RF13
	_BV( 12 ),		//AC1RX/SS4/U5RX/U2CTS/RF12
	_BV( 12 ),		//AN12/ERXD0/AECRS/PMA11/RB12
	_BV( 13 ),		//AN13/ERXD1/AECOL/PMA10/RB13
	_BV( 14 ),		//AN14/ERXD2/AETXD3/PMALH/PMA1/RB14
	_BV( 15 ),		//AN15/ERXD3/AETXD2/OCFB/PMALL/PMA0/CN12/RB15
	NOT_A_PIN,		////VSS
	NOT_A_PIN,		////VDD
	_BV( 14 ),		//AETXD0/SS3/U4RX/U1CTS/CN20/RD14
	_BV( 15 ),		//AETXD1/SCK3/U4TX/U1RTS/CN21/RD15
	_BV( 4 ),		//SDA5/SDI4/U2RX/PMA9/CN17/RF4
	_BV( 5 ),		//SCL5/SDO4/U2TX/PMA8/CN18/RF5
	_BV( 3 ),		//USBID/RF3
	_BV( 2 ),		//SDA3/SDI3/U1RX/RF2
	_BV( 8 ),		//SCL3/SDO3/U1TX/RF8
	NOT_A_PIN,		////VBUS
	NOT_A_PIN,		////VUSB3V3
	_BV( 3 ),		//D-/RG3
	_BV( 2 ),		//D+/RG2
	_BV( 2 ),		//SCL2/RA2
	_BV( 3 ),		//SDA2/RA3
	_BV( 4 ),		//TDI/RA4
	_BV( 5 ),		//TDO/RA5
	NOT_A_PIN,		////VDD
	NOT_A_PIN,		////OSC1/CLKI/RC12
	NOT_A_PIN,		////OSC2/CLKO/RC15
	NOT_A_PIN,		//VSS
	_BV( 14 ),		//AETXCLK/SCL1/INT3/RA14
	_BV( 15 ),		//AETXEN/SDA1/INT4/RA15
	_BV( 8 ),		//RTCC/EMDIO/AEMDIO/IC1/RD8
	_BV( 9 ),		//SS1/IC2/RD9
	_BV( 10 ),		//SCK1/IC3/PMCS2/PMA15/RD10
	_BV( 11 ),		//EMDC/AEMDC/IC4/PMCS1/PMA14/RD11
	_BV( 0 ),		//SDO1/OC1/INT0/RD0
	_BV( 13 ),		////SOSCI/CN1/RC13
	_BV( 14 ),		////SOSCO/T1CK/CN0/RC14
	NOT_A_PIN,		////VSS
	_BV( 1 ),		//OC2/RD1
	_BV( 2 ),		//OC3/RD2
	_BV( 3 ),		//OC4/RD3
	_BV( 12 ),		//ETXD2/IC5/PMD12/RD12
	_BV( 13 ),		//ETXD3/PMD13/CN19/RD13
	_BV( 4 ),		//OC5/PMWR/CN13/RD4
	_BV( 5 ),		//PMRD/CN14/RD5
	_BV( 6 ),		//ETXEN/PMD14/CN15/RD6
	_BV( 7 ),		//ETXCLK/PMD15/CN16/RD7
	NOT_A_PIN,		////VCAP/VDDCORE
	NOT_A_PIN,		////VDD
	_BV( 0 ),		//C1RX/ETXD1/PMD11/RF0
	_BV( 1 ),		//C1TX/ETXD0/PMD10/RF1
	_BV( 1 ),		//C2TX/ETXERR/PMD9/RG1
	_BV( 0 ),		//C2RX/PMD8/RG0
	_BV( 6 ),		//TRCLK/RA6
	_BV( 7 ),		//TRD3/RA7
	_BV( 0 ),		//PMD0/RE0
	_BV( 1 ),		//PMD1/RE1
	_BV( 14 ),		//TRD2/RG14
	_BV( 12 ),		//TRD1/RG12
	_BV( 13 ),		//TRD0/RG13
	_BV( 2 ),		//PMD2/RE2
	_BV( 3 ),		//PMD3/RE3
	_BV( 4 ),		//PMD4/RE4
};

/* ------------------------------------------------------------ */
/* This table is used to map from digital pin number to the output
** compare number, input capture number, and timer external clock
** input associated with that pin.
*/
const uint16_t digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,		//0
	NOT_ON_TIMER,		//1		AERXERR/RG15
	NOT_ON_TIMER,		//2		VDD
	NOT_ON_TIMER,		//3		PMD5/RE5
	NOT_ON_TIMER,		//4		PMD6/RE6
	NOT_ON_TIMER,		//5		PMD7/RE7
	_TIMER_TCK2,		//6		T2CK/RC1
	_TIMER_TCK3,		//7		T3CK/AC2TX/RC2
	_TIMER_TCK4,		//8		T4CK/AC2RX/RC3
	_TIMER_TCK5,		//9		T5CK/SDI1/RC4
	NOT_ON_TIMER,		//10	ECOL/SCK2/U6TX/U3RTS/PMA5/CN8/RG6
	NOT_ON_TIMER,		//11	ECRS/SDA4/SDI2/U3RX/PMA4/CN9/RG7
	NOT_ON_TIMER,		//12	ERXDV/AERXDV/ECRSDV/AECRSDV/SCL4/SDO2/U3TX/PMA3/CN10/RG8
	NOT_ON_TIMER,		//13	//MCLR
	NOT_ON_TIMER,		//14	ERXCLK/AERXCLK/EREFCLK/AEREFCLK/SS2/U6RX/U3CTS/PMA2/CN11/RG9
	NOT_ON_TIMER,		//15	//VSS
	NOT_ON_TIMER,		//16	//VDD
	NOT_ON_TIMER,		//17	TMS/RA0
	NOT_ON_TIMER,		//18	AERXD0/INT1/RE8
	NOT_ON_TIMER,		//19	AERXD1/INT2/RE9
	NOT_ON_TIMER,		//20	AN5/C1IN+/VBUSON/CN7/RB5
	NOT_ON_TIMER,		//21	AN4/C1IN-/CN6/RB4
	NOT_ON_TIMER,		//22	AN3/C2IN+/CN5/RB3
	NOT_ON_TIMER,		//23	AN2/C2IN-/CN4/RB2
	NOT_ON_TIMER,		//24	PGEC1/AN1/CN3/RB1
	NOT_ON_TIMER,		//25	PGED1/AN0/CN2/RB0
	NOT_ON_TIMER,		//26	PGEC2/AN6/OCFA/RB6
	NOT_ON_TIMER,		//27	PGED2/AN7/RB7
	NOT_ON_TIMER,		//28	VREF+/CVREF+/AERXD3/PMA6/RA10
	NOT_ON_TIMER,		//29	VREF-/CVREF-/AERXD2/PMA7/RA9
	NOT_ON_TIMER,		//30	//AVDD
	NOT_ON_TIMER,		//31	//AVSS
	NOT_ON_TIMER,		//32	AN8/C1OUT/RB8
	NOT_ON_TIMER,		//33	AN9/C2OUT/RB9
	NOT_ON_TIMER,		//34	AN10/CVREFOUT/PMA13/RB10
	NOT_ON_TIMER,		//35	AN11/ERXERR/AETXERR/PMA12/RB11
	NOT_ON_TIMER,		//36	//VSS
	NOT_ON_TIMER,		//37	//VDD
	NOT_ON_TIMER,		//38	TCK/RA1
	NOT_ON_TIMER,		//39	AC1TX/SCK4/U5TX/U2RTS/RF13
	NOT_ON_TIMER,		//40	AC1RX/SS4/U5RX/U2CTS/RF12
	NOT_ON_TIMER,		//41	AN12/ERXD0/AECRS/PMA11/RB12
	NOT_ON_TIMER,		//42	AN13/ERXD1/AECOL/PMA10/RB13
	NOT_ON_TIMER,		//43	AN14/ERXD2/AETXD3/PMALH/PMA1/RB14
	NOT_ON_TIMER,		//44	AN15/ERXD3/AETXD2/OCFB/PMALL/PMA0/CN12/RB15
	NOT_ON_TIMER,		//45	//VSS
	NOT_ON_TIMER,		//46	//VDD
	NOT_ON_TIMER,		//47	AETXD0/SS3/U4RX/U1CTS/CN20/RD14
	NOT_ON_TIMER,		//48	AETXD1/SCK3/U4TX/U1RTS/CN21/RD15
	NOT_ON_TIMER,		//49	SDA5/SDI4/U2RX/PMA9/CN17/RF4
	NOT_ON_TIMER,		//50	SCL5/SDO4/U2TX/PMA8/CN18/RF5
	NOT_ON_TIMER,		//51	USBID/RF3
	NOT_ON_TIMER,		//52	SDA3/SDI3/U1RX/RF2
	NOT_ON_TIMER,		//53	SCL3/SDO3/U1TX/RF8
	NOT_ON_TIMER,		//54	//VBUS
	NOT_ON_TIMER,		//55	//VUSB3V3
	NOT_ON_TIMER,		//56	D-/RG3
	NOT_ON_TIMER,		//57	D+/RG2
	NOT_ON_TIMER,		//58	SCL2/RA2
	NOT_ON_TIMER,		//59	SDA2/RA3
	NOT_ON_TIMER,		//60	TDI/RA4
	NOT_ON_TIMER,		//61	TDO/RA5
	NOT_ON_TIMER,		//62	//VDD
	NOT_ON_TIMER,		//63	//OSC1/CLKI/RC12
	NOT_ON_TIMER,		//64	//OSC2/CLKO/RC15
	NOT_ON_TIMER,		//65	//VSS
	NOT_ON_TIMER,		//66	AETXCLK/SCL1/INT3/RA14
	NOT_ON_TIMER,		//67	AETXEN/SDA1/INT4/RA15
	_TIMER_IC1,			//68	RTCC/EMDIO/AEMDIO/IC1/RD8
	_TIMER_IC2,			//69	SS1/IC2/RD9
	_TIMER_IC3,			//70	SCK1/IC3/PMCS2/PMA15/RD10
	_TIMER_IC4,			//71	EMDC/AEMDC/IC4/PMCS1/PMA14/RD11
	_TIMER_OC1,			//72	SDO1/OC1/INT0/RD0
	NOT_ON_TIMER,		//73	//SOSCI/CN1/RC13
	_TIMER_TCK1,		//74	//SOSCO/T1CK/CN0/RC14
	NOT_ON_TIMER,		//75	//VSS
	_TIMER_OC2,			//76	OC2/RD1
	_TIMER_OC3,			//77	OC3/RD2
	_TIMER_OC4,			//78	OC4/RD3
	_TIMER_IC5,			//79	ETXD2/IC5/PMD12/RD12
	NOT_ON_TIMER,		//80	ETXD3/PMD13/CN19/RD13
	_TIMER_OC5,			//81	OC5/PMWR/CN13/RD4
	NOT_ON_TIMER,		//82	PMRD/CN14/RD5
	NOT_ON_TIMER,		//83	ETXEN/PMD14/CN15/RD6
	NOT_ON_TIMER,		//84	ETXCLK/PMD15/CN16/RD7
	NOT_ON_TIMER,		//85	//VCAP/VDDCORE
	NOT_ON_TIMER,		//86	//VDD
	NOT_ON_TIMER,		//87	C1RX/ETXD1/PMD11/RF0
	NOT_ON_TIMER,		//88	C1TX/ETXD0/PMD10/RF1
	NOT_ON_TIMER,		//89	C2TX/ETXERR/PMD9/RG1
	NOT_ON_TIMER,		//90	C2RX/PMD8/RG0
	NOT_ON_TIMER,		//91	TRCLK/RA6
	NOT_ON_TIMER,		//92	TRD3/RA7
	NOT_ON_TIMER,		//93	PMD0/RE0
	NOT_ON_TIMER,		//94	PMD1/RE1
	NOT_ON_TIMER,		//95	TRD2/RG14
	NOT_ON_TIMER,		//96	TRD1/RG12
	NOT_ON_TIMER,		//97	TRD0/RG13
	NOT_ON_TIMER,		//98	PMD2/RE2
	NOT_ON_TIMER,		//99	PMD3/RE3
	NOT_ON_TIMER,		//100	PMD4/RE4
};

const uint32_t digital_pin_to_cn_PGM[] =
{
   	NOT_CN_PIN,		//0
	NOT_CN_PIN,		//1		AERXERR/RG15
	NOT_CN_PIN,		//2		VDD
	NOT_CN_PIN,		//3		PMD5/RE5
	NOT_CN_PIN,		//4		PMD6/RE6
	NOT_CN_PIN,		//5		PMD7/RE7
	NOT_CN_PIN,		//6		T2CK/RC1
	NOT_CN_PIN,		//7		T3CK/AC2TX/RC2
	NOT_CN_PIN,		//8		T4CK/AC2RX/RC3
	NOT_CN_PIN,		//9		T5CK/SDI1/RC4
	_BV(8),			//10	ECOL/SCK2/U6TX/U3RTS/PMA5/CN8/RG6
	_BV(9),			//11	ECRS/SDA4/SDI2/U3RX/PMA4/CN9/RG7
	_BV(10),		//12	ERXDV/AERXDV/ECRSDV/AECRSDV/SCL4/SDO2/U3TX/PMA3/CN10/RG8
	NOT_CN_PIN,		//13	//MCLR
	_BV(11),		//14	ERXCLK/AERXCLK/EREFCLK/AEREFCLK/SS2/U6RX/U3CTS/PMA2/CN11/RG9
	NOT_CN_PIN,		//15	//VSS
	NOT_CN_PIN,		//16	//VDD
	NOT_CN_PIN,		//17	TMS/RA0
	NOT_CN_PIN,		//18	AERXD0/INT1/RE8
	NOT_CN_PIN,		//19	AERXD1/INT2/RE9
	NOT_CN_PIN,		//20	AN5/C1IN+/VBUSON/CN7/RB5
	_BV(6),			//21	AN4/C1IN-/CN6/RB4
	_BV(5),			//22	AN3/C2IN+/CN5/RB3
	_BV(4),			//23	AN2/C2IN-/CN4/RB2
	_BV(3),			//24	PGEC1/AN1/CN3/RB1
	_BV(2),			//25	PGED1/AN0/CN2/RB0
	NOT_CN_PIN,		//26	PGEC2/AN6/OCFA/RB6
	NOT_CN_PIN,		//27	PGED2/AN7/RB7
	NOT_CN_PIN,		//28	VREF+/CVREF+/AERXD3/PMA6/RA10
	NOT_CN_PIN,		//29	VREF-/CVREF-/AERXD2/PMA7/RA9
	NOT_CN_PIN,		//30	//AVDD
	NOT_CN_PIN,		//31	//AVSS
	NOT_CN_PIN,		//32	AN8/C1OUT/RB8
	NOT_CN_PIN,		//33	AN9/C2OUT/RB9
	NOT_CN_PIN,		//34	AN10/CVREFOUT/PMA13/RB10
	NOT_CN_PIN,		//35	AN11/ERXERR/AETXERR/PMA12/RB11
	NOT_CN_PIN,		//36	//VSS
	NOT_CN_PIN,		//37	//VDD
	NOT_CN_PIN,		//38	TCK/RA1
	NOT_CN_PIN,		//39	AC1TX/SCK4/U5TX/U2RTS/RF13
	NOT_CN_PIN,		//40	AC1RX/SS4/U5RX/U2CTS/RF12
	NOT_CN_PIN,		//41	AN12/ERXD0/AECRS/PMA11/RB12
	NOT_CN_PIN,		//42	AN13/ERXD1/AECOL/PMA10/RB13
	NOT_CN_PIN,		//43	AN14/ERXD2/AETXD3/PMALH/PMA1/RB14
	_BV(12),		//44	AN15/ERXD3/AETXD2/OCFB/PMALL/PMA0/CN12/RB15
	NOT_CN_PIN,		//45	//VSS
	NOT_CN_PIN,		//46	//VDD
	_BV(20),		//47	AETXD0/SS3/U4RX/U1CTS/CN20/RD14
	_BV(21),		//48	AETXD1/SCK3/U4TX/U1RTS/CN21/RD15
	_BV(17),		//49	SDA5/SDI4/U2RX/PMA9/CN17/RF4
	_BV(18),		//50	SCL5/SDO4/U2TX/PMA8/CN18/RF5
	NOT_CN_PIN,		//51	USBID/RF3
	NOT_CN_PIN,		//52	SDA3/SDI3/U1RX/RF2
	NOT_CN_PIN,		//53	SCL3/SDO3/U1TX/RF8
	NOT_CN_PIN,		//54	//VBUS
	NOT_CN_PIN,		//55	//VUSB3V3
	NOT_CN_PIN,		//56	D-/RG3
	NOT_CN_PIN,		//57	D+/RG2
	NOT_CN_PIN,		//58	SCL2/RA2
	NOT_CN_PIN,		//59	SDA2/RA3
	NOT_CN_PIN,		//60	TDI/RA4
	NOT_CN_PIN,		//61	TDO/RA5
	NOT_CN_PIN,		//62	//VDD
	NOT_CN_PIN,		//63	//OSC1/CLKI/RC12
	NOT_CN_PIN,		//64	//OSC2/CLKO/RC15
	NOT_CN_PIN,		//65	//VSS
	NOT_CN_PIN,		//66	AETXCLK/SCL1/INT3/RA14
	NOT_CN_PIN,		//67	AETXEN/SDA1/INT4/RA15
	NOT_CN_PIN,		//68	RTCC/EMDIO/AEMDIO/IC1/RD8
	NOT_CN_PIN,		//69	SS1/IC2/RD9
	NOT_CN_PIN,		//70	SCK1/IC3/PMCS2/PMA15/RD10
	NOT_CN_PIN,		//71	EMDC/AEMDC/IC4/PMCS1/PMA14/RD11
	NOT_CN_PIN,		//72	SDO1/OC1/INT0/RD0
	_BV(1),			//73	//SOSCI/CN1/RC13
	_BV(0),			//74	//SOSCO/T1CK/CN0/RC14
	NOT_CN_PIN,		//75	//VSS
	NOT_CN_PIN,		//76	OC2/RD1
	NOT_CN_PIN,		//77	OC3/RD2
	NOT_CN_PIN,		//78	OC4/RD3
	NOT_CN_PIN,		//79	ETXD2/IC5/PMD12/RD12
	_BV(19),		//80	ETXD3/PMD13/CN19/RD13
	_BV(13),		//81	OC5/PMWR/CN13/RD4
	_BV(14),		//82	PMRD/CN14/RD5
	_BV(15),		//83	ETXEN/PMD14/CN15/RD6
	_BV(16),		//84	ETXCLK/PMD15/CN16/RD7
	NOT_CN_PIN,		//85	//VCAP/VDDCORE
	NOT_CN_PIN,		//86	//VDD
	NOT_CN_PIN,		//87	C1RX/ETXD1/PMD11/RF0
	NOT_CN_PIN,		//88	C1TX/ETXD0/PMD10/RF1
	NOT_CN_PIN,		//89	C2TX/ETXERR/PMD9/RG1
	NOT_CN_PIN,		//90	C2RX/PMD8/RG0
	NOT_CN_PIN,		//91	TRCLK/RA6
	NOT_CN_PIN,		//92	TRD3/RA7
	NOT_CN_PIN,		//93	PMD0/RE0
	NOT_CN_PIN,		//94	PMD1/RE1
	NOT_CN_PIN,		//95	TRD2/RG14
	NOT_CN_PIN,		//96	TRD1/RG12
	NOT_CN_PIN,		//97	TRD0/RG13
	NOT_CN_PIN,		//98	PMD2/RE2
	NOT_CN_PIN,		//99	PMD3/RE3
	NOT_CN_PIN,		//100	PMD4/RE4
};

/* ------------------------------------------------------------ */
/*		Include Files for Board Customization Functions			*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Board Customization Functions					*/
/* ------------------------------------------------------------ */
/*																*/
/* The following can be used to customize the behavior of some	*/
/* of the core API functions. These provide hooks that can be	*/
/* used to extend or replace the default behavior of the core	*/
/* functions. To use one of these functions, add the desired	*/
/* code to the function skeleton below and then set the value	*/
/* of the appropriate compile switch above to 1. This will		*/
/* cause the hook function to be compiled into the build and	*/
/* to cause the code to call the hook function to be compiled	*/
/* into the appropriate core function.							*/
/*																*/
/* ------------------------------------------------------------ */
/***	_board_init
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function is called from the core init() function.
**		This can be used to perform any board specific init
**		that needs to be done when the processor comes out of
**		reset and before the user sketch is run.
*/
#if	(OPT_BOARD_INIT != 0)

void _board_init(void) {

	/*	Turn off Secondary oscillator so pins can be used as GPIO
	*/
	//OSCCONCLR	=	_OSCCON_SOSCEN_MASK;

}

#endif

/* ------------------------------------------------------------ */
/***	_board_pinMode
**
**	Parameters:
**		pin		- digital pin number to configure
**		mode	- mode to which the pin should be configured
**
**	Return Value:
**		Returns 0 if not handled, !0 if handled.
**
**	Errors:
**		none
**
**	Description:
**		This function is called at the beginning of the pinMode
**		function. It can perform any special processing needed
**		when setting the pin mode. If this function returns zero,
**		control will pass through the normal pinMode code. If
**		it returns a non-zero value the normal pinMode code isn't
**		executed.
*/
#if	(OPT_BOARD_DIGITAL_IO != 0)

int	_board_pinMode(uint8_t pin, uint8_t mode) {
	
	return 0;

}

#endif

/* ------------------------------------------------------------ */
/***	_board_getPinMode
**
**	Parameters:
**		pin		- digital pin number
**		mode	- pointer to variable to receive mode value
**
**	Return Value:
**		Returns 0 if not handled, !0 if handled.
**
**	Errors:
**		none
**
**	Description:
**		This function is called at the beginning of the getPinMode
**		function. It can perform any special processing needed
**		when getting the pin mode. If this function returns zero,
**		control will pass through the normal getPinMode code. If
**		it returns a non-zero value the normal getPinMode code isn't
**		executed.
*/
#if	(OPT_BOARD_DIGITAL_IO != 0)

int	_board_getPinMode(uint8_t pin, uint8_t * mode) {
	
	return 0;

}

#endif

/* ------------------------------------------------------------ */
/***	_board_digitalWrite
**
**	Parameters:
**		pin		- digital pin number
**		val		- value to write to the pin
**
**	Return Value:
**		Returns 0 if not handled, !0 if handled.
**
**	Errors:
**		none
**
**	Description:
**		This function is called at the beginning of the digitalWrite
**		function. It can perform any special processing needed
**		in writing to the pin. If this function returns zero,
**		control will pass through the normal digitalWrite code. If
**		it returns a non-zero value the normal digitalWrite code isn't
**		executed.
*/#if	(OPT_BOARD_DIGITAL_IO != 0)

int	_board_digitalWrite(uint8_t pin, uint8_t val) {
	
	return 0;

}

#endif

/* ------------------------------------------------------------ */
/***	_board_digitalRead
**
**	Parameters:
**		pin		- digital pin number
**		val		- pointer to variable to receive pin value
**
**	Return Value:
**		Returns 0 if not handled, !0 if handled.
**
**	Errors:
**		none
**
**	Description:
**		This function is called at the beginning of the digitalRead
**		function. It can perform any special processing needed
**		in reading from the pin. If this function returns zero,
**		control will pass through the normal digitalRead code. If
**		it returns a non-zero value the normal digitalRead code isn't
**		executed.
*/
#if	(OPT_BOARD_DIGITAL_IO != 0)

int	_board_digitalRead(uint8_t pin, uint8_t * val) {
	
	return 0;

}

#endif

/* ------------------------------------------------------------ */
/***	_board_analogRead
**
**	Parameters:
**		pin		- analog channel number
**		val		- pointer to variable to receive analog value
**
**	Return Value:
**		Returns 0 if not handled, !0 if handled.
**
**	Errors:
**		none
**
**	Description:
**		This function is called at the beginning of the analogRead
**		function. It can perform any special processing needed
**		in reading from the pin. If this function returns zero,
**		control will pass through the normal analogRead code. If
**		it returns a non-zero value the normal analogRead code isn't
**		executed.
*/
#if (OPT_BOARD_ANALOG_READ != 0)

int	_board_analogRead(uint8_t pin, int * val) {

	return 0;

}

#endif

/* ------------------------------------------------------------ */
/***	_board_analogReference
**
**	Parameters:
**
**	Return Value:
**		Returns 0 if not handled, !0 if handled.
**
**	Errors:
**		none
**
**	Description:
**		This function is called at the beginning of the analogReference
**		function. It can perform any special processing needed
**		to set the reference voltage. If this function returns zero,
**		control will pass through the normal analogReference code. If
**		it returns a non-zero value the normal analogReference code isn't
**		executed.
*/
#if (OPT_BOARD_ANALOG_READ != 0)

int	_board_analogReference(uint8_t mode) {

	return 0;

}

#endif

/* ------------------------------------------------------------ */
/***	_board_analogWrite
**
**	Parameters:
**		pin		- pin number
**		val		- analog value to write
**
**	Return Value:
**		Returns 0 if not handled, !0 if handled.
**
**	Errors:
**		none
**
**	Description:
**		This function is called at the beginning of the analogWrite
**		function. It can perform any special processing needed
**		in writing to the pin. If this function returns zero,
**		control will pass through the normal analogWrite code. If
**		it returns a non-zero value the normal analogWrite code isn't
**		executed.
*/
#if (OPT_BOARD_ANALOG_WRITE != 0)

int	_board_analogWrite(uint8_t pin, int val) {

	return 0;

}

#endif

#endif // OPT_BOARD_DATA

/* ------------------------------------------------------------ */

#endif	// BOARD_DATA_C

/************************************************************************/
