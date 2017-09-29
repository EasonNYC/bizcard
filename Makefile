default:
	# compile for attiny85 running at 1 MHz
	avr-gcc -std=c99 -Wall -Os -DF_CPU=1000000 -mmcu=attiny85 -o bizcard.o bizcard.c
	avr-objcopy -j .text -j .data -O ihex bizcard.o bizcard.hex
	avrdude -c usbtiny -p t85 -U flash:w:bizcard.hex:i

clean: /dev/null
	rm -f bizcard.o bizcard.hex
