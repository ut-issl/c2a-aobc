#ifndef NBC_HEADER_H_
#define NBC_HEADER_H_

#include <src_core/TlmCmd/block_command_table.h>

// AOCS
void BCL_load_start_hk_tlm(void);
void BCL_load_select_rm3100_on_aobc(void);
void BCL_load_select_rm3100_external(void);
void BCL_set_sagitta_parameter(void);
void BCL_read_sagitta_parameter(void);

// Power
void BCL_load_power_on_mpu9250(void);
void BCL_load_power_on_rm3100(void);
void BCL_load_power_on_nanossoc_d60(void);
void BCL_load_power_on_stim210(void);
void BCL_load_power_on_sagitta(void);
void BCL_load_power_on_oem7600(void);
void BCL_load_power_on_mtq(void);
void BCL_load_power_on_rwx(void);
void BCL_load_power_on_rwy(void);
void BCL_load_power_on_rwz(void);
void BCL_load_power_on_ina260(void);
void BCL_load_set_ina260_oc_limit(void);
void BCL_load_power_off_mpu9250(void);
void BCL_load_power_off_rm3100(void);
void BCL_load_power_off_nanossoc_d60(void);
void BCL_load_power_off_stim210(void);
void BCL_load_power_off_sagitta(void);
void BCL_load_power_off_oem7600(void);
void BCL_load_power_off_mtq(void);
void BCL_load_power_off_rwx(void);
void BCL_load_power_off_rwy(void);
void BCL_load_power_off_rwz(void);
void BCL_load_power_off_ina260(void);
void BCL_load_reset_mpu9250(void);
void BCL_load_reset_rm3100(void);
void BCL_load_reset_nanossoc_d60(void);
void BCL_load_reset_stim210(void);
void BCL_load_reset_sagitta(void);
void BCL_load_reset_oem7600(void);
void BCL_load_reset_mtq(void);
void BCL_load_reset_rwx(void);
void BCL_load_reset_rwy(void);
void BCL_load_reset_rwz(void);
void BCL_load_reset_ina260(void);

// Anomaly
void BCL_load_activate_ina260_eh(void);
void BCL_load_activate_nanossoc_d60_eh(void);
void BCL_load_inactivate_nanossoc_d60_eh(void);
void BCL_load_activate_stim210_el_eh(void);
void BCL_load_inactivate_stim210_el_eh(void);
void BCL_load_activate_sagitta_el_eh(void);
void BCL_load_inactivate_sagitta_el_eh(void);
void BCL_load_activate_oem7600_el_eh(void);
void BCL_load_inactivate_oem7600_el_eh(void);

#endif
