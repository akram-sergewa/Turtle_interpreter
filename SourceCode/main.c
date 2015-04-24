#include "turtle_header.h"
/*-----------main.c--------*/
/*THE TURTLE PROGRAMMING LANGUAGE
WRITTEN BY : AKRAM SERGEWA
*/


/*------------------------------------------------------*/
/*--------------THE START OF THE PROGRAM----------------*/
/*------------------------------------------------------*/
int main(int argc, char* argv[])
{

checkCommand(argc, argv);
char fileName[MAXFILENAME];
strcpy(fileName, argv[1]);


/*-----------------initialize the structure prog and file------------*/
Program prog;
int i;
FILE *fp;

progInitializer(&prog);




/*------------------------------------------------------*/
/*--------------CHECK IF THE FILE EXIST----------------*/
/*------------------------------------------------------*/
if(!(fp = fopen(fileName, "r")))
{
	fprintf(stderr, "Cannot open %s\n",fileName);
	exit(2);
}



/*------COPY THE TOKENS FROM THE FILE AND CHECK THE SIZE
OF THE TOKEN AND CHECK THE FOR THE SIGN ANS SYMBOLS AND
SEPERATE  INTO SEPERATE TOKENS----------------*/
/*------------------------------------------------------*/

i=0;
while(fscanf(fp, "%s", prog.word[i])==1 && i<MAXNUMTOKENS){
    checkTokenSize(&prog, &i);
    checkToken (&prog, &i);
	i++;
}

//for (int j = 0 ; j<i;j++)
//{
//   printf("%d   %s \n", j, prog.word[j]);
//}


/*-----START THE GRAPHICS STRUCTURE AND SDL------*/
Graphics graphics;
SDLFunction(&graphics);


/*--------------START THE INTEPRETATION----------------*/
/*------------------------------------------------------*/
mainChecker(&prog, &graphics);

/*-------IF NO ERRORS WERE FOUND PRINT PARSED OK------*/
/*------------------------------------------------------*/




/*------------DISPLAY THE FINAL DRAWING UNTIL THE USER DECIDES
TO TURN IT OF BY PRESSING ENTER ------------------------------*/
/*------------------------------------------------------*/
printf("Parsed OK\n");
printf("PRESS ENTER TO EXIT\n");
getchar();




return 0;
}
