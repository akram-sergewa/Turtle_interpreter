#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <SDL.h>
#include <assert.h>


#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 20
#define MAXVARNAME 10
#define MAXVAR 30
#define MAXTHICKNESS 100
#define MAXFILENAME 200

#define SAME 0
#define FALSE 0
#define TRUE 1
#define VARIABLE 2
#define NUMBER 3
#define FIRSTANGLE 90
#define FIRSTX 700
#define FIRSTY 460
#define TWOPI 6.283185307
#define ONESIGN 1
#define TWOSIGNS 2
#define COLOR 255


#define SCREENWIDTH 1400
#define SCREENHEIGHT 920
#define STACKSIZE 50


//<MAIN> ::= "{" <INSTRCTLST>
//<INSTRCTLST> ::= <INSTRUCTION><INSTRCTLST> |
//"}"
//<INSTRUCTION> ::= <FD> |
//<LT> |
//<RT> |
//<DO> |
//<SET> 

//<FD> ::= "FD" <VARNUM>
//<LT> ::= "LT" <VARNUM>
//<RT> ::= "RT" <VARNUM>

//<DO> ::= "DO" <VAR> "FROM" <VARNUM> "TO"
//<VARNUM> "{" <INSTRCTLST>
//<VAR> ::= [A-Z]
//<VARNUM> ::= number |
//<VAR>
//<SET> ::= "SET" <VAR> ":=" <POLISH>
//<POLISH> ::= <OP> <POLISH> |
//<VARNUM> <POLISH> |
//";"
//<OP> ::= "+" |
//"-" |
//"*" |
//"/"


struct varnode{
	float var;
	char varName[MAXVAR];
	struct varnode* next;
};
typedef struct varnode Varnode;


struct prog{
char word[MAXNUMTOKENS][MAXTOKENSIZE];
int currentWord;
struct varnode* varFirst;
float angle;
float posY;
float posX;
};
typedef struct prog Program;




struct graph{
	SDL_Window *window;
	SDL_Surface *screen;
	SDL_Renderer *renderer;
	SDL_Surface* Loading_Surf;
	SDL_Texture* texture;
	SDL_Event event;
};
typedef struct graph Graphics;


struct thestack{
    float valueStk[STACKSIZE];
    int top;
};
typedef struct thestack Stack;






/*-------------------------*/
void SDLFunction (Graphics *graphics);
void graphicsInit(Graphics *graphics);
void draw(Graphics *graphics);
void SDLEvents (Graphics *graphics);

void checkCommand(int argc, char *argv[]);


int stringChecker (Program *p, char* token);
void instrcListChecker(Program *p, Graphics *graphics);
void instrctionChecker(Program *p, Graphics *graphics);
int FDChecker(Program *p, Graphics *graphics);
int LTChecker(Program *p);
int DOChecker(Program *p, Graphics *graphics);
int OPChecker(Program *p, Stack *stk);
int POLISHChecker(Program *p, Stack *stk);
int RTChecker(Program *p);
int VARChecker(Program *p);
int SETChecker(Program *p);


int VARNUMChecker(Program *p);
void mainChecker(Program *p, Graphics *graphics);

/*------------------------------*/
float getVar (Program *p, char *varName);
int setVar (Program *p, char *varName, float var);
void progInitializer (Program *p);
void FDMaker (Program *p, Graphics *graphics, float value);
void LTMaker(Program *p, float angle);
void RTMaker(Program *p, float angle);
int VARNUMValue(Program *p, float* result);
void checkToken (Program *p, int *pcw);
void checkTokenSize (Program *p, int *pcw);

int commentRemover(Program *p, int *pcw);

void signStrFix(Program *p, int *pcw, int *ltCount, int nSign);
void signMidFix(Program *p, int *pcw, int *ltCount, int nSign);
void foundError(Program *p);


/*--------------------------*/
void InitialiseStack(Stack *stk);
void Push(Stack *stk, float currenValue);
float Pop(Stack *stk);
