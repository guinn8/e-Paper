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

$(shell mkdir -p $(DIR_BIN))

.PHONY: RPI clean
RPI:  RPI_epd 
	
LIB_INC = -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) $(DEBUG)
define compile_template
${DIR_BIN}/%.o:$(1)/%.c
	$$(CC) $$(CFLAGS) -c $$< -o $$@ $$(LIB_INC)
endef

DIRECTORIES = $(DIR_FONTS) $(DIR_GUI) $(DIR_Examples) $(DIR_EPD)
$(foreach dir,$(DIRECTORIES), \
	$(eval $(call compile_template,$(dir))))

LIB_RPI=-Wl,--gc-sections -lbcm2835 -lm 
RPI_DEV_FILES = dev_hardware_SPI RPI_sysfs_gpio DEV_Config
RPI_DEV_C = $(patsubst %,$(DIR_BIN)/%.o,$(RPI_DEV_FILES))

RPI_epd: ${OBJ_O}
	$(foreach file,$(RPI_DEV_FILES), \
		$(CC) $(CFLAGS) $(DEBUG_RPI) -c $(DIR_Config)/$(file).c -o $(DIR_BIN)/$(file).o $(LIB_RPI) $(DEBUG);)
	
	$(CC) $(CFLAGS) -D RPI $(OBJ_O) $(RPI_DEV_C) -o $(TARGET) $(LIB_RPI) $(DEBUG)

	

clean :
	rm -rf $(DIR_BIN)/*.* 
	rm -rf $(TARGET) 
