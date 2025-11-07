#ifndef SILS_PORT_CONFIG_H_
#define SILS_PORT_CONFIG_H_

// ============================================ //
//             SILS関連のポート設定              //
// ============================================ //
#ifdef USE_UART_COM
#ifdef WIN32
#define COM_PORT_SILS (11)  //!< COM11
#else
#define COM_PORT_SILS (1)  //!< /dev/tnt1
#endif
#endif

#endif
