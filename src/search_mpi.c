#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
long long ae_load_file_to_memory(const char *filename, 
                                 char       **result, 
                                 int        my_rank, 
                                 int        numprocs, 
                                 long long  nr_lines,
                                 int        line_size) 
{
  //  filename:		path to the file to read
  //  result:		pointer to character array that contains the content of the file. 
  //  my_rank:          my rank number
  //  numprocs:         number of processes
		
  long long my_size = (nr_lines+my_rank)/numprocs; // number of lines the current process will read
        

  FILE *f = fopen(filename, "rb");
  if (f == NULL) 
  { 
    *result = NULL;
    return -1; // -1 means file opening fail 
  } 
  *result = (char *)malloc(line_size*my_size);
  long long start_read = my_rank*( nr_lines/numprocs + my_rank/(numprocs-nr_lines%numprocs+1));
  fseek(f, start_read*line_size, SEEK_SET);       

  if ( line_size*my_size != fread(*result, sizeof(char), line_size*my_size, f)) 
  { 
    free(*result);
    return -2; // -2 means file reading fail 
  } 
  fclose(f);

  return line_size*my_size;
}

int read_file(char*      input_file, 
              char*      key, 
              int        result_size_block, 
              long long  nr_lines, 
              int        line_size)
{
  //  input_file:        is direction to a data file.
  //  key:               the search string.
  //  result_size_block: is the block size of the array that 
  //                     contains the resutls which contains the id 
  //                     of the mathing data. 
  //  nr_lines:          nr of lines in file. 
  //  line_sie:          size of line in char
  //
  // It is assumed that all lines in input_file have the same length and are on the form: "0000001	123123\n" where first is an index and then the search values

  char line[line_size]; // line in file
  long long i;
  int result_size = result_size_block; // initualized result size
  int *result;
  int result_counter=0;
  int key_len = strlen(key);

  result = malloc(sizeof(*result)*result_size);
  for ( i = 0; i < nr_lines; i++ ) {
    strncpy(line,&input_file[i*line_size],line_size);
    
    line[line_size - 1] = '\0';
    // line contains a single line from the file. We compare the last part of the line to the search key.
    if ( strcmp(&line[line_size-(key_len+1)], key) == 0){
      // if we have used all spaces in result_counter we need to reallocate and increase the size.
      if (result_size == result_counter ){
        result_size = result_size + result_size_block;
        result = realloc(result, result_size*sizeof(*result) );
        if (result == NULL){
          printf("Error reallocating memory\n");
          exit(1);
        }
      }
      // we keep the line number where we found the key
      result[result_counter] = atoi(strtok(line,"\t\n"));  
      result_counter++;
    }
  }
  return result_counter;
}


int main( int argc,  char* argv[] )
{
  if ( argc != 2 ) /* argc should be 2 for correct execution */
  {
      /* We print argv[0] assuming it is the program name */
      printf( "usage: %s filename", argv[0] );
      exit(2);
  }
  // Paralell starts:
  int numprocs; // number of processes
  int myid;     // my rank
  MPI_Status stat;
  /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
  MPI_Init(&argc,&argv);
  /* find out how big the SPMD world is */
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  /* and this processes' rank is */
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  double start,end;
  double dif;
  
  char* file_name = "../data/file.txt";
  char* result;
  char* search_key = "123123";

  long long nr_bytes; 
  long long i;
  long long nr_lines;

  int string_size = strlen(search_key);
  int block_size; //
  int read_count; // result: number of found elements
  int line_size;  // line size
  int ierr, sum;

  // Find file size in bytes:
  long long size = 0;
  FILE *f = fopen(argv[1], "rb");
  if (f == NULL)
  {
    return -1; // -1 means file opening fail 
  }
  fseek(f, 0, SEEK_END);
  size = ftell(f); // number of bytes in file
  fseek(f, 0, SEEK_SET);

  // Find the line size in byte:
  char temp_read[1000];
  fread(temp_read,sizeof(char),1000 ,f );
  char* pchr = strchr( temp_read, '\n');
  line_size = pchr-temp_read+1; 

  // get number of lines:
  nr_lines = size/line_size;

  start = MPI_Wtime();
  nr_bytes = ae_load_file_to_memory(argv[1],&result, myid, numprocs, nr_lines, line_size);
  nr_lines = nr_bytes / line_size;
  end = MPI_Wtime();
  dif = end-start;
  printf("LoadFile: %f\n", dif);

  block_size = 1.5*(nr_lines/pow(1,string_size));
  start = MPI_Wtime();

  read_count = read_file(result,search_key,block_size,nr_lines,line_size);
  end = MPI_Wtime();
  dif = end - start;
  printf("Search: %f\n",dif);
  ierr = MPI_Reduce(&read_count, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(myid == 0) {
    printf("The sum is %i\n", sum);
  } 

  MPI_Finalize();
  return 0;
}
