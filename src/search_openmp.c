#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
long long ae_load_file_to_memory(const char *filename, 
                                 char       **result) 
{
  //  filename: path to the file to read
  //  result:   pointer to character array that 
  //            contains the content of the file. 
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

int read_file( char*     input_file, 
               char*     key, 
               int       result_size_block, 
               long long nr_lines, 
               int       line_size)
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
  #pragma omp parallel for private(i,line) shared(result,result_counter,result_size,key,result_size_block)
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


int main( int argc, const char* argv[] )
{
  double start,end;
  double dif;
  
  char* file_name = "../data/file.txt";
  char* result;
  char* search_key = "123123";

  long long nr_bytes; 
  long long i;
  long long nr_lines;

  int string_size = strlen(search_key);
  int block_size;
  int read_count;
  int line_size;
 
  start = omp_get_wtime();
  nr_bytes = ae_load_file_to_memory(file_name,&result);
  end = omp_get_wtime();
  dif = end-start;
  printf("LoadFile core %i: %f\n",omp_get_thread_num(), dif);

  // assume each line in file is equally long. here we get the line size.
  for ( i=0 ; i<nr_bytes ; i++ ){
    if ( result[i]=='\n' ){
      line_size = i+1;
      break;   
    }
  }
  
  // 
  nr_lines = nr_bytes / line_size;

  block_size = 1.5*(nr_lines/pow(6,string_size));
  read_count = 0;
  start = omp_get_wtime();
  read_count = read_file(result,search_key,block_size,nr_lines,line_size);
  end = omp_get_wtime();
  dif = end - start;
  printf("Search core %i: %f\n",omp_get_thread_num(),dif);
  printf("result found: %i\n", read_count);

}
