#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>    /* malloc, free, rand */

int Fsize=50; /* max string length of formulas*/
int inputs=10;


int i;
int j;

char *partone(char *g);
char *parttwo(char *g);

/*put all your functions here.  You will need
1.
int parse(char *g) which returns 1 if a proposition, 2 if neg, 3 if binary, ow 0
Of course you will almost certainly need other functions.

For binary formulas you will also need functions that return the first part and the second part of the binary formula.

char *partone(char *g)

char *parttwo(char *g)


You may vary this program provided it reads 10 formulas in a file called "input.txt" and outputs in the way indicated below to a file called "output.txt".
*/
char *get_token(char token){
  if(token=='p'||token=='q'||token=='r'){
    return "F";

  }
  else if(token == '^' || token == 'v'||token=='>'){
    return "B";

  }
  else if(token == '(' ){
    return "L";

  }
  else if(token == ')' ){
    return "R";

  }
  else if(token == '-' ){
    return "N";

  }
  else{
    return "error";
  }
  
}

char *strrep(char * str, char * str2, char * str3){
    char *str1 = str;
    int i, j, k, done, count = 0, gap = 0;
    char temp[50];
    for(i = 0; i < strlen(str1); i += gap){
        if(str1[i] == str2[0]){
            done = 0;
            for(j = i, k = 0; k < strlen(str2); j++, k++){
                if(str1[j] != str2[k]){
                    done = 1;
                    gap = k;
                    break;
                }
            }
            if(done == 0){ // 已找到待替换字符串并替换
                for(j = i + strlen(str2), k = 0; j < strlen(str1); j++, k++){ // 保存原字符串中剩余的字符
                    temp[k] = str1[j];
                }
                temp[k] = '\0'; // 将字符数组变成字符串
                for(j = i, k = 0; k < strlen(str3); j++, k++){ // 字符串替换
                    str1[j] = str3[k];
                    count++;
                }
                for(k = 0; k < strlen(temp); j++, k++){ // 剩余字符串回接
                    str1[j] = temp[k];
                }
                str1[j] = '\0'; // 将字符数组变成字符串
                gap = strlen(str2);
            }
        }else{
            gap = 1;
        }
    }
    if(count == 0){
        return NULL;
    }
    return str1;
}








int parse(char *g)
{
  char *tk = malloc(Fsize);
  for(int i=0;i<Fsize;i++){
    if(g[i]){
      char *a = get_token(g[i]);
      if(strcmp(a,"error")==0){return 0;}
      strcat(tk,a);      
    }
    else{break;}
    
  }
  char* fmla = malloc(Fsize);
  strcpy(fmla,tk);//tk（token串）的copy，用来做parser分析

  while(strstr(fmla,"NF")||strstr(fmla,"LFBFR")){
    if (strstr(fmla,"NF"))
    {
      fmla = strrep(fmla,"NF","F");
    }
    if (strstr(fmla,"LFBFR"))
    {
      fmla = strrep(fmla,"LFBFR","F");
    }
  }
  printf("%s %s\n",tk,fmla);
  if (strcmp(fmla,"F")!=0){
    printf("%s is not a formula.  \n", g);
    return 0;
  }
  else{
    if(strcmp(tk,"F")==0){
      printf("%s is a proposition. \n ", g);
      return 1;
    }
    else if(strncmp(tk,"N",1)==0){
      printf("%s is a negation.  \n", g);
      return 2;
    }
    else if(strncmp(tk,"L",1)==0){
      printf("%s is a binary. The first part is %s and the second part is %s  \n", g, partone(g), parttwo(g));
      return 3;

    }
      
  }
  return -1;


}

char *partone(char *g)
{
  char *a = g;
  int b = 0;//count brackets
  char *one = malloc(Fsize);
  int p =1;
  if(a[1]!='('){
    do{
      one[p-1]=a[p];
      p++;
    }
    while(a[p]!='v'&&a[p]!='^'&&a[p]!='>'&&a[p]!='(');
  }
  if (a[p]=='('){
    for(int i=p;i<=strlen(a);i++){
      if(a[i]=='('){b++;}
      else if(a[i]==')'){b--;}
      one[i-1]=a[i];
      if(b==0){
        break;
      }   
    }
  } 
  
  return one;

}


char *parttwo(char *g)
{
  char *a = partone(g);
  char *two = malloc(Fsize);
  int p = 0;
  for(int i=strlen(a)+2;i<strlen(g)-1;i++){
    two[p] = g[i];
    p++;  
  }
  return two;
}

int main()

{ /*input 10 strings from "input.txt" */


  char *name = malloc(Fsize);
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<inputs;j++)
    {
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name))
      {
          case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
          case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
          case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
          case(3):fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
          default:fprintf(fpout, "What the f***!  ");
    }

    }



  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);
}