

all: data

data: data_generator  
	./../bin/data_generator

serial: data ../bin/search_serial
	cc search_serial.c -O2 -o ../bin/search_serial

data_generator: data_generator.c
	cc data_generator.c -O2 -o ../bin/data_generator

	
