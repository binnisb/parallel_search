#include <stdio.h>
#include <stdlib.h> /* required for randomize() and random() */
#include <string.h>
#include <time.h>
#include <math.h>
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


int main( int argc, const char* argv[] )
{
  long start,end;
  double dif;

  int i;

  int data_size = 100000000;
  int string_size = 6;
  char* file_name = "../data/file.txt";
  char buf [1000];
  itoa(data_size,buf,1)
  //random_generatior(file_name,data_size,string_size);

  printf("File created: %s\n", file_name);
}
