#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
int ae_load_file_to_memory(const char *filename, char **result) 
{ 
	int size = 0;
	FILE *f = fopen(filename, "rb");
	if (f == NULL) 
	{ 
		*result = NULL;
		return -1; // -1 means file opening fail 
	} 
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	*result = (char *)malloc(size+1);
	if (size != fread(*result, sizeof(char), size, f)) 
	{ 
		free(*result);
		return -2; // -2 means file reading fail 
	} 
	fclose(f);
	(*result)[size] = 0;
	return size;
}

int read_file(char* input_file, char* key, int result_size_block, int nr_lines, int line_size){
  //  input_file:        is direction to a data file.
  //  key:               the search string.
  //  result_size_block: is the block size of the array that 
  //                     contains the resutls which contains the id 
  //                     of the mathing data. 
  //  nr_lines:          nr of lines in file. 
  //  line_sie:          size of line in char
  //
  // It is assumed that all lines in input_file have the same length.

  // open up file
  //FILE *file;
  //file = fopen(file_name,"r");
  char line[128]; // line in file


  int pch_id; // first column in the current line, corresponds to line id
  char* pch_seq; // second column in the current file, corresponds to the seq in this line

  
  int result_size = result_size_block; // initualized result size
  int *result;
  result = malloc(sizeof(*result)*result_size);
  int result_counter = 0;

//  while ( fgets( line, sizeof(line),file) != NULL){
  int i;
  for ( i = 0; i < nr_lines; i++ ) {
    pch_id = atoi(strtok(&input_file[i*line_size],"\t\n"));
    pch_seq = strtok(NULL,"\t\n");
    if ( strcmp(pch_seq, key) == 0){
      if (result_size == result_counter ){
        result_size = result_size + result_size_block;
        result = realloc(result, result_size*sizeof(*result) );
        if (result == NULL){
          printf("Error reallocating memory\n");
          exit(1);
        }
      }
      result[result_counter] = pch_id;  
      result_counter++;
    }
  }
  return result_counter;
}


int main( int argc, const char* argv[] )
{
  long start,end;
  double dif;

  //int data_size = 100000000;
//  int string_size = 6;
//  int block_size = 1.5*(data_size/pow(10,string_size));
//  int read_count;
  char* file_name = "../data/file.txt";
  char* result;
  int nr_bytes;
  nr_bytes = ae_load_file_to_memory(file_name,&result);

  int line_size;
  int i;
  for ( i=0 ; i<nr_bytes ; i++ ){
    if ( result[i]=='\n' ){
      line_size = i+1;
      break;   
    }
  }
  
  // assume that all the lines in the file have the same size
  int data_size = nr_bytes / line_size;

  int string_size = 6;
  int block_size = 1.5*(data_size/pow(10,string_size));
  int read_count;

  read_count = read_file(result,"123123",block_size,data_size,line_size);
  printf("result found: %i\n", read_count);
}
