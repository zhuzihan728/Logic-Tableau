#include <stdio.h>
#include <string.h> /* for all the new-fangled string functions */
#include <stdlib.h> /* malloc, free, rand */

int Fsize = 50;    /*maximum formula length*/
int inputs = 5000;   /* number of formulas expected in input.txt*/
int ThSize = 100;  /* maximum size of set of formulas, if needed*/
int TabSize = 500; /*maximum length of tableau queue, if needed*/

char *partone1(char *g);
char *parttwo1(char *g);

/* A set will contain a list of words. Use NULL for emptyset.  */
struct set
{
  char *item;       /*first word of non-empty set*/
  struct set *tail; /*remaining words in the set*/
};

/* A tableau will contain a list of pointers to sets (of words).  Use NULL for empty list.*/
struct tableau
{
  struct set *S;        /* pointer to first set in non-empty list */
  struct tableau *rest; /*list of pointers to other sets*/
};

/* A fuction to free all the allocated bits on the tableau */
void deepFree(struct tableau *t)
{
  if (!t)
    return;
  while (t->S)
  {
    //free(t->S->item);
    struct set *tempSet = t->S;
    t->S = t->S->tail;
    free(tempSet);
  }
  struct tableau *tempTableau = t;
  t = t->rest;
  free(tempTableau);
  deepFree(t);
}

/*You need to change this next bit and include functions for parse, closed and complete.*/
char *get_token(char token)
{
  if (token == 'p' || token == 'q' || token == 'r')
  {
    return "F";
  }
  else if (token == '^' || token == 'v' || token == '>')
  {
    return "B";
  }
  else if (token == '(')
  {
    return "L";
  }
  else if (token == ')')
  {
    return "R";
  }
  else if (token == '-')
  {
    return "N";
  }
  else
  {
    return "error";
  }
}

char *strrep(char *str, char *str2, char *str3)
{
  char *str1 = str;
  int i, j, k, done, count = 0, gap = 0;
  char temp[50];
  for (i = 0; i < strlen(str1); i += gap)
  {
    if (str1[i] == str2[0])
    {
      done = 0;
      for (j = i, k = 0; k < strlen(str2); j++, k++)
      {
        if (str1[j] != str2[k])
        {
          done = 1;
          gap = k;
          break;
        }
      }
      if (done == 0)
      { 
        for (j = i + strlen(str2), k = 0; j < strlen(str1); j++, k++)
        { 
          temp[k] = str1[j];
        }
        temp[k] = '\0'; 
        for (j = i, k = 0; k < strlen(str3); j++, k++)
        { 
          str1[j] = str3[k];
          count++;
        }
        for (k = 0; k < strlen(temp); j++, k++)
        { 
          str1[j] = temp[k];
        }
        str1[j] = '\0'; 
        gap = strlen(str2);
      }
    }
    else
    {
      gap = 1;
    }
  }
  if (count == 0)
  {
    return NULL;
  }
  return str1;
}

int parse(char *g)
{
  char *tk = calloc(Fsize, sizeof(char));
  for (int i = 0; i < Fsize; i++)
  {
    if (g[i])
    {
      char *a = get_token(g[i]);
      if (strcmp(a, "error") == 0)
      {
        return 0;
      }
      strcat(tk, a);
    }
    else
    {
      break;
    }
  }
  char *fmla = calloc(Fsize, sizeof(char));
  strcpy(fmla, tk); 

  while (strstr(fmla, "NF") || strstr(fmla, "LFBFR"))
  {
    if (strstr(fmla, "NF"))
    {
      fmla = strrep(fmla, "NF", "F");
    }
    if (strstr(fmla, "LFBFR"))
    {
      fmla = strrep(fmla, "LFBFR", "F");
    }
  }
  //printf("%s %s\n", tk, fmla);
  if (strcmp(fmla, "F") != 0)
  {
    //printf("%s is not a formula.  \n", g);
    return 0;
  }
  else
  {
    if (strcmp(tk, "F") == 0)
    {
      //printf("%s is a proposition. \n ", g);
      return 1;
    }
    else if (strncmp(tk, "N", 1) == 0)
    {
      //printf("%s is a negation.  \n", g);
      return 2;
    }
    else if (strncmp(tk, "L", 1) == 0)
    {
      //printf("%s is a binary. The first part is %s and the second part is %s  \n", g, partone1(g), parttwo1(g));
      return 3;
    }
  }
  return -1;
}

