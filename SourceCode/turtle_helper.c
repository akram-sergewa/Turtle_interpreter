#include "turtle_header.h"


/*-----------------------------------------------------------------------------------------------*/
/*-----------------------INITIALIZE THE prog STRUCTURE AND FILL IT WITH--------------------------*/
/*----------------------INITIAL VALUES FROM STANDARD HASH DEFINED VALUES-------------------------*/
/*-----------------------------------------------------------------------------------------------*/
void progInitializer (Program *p)
{
    p->currentWord = 0;   /*the first word's number in zero*/
    p->angle = FIRSTANGLE;
    p->posY = FIRSTY;
    p->posX = FIRSTX;


/*------FILL THE TOKENS OF THE STRUCTURE BY EMPTY STRINGS '\0'------*/
    for(int i=0; i<MAXNUMTOKENS; i++)
    {
        p->word[i][0] = '\0';	  /*Make all word in the structure as empty*/
    }


/*--------------------------------------------------------------------------------------------------*/
/*-----SET THE FIRST VARIABLE IN THE LIST AND MAKE IT EMPTY BY GIVING IT ASCII VALUE OF '\0'--------*/
/*--------------------------------------------------------------------------------------------------*/
    Varnode *varFirst;
    varFirst = (Varnode *) malloc (sizeof (Varnode));
    p->varFirst = varFirst;
    p->varFirst->next = NULL;
    p->varFirst->varName[0] = '\0';   /*--------------give the first char of the variable name the value '\0' that is to indicate that it is empty------------*/
}


/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------CHECKS THE TYPES COMMAND------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
void checkCommand(int argc, char *argv[])
{

if (argc == 2) /*---THIS MEANS THE NAME OF THE PROGRAM AND THE NAME OF THE FILE WERE TYPED IN THE COMMAND LINE-----*/
    {
        int counter = 0;
        while (argv[1][counter] != '\0')
        {
            if (counter < MAXFILENAME)
            {
            counter++;  
            }else{
            printf("ERROR:\n THERE IS A MISTAKE WITH THE TYPED COMMAND\n THE FILE NAME IS BIGGER THAN MAXFILENAME\n");
            exit(1);
            }
            
        }
        for (int i = 0; i < MAXFILENAME; i++)
        {

        }
    }else{
        printf("ERROR:\n THERE IS A MISTAKE WITH THE TYPED COMMAND\n SPECIFY YOUR FILE NAME JUST AFTER THE NAME OF THE PROGRAM\n");
        exit(1);
    }
}





/*-----------------------------------------------------------------------------------------------*/
/*----------------START CHECKING ALL POSSIBLE TOKENS, AND SHOULD RETURN EVERYTIME----------------*/
/*------------------------------OTHERWISE THERE IS AN ERROR IN THE CODE--------------------------*/
/*-----------------------------------------------------------------------------------------------*/
void instrctionChecker(Program *p, Graphics *graphics)
{

    if (FDChecker(p, graphics))
    {
        return;
    }
    if (LTChecker(p))
    {
        return;
    }
    if (RTChecker(p))
    {
        return;
    }
    if (DOChecker(p, graphics))
    {
        return;
    }
    if (SETChecker(p))
    {
        return;
    }
    
	//ELSE, THERE IS AN ERROR
	printf("\nTHERE IS WRONG WORD IN THE CODE\n");
    foundError(p);

}


/*-----------------------------------------------------------------------------------------------*/
/*-------------------CHECKS THE COMMAND FD AND THE VARIABLE OR NUMBER THAT FOLLOWS---------------*/
/*---------------------RETURNS FALSE WHENEVER THERE IS A PROBLEM IN THE PROGRAMS-----------------*/
/*-----------------------------------------------------------------------------------------------*/
int FDChecker(Program *p, Graphics *graphics)
{
 	if (stringChecker(p, "FD"))
    {
        float value;
        if (VARNUMValue(p,&value)) //if there is a value in VARNUM
        {
            FDMaker(p, graphics, value);
            return TRUE;  //return to instrcListChecker and check the next words
        }else{
        printf("ERROR WITH THE COMMAND FD, NO VARIABLE OR NUMBER IS FOUND\n");
        foundError(p);
        }

	}

return FALSE;
}


