SYSROOT := /home/adonisking/Learning_Linux/3.Device_driver/SDK/sysroots/cortexa72-poky-linux

CROSS_COMPILE ?= aarch64-poky-linux-
CC := $(CROSS_COMPILE)gcc
CFLAGS := -O2 -Wall --sysroot=$(SYSROOT)

INCLUDE_DIR := /home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/include

# Thêm tất cả thư mục con con trong include, nếu cần thiết, có thể thêm lần lượt từng thư mục con gồm drivers, utils
CFLAGS += -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/drivers -I$(INCLUDE_DIR)/utils

SRC_DIR := /home/adonisking/Learning_Linux/4.user_space_application/1.oled_app/src

TARGET := oled_write_pixel

# Thu thập tất cả file .c trong src và các thư mục con
SRCS := oled_write_pixel.c $(shell find $(SRC_DIR) -name '*.c')

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
