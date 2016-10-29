#PRODUCT?=INFINEON_DSL699E7.5U1-2
#TBS_DSL_DIR=$(shell (echo ${PRODUCT}|awk -F_ '{print $$1}';))

#subdir-y+=API
include $(ROOTDIR)/.config 

subdir-${CONFIG_AMAZON_SE} += infineon
subdir-${CONFIG_VX180} += ikanos
 
include $(ROOTDIR)/Rules.make