/*-----------------------------------------------------------------------------------------------*/
/*-------------------CHECKS THE COMMAND LT AND THE VARIABLE OR NUMBER THAT FOLLOWS---------------*/
/*---------------------RETURNS FALSE WHENEVER THERE IS A PROBLEM IN THE PROGRAMS-----------------*/
/*-----------------------------------------------------------------------------------------------*/
int LTChecker(Program *p)
{
 	if (stringChecker(p, "LT"))
     {
    float value;
    if (VARNUMValue(p,&value)) //if there is a value in VARNUM
    {
        LTMaker(p, value);
        return TRUE;  //return to instrcListChecker and check the next words
    }else{
        printf("ERROR:\n WITH THE COMMAND LT, NO VARIABLE OR NUMBER IS FOUND\n");
        foundError(p);
        }

	}

return FALSE;
}



/*-----------------------------------------------------------------------------------------------*/
/*-------------------CHECKS THE COMMAND RT AND THE VARIABLE OR NUMBER THAT FOLLOWS---------------*/
/*---------------------RETURNS FALSE WHENEVER THERE IS A PROBLEM IN THE PROGRAMS-----------------*/
/*-----------------------------------------------------------------------------------------------*/
int RTChecker(Program *p)
{
 	if (stringChecker(p, "RT"))
	 {
    float value;
    if (VARNUMValue(p,&value)) //if there is a value in VARNUM
    {
        RTMaker(p, value);
        return TRUE;  //return to instrcListChecker and check the next words
    }else{
        printf("ERROR WITH THE COMMAND RT, NO VARIABLE OR NUMBER IS FOUND\n");
        foundError(p);
        }

	}

return FALSE;
}



/*-----------------------------------------------------------------------------------------------*/
/*-----------CHECKS THE GRAMMAR VARNUM AND RETURNS THE TYPE (VARIABLE, VALUE) OR FALSE-----------*/
/*-----------------------------------------------------------------------------------------------*/
int VARNUMChecker(Program *p)
{
    int i = 0;
    while (p->word[p->currentWord][i] != '\0')  /*Checks if all the char in the string are numbers*/
    {
       if (isdigit(p->word[p->currentWord][i]) == FALSE && p->word[p->currentWord][i] != '.' && p->word[p->currentWord][i] != '-') //means it is not a number
        {
            if (VARChecker(p))
                {
                  return VARIABLE;
                }else{
                    return FALSE;
                }

        }
        i++;
    }
    if (i == 0) /*-------THIS MEANS THAT THE TOKEN IS AT THE END OF THE PROGRAM AND IT IS EMPTY---*/
        {
          return FALSE;
        }
        p->currentWord = p->currentWord + 1; //move one word forward
        return NUMBER;
}


/*-----------------------------------------------------------------------------------------------*/
/*-----------------CHECKS THE GRAMMAR VARIABLE RETURNS TRUE IF IT WAS A VARIABLE-----------------*/
/*-----------------------------------------------------------------------------------------------*/
int VARChecker(Program *p)
{
    if (p->word[p->currentWord][1] == '\0')  /*checks if the string has only one char*/
    {
       if (p->word[p->currentWord][0] < 'A' || p->word[p->currentWord][0] > 'Z') //if the char is not capital
        {
            return FALSE;
        }else{
        p->currentWord = p->currentWord + 1;
        return TRUE;
        }
    }
return FALSE;

}


