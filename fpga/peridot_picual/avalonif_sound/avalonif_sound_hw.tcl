# TCL File Generated by Component Editor 9.0
# Tue Jul 07 18:14:23 JST 2009
# DO NOT MODIFY


# +-----------------------------------
# | 
# | avalonif_sound "avalonif_sound" v1.0
# | null 2009.07.07.18:14:23
# | 
# | 
# | D:/project/c75_tourmaline/kotori/sound/avalonif_sound.vhd
# | 
# |    ./avalonif_sound.vhd syn, sim
# |    ./sound_fifo.vhd syn, sim
# | 
# +-----------------------------------


# +-----------------------------------
# | module avalonif_sound
# | 
set_module_property NAME avalonif_sound
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property GROUP Peripherals
set_module_property DISPLAY_NAME avalonif_sound
set_module_property LIBRARIES {ieee.std_logic_1164.all ieee.std_logic_arith.all ieee.std_logic_unsigned.all std.standard.all}
set_module_property TOP_LEVEL_HDL_FILE avalonif_sound.vhd
set_module_property TOP_LEVEL_HDL_MODULE avalonif_sound
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE false
# | 
# +-----------------------------------

# +-----------------------------------
# | files
# | 
add_file avalonif_sound.vhd {SYNTHESIS SIMULATION}
add_file sound_fifo.vhd {SYNTHESIS SIMULATION}
# | 
# +-----------------------------------

# +-----------------------------------
# | parameters
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point global
# | 
add_interface global clock end
set_interface_property global ptfSchematicName ""

set_interface_property global ENABLED true

add_interface_port global csi_global_reset reset Input 1
add_interface_port global csi_global_clk clk Input 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point s1
# | 
add_interface s1 avalon end
set_interface_property s1 addressAlignment NATIVE
set_interface_property s1 bridgesToMaster ""
set_interface_property s1 burstOnBurstBoundariesOnly false
set_interface_property s1 holdTime 0
set_interface_property s1 isMemoryDevice false
set_interface_property s1 isNonVolatileStorage false
set_interface_property s1 linewrapBursts false
set_interface_property s1 maximumPendingReadTransactions 0
set_interface_property s1 printableDevice false
set_interface_property s1 readLatency 0
set_interface_property s1 readWaitTime 1
set_interface_property s1 setupTime 0
set_interface_property s1 timingUnits Cycles
set_interface_property s1 writeWaitTime 0

set_interface_property s1 ASSOCIATED_CLOCK global
set_interface_property s1 ENABLED true

add_interface_port s1 avs_s1_address address Input 2
add_interface_port s1 avs_s1_read read Input 1
add_interface_port s1 avs_s1_readdata readdata Output 8
add_interface_port s1 avs_s1_write write Input 1
add_interface_port s1 avs_s1_writedata writedata Input 8
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point irq_s1
# | 
add_interface irq_s1 interrupt end
set_interface_property irq_s1 associatedAddressablePoint s1

set_interface_property irq_s1 ASSOCIATED_CLOCK global
set_interface_property irq_s1 ENABLED true

add_interface_port irq_s1 avs_s1_irq irq Output 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point conduit_end
# | 
add_interface conduit_end conduit end

set_interface_property conduit_end ENABLED true

add_interface_port conduit_end dac_out export Output 1
# | 
# +-----------------------------------