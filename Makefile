#
# e-paper makefile
# Author: gavinguinn1@gmail.com
#

CC      = gcc
TARGET  = epd

CONFIG_DIR   = ./lib/Config
EPD_DIR      = ./lib/e-Paper
FONTS_DIR    = ./lib/Fonts
GUI_DIR      = ./lib/GUI
EXAMPLES_DIR = ./examples
BIN_DIR      = ./bin

SRC_FILES = $(wildcard $(EPD_DIR)/EPD_2in13_V3.c \
            $(GUI_DIR)/*.c \
            $(CONFIG_DIR)/dev_hardware_SPI.c \
            $(CONFIG_DIR)/DEV_Config.c \
            $(CONFIG_DIR)/RPI_sysfs_gpio.c \
            $(EXAMPLES_DIR)/*.c)

OBJ_FILES = $(patsubst %.c, $(BIN_DIR)/%.o, $(notdir $(SRC_FILES)))

# Libraries and Flags
CFLAGS += -g -O -ffunction-sections -fdata-sections 
CFLAGS += -D DEBUG -D USE_BCM2835_LIB -D RPI

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

LIB_INC = -I $(CONFIG_DIR) -I $(GUI_DIR) -I $(EPD_DIR) $(DEBUG)

# compile all the fonts
$(BIN_DIR)/%.o: $(FONTS_DIR)/%.c
	$(CC) $(CFLAGS) -w -c $< -o $@ 

# compile the graphics library
$(BIN_DIR)/%.o: $(GUI_DIR)/%.c
	$(CC) $(CFLAGS) -w -c $< -o $@ -I $(CONFIG_DIR)

# compile the screen drivers
$(BIN_DIR)/%.o: $(EPD_DIR)/%.c
	$(CC) $(CFLAGS) -w -c $< -o $@ -I $(CONFIG_DIR)

# compile the raspberry pi config
$(BIN_DIR)/%.o: $(CONFIG_DIR)/%.c
	$(CC) $(CFLAGS) -w -c $< -o $@ 

# compile our src files
$(BIN_DIR)/%.o: $(EXAMPLES_DIR)/%.c
	$(CC) $(CFLAGS) -Wall -c $< -o $@ $(LIB_INC)

# Link the .o's together
LINKER_FLAGS  = -Wl,--gc-sections -lbcm2835 -lm
epd: $(OBJ_FILES)
	$(CC) $(CFLAGS) -D RPI $(OBJ_FILES) -o $(TARGET) $(LINKER_FLAGS)

.PHONY: clean

clean:
	rm -rf $(BIN_DIR)/*.*
	rm -rf $(TARGET)