/*------------------------------------------------------------------------------------------------------------------*/
/*----CHECKS THE GRAMMAR COMMAND 'DO' AND ALL ITS CONTENT INCLUDING (VARIABLE FROM VARNUM TO VARNUM AND BRACKETS----*/
/*------------------------------------------------------------------------------------------------------------------*/
int DOChecker(Program *p, Graphics *graphics)
{
    if (stringChecker(p, "DO"))
        {
        if (VARChecker(p))
            {
                char varName[MAXVARNAME];  //hold the name of the variable
                int prevWord = p->currentWord - 1;
                strcpy (varName, p->word[prevWord]);

            if (stringChecker(p, "FROM"))
                {
                    float valueFrom;
                    if (VARNUMValue(p,&valueFrom)) //if there is a value in VARNUM
                    {
                        if (stringChecker(p, "TO"))
                        {
                        float valueTo;
                        if (VARNUMValue(p,&valueTo)) //if there is a value in VARNUM
                            {
                                int loopStart = p->currentWord;
                                while (valueFrom <= valueTo) //if it was set to != it might have infinite loops in case the variable was changed and became larger than valueTo
                                {
                                    //printf("from %f to %f\n", valueFrom, valueTo);
                                    setVar(p,varName,valueFrom); //the variable (varName) takes the value from valueFrom
                                    p->currentWord = loopStart; //branch to the start of the loop
                                    mainChecker(p, graphics);
                                    valueFrom = getVar(p,varName);  //copy the new value from the variable to the valueFrom in case it was changed in the loop
                                    valueFrom++; //just increment if the value of the variable was not changed in the loop

                                }
                                return TRUE;
                            }else{
                                printf("ERROR:\nTHERE IS NO VARIABLE OR A NUMBER AFTER (TO) IN THE DO COMMAND\n");
                                foundError(p);
                                  }

                        }else{
                            printf("ERROR:\nTHERE IS NO (TO) IN THE DO COMMAND\n");
                            foundError(p);
                              }

                    }else{
                        printf("ERROR:\nTHERE IS NO VARIABLE OR A NUMBER AFTER (FROM) IN THE DO COMMAND\n");
                        foundError(p);
                          }

                }else{
                    printf("ERROR:\nTHERE IS NO (FROM) IN THE DO COMMAND\n");
                    foundError(p);
                      }
            }else{
                printf("ERROR:\nTHERE IS NO VARIABLE AFTER DO COMMAND\n");
                foundError(p);
                  }
        }
return FALSE;
}


