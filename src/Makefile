all: serial data_generator openmp mpi mpio

data: data_generator
	./../bin/data_generator

serial: search_serial.c 
	cc search_serial.c -O2 -o ../bin/search_serial

data_generator: data_generator.c
	cc data_generator.c -O2 -o ../bin/data_generator
openmp: search_openmp.c
	cc search_openmp.c -O2 -openmp -o ../bin/search_openmp
mpi: search_mpi.c
	cc search_mpi.c -O2 -o ../bin/search_mpi
mpio: search_mpio.c
	cc search_mpio.c -O2 -o ../bin/search_mpio
mpi_openmp: search_mpi_openmp.c
	cc search_mpi_openmp.c -O2 -openmp -o ../bin/search_mpi_openmp
