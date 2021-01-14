#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */


int Fsize=50; /*maximum formula length*/
int inputs =5000;/* number of formulas expected in input.txt*/
int ThSize=100;/* maximum size of set of formulas, if needed*/
int TabSize=500; /*maximum length of tableau queue, if needed*/

// Reference: https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
// Date of access: December 13rd 2020
struct Stack {
    int top;
    unsigned capacity;
    char* array;
};
 
struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char*)malloc(stack->capacity * sizeof(char));
    return stack;
}
 
int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}
 
void push(struct Stack* stack, char item)
{
    stack->array[++stack->top] = item;
}
 
char pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top--];
}

char peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top];
}

/* A set will contain a list of words. Use NULL for emptyset.  */
  struct set{
    char *item;/*first word of non-empty set*/
    struct set *tail;/*remaining words in the set*/
  };

/* A tableau will contain a list of pointers to sets (of words).  Use NULL for empty list.*/
struct tableau {
  struct set * S; /* pointer to first set in non-empty list */
  struct tableau *rest; /*list of pointers to other sets*/
};

/* A fuction to free all the allocated bits on the tableau */
void deepFree(struct tableau *t) {
  if(!t) {
    return;
  }
  while(t->S) {
    free(t->S->item);
    struct set *tempSet = t->S;
    t->S = t->S->tail;
    free(tempSet);
  }
  struct tableau *tempTableau = t;
  t = t->rest;
  free(tempTableau);
  // deepFree(t);
}

/*You need to change this next bit and include functions for parse, closed and complete.*/
int parse(char* string, char* part1, char* part2) {
    struct Stack* operator_stack = createStack(Fsize);
    struct Stack* output_stack = createStack(Fsize);

    int left_par_count = 0;
    for(int i=0; i<strlen(string); i++){
      char curr = string[i];
      if(curr == 'p' || curr == 'q' || curr == 'r'){
        push(output_stack, curr);
        while(!isEmpty(operator_stack) && peek(operator_stack) == '-'){
          push(output_stack, pop(operator_stack));
        }
      }else if(curr == '^' || curr == '-' || curr == 'v' || curr == '>'){
        push(operator_stack, curr);
      }else if(curr == '('){
        left_par_count++;
        push(operator_stack, curr);
      }else if(curr == ')'){
        left_par_count--;
        int op_count = 0;
        while(!isEmpty(operator_stack) && peek(operator_stack) != '('){
          char top_op = pop(operator_stack);
          push(output_stack, top_op);
          op_count++;
        }
        if(op_count != 1){
          return 0;
        }
        pop(operator_stack);
        while(!isEmpty(operator_stack) && peek(operator_stack) == '-'){
          push(output_stack, pop(operator_stack));
        }
      }else{
        return 0;
      }
    }
    if(left_par_count != 0){
      return 0;
    }
    while(!isEmpty(operator_stack)){
      push(output_stack, pop(operator_stack));
    }

    char top = pop(output_stack);
    if(top == 'p' || top == 'q' || top == 'r'){ // 1 for single propsitional letter.
      return 1;
    }

    if(top == '-'){ // 2 for negation
      return 2;
    }

    if(string[0] != '('){ // A binary formula must start with a left parentheses.
      return 0;
    }

    int op_index = 0;
    int par_count = 0;
    for(int i=0; i<strlen(string); i++){
      if(string[i] == '('){
        par_count++;
      }else if(string[i] == ')'){
        par_count--;
      }else if(par_count == 1 && string[i] == top){
        op_index = i;
        break;
      }
    }

    memcpy(part1, string+1, op_index-1);
    part1[op_index-1] = '\0';
    memcpy(part2, string+op_index+1,strlen(string)-op_index-2);
    part2[strlen(string)-op_index-2] = '\0';

    if(top == '^'){ // 3 for AND
      return 3;
    }else if(top == 'v'){ //4 for OR
      return 4;
    }else if(top == '>'){ //5 for implication
      return 5;
    }
    return 0;

  free(operator_stack);
  free(output_stack);
}

struct set* init_set(char* str){
  char* s = calloc(Fsize, sizeof(char));
  strcpy(s, str);
  struct set* S = calloc(1, sizeof(struct set));
  S->item = s;
  return S;
}

int expand(struct set* set){
  struct set* head = set;
  while(head){
    if(strlen(head->item)>2){
      return 0;
    }
    head = head->tail;
  }
  return 1;
}

int contradict(struct set* set){
  struct set* head = set;
  int prop[3] = {0,0,0};
  int neg_prop[3] = {0,0,0};
  while(head){
    char* literal = head -> item;
    if(strlen(literal) == 1){
      switch(literal[0]){
        case('p'): prop[0] = 1; break;
        case('q'): prop[1] = 1; break;
        case('r'): prop[2] = 1; break;
      }
    }else if(strlen(literal) == 2){
      switch(literal[1]){
        case('p'): neg_prop[0] = 1; break;
        case('q'): neg_prop[1] = 1; break;
        case('r'): neg_prop[2] = 1; break;
      }
    }
    head = head -> tail;
  }
  for(int i=0; i<3; i++){
    if(prop[i] == 1 && neg_prop[i] == 1){
      return 1;
    }
  }
  return 0;
}

struct set* copy_list(struct set* from){
  if(from == NULL){
    return NULL;
  }else{
    struct set* copy = init_set(from->item);
    copy->tail = copy_list(from->tail);
    return copy;
  }
}

void append_tab(struct tableau* t, struct set* s){
  struct tableau* head = t;
  while(head){
    if(head->rest == NULL){
      struct tableau* temp = calloc(1, sizeof(struct tableau));
      temp -> S = s;
      head->rest = temp;
      break;
    }
    head = head -> rest;
  }
}

