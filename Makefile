default:
	#attiny85 running at 1 MHz
	avr-gcc -std=c99 -Wall -Os -DF_CPU=1000000 -mmcu=attiny85 -o main.o main.c
	avr-objcopy -j .text -j .data -O ihex main.o main.hex
	avrdude -c usbtiny -p t85 -U flash:w:main.hex:i

clean: /dev/null
	rm -f main.o main.hex
