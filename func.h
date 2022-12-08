#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define VAL_FUNC_IMPORT int i, int j, int size, char color, char *logic, char *board
#define VAL_FUNC_EXPORT i, j, size, color, logic, board
#define WINDOW_CENTER 

#define BOARD_BASE_PADDING (3)
#define BOARD_X_PADDING (2)

#define CUSTOM_ARR_SIZE (4)

#define START_MENU_HEIGHT (6)
#define START_MENU_WIDTH (20)

#define BOARD_MENU_HEIGHT (20)
#define BOARD_MENU_WIDTH (30)

#define BLANK8 "        "
#define BLANK16 "                "




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
