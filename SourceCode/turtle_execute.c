#include "turtle_header.h"

/*---------------------------------------------------------------------------------------------*/
/*-----------------STORES A VARIABLE INSIDE THE LINK LIST AND RETURNS TRUE --------------------*/
/*---------------------------------------------------------------------------------------------*/
int setVar (Program *p, char* varName, float var)
{

   /*----IF THE FIRST VARIABLE IN THE LIST IS EMPTY OR IT HAS THE SAME VARIABLE NAME THEN STORE THE VARIABLE IN IT------*/
  if (p->varFirst->varName[0] == '\0') 
  {
   p->varFirst->var = var;
   strcpy (p->varFirst->varName, varName);
   p->varFirst->next = NULL;
   return TRUE;
  }else if (strcmp (p->varFirst->varName, varName) == SAME)
  {
    p->varFirst->var = var;
    return TRUE;
  }

/*-----START A POINTER FOR THE FIRST NODE IN THE LIST THAT TO BE USED TO MOVE FROM ONE NODE TO ANOTHER TO FIND THE VARIABLE OR AN EMPTY PLACE-----*/
  Varnode *varTemp;
  varTemp = p->varFirst;

int varCounter = 1;  /*---THIS COUNTER IS USED TO COUNT THE NUMBER IF VARIABLE AND TO CHECK IF THEY EXCEEDED THE PREMITTED NUMBER MAXVAR----*/
  while (varTemp->next != NULL) /*----WHILE THERE IS A STORED VARIABLE MOVE TO THE NEX UNTIL FIND AN EMPTY PLACE----*/
  {
    varCounter++;
    if (varCounter > MAXVAR)
    {
      printf ("ERROR: THE NUMBER OF USED VARIABLE IS MORE THAN THE ALLOWED NUMBER\n");
      exit(1);
    }
    varTemp = varTemp->next;
     if (strcmp (varTemp->varName, varName) == SAME) /*------IF A VARIABLE WITH THE SAME NAME IS FOUND IN THE LIST THEN USE THE CURRENT NODE OF THE VARIABLE WITH THE SAME NAME TO STORE THE VALUE IN IT---------*/
      {
        varTemp->var = var;
        return TRUE;
      }
  }
  /*------IF A VARIABLE WITH THE SAME NAME WAS NOT FIND THEN MAKE ANOTHER NODE AND STORE THE NEW VARIABLE NAME AND VALUE IN IT-------*/
  Varnode *newVar;
  newVar = (Varnode *) malloc (sizeof (Varnode));
  varTemp->next = newVar;
  newVar->next = NULL;
  newVar->var = var;
  strcpy (newVar->varName, varName);
  return TRUE;
}






/*---------------------------------------------------------------------------------------------*/
/*------------------------SEARCHES FOR A VARIABLE AND RETURNS ITS VALUE------------------------*/
/*---------------------------------------------------------------------------------------------*/
float getVar (Program *p, char* varName)
{
  
  if (strcmp (p->varFirst->varName, varName) == SAME)
  {
    return p->varFirst->var;
  }

  Varnode *varTemp;
  varTemp = p->varFirst;


  while (varTemp->next != NULL) /*----WHILE THERE IS A STORED VARIABLE MOVE TO THE NEX UNTIL FIND AN EMPTY PLACE----*/
  {
    varTemp = varTemp->next;
    if (strcmp (varTemp->varName, varName) == SAME)
      {
        return varTemp->var;
      } 
  }
  printf("\nERROR, THE VARIABLE %s DOESN'T EXIST\n", varName);
  foundError(p);
  exit(1);
  return FALSE;
}



