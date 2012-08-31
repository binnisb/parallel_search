#include <stdio.h>
#include <string.h>

int sort(char*  key, char* text){
  int i;
  long end = (int)(strlen(text)-strlen(key));
  //for(i=0; i<end ; i++){
    if ( strstr(text,key) != NULL)
      return 100;
    else 
      return 0;
}  

int main(int argc, char *argv[]){
  int i,j;
  char foo[][10]={"snakesonplane","on","a","plane"};
  //char *ptr = malloc(10 * sizeof(char));
//  gaur[0] = "asfaeafa";
//  gaur[1] = "afe";
 for(i=0; i<10; i++)
   printf("string=%s\n",foo[i]);
 return 0; 

}
