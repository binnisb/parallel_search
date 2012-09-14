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

int read_file(char* file_name, char* key, int result_size_block, int no_lines){
  //  file_name:   is direction to a data file.
  //  key:         the search string.
  //  iresult_size_block: is the block size of the array that 
  //                     contains the resutls which contains the id 
  //                     of the mathing data. 
  // 
  //

  // open up file
  FILE *file;
  file = fopen(file_name,"r");
  char line[128]; // line in file


  int pch_id; // first column in the current line, corresponds to line id
  char* pch_seq; // second column in the current file, corresponds to the seq in this line

  
  int result_size = result_size_block; // initualized result size
  int *result;
  result = malloc(sizeof(*result)*result_size);
  int result_counter = 0;

//  while ( fgets( line, sizeof(line),file) != NULL){
  for (int i = 0; i < no_lines; i++ ) {
    pch_id = atoi(strtok(line,"\t\n"));
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
  fclose(file);
  return result_counter;
}


int main( int argc, const char* argv[] )
{
  long start,end;
  double dif;

  int i;
  int data_size = 100000000;
  int string_size = 6;
  int block_size = 1.5*(data_size/pow(10,string_size));
  int read_count;
  char* file_name = "../data/file.txt";
  char* result;
  int no_bytes;
  no_bytes = ae_load_file_to_memory(file_name,&result);

  read_count = read_file(file_name,"123123",block_size,data_size);
  printf("result found: %i\n", read_count);
}
