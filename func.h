#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/*
    Variables for funtions validating stone placement
    and capture
*/
#define VAL_FUNC_IMPORT WINDOW *win, int i, int j, int size, char color, char *logic, char *board
#define VAL_FUNC_EXPORT win, i, j, size, color, logic, board



#define BOARD_BASE_PADDING (3)  //Game Board X and Y padding
#define BOARD_X_PADDING (2)     //Game Board additional X padding

#define CUSTOM_ARR_SIZE (4)     //Array size for custom board size

#define START_MENU_HEIGHT (6)   //Start Menu size
#define START_MENU_WIDTH (20)

#define BOARD_MENU_HEIGHT (20)  //Game Board size
#define BOARD_MENU_WIDTH (30)

#define BLANK8 "        "           //Strings for board clearning
#define BLANK16 "                "

#define BLACK_CHAR '@'  //Chars used on the board
#define WHITE_CHAR '#'
#define EMPTY_CHAR ' '

#define QUIT_KEY 'q'    //Control Keys
#define PUT_KEY 'i'

#define BASE_ROW_CEN " "    //Board Decorations
#define BASE_ROW_COR " "
#define BASE_ROW_END " "
#define BASE_ROW_SEP "|"

//Funtion used in stone capturing
#define KILL_DFS_FUNC {\
    memset(logic,0,size*size);\
    if(!isValid(VAL_FUNC_EXPORT,0))\
    {\
        memset(logic,0,size*size);\
        dfs(VAL_FUNC_EXPORT, kill,&killOut);\
    }\
}

//Function used in move validation
#define DFS_COND_FUNC {\
    if (dfsCond(VAL_FUNC_EXPORT,kill,killOut))\
        return 1;\
}



void printStart(WINDOW *win, int i, int pos, char *def, char *out);
void printStartM(int offset,const char tab[], int maxy, int maxx);

void printBoard(WINDOW *win, char *board, int size);
void printBoardM(WINDOW *win, int offsety, int offsetx,const char tab[]);

WINDOW* createStartMenu(int maxy,int maxx);
int customarrToInt(char tab[3]);

int arrLoc(int i,int j,int offset);

int isValid(VAL_FUNC_IMPORT, char kill );
int dfs    (VAL_FUNC_IMPORT, char kill, int *killOut);
char dfsCond(VAL_FUNC_IMPORT, char kill, int *killOut);

void runtime(int height, int maxy,int maxx);
