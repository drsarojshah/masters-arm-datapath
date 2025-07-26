set moduleName processor
set isTopModule 1
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set hasInterrupt 0
set DLRegFirstOffset 0
set DLRegItemOffset 0
set svuvm_can_support 1
set cdfgNum 2
set C_modelName {processor}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
set C_modelArgList {
	{ pc_in int 32 regular  }
	{ main_memory_out int 32 regular {pointer 1}  }
	{ opcode_out int 32 regular {pointer 1}  }
	{ op_value_1_out int 32 regular {pointer 1}  }
	{ op_value_2_out int 32 regular {pointer 1}  }
	{ result_out int 32 regular {pointer 1}  }
}
set hasAXIMCache 0
set l_AXIML2Cache [list]
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "pc_in", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "main_memory_out", "interface" : "wire", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "opcode_out", "interface" : "wire", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "op_value_1_out", "interface" : "wire", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "op_value_2_out", "interface" : "wire", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "result_out", "interface" : "wire", "bitwidth" : 32, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 17
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ pc_in sc_in sc_lv 32 signal 0 } 
	{ main_memory_out sc_out sc_lv 32 signal 1 } 
	{ main_memory_out_ap_vld sc_out sc_logic 1 outvld 1 } 
	{ opcode_out sc_out sc_lv 32 signal 2 } 
	{ opcode_out_ap_vld sc_out sc_logic 1 outvld 2 } 
	{ op_value_1_out sc_out sc_lv 32 signal 3 } 
	{ op_value_1_out_ap_vld sc_out sc_logic 1 outvld 3 } 
	{ op_value_2_out sc_out sc_lv 32 signal 4 } 
	{ op_value_2_out_ap_vld sc_out sc_logic 1 outvld 4 } 
	{ result_out sc_out sc_lv 32 signal 5 } 
	{ result_out_ap_vld sc_out sc_logic 1 outvld 5 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "pc_in", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "pc_in", "role": "default" }} , 
 	{ "name": "main_memory_out", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "main_memory_out", "role": "default" }} , 
 	{ "name": "main_memory_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "main_memory_out", "role": "ap_vld" }} , 
 	{ "name": "opcode_out", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "opcode_out", "role": "default" }} , 
 	{ "name": "opcode_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "opcode_out", "role": "ap_vld" }} , 
 	{ "name": "op_value_1_out", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "op_value_1_out", "role": "default" }} , 
 	{ "name": "op_value_1_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "op_value_1_out", "role": "ap_vld" }} , 
 	{ "name": "op_value_2_out", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "op_value_2_out", "role": "default" }} , 
 	{ "name": "op_value_2_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "op_value_2_out", "role": "ap_vld" }} , 
 	{ "name": "result_out", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "result_out", "role": "default" }} , 
 	{ "name": "result_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "result_out", "role": "ap_vld" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2"],
		"CDFG" : "processor",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "7",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "pc_in", "Type" : "None", "Direction" : "I"},
			{"Name" : "main_memory_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "opcode_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "op_value_1_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "op_value_2_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "result_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "main_memory_2", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "register_file_1", "Type" : "Memory", "Direction" : "IO"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.main_memory_2_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.register_file_1_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	processor {
		pc_in {Type I LastRead 0 FirstWrite -1}
		main_memory_out {Type O LastRead -1 FirstWrite 2}
		opcode_out {Type O LastRead -1 FirstWrite 2}
		op_value_1_out {Type O LastRead -1 FirstWrite 4}
		op_value_2_out {Type O LastRead -1 FirstWrite 4}
		result_out {Type O LastRead -1 FirstWrite 7}
		main_memory_2 {Type IO LastRead -1 FirstWrite -1}
		register_file_1 {Type IO LastRead -1 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "7"}
	, {"Name" : "Interval", "Min" : "2", "Max" : "8"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	pc_in { ap_none {  { pc_in in_data 0 32 } } }
	main_memory_out { ap_vld {  { main_memory_out out_data 1 32 }  { main_memory_out_ap_vld out_vld 1 1 } } }
	opcode_out { ap_vld {  { opcode_out out_data 1 32 }  { opcode_out_ap_vld out_vld 1 1 } } }
	op_value_1_out { ap_vld {  { op_value_1_out out_data 1 32 }  { op_value_1_out_ap_vld out_vld 1 1 } } }
	op_value_2_out { ap_vld {  { op_value_2_out out_data 1 32 }  { op_value_2_out_ap_vld out_vld 1 1 } } }
	result_out { ap_vld {  { result_out out_data 1 32 }  { result_out_ap_vld out_vld 1 1 } } }
}

set maxi_interface_dict [dict create]

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
