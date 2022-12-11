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

#define SAVE_FUNC_IMPORT \
    char *board, int height,\
    int posx, int posy, int pposx, int pposy,\
    char blackTurn, char stonePlaced, int location,\
    int lastX, int lastY, int lastLoc, int killCount,\
    float blackScore, float whiteScore,\
    int killCond, int killCondLast        

#define SAVE_FUNC_EXPORT \
    height,\
    posx, posy, pposx, pposy,\
    blackTurn, stonePlaced, location,\
    lastX, lastY, lastLoc, killCount,\
    blackScore, whiteScore,\
    killCond, killCondLast  

#define SAVE_FUNC_STRUCT \
    int height;\
    int posx; int posy; int pposx; int pposy;\
    char blackTurn; char stonePlaced; int location;\
    int lastX; int lastY; int lastLoc; int killCount;\
    float blackScore; float whiteScore;\
    int killCond; int killCondLast;   

#define LOAD_FUNC_IMPORT \
    char *board, int *height,\
    int *posx, int *posy, int *pposx, int *pposy,\
    char *blackTurn, char *stonePlaced, int *location,\
    int *lastX, int *lastY, int *lastLoc, int *killCount,\
    float *blackScore, float *whiteScore,\
    int *killCond, int *killCondLast    

#define LOAD_FUNC_EXPORT\
    board,&height,\
    &posx, &posy, &pposx, &pposy,\
    &blackTurn, &stonePlaced, &location,\
    &lastX, &lastY, &lastLoc, &killCount,\
    &blackScore, &whiteScore,\
    &killCond, &killCondLast  


#define BOARD_BASE_PADDING (3)  //Game Board X and Y padding
#define BOARD_X_PADDING (2)     //Game Board additional X padding
#define CUSTOM_ARR_SIZE (4)     //Array size for custom board size
#define SCORE_ARR_SIZE (14)

#define MAX_BOARD_SIZE_Y (LINES/2-1)
#define MAX_BOARD_SIZE_X (COLS/4)
#define MIN_BOARD_SIZE (2)
#define MAX_FILENAME_SIZE (32)

#define START_MENU_HEIGHT (6)   //Menu sizes
#define START_MENU_WIDTH (20)

#define BOARD_MENU_HEIGHT (20)  
#define BOARD_MENU_WIDTH (30)

#define HANDI_MENU_HEIGHT (6)   
#define HANDI_MENU_WIDTH  (20)

#define SAVE_MENU_HEIGHT (6)
#define SAVE_MENU_WIDTH  (40)

#define BLANK8 "        "           //Strings for board clearning
#define BLANK16 "                "
#define BLANK18 "                  \0"
#define BLANK24 "                        "
#define BLANK28 "                            "
#define BLANK32 "                                "
#define BLANK1  " "

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
        dfsCond(VAL_FUNC_EXPORT,kill,&killOut);;\
    }\
}

//Function used in move validation
#define DFS_COND_FUNC {\
    if (dfsCond(VAL_FUNC_EXPORT,kill,killOut))\
        return 1;\
}

#define BOARD_MOVE_SWITCH {\
    case KEY_UP:\
        if(posx>1)\
        {\
            --posx;\
        }\
        break;\
    case KEY_LEFT:\
        if(posy>1)\
        {\
            --posy;\
        }\
        break;\
    case KEY_DOWN:\
        if(posx<height)\
        {\
            ++posx;\
        }\
        break;\
    case KEY_RIGHT:\
        if(posy<height)\
        {\
            ++posy;\
        }\
        break;\
}

#define BOARD_MOVE_CURS {\
    mvwchgat(table,pposx*2,pposy*2+1,1,A_NORMAL,0,NULL);\
	pposx=posx;\
	pposy=posy;\
	mvwchgat(table,posx*2,posy*2+1,1,A_REVERSE,0,NULL);\
	wmove(table, posx*2, posy*2+1);\
}

#define REDRAW {\
    heights = height * 2 + BOARD_BASE_PADDING;\
    logic = realloc(logic,(height)*(height)*sizeof(char));\
    wresize(table,heights,heights+BOARD_X_PADDING);\
    mvwin(table,(maxy-heights)/2,(maxx/2));\
    printBoard(table,board,height);\
    box(table, 0, 0);\
	box(side, 0, 0);\
    mvwchgat(table,posx*2,posy*2+1,1,A_REVERSE,0,NULL);\
\
    printBoardM(side,1,1,"Wiktor Gawronski 193285");\
	printBoardM(side,2,1,"q - quit");\
	printBoardM(side,3,1,"n - restart");\
	printBoardM(side,5,1,"Arrows - movement");\
	printBoardM(side,6,1,"Enter - end turn");\
	printBoardM(side,7,1,"Esc - undo");\
	printBoardM(side,8,1,"i - place a stone");\
\
    if(blackTurn)\
        printBoardM(side,16,1,"Black Turn");\
    else\
        printBoardM(side,16,1,"White Turn");\
\
    snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);\
	mvwprintw(side,18,1,positionOut);\
\
    snprintf(whiteSrcOut, sizeof(whiteSrcOut), "%s %.1f",whiteSrcBase , whiteScore);\
	snprintf(blackSrcOut, sizeof(blackSrcOut), "%s %.1f",blackSrcBase , blackScore);\
	mvwprintw(side,13,1,blackSrcOut);\
	mvwprintw(side,14,1,whiteSrcOut);\
\
	wrefresh(table);\
	wrefresh(side);\
	refresh();\
}



void printStart(WINDOW *win, int i, int pos, const char def[],char *out);
void printStartM(int offset,const char tab[], int maxy, int maxx);

void printBoard(WINDOW *win, char *board, int size);
void printBoardM(WINDOW *win, int offsety, int offsetx,const char tab[]);

WINDOW* createMenu(int maxy,int maxx,int height,int width);
int customarrToInt(char tab[CUSTOM_ARR_SIZE],int customsiz);
int customarrToMax(char *tab,int maxsize);

int arrLoc(int i,int j,int offset);

int isValid(VAL_FUNC_IMPORT, char kill );
int dfs    (VAL_FUNC_IMPORT, char kill, int *killOut);
char dfsCond(VAL_FUNC_IMPORT, char kill, int *killOut);

char runtime(int height, int maxy,int maxx,char isHandi);
char handi(int maxy,int maxx);
char isKo(int killCond,int killCondLast,int posx,int posy,int lastX, int lastY);

char save(int maxy,int maxx,SAVE_FUNC_IMPORT);
char *load(int maxy,int maxx,LOAD_FUNC_IMPORT);

struct data
{
    SAVE_FUNC_STRUCT
};