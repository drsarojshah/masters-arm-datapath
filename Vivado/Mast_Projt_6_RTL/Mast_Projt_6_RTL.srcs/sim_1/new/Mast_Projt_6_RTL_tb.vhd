----------------------------------------------------------------------------------
-- Project Name:"Modeling the execution engine (datapath) portion of a 32-bit ARM processor" 

-- Student# S. Shah 
-- Master’s Project (IECE - 695)| UAlbany
-- Create Date: 05/06/2025
-- Advisor Name: Prof. James R. Moulic
-- Module Name: Mast_Projt_6_RTL_tb - Behavioral

-- Description:This project focuses on modeling and simulating a simplified execution engine 
--              (datapath), main memory, and register-file of an ARM Cortex A76 processor using 
--              a selected subset of the ARMv8/ARMv7 instruction set architecture. 
-- Testbench created online at:
--   https://www.doulos.com/knowhow/perl/vhdl-testbench-creation-using-perl/
-- Copyright Doulos Ltd

----------------------------------------------------------------------------------
library IEEE;
use IEEE.Std_logic_1164.all;
use IEEE.Numeric_Std.all;

entity processor_tb is
end;

architecture bench of processor_tb is

  component processor
  port (
      ap_clk : IN STD_LOGIC;
      ap_rst : IN STD_LOGIC;
      ap_start : IN STD_LOGIC;
      ap_done : OUT STD_LOGIC;
      ap_idle : OUT STD_LOGIC;
      ap_ready : OUT STD_LOGIC;
      pc_in : IN STD_LOGIC_VECTOR (31 downto 0);
      main_memory_out : OUT STD_LOGIC_VECTOR (31 downto 0);
      main_memory_out_ap_vld : OUT STD_LOGIC;
      opcode_out : OUT STD_LOGIC_VECTOR (31 downto 0);
      opcode_out_ap_vld : OUT STD_LOGIC;
      op_value_1_out : OUT STD_LOGIC_VECTOR (31 downto 0);
      op_value_1_out_ap_vld : OUT STD_LOGIC;
      op_value_2_out : OUT STD_LOGIC_VECTOR (31 downto 0);
      op_value_2_out_ap_vld : OUT STD_LOGIC;
      result_out : OUT STD_LOGIC_VECTOR (31 downto 0);
      result_out_ap_vld : OUT STD_LOGIC );
  end component;

  signal ap_clk: STD_LOGIC;
  signal ap_rst: STD_LOGIC;
  signal ap_start: STD_LOGIC;
  signal ap_done: STD_LOGIC;
  signal ap_idle: STD_LOGIC;
  signal ap_ready: STD_LOGIC;
  signal pc_in: STD_LOGIC_VECTOR (31 downto 0);
  signal main_memory_out: STD_LOGIC_VECTOR (31 downto 0);
  signal main_memory_out_ap_vld: STD_LOGIC;
  signal opcode_out: STD_LOGIC_VECTOR (31 downto 0);
  signal opcode_out_ap_vld: STD_LOGIC;
  signal op_value_1_out: STD_LOGIC_VECTOR (31 downto 0);
  signal op_value_1_out_ap_vld: STD_LOGIC;
  signal op_value_2_out: STD_LOGIC_VECTOR (31 downto 0);
  signal op_value_2_out_ap_vld: STD_LOGIC;
  signal result_out: STD_LOGIC_VECTOR (31 downto 0);
  signal result_out_ap_vld: STD_LOGIC ;

  constant clock_period: time := 10 ns;
  constant system_cycle: time := 120 ns;
  --constant long_system_cycle: time := 120ns;
  signal stop_the_clock: boolean;

begin

  uut: processor port map ( ap_clk                 => ap_clk,
                            ap_rst                 => ap_rst,
                            ap_start               => ap_start,
                            ap_done                => ap_done,
                            ap_idle                => ap_idle,
                            ap_ready               => ap_ready,
                            pc_in                  => pc_in,
                            main_memory_out        => main_memory_out,
                            main_memory_out_ap_vld => main_memory_out_ap_vld,
                            opcode_out             => opcode_out,
                            opcode_out_ap_vld      => opcode_out_ap_vld,
                            op_value_1_out         => op_value_1_out,
                            op_value_1_out_ap_vld  => op_value_1_out_ap_vld,
                            op_value_2_out         => op_value_2_out,
                            op_value_2_out_ap_vld  => op_value_2_out_ap_vld,
                            result_out             => result_out,
                            result_out_ap_vld      => result_out_ap_vld );

  stimulus: process
  begin   
    -- Initialization code
    ap_rst <= '1';
    wait for 10ns;    
    ap_rst <= '0';
    wait for 10ns;    
    ap_start <= '1';
    wait for 10ns; 
    
    -- code begins   
    -- pc_in = 0
    pc_in <= "00000000000000000000000000000000";
    wait for system_cycle; 

    -- pc_in = 1
    pc_in <= "00000000000000000000000000000001";
    wait for system_cycle; 

    -- pc_in = 2
    pc_in <= "00000000000000000000000000000010";
    wait for system_cycle;

    -- pc_in = 3
    pc_in <= "00000000000000000000000000000011";
    -- wait for long_system_cycle;
    wait for system_cycle;

    -- pc_in = 4
    pc_in <= "00000000000000000000000000000100";
    wait for system_cycle;

    -- pc_in = 5
    pc_in <= "00000000000000000000000000000101";
    wait for system_cycle;

    -- pc_in = 6
    pc_in <= "00000000000000000000000000000110";
    wait for system_cycle;

    -- pc_in = 7
    pc_in <= "00000000000000000000000000000111";
    wait for system_cycle;

    -- End simulation    
    stop_the_clock <= true;
    wait;
  end process;

  clocking: process
  begin
    while not stop_the_clock loop
      ap_clk <= '0', '1' after clock_period / 2;
      wait for clock_period;
    end loop;
    wait;
  end process;

end;