char *partone1(char *g)
{
  char *a = g;
  int b = 0; //count brackets
  char *one = calloc(Fsize, sizeof(char));
  int p = 1;
  if (a[1] != '(')
  {
    do
    {
      one[p - 1] = a[p];
      p++;
    } while (a[p] != 'v' && a[p] != '^' && a[p] != '>' && a[p] != '(');
  }
  if (a[p] == '(')
  {
    for (int i = p; i <= strlen(a); i++)
    {
      if (a[i] == '(')
      {
        b++;
      }
      else if (a[i] == ')')
      {
        b--;
      }
      one[i - 1] = a[i];
      if (b == 0)
      {
        break;
      }
    }
  }

  return one;
}

char *parttwo1(char *g)
{
  char *a = partone1(g);
  char *two = calloc(Fsize, sizeof(char));
  int p = 0;
  for (int i = strlen(a) + 2; i < strlen(g) - 1; i++)
  {
    two[p] = g[i];
    p++;
  }
  return two;
}

void partone(char *fmla, char *dest)
{
  char *a = partone1(fmla);
  int i;
  for (i = 0; i < strlen(a); i++)
  {
    dest[i] = a[i];
  }
  dest[i] = '\0';
}

void parttwo(char *fmla, char *dest)
{
  char *a = parttwo1(fmla);
  int i;
  for (i = 0; i < strlen(a); i++)
  {
    dest[i] = a[i];
  }
  dest[i] = '\0';
}

char *negate(char *fmla)
{
  char *negated = calloc(Fsize, sizeof(char));
  int originalLen = strlen(fmla);
  if (fmla[0] == '-')
  {
    for (int i = 0; i < strlen(fmla); i++)
    {
      negated[i] = fmla[i + 1];
    }
    negated[originalLen - 1] = '\0';
  }
  else
  {
    negated[0] = '-';
    for (int i = 0; i < strlen(fmla); i++)
    {
      negated[i + 1] = fmla[i];
    }
    fmla[originalLen + 1] = '\0';
  }
  return negated;
}

int getConnective(char *fmla)
{
  char *left = partone1(fmla);
  int len = strlen(left);
  char connective = fmla[len + 1];
  if (connective == 'v')
  { //or 1
    return 1;
  }
  else if (connective == '^')
  { // and 2
    return 2;
  }
  else if (connective == '>')
  { // imply 3
    return 3;
  }

  return -1;
}

int checkForm(char *fmla)
{
  int type = parse(fmla);
  if (type == 1)
  { //literal
    return 0;
  }
  else if (type == 2)
  {
    char *negated = negate(fmla);
    if (parse(negated) == 1)
    { //-literal
      return 1;
    }
    else if (parse(negated) == 2)
    { //--fmla  a
      return 2;
    }
    else if (parse(negated) == 3)
    {
      int connective = getConnective(negated);
      if (connective == 1)
      { //-(fmla v fmla) a
        return 3;
      }
      else if (connective == 2)
      { //-(fmla ^ fmla) b
        return 4;
      }
      else
      { //-(fmla > fmla) a
        return 5;
      }
    }
  }
  else if (type == 3)
  {
    int connective = getConnective(fmla);
    if (connective == 1)
    { //(fmla v fmla) b
      return 6;
    }
    else if (connective == 2)
    { //(fmla ^ fmla) a
      return 7;
    }
    else
    { //(fmla > fmla) b
      return 8;
    }
  }
  return -1;
}
void printSet(struct set *s)
{
  printf("the set consists: ");
  while (s != NULL)
  {
    printf("%s ", s->item);
    s = s->tail;
  }
  printf("\n");
}
int contains(struct set *a, char *literal)
{
  while (a != NULL)
  {
    if (strcmp(a->item, literal) == 0)
    {
      return 1;
    }
    a = a->tail;
  }
  return 0;
}

int closed(struct tableau *t)
{
  while (t != NULL)
  {
    struct set *branch = t->S;
    if ((contains(branch, "p") && contains(branch, "-p")) || (contains(branch, "q") && contains(branch, "-q")) || (contains(branch, "r") && contains(branch, "-r")))
    {
      t = t->rest;
    }
    else
    {
      //printf("open\n");
      return 0;
    }
  }
  //printf("close\n");
  return 1;
}

void addSet(struct set *a, char *toadd)
{
  struct set *setToAdd = calloc(1, sizeof(struct set));
  setToAdd->item = toadd;
  setToAdd->tail = a->tail;
  a->tail = setToAdd;
}