void printList(struct set* head) 
{ 
    struct set* ptr = head; 
    while (ptr) { 
  
        printf("%s -> ", ptr->item); 
        ptr = ptr->tail; 
    } 
  
    printf("NULL\n"); 
} 

int complete(struct tableau *t){
  struct tableau* head_t = t;
  while(head_t){
    struct set* head_set = head_t->S;
    if(expand(head_set) && !contradict(head_set)){
      return 1;
    }
    while(head_set){
      char* str = head_set -> item;
      if(strlen(str) > 2){
        char neg = '-';
        char* part1 = calloc(Fsize, sizeof(char));
        char* part2 = calloc(Fsize, sizeof(char));
        int middle = parse(str,part1,part2);
        if(middle == 2){ // negation, for now part1 and part2 are both empty
          char* inner_str = calloc(Fsize, sizeof(char));
          memcpy(inner_str, str+1, strlen(str)-1);
          int inner_mid = parse(inner_str,part1,part2);
          if(inner_mid == 3 || inner_mid == 4){ 
            char* new_part1 = calloc(Fsize, sizeof(char));
            strncat(new_part1,&neg,1);
            strcat(new_part1,part1);
            free(part1);
            char* new_part2 = calloc(Fsize, sizeof(char));
            strncat(new_part2,&neg,1);
            strcat(new_part2,part2);
            free(part2);
            if(inner_mid == 4){ // 4 for OR, negation + OR = alpha expansion
              head_set -> item = new_part1;
              struct set* new_set = init_set(new_part2);
              new_set -> tail = head_set -> tail;
              head_set -> tail = new_set;
              append_tab(t,head_t->S);
            }else{ // 3 for AND, negation + AND = beta expansion
              head_set -> item = new_part1;
              struct set* new_set1 = copy_list(head_t->S);
              append_tab(t,new_set1);
              head_set -> item = new_part2;
              struct set* new_set2 = copy_list(head_t->S);
              append_tab(t,new_set2);
            }
          }else if(inner_mid == 2){ // 2 for negation, double negation = alpha expansion
            char* result = calloc(Fsize,sizeof(char));
            memcpy(result,inner_str+1,strlen(str)-1);
            head_set -> item = result;
            struct set* new_set = copy_list(head_t->S);
            append_tab(t,head_t->S);
          }else if(inner_mid == 5){ // 5 for implication, negation + implication = alpha expansion   
            char* new_part2 = calloc(Fsize, sizeof(char));
            strncat(new_part2,&neg,1);
            strcat(new_part2,part2);
            free(part2);
            head_set -> item = part1;
            struct set* new_set = init_set(new_part2);
            new_set -> tail = head_set -> tail;
            head_set -> tail = new_set;
            append_tab(t,head_t->S);
          }
          free(inner_str);
        }else if(middle == 3){ // 3 for AND, alpha expansion
          head_set -> item = part1;
          struct set* new_set = init_set(part2);
          new_set -> tail = head_set -> tail;
          head_set -> tail = new_set;
          append_tab(t,head_t->S);
        }else if(middle == 4){ // 4 for OR, beta expansion 
          head_set -> item = part1;
          struct set* new_set1 = copy_list(head_t->S);
          append_tab(t,new_set1);
          head_set -> item = part2;
          struct set* new_set2 = copy_list(head_t->S);
          append_tab(t,new_set2);
        }else if(middle == 5){ // 5 for implication, beta expansion
          char* new_part1 = calloc(Fsize, sizeof(char));
          strncat(new_part1,&neg,1);
          strcat(new_part1,part1);
          free(part1);
          head_set -> item = new_part1;
          struct set* new_set1 = copy_list(head_t->S);
          append_tab(t,new_set1);
          head_set -> item = part2;
          struct set* new_set2 = copy_list(head_t->S);
          append_tab(t,new_set2);
        }
      }
      head_set = head_set -> tail;
    }
    head_t = head_t -> rest;
    }
    return 0;
  }

int main()
{/*input 10 strings from "input.txt" */

    /*You should not need to alter the input and output parts of the program below.*/
    char *name = calloc(Fsize, sizeof(char));
    char *left = calloc(Fsize, sizeof(char));
    char *right = calloc(Fsize, sizeof(char));
    FILE *fp, *fpout;

    /* reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
    if ((  fpout=fopen("output2.txt","w"))==NULL){printf("Error opening file");exit(1);}

    int j;

    for(j=0;j<inputs;j++)
    {
        fscanf(fp, "%s",name);/*read formula*/
        int parse_result = parse(name,left,right);
        switch (parse_result)
        {
            case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
            case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
            case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
            case(3):
            case(4):
            case(5):
              fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, left, right);
              break;
            default:fprintf(fpout, "What the f***!  ");
        }

        if (parse_result!=0) 
        {
          /* Iitialise the tableau with the formula */
          char* s = calloc(Fsize, sizeof(char));
          strcpy(s, name);
          struct set* S = calloc(1, sizeof(struct set));
          S->item = s;
          struct tableau* t = calloc(1, sizeof(struct tableau));
          t->S = S;

          /* Completes the tableau and checks if it is closed */
          int sat = complete(t);
          if (!sat)  fprintf(fpout, "%s is not satisfiable.\n", name);
          else fprintf(fpout, "%s is satisfiable.\n", name);

          /* Frees all the bits of memory on the tableau*/
          deepFree(t);
        }
        else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }

    fclose(fp);
    fclose(fpout);
    free(left);
    free(right);
    free(name);

  return(0);
}
