test:
ifdef title
	avr-g++ $(title).cpp -o $(title).elf -std=c++17 -mmcu=atmega328p -DF_CPU=16000000UL
	avr-objcopy $(title).elf -O ihex $(title).hex
	avrdude -c arduino -p m328p -U flash:w:"$(title).hex":a -P /dev/ttyUSB0
else
	echo "need title"
endif