/*-------------------------------------------------------------------------------*/
/*-------------------EXECUTES THE COMMAND FD AND DRAW THE LINE-------------------*/
/*-------------------------------------------------------------------------------*/
void FDMaker (Program *p, Graphics *graphics, float value)
{
    float angle = p->angle;   /*INITIALIZE A VARIABLE TO HOLD THE VALUE OF ANGLE AND MODULISE IT*/
  

  /*----IF THE ANGLE IS BIGGER THAN 360 MODULIZE TO 360-----*/
      while (angle > 360){
       angle = angle - 360;
        }

    /*-----TWOPI is 3.14 * 2-------*/
    /*------THIS EQUATION IS TO GET THE X AND Y POSITION OF THE END OF THE LINE WHEN ITS BEGINNING IS THE CENTER OF A CIRCLE-----*/
    float x = p->posX + (value * cos(TWOPI*angle/360));
    float y = p->posY + (value * sin(TWOPI*angle/360));

    SDL_SetRenderDrawColor(graphics->renderer,COLOR,COLOR,COLOR,255);


    SDL_RenderDrawLine(graphics->renderer, p->posX, p->posY, x, y);

    



    /*-------UPDATE THE NEW POSITION OF X AND Y, THEY BECOME THE END OF THE LINE-----*/
    p->posX = x;
    p->posY = y;

    /*--------CALL THE FUNCTION THAT EXCUTE THE DRAWING ON THE SCREEN-------*/
    draw(graphics);
}



/*----------------------------------------------------------------------------------------*/
/*-------------------------EXECUTE THE CHANGE OF THE ANGLE FOR LT-------------------------*/
/*----------------------------------------------------------------------------------------*/
void LTMaker(Program *p, float angle)
{
    p->angle = p->angle - angle;
}


/*----------------------------------------------------------------------------------------*/
/*-------------------------EXECUTE THE CHANGE OF THE ANGLE FOR RT-------------------------*/
/*----------------------------------------------------------------------------------------*/
void RTMaker(Program *p, float angle)
{
    p->angle = p->angle + angle;
}


/*----------------------------------------------------------------------------------------*/
/*---------------------------------initializes a stack------------------------------------*/
/*----------------------------------------------------------------------------------------*/
void InitialiseStack(Stack *stk)
{
stk->top = 0;
}


/*----------------------------------------------------------------------------------------*/
/*-------------------------------PUSH A VALUE IN THE STACK--------------------------------*/
/*----------------------------------------------------------------------------------------*/
void Push(Stack *stk, float currenValue)
{
/* Make sure stack doesnt overflow */
assert(stk->top < STACKSIZE);
stk->valueStk[stk->top] = currenValue;
stk->top = stk->top + 1;
}


/*----------------------------------------------------------------------------------------*/
/*-------------------------------POP A VALUE FROM THE STACK-------------------------------*/
/*----------------------------------------------------------------------------------------*/
float Pop(Stack *stk)
{
/* Can’t pop empty stack (underflow) */
assert(stk->top > 0);
stk->top = stk->top - 1;
return stk->valueStk[stk->top];
}


/*----------------------------------------------------------------------------------------*/
/*----------------------CHECKS EACH TOKEN AND TRY TO MAKE SENSE OF IT---------------------*/
/*---------------------SEPERATE SIGNS AND PUT THEM IN SEPERATE TOKENS---------------------*/
/*----------------------------------------------------------------------------------------*/
void checkToken (Program *p, int *pcw)
{
    int ltCount = 0;   /*LETTER COUNTER, COUNTS INSIDE A STRING LETTER BY LETTER*/
    char tmpCur;  /*A CHAR TO HOLD THE CURRENT CHARACTER*/
    char tmpNxt; /*A CHAR TO HOLD THE NEXT CHARACTER*/

  while  (p->word[*pcw][ltCount] != '\0')   /*GO THROUGH ALL LETTER IN A TOKEN TO ANALYZE IT*/
  {
      tmpCur = p->word[*pcw][ltCount]; /*AT EACH LOOP TMOCUR HAS A CHAR WHILE TMPNXT HAS THE NEXT ONE. USEFUL TO CHECK FOR COMMENT SIGNS AND EQUAL SIGN :=---*/
      tmpNxt = p->word[*pcw][ltCount+1];
      
      if ((tmpCur == ':' && tmpNxt == '=') || (tmpCur == '/' && tmpNxt == '*') || (tmpCur == '*' && tmpNxt == '/')) /*CHECK SIGN OF TWO CHARACTER FIRST*/
        {
                 if (ltCount > 0)  /*---if the sign was not at the start of the token---*/
                 {
                     signMidFix(p, pcw, &ltCount, TWOSIGNS);
                 }else if (p->word[*pcw][ltCount+TWOSIGNS] != '\0'){
                     signStrFix(p, pcw, &ltCount, TWOSIGNS);
                 }else if (p->word[*pcw][ltCount+TWOSIGNS] == '\0' && ltCount == 0) /*---if the sign was located in one token alone---*/
                 {
                     return;
                 }
                 /*----ELSE CHECK FOR SIGN OF ONE CHAR LIKE + - / * AND SO ON------*/
        } else if (tmpCur == '{' || tmpCur == '}'|| tmpCur == ';'|| tmpCur == '*' || tmpCur == '/' 
                    || (tmpCur == '-' && isdigit(tmpNxt) == FALSE)/*SPECIAL CASE IF THE SIGN - MEANT A NEGATIVE VALUE AND NOT SUBMISSION*/
                    || tmpCur == '+'|| tmpCur == '('|| tmpCur == ')'|| tmpCur == '&'|| tmpCur == '>'
                    || tmpCur == '<' || tmpCur == '=' || tmpCur == '"')
                {
                if (ltCount > 0)  //if the sign was not at the start of the token
                 {
                     signMidFix(p, pcw, &ltCount, ONESIGN);
                 }else if (p->word[*pcw][ltCount+ONESIGN] != '\0'){
                     signStrFix(p, pcw, &ltCount, ONESIGN);
                 }else if (p->word[*pcw][ltCount+ONESIGN] == '\0' && ltCount == 0) /*----if the sign was located in one token alone-----*/
                 {
                     return;
                 }
                }

    ltCount++;

  }
checkTokenSize(p, pcw);
}