/*------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------CHECKS THE GRAMMAR COMMAND 'SET'-----------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
int SETChecker(Program *p)
{
    if (stringChecker(p, "SET"))
        {
        if (VARChecker(p))
            {
                char varName[MAXVARNAME];  //hold the name of the variable
                int prevWord = p->currentWord - 1;
                strcpy (varName, p->word[prevWord]);
                //printf("%s\n",varName);

           if (stringChecker(p, ":="))
                {
                    Stack stk;
                    InitialiseStack(&stk);
                if (POLISHChecker(p, &stk))
                    {
                    float value = Pop(&stk);
                    if (stk.top != 0)
                    {
                        printf("ERROR:\n WITH THE POLISH EQUATION\n");
                        foundError(p);
                    }
                    //printf("%f\n", value);
                    setVar(p, varName, value);
                    return TRUE;
                    }

                }
            }
        }
return FALSE;
}



/*------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------CHECKS THE GRAMMAR COMMAND 'POLISH'--------------------------------------*/
/*--------------------------------USED FOR WRITING EQUATION IN THE POLISH WAY '= 1 2 +'-----------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
int POLISHChecker(Program *p, Stack *stk)
{

if (stringChecker(p, ";"))
    {
        return TRUE;
    }

if (OPChecker(p, stk))
    {
    if (POLISHChecker(p, stk))
        {
        return TRUE;
        }
    }


float value;
if (VARNUMValue(p,&value)) //if there is a value in VARNUM
{
    Push(stk, value);

    if (POLISHChecker(p, stk))
    {
    return TRUE;
    }
}

return FALSE;  //if nothing happened in this function return FALSE
}



/*------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------CHECKS THE GRAMMAR OP FOR OPERATION FOR POLISH----------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
int OPChecker(Program *p, Stack *stk)
{
    if (stringChecker(p, "+"))
    {
        float firstValue = Pop(stk);
        float secondValue = Pop(stk);
        Push(stk, firstValue+secondValue);
        return TRUE;
    }
    if (stringChecker(p, "-"))
    {
        float firstValue = Pop(stk);
        float secondValue = Pop(stk);
        Push(stk, secondValue - firstValue);
        return TRUE;
    }
    if (stringChecker(p, "*"))
    {
        float firstValue = Pop(stk);
        float secondValue = Pop(stk);
        Push(stk, firstValue*secondValue);
        return TRUE;
    }
    if (stringChecker(p, "/"))
    {
        float firstValue = Pop(stk);
        float secondValue = Pop(stk);
        Push(stk, secondValue/firstValue);
        return TRUE;
    }
return FALSE;
}


/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------CHECKS THE GRAMMAR INSTRCTLIST ADN CALL THE INSTRUCTION AND IT SELF UNTIL IT FINDS THE END BRACKET '}'-------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
void instrcListChecker(Program *p, Graphics *graphics)
{
	if (strcmp(p->word[p->currentWord], "}") == SAME) // if the next word is END then return else check the word id it is statement
		{
		    p->currentWord = p->currentWord + 1;
			return;
		}
	instrctionChecker(p, graphics);
	instrcListChecker(p, graphics);
}


/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------CHECKS THE BRACKET FOR BEGINNING OF THE CODE OR BEGINNING OF LOOPS AND IF---------------------------------*/
/*---------------------------------------------------EXCUTES THE CONTENT OF THE BRACKETS--------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
void mainChecker(Program *p, Graphics *graphics)
{
	if (strcmp(p->word[p->currentWord], "{") != SAME) //not the same with the first word (wordNumber == 0 in this case)
		{
			printf("\nERROR: THERE IS NO BEGINNING SIGN ({)\n");
			foundError(p);
			exit(1);
		}
	//if the first word is BEGIN then move to the next word by incrementing the currentWord
	p->currentWord = p->currentWord + 1;
	instrcListChecker (p, graphics);
}



/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------CHECKS ONLY ONE STRING IN THE TOKEN, IF THE COMPARISON WAS SUCCESSFUL MOVE TO THE NEXT WORD AND RETURN TRUE-------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
int stringChecker (Program *p, char* token)
{
    if (strcmp(p->word[p->currentWord], token) == SAME)
    {
        p->currentWord = p->currentWord + 1; //move to the next word
        return TRUE;
    }
return FALSE;
}



/*----------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------RETURNS THE VALUE OF A VARIABLE OR A NUMBER IN THE TOKEN-------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/
int VARNUMValue(Program *p, float* result)
{
  int VARNUMResult = VARNUMChecker(p); //holds the value from VARNUMChecker
    int prevWord = p->currentWord -1;
    if (VARNUMResult == VARIABLE)
    {
        float value = getVar(p, p->word[prevWord]);
        *result = value;
        return TRUE;
    }else if (VARNUMResult == NUMBER)
    {
        float value =  atof (p->word[prevWord]);
        *result = value;
        return TRUE;
    }
return FALSE;
}


/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------PRINTS AND ERROR MESSAGE AND PRINTS THE PROGRAM UNTIL THE ERROR-----------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
void foundError(Program *p)
{
        printf("PROGRAM STOPPED AT THE TOKEN \"%s\" WITH THE NUMBER %d \n",p->word[p->currentWord], p->currentWord);
        int i = 0;
        printf("**********************************************************\n");
        while (i <= p->currentWord)
            {

              printf("%s  ", p->word[i]);
              i++;
            }
        printf("\n**********************************************************\n");
printf("PRESS ENTER TO EXIT\n");
getchar();
        exit(1);
}