void addTab(struct tableau *t)
{
  struct set *from = t->S;
  struct set *to = calloc(1, sizeof(struct set));
  to->item = from->item;
  from = from->tail;
  while (from != NULL)
  {
    addSet(to, from->item);
    from = from->tail;
  }
  struct tableau *tab = calloc(1, sizeof(struct tableau));
  tab->S = to;
  tab->rest = t->rest;
  t->rest = tab;
}

void completeOneBranch(struct tableau *t)
{
  char *left;
  char *right;
  struct set *branch = t->S;
  //t->S = branch;
  //printf("fmla: ");
  //printSet(branch);
  while (branch != NULL)
  {
    char *fmla = branch->item;
    //printf("this round fmla is %s\n", fmla);
    int type = checkForm(fmla);
    //printf("type is %d\n",type);
    switch (type)
    {
    case 0:
      branch = branch->tail;
      break;
    case 1:
      branch = branch->tail;
      break;
    case 2:
      fmla = negate(fmla);
      fmla = negate(fmla);
      branch->item = fmla;
      break;
    case 3: //-(fmla v fmla) a
      fmla = negate(fmla);
      left = partone1(fmla);
      right = parttwo1(fmla);
      left = negate(left);
      right = negate(right);
      branch->item = left;
      addSet(branch, right);
      break;
    case 4: //-(fmla ^ fmla) b
      fmla = negate(fmla);
      left = partone1(fmla);
      right = parttwo1(fmla);
      left = negate(left);
      right = negate(right);
      branch->item = right;
      addTab(t);
      branch->item = left;
      break;
    case 5: //-(fmla > fmla) a
      fmla = negate(fmla);
      left = partone1(fmla);
      right = parttwo1(fmla);
      right = negate(right);
      branch->item = left;
      addSet(branch, right);
      break;
    case 6: //(fmla v fmla) b
      left = partone1(fmla);
      right = parttwo1(fmla);
      branch->item = right;
      addTab(t);
      branch->item = left;
      break;
    case 7: //(fmla ^ fmla) a
      left = partone1(fmla);
      right = parttwo1(fmla);
      branch->item = left;
      addSet(branch, right);
      break;
    case 8: //(fmla > fmla) b
      left = partone1(fmla);
      right = parttwo1(fmla);
      left = negate(left);
      branch->item = right;
      addTab(t);
      branch->item = left;
      break;
    default:
      break;
    }
  }
}

void complete(struct tableau *t)
{
  while (t != NULL)
  {
    completeOneBranch(t);
    //printf("after: ");
    //printSet(t->S);
    t = t->rest;
  }
}

int main()
{ /*input 10 strings from "input.txt" */

  /*You should not need to alter the input and output parts of the program below.*/

  char *name = calloc(Fsize, sizeof(char));
  char *left = calloc(Fsize, sizeof(char));
  char *right = calloc(Fsize, sizeof(char));
  FILE *fp, *fpout;

  /* reads from input.txt, writes to output.txt*/
  if ((fp = fopen("input.txt", "r")) == NULL)
  {
    printf("Error opening file");
    exit(1);
  }
  if ((fpout = fopen("output1.txt", "w")) == NULL)
  {
    printf("Error opening file");
    exit(1);
  }

  int j;

  for (j = 0; j < inputs; j++)
  {
    fscanf(fp, "%s", name); /*read formula*/
    switch (parse(name))
    {
    case (0):
      fprintf(fpout, "%s is not a formula.  \n", name);
      break;
    case (1):
      fprintf(fpout, "%s is a proposition. \n ", name);
      break;
    case (2):
      fprintf(fpout, "%s is a negation.  \n", name);
      break;
    case (3):
      partone(name, left);
      parttwo(name, right);
      fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, left, right);
      break;
    default:
      fprintf(fpout, "What the f***!  ");
    }

    if (parse(name) != 0)
    {
      /* Iitialise the tableau with the formula */
      char *s = calloc(Fsize, sizeof(char));
      strcpy(s, name);
      struct set *S = calloc(1, sizeof(struct set));
      S->item = s;
      struct tableau *t = calloc(1, sizeof(struct tableau));
      t->S = S;

      /* Completes the tableau and checks if it is closed */
      complete(t);
      if (closed(t))
        fprintf(fpout, "%s is not satisfiable.\n", name);
      else
        fprintf(fpout, "%s is satisfiable.\n", name);

      /* Frees all the bits of memory on the tableau*/
      deepFree(t);
    }
    else
      fprintf(fpout, "I told you, %s is not a formula.\n", name);
  }

  fclose(fp);
  fclose(fpout);
  free(left);
  free(right);
  free(name);

  return (0);
}