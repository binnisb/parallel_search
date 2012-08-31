#include <stdio.h>
#include <stdlib.h> /* required for randomize() and random() */
#include <string.h>


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



void read_file(char* file_name, char* key){
  //  file_name: is direction to a data file.
  //  key:       the search string.
  //
  // 
  //


  // open up file
  FILE *file;
  file = fopen(file_name,"r");

  char line[128]; // line in file


  char* pch_id; // first column in the current line, corresponds to line id
  char* pch_seq; // second column in the current file, corresponds to the seq in this line

  int result[5000];
  int i = 0;

  while ( fgets( line, sizeof(line),file) != NULL){
    pch_id = (int)* (strtok(line,"\t\n")-'0');
    pch_seq = strtok(NULL,"\t\n");
    //printf("key=%i, seq=%i \n",(int)strlen(key),(int)strlen(pch_seq));
    if ( strcmp(pch_seq, key) == 0){
      printf("ok\n");
      result[i] = pch_id;  
      i++;
    }
    //fputs(line,stdout);
  }
  fclose(file);


  int j;
  for (j=0;j<i;j++){
    printf("result[%i]=%i \n",j,result[j]);
  }

}


int main( int argc, const char* argv[] )
{
  random_generatior(5000,20);
  read_file("file.txt","32588910672835001113"); 
}
