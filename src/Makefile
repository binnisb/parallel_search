all: serial data_generator openmp

data: data_generator
	./../bin/data_generator

serial: search_serial.c 
	cc search_serial.c -O2 -o ../bin/search_serial

data_generator: data_generator.c
	cc data_generator.c -O2 -o ../bin/data_generator
openmp: search_openmp.c
	cc search_openmp.c -O2 -fopenmp -o ../bin/search_openmp

	
