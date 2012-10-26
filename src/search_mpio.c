#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
long long ae_load_file_to_memory(MPI_File   infile, 
                                 char       **result, 
                                 int        my_rank, 
                                 int        numprocs, 
                                 long long  nr_lines,
                                 int        line_size) 
{
  //  infile:      the data file
  //  result:      pointer to character array that contains the content of the file. 
  //  my_rank:     my rank number
  //  numprocs:    number of processes
  //  nr_lines:    nr of lines in file       
  //  line_size:   line size in the data file
  //
  //  putts the data the process my_rank is suppose to search in into resutls.
  //  returns the size of resutls in bytes.

  MPI_Status stat;		

  // number of lines the current process will read
  long long my_size = (nr_lines+my_rank)/numprocs;        

  // allocate memory for result
  *result = (char *)malloc(line_size*my_size); 

  // the first line this process will read:
  MPI_Offset start_read = my_rank*( nr_lines/numprocs + my_rank/(numprocs-nr_lines%numprocs+1));

  // Set the pointer to the first line this process will read:
  MPI_File_seek(infile, start_read*line_size, MPI_SEEK_SET); 

  // Read lines start_read,start_read+1,...,start_read+my_size-1 to result:
  MPI_File_read( infile, *result, line_size*my_size, MPI_CHAR, &stat );

  // Close the 
  MPI_File_close(&infile);

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
  //                     contains the resutls, the id's 
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
      printf("%s \n",line);
    }
  }
  return result_counter;
}


int main( int argc,  char* argv[] )
{

  int numprocs;  // number of processes
  int myid;      // my rank

  MPI_Status stat;
  /* MPI programs start with MPI_Init; all 'N' processes exist thereafter */
  MPI_Init(&argc,&argv);
  /* find out how big the SPMD world is */
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  /* and this processes' rank is */
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  MPI_File infile; // the data file

  // Variables to measure time performance
  double start,end;
  double dif;
  
  char* file_name = "../data/file.txt"; // name of the data file
  char* search_key = "123123";          // the search key
  // An array of char's that contains all the lines from infile that this 
  // process is responseble to search in:
  char* result;

  long long nr_lines; // nr of lines in the data file 
  long long nr_bytes; // nr of bytes in th 
  long long size;     // nr of bytes in the data file

  int string_size = strlen(search_key);
  int block_size; // 
  int read_count; // result: number of found elements
  int line_size;  // line size in the data file
  int ierr, sum;  

  // Find file size in bytes:
  size = 0;
  MPI_File_open( MPI_COMM_WORLD, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &infile );
  MPI_File_get_size(infile,&size); // get number of bytes in the data file
  
  // Find the line size in byte:
  char temp_read[1000];
  MPI_File_set_view( infile, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL );
  MPI_File_read( infile, &temp_read, 1000, MPI_CHAR, &stat );  
  char* pchr = strchr( temp_read, '\n');
  line_size = pchr-temp_read+1; 

  // get number of lines:
  nr_lines = size/line_size;
  
  // put the part of the data file that this process is responseble
  // for searching in into results:
  start = MPI_Wtime();
  nr_bytes = ae_load_file_to_memory(infile ,&result, myid, numprocs, nr_lines, line_size);
  end = MPI_Wtime();
  dif = end-start;
  printf("LoadFile: %f\n", dif);  

  // nr of lines in results
  nr_lines = nr_bytes / line_size; 

  block_size = 1.5*(nr_lines/pow(1,string_size));

  // search in the part of the data this process is responseble for:
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
