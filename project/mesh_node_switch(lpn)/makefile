#########################################################################################
# As a new project, you should  the following variables		

# First, select the chip type, the chip type can be:					
#			TEL_CHIP := -DCHIP_TYPE=CHIP_TYPE_8261
#			TEL_CHIP := -DCHIP_TYPE=CHIP_TYPE_8266
#			TEL_CHIP := -DCHIP_TYPE=CHIP_TYPE_8267
#			TEL_CHIP := -DCHIP_TYPE=CHIP_TYPE_8269
#
# Second, select the link lib, the available link lib file in Telink SDK proj_lib folder
#
# Third, Set Telink SDK path to  variable TEL_PATH
# 
# Forth, Set Project Name
#########################################################################################

# Select Chip  
TEL_CHIP := -DCHIP_TYPE=CHIP_TYPE_8258 -D__PROJECT_MESH_SWITCH__=1 -D__PROJECT_NODE_ONOFF_SWITCH__=1
# Select link lib
LIBS :=  -llt_8258_mesh -lsoft-fp -lsig_mesh_825x

##########################################################################################
#Set SDK Path , if you copy this project other dir, please set TEL_PATH absolute path
#########################################################################################
TEL_PATH := ../..

# Set Project Name
PROJECT_NAME := mesh_onoff_switch_node

# 设置下载固件的串口号
DOWNLOAD_PORT := /dev/ttyUSB0

#########################################################################################
# The following content need not be modified by the user!!!!!							#
#########################################################################################
PROJECT_PATH := .
OUT_PATH :=$(PROJECT_PATH)/out

ifneq ($(TEL_PATH)/make/makefile, $(wildcard $(TEL_PATH)/make/makefile))
$(error "Please check if the TEL_PATH is correct")
endif

#include the SDK makefile
-include $(TEL_PATH)/make/makefile