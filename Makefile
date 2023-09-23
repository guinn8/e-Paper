CC = gcc
TARGET = epd

DIR_Config	 = ./lib/Config
DIR_EPD		 = ./lib/e-Paper
DIR_FONTS	 = ./lib/Fonts
DIR_GUI		 = ./lib/GUI
DIR_Examples = ./examples
DIR_BIN		 = ./bin

OBJ_C = $(wildcard ${DIR_EPD}/EPD_2in13_V3.c ${DIR_GUI}/*.c ${DIR_Examples}/*.c )
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

CFLAGS += -g -O -ffunction-sections -fdata-sections -Wall -D $(EPD)
DEBUG = -D DEBUG -D USE_BCM2835_LIB -D RPI
LIB_RPI=-Wl,--gc-sections -lbcm2835 -lm 

$(shell mkdir -p $(DIR_BIN))

.PHONY : RPI clean
RPI:RPI_DEV RPI_epd 

RPI_DEV_C = $(wildcard $(DIR_BIN)/dev_hardware_SPI.o $(DIR_BIN)/RPI_sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )
RPI_epd:${OBJ_O}
	echo $(@)
	$(CC) $(CFLAGS) -D RPI $(OBJ_O) $(RPI_DEV_C) -o $(TARGET) $(LIB_RPI) $(DEBUG)
	

LIB_INC = -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) $(DEBUG)
define compile_template
${DIR_BIN}/%.o:$(1)/%.c
	$$(CC) $$(CFLAGS) -c $$< -o $$@ $$(LIB_INC)
endef

DIRECTORIES = $(DIR_FONTS) $(DIR_GUI) $(DIR_Examples) $(DIR_EPD)
$(foreach dir,$(DIRECTORIES),$(eval $(call compile_template,$(dir))))

RPI_DEV:
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/dev_hardware_SPI.c -o $(DIR_BIN)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/RPI_sysfs_gpio.c -o $(DIR_BIN)/RPI_sysfs_gpio.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_RPI) $(DEBUG)
	
clean :
	rm -rf $(DIR_BIN)/*.* 
	rm -rf $(TARGET) 