/*----------------------------------------------------------------------------------------*/
/*------------------SEPERATE THE SIGN IF LOCATED IN THE MIDDLE OF A TOKEN-----------------*/
/*----------------------------------------------------------------------------------------*/
void signMidFix(Program *p, int *pcw, int *ltCount, int nSigns)
{
    int newCount = 0;  //set the counter for the new word's letters
    int oldCount = (*ltCount); //set the counter for the old word to position of the char just after the sign
    while  (p->word[*pcw][oldCount] != '\0') //copy the characters after the sign to a new word
    {
        p->word[(*pcw)+1][newCount] = p->word[*pcw][oldCount];
        p->word[(*pcw)+1][newCount+1] = '\0'; //just in case
        newCount++;
        oldCount++;
    }
     p->word[*pcw][(*ltCount)] = '\0'; //end the current word by putting \0 in place of first sign

    *pcw = *pcw + 1;
    (*ltCount) = -1;

}



/*----------------------------------------------------------------------------------------*/
/*------------------SEPERATE THE SIGN IF LOCATED IN THE START OF A TOKEN------------------*/
/*----------------------------------------------------------------------------------------*/
void signStrFix(Program *p, int *pcw, int *ltCount, int nSigns)

{
    int newCount = 0;  //set the counter for the new word's letters
    int oldCount = (*ltCount)+nSigns; //set the counter for the old word to the letter just after the symbol
    int tknNxt = (*pcw)+1;
    while  (p->word[*pcw][oldCount] != '\0') //copy the letters after the symbol to a new word
    {
        p->word[tknNxt][newCount] = p->word[*pcw][oldCount];
        p->word[tknNxt][newCount+1] = '\0'; //just in case
        newCount++;
        oldCount++;
    }
    p->word[*pcw][(*ltCount)+nSigns] = '\0';  //make the first token hold only the sign


    *pcw = *pcw + 1; //move to the next word

       (*ltCount) = -1;  //set the counter to zero by -1 to be incremented down the loop
}



/*----------------------------------------------------------------------------------------*/
/*----------------------CHECKS THE SIZE OF A TOKEN AND GIVES AN ERROR---------------------*/
/*----------------------------------------------------------------------------------------*/
void checkTokenSize (Program *p, int *pcw)
{
    int ltCount = 0;
  while  (ltCount < MAXTOKENSIZE && p->word[*pcw][ltCount] != '\0')
  {
    ltCount++;
  }
  if (ltCount == MAXTOKENSIZE)
  {
      printf("Error with the size of token  \"%s\"\n", p->word[*pcw]);
      exit(1);
  }
}

