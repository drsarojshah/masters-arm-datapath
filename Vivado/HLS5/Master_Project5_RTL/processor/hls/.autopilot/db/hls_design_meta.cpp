#include "hls_design_meta.h"
const Port_Property HLS_Design_Meta::port_props[]={
	Port_Property("ap_clk", 1, hls_in, -1, "", "", 1),
	Port_Property("ap_rst", 1, hls_in, -1, "", "", 1),
	Port_Property("ap_start", 1, hls_in, -1, "", "", 1),
	Port_Property("ap_done", 1, hls_out, -1, "", "", 1),
	Port_Property("ap_idle", 1, hls_out, -1, "", "", 1),
	Port_Property("ap_ready", 1, hls_out, -1, "", "", 1),
	Port_Property("pc_in", 32, hls_in, 0, "ap_none", "in_data", 1),
	Port_Property("main_memory_out", 32, hls_out, 1, "ap_vld", "out_data", 1),
	Port_Property("main_memory_out_ap_vld", 1, hls_out, 1, "ap_vld", "out_vld", 1),
	Port_Property("opcode_out", 32, hls_out, 2, "ap_vld", "out_data", 1),
	Port_Property("opcode_out_ap_vld", 1, hls_out, 2, "ap_vld", "out_vld", 1),
	Port_Property("op_value_1_out", 32, hls_out, 3, "ap_vld", "out_data", 1),
	Port_Property("op_value_1_out_ap_vld", 1, hls_out, 3, "ap_vld", "out_vld", 1),
	Port_Property("op_value_2_out", 32, hls_out, 4, "ap_vld", "out_data", 1),
	Port_Property("op_value_2_out_ap_vld", 1, hls_out, 4, "ap_vld", "out_vld", 1),
	Port_Property("result_out", 32, hls_out, 5, "ap_vld", "out_data", 1),
	Port_Property("result_out_ap_vld", 1, hls_out, 5, "ap_vld", "out_vld", 1),
};
const char* HLS_Design_Meta::dut_name = "processor";
