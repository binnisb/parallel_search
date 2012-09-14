all: serial data_generator openmp

data: data_generator
	./../bin/data_generator

serial: search_serial.c 
	gcc search_serial.c -O2 -o ../bin/search_serial

data_generator: data_generator.c
	gcc data_generator.c -O2 -o ../bin/data_generator
openmp: search_openmp.c
	gcc search_openmp.c -O2 -o ../bin/search_openmp

	
