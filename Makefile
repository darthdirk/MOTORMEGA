# Makefile for ATMEGA2560

CC=avr-gcc
CFLAGS= -Os -DF_CPU=16000000UL -mmcu=atmega2560 -I/home/dirk/avr8-gnu-toolchain-linux_x86_64/avr/include/avr -I/home/dirk/avr8-gnu-toolchain-linux_x86_64/avr/include -I./inc ./src/*.c 
BINS=main 
# Compiling
all: $(BINS)

# Detect Arduino port
USBPORT:=$(shell ls /dev/ttyACM*)

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

%.hex: %.out
	avr-objcopy -O ihex -R .eeprom $< $@


install.%: %.hex
	avrdude -F -V -c arduino -patmega2560 -cwiring "-P/dev/ttyACM0" -b115200 -D "-Uflash:w:/home/dirk/MOTORMEGA/main.hex:i"


# Clean the build files
clean:
	rm -f *.hex *.out $(BINS)


