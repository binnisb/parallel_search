#include <stdio.h>
#include <stdlib.h> /* required for randomize() and random() */
#include <string.h>
#include <time.h>

void random_generatior(char* file_name,int nr_of_lines, int length_of_seq){
  // file_name: string with suffix .txt.  
  // nr_of_lines
  // length_of_seq
  //
  // Returns: A file with the name file_name, that contains in nr_of_lines
  //          lines where each line contains a string on the following form:
  //            id \t seq
  //          Where id is the id of the line and seq is a sequence of length
  //          length_of_seq of integers.
  //        
  int i, j, R;

  // open up file
  FILE *file; 
  file = fopen(file_name,"w");

  for (i=0;i<nr_of_lines;i++){
    fprintf(file,"%i\t",i);
    for (j=0;j<length_of_seq;j++){
      fprintf(file,"%i",rand()%10);  
    }
    fprintf(file,"\n");
  }
  fclose(file); 
}



void read_file(char* file_name, char* key, int result_size_block){
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
    printf("line=%s\n",line);
    pch_id = atoi(strtok(line,"\t\n"));
    pch_seq = strtok(NULL,"\t\n");
    //printf("pch_id0%i, pch_seq=%s\n",pch_id,pch_seq);
    if ( strcmp(pch_seq, key) == 0){
      printf("pch_id=%i, pch_seq=%s, result_counter=%i\n",pch_id,pch_seq,result_counter);
/*      if (result_size == result_counter ){
        result_size = result_size + result_size_block;
        result = realloc(result, result_size*sizeof(*result) );
        if (result == NULL){
          printf("Error reallocating memory\n");
          exit(1);
        }
      }
*/      result[result_counter] = pch_id;  
      printf("ok1\n");
      result_counter++;
      printf("ok1\n");
    }
    //fputs(line,stdout);
  }
  fclose(file);



  // print out results:
  int j;
  for (j=0;j<result_counter;j++){
    //printf("result[%i]=%i \n",j,result[j]);
  }

}


int main( int argc, const char* argv[] )
{
  time_t start,end;
  double dif;

  int i;

  char* file_name = "../data/file.txt";
  random_generatior(file_name,4,4);

  //read_file(file_name,"6942264128",100);

  int rec[3];
  rec[0] = 1;
  rec[1] = 1000;
  rec[2] = 1000000;
/*
  for ( i=0;i<3;i++){
    time (&start);
    read_file(file_name,"1234567890",rec[i]);
    time (&end);
    dif = difftime(end,start);
    printf("For rec=%i we get time=%f\n,stime=%f,etime=%f\n",rec[i],dif,(double)start,(double)end); 
  }*/ 
}
