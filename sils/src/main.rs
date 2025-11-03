use std::net::Ipv4Addr;

use c2a_dev_runtime as c2a_runtime;
use c2a_tobc_geox as _;

use c2a_monazite_adc_dev::Adc;
use c2a_monazite_btmgr_dev::Btmgr;
use c2a_monazite_ccsds_dev::Ccsds;
use c2a_monazite_gpio_dev::Gpio;
use c2a_monazite_iflash_dev::Iflash;
use c2a_monazite_ramecc_dev::Ramecc;
use c2a_monazite_thermometer_dev::Thermometer;
use c2a_monazite_uart_dev::Uart;
use c2a_monazite_wdt_dev::Wdt;

use c2a_monazite_adc_bind::C2A_MONAZITE_ADC;
use c2a_monazite_btmgr_bind::C2A_MONAZITE_BTMGR;
use c2a_monazite_ccsds_bind::C2A_MONAZITE_CCSDS;
use c2a_monazite_gpio_bind::C2A_MONAZITE_GPIO;
use c2a_monazite_iflash_bind::C2A_MONAZITE_IFLASH;
use c2a_monazite_ramecc_bind::C2A_MONAZITE_RAMECC;
use c2a_monazite_thermometer_bind::C2A_MONAZITE_THERMOMETER;
use c2a_monazite_uart_bind::C2A_MONAZITE_UART;
use c2a_monazite_wdt_bind::C2A_MONAZITE_WDT;

macro_rules! dyn_static {
    ($v:expr) => {
        Box::leak(Box::new(Box::leak(Box::new($v)) as &'static _))
    };
}

#[allow(clippy::similar_names)]
fn main() {
    let wdt = Wdt::new();
    C2A_MONAZITE_WDT.set(dyn_static!(wdt));

    let gpio = Gpio::default();
    C2A_MONAZITE_GPIO.set(dyn_static!(gpio));

    let adc = Adc::new();
    C2A_MONAZITE_ADC.set(dyn_static!(adc));

    let thermometer = Thermometer::new();
    C2A_MONAZITE_THERMOMETER.set(dyn_static!(thermometer));

    let btmgr = Btmgr::new();
    C2A_MONAZITE_BTMGR.set(dyn_static!(btmgr));

    let ccsds = Ccsds::new((Ipv4Addr::UNSPECIFIED, 22545).into());
    C2A_MONAZITE_CCSDS.set(dyn_static!(ccsds));

    let uart = Uart::new((Ipv4Addr::UNSPECIFIED, 9696).into());
    C2A_MONAZITE_UART.set(dyn_static!(uart));

    let iflash = Iflash::new();
    C2A_MONAZITE_IFLASH.set(dyn_static!(iflash));

    let ramecc = Ramecc::new();
    C2A_MONAZITE_RAMECC.set(dyn_static!(ramecc));

    c2a_runtime::c2a_init();
    c2a_runtime::c2a_main();
}
