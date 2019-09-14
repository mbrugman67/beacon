# tool locations
# get the correct location of avr-gcc on OS X
AVR_INSTALL=$(shell dirname $(shell which avr-gcc))

# get the correction location of the include files for gcc-avr
AVR_INCL_DIR=$(shell dirname $(shell locate unistd.h | grep avr-gcc))

# executables
CC=$(AVR_INSTALL)/avr-gcc
CXX=$(AVR_INSTALL)//avr-g++
OBJCOPY=$(AVR_INSTALL)/avr-objcopy
OBJDUMP=$(AVR_INSTALL)/avr-objdump
AVRSIZE=$(AVR_INSTALL)/avr-size
SREC=$(AVR_INSTALL)/srec_cat

# for loading using USB DFU
DUDE=$(AVR_INSTALL)/avrdude
SLEEP=/bin/sleep

OBJDIR=./obj/
OUTDIR=./out/
TARGET=$(OUTDIR)beacon

#LIBPATH=-L$(shell dirname $(shell locate crtat90c8534.o | grep avr-gcc))

CFLAGS=-Wall -mmcu=atmega32u4 -DF_CPU=16000000UL -DLITTLE_ENDIAN -Os -I$(AVR_INCL_DIR) -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections
INCDIRS=-I./ 

LDFLAGS=-mmcu=atmega32u4 -Wl,-u,--gc-sections,-Map=$(TARGET).map,--cref
LDLIBS=

SOURCES=$(shell find . -type f -name '*.cpp')
HEADERS=$(shell find . -type f -name '*.h')

OBJECTS=$(addprefix $(OBJDIR),$(SOURCES:.cpp=.o))

$(OBJDIR)%.o: %.cpp $(HEADERS) Makefile
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CXX) $(CFLAGS) $(INCDIRS) -c -o $@ $<;

$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(@D)
	@echo ""
	@echo "Linking $@"
	@echo ""
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

	@echo "Finished size:"
	@$(AVRSIZE) $(TARGET).elf
	
	@echo ""
	@echo "Dumping list and building hex files..."
	@$(OBJDUMP) -h -S $(TARGET).elf > $(TARGET).lss
	@$(OBJCOPY) -R .eepromp -R .fuse -R .lock -R .signature -O ihex $(TARGET).elf $(TARGET).hex
	@$(OBJCOPY) --no-change-warnings -j .eeprom --change-section-lma .eeprom=0 -O ihex $(TARGET).elf $(TARGET).eep
	@$(OBJCOPY) --no-change-warnings -j .lock --change-section-lma .lock=0 -O ihex $(TARGET).elf $(TARGET).lock
	@$(OBJCOPY) --no-change-warnings -j .signature --change-section-lma .signature=0 -O ihex $(TARGET).elf $(TARGET).sig
	@$(OBJCOPY) --no-change-warnings -j .fuse --change-section-lma .fuse=0 -O ihex $(TARGET).elf $(TARGET).fuse
	@$(SREC) $(TARGET).fuse -Intel -crop 0x00 0x01 -offset  0x00 -O $(TARGET).lfs -Intel
	@$(SREC) $(TARGET).fuse -Intel -crop 0x01 0x02 -offset -0x01 -O $(TARGET).hfs -Intel
	@$(SREC) $(TARGET).fuse -Intel -crop 0x02 0x03 -offset -0x02 -O $(TARGET).efs -Intel

	@echo ""
	@echo "DONE!!"

.PHONY: all clean load rebuild

rebuild: clean $(TARGET).hex

all: $(TARGET).hex

clean:
	rm -rf $(OUTDIR) $(OBJDIR)

load:
	$(eval BOARD := $(shell btBootycall 1 && sleep 1 && ls /dev/cu.usbmod*))
	@echo target device is $(BOARD), writing flash 
	$(DUDE) -F -V -u -c avr109 -p m32u4 -P $(BOARD) -b 57600 -D -U flash:w:$(TARGET).hex 
