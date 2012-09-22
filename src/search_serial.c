#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
long long ae_load_file_to_memory(const char *filename, char **result) 
{ 
	long long size = 0;
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

int read_file(char* input_file, char* key, int result_size_block, long long nr_lines, int line_size){
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
  char line[line_size]; // line in file


  int pch_id; // first column in the current line, corresponds to line id
  char* pch_seq; // second column in the current file, corresponds to the seq in this line

  int result_size = result_size_block; // initualized result size
  int *result;
  result = malloc(sizeof(*result)*result_size);
  int result_counter = 0;
  long long i;
  int key_len = strlen(key); 
  for ( i = 0; i < nr_lines; i++ ) {
    strncpy(line,&input_file[i*line_size],line_size);
//    pch_id = atoi(strtok(line,"\t\n"));
//    pch_seq = strtok(NULL,"\t\n");
//    if (pch_seq == NULL){
//      printf("pch_id,%i", pch_id);
//    }
    //if ( strcmp(pch_seq, key) == 0){
    line[line_size - 1] = '\0';
    if ( strcmp(&line[line_size-(key_len+1)], key) == 0){
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
  time_t start,end;
  //int data_size = 100000000;
//  int string_size = 6;
//  int block_size = 1.5*(data_size/pow(10,string_size));
//  int read_count;
  char* file_name = "../data/file.txt";
  char* result;
  long long nr_bytes;
  start = time(NULL);
  nr_bytes = ae_load_file_to_memory(file_name,&result);
  end = time(NULL);
  printf("load file %f\n",difftime(end,start));
  int line_size;
  long long i;
  for ( i=0 ; i<nr_bytes ; i++ ){
    if ( result[i]=='\n' ){
      line_size = i+1;
      break;   
    }
  }
  
  // assume that all the lines in the file have the same size
  long long data_size = nr_bytes / line_size;

  int string_size = 6;
  int block_size = 1.5*(data_size/pow(10,string_size));
  int read_count;
  start = time(NULL);
  read_count = read_file(result,"123123",block_size,data_size,line_size);
  end = time(NULL);
  printf("search: %f\n",difftime(end,start));
  printf("result found: %i\n", read_count);

}
