#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>


int read_file(char* file_name, char* key, int result_size_block){
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

  while ( fgets( line, sizeof(line),file) != NULL){
    pch_id = atoi(strtok(line,"\t\n"));
    pch_seq = strtok(NULL,"\t\n");
    //printf("pch_id0%i, pch_seq=%s\n",pch_id,pch_seq);
    if ( strcmp(pch_seq, key) == 0){
      //printf("pch_id=%i, pch_seq=%s, result_counter=%i\n",pch_id,pch_seq,result_counter);
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
    //fputs(line,stdout);
  }
  fclose(file);
  return result_counter;


  // print out results:
  //int j;
  //for (j=0;j<result_counter;j++){
    //printf("result[%i]=%i \n",j,result[j]);
  //}

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
  //random_generatior(file_name,data_size,string_size);

  read_count = read_file(file_name,"123123",block_size);
  printf("result found: %i\n", read_count);
}
