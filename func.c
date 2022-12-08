#include "func.h"

void printStart(WINDOW *win, int i, int pos, char *def,char *out)
{
	mvwprintw(win, 4, 1, def);
	if(i==pos) wattron(win, A_STANDOUT);
	mvwprintw(win, i+1, 1, out);
	wattroff(win, A_STANDOUT);
}

void printStartM(int offset,const char tab[], int maxy, int maxx)
{
    mvprintw((maxy-START_MENU_HEIGHT)/2+offset, (maxx-START_MENU_WIDTH)/2, tab);

}

void printBoard(WINDOW *win, char *board, int size)
{	
	int i,j;
	const char *baseRowCen = " ";
	const char *baseRowCor = " ";
	const char *baseRowEnd = " ";
	const char *baseRowS = "|";
	for(i=1;i<=size*2-1;i+=2)
	{
		for(j=1;j<=size*2-1;j+=2)
		{
			mvwprintw(win, i, j+1, baseRowCen);
			mvwprintw(win, i, j+2, baseRowCor);	
		}
		//mvwprintw(win, i, 1, baseRowEnd);
		for(j=1;j<=size*2-1;j+=2)
		{
			mvwprintw(win, i+1, j+1, baseRowS);
			mvwprintw(win, i+1, j+2, &board[size*(((i-1)/2))+(j-1)/2]);	
		}
		mvwprintw(win, i+1, size*2+2, baseRowS);
	}
	box(win, 0, 0);
}


void printBoardM(WINDOW *win, int offsety, int offsetx,const char tab[])
{
    mvwprintw(win,offsety,offsetx,tab);

}


WINDOW* createStartMenu(int maxy,int maxx)
{
    WINDOW *window = newwin(START_MENU_HEIGHT,START_MENU_WIDTH,(maxy-START_MENU_HEIGHT)/2,(maxx-START_MENU_WIDTH)/2);
    box(window, 0, 0);
    keypad(window,true);
    return window;
}

int customarrToInt(char tab[3])
{
	return ((tab[0]-'0')*100)+((tab[1]-'0')*10)+tab[2]-'0';
}

int arrLoc(int i,int j,int offset)
{
	return (i-1)*offset+j-1;
}

int isValid(VAL_FUNC_IMPORT, char kill)
{
	memset(logic,0,size*size);
	int location=arrLoc(i,j,size);
	int killOut=0;

	int out = dfs(VAL_FUNC_EXPORT, kill, &killOut);
	if(kill) 
	{
		int basei=i,basej=j;
		j=basej+1;
		dfs(VAL_FUNC_EXPORT, kill,&killOut);
		j=basej-1;
		dfs(VAL_FUNC_EXPORT, kill,&killOut);
		j=basej;
		i=basei+1;
		dfs(VAL_FUNC_EXPORT, kill,&killOut);
		i=basei-1;
		dfs(VAL_FUNC_EXPORT, kill,&killOut);
		return killOut;
	}
	else return dfs(VAL_FUNC_EXPORT, kill,&killOut);
}

int dfs(VAL_FUNC_IMPORT,char kill, int *killOut)
{
	int location=arrLoc(i,j,size);
	logic[location]=1;
	int basei=i,basej=j;
	if(basej<size)
	{
		j=basej+1;
		if (dfsCond(VAL_FUNC_EXPORT,kill,killOut))
			return 1;
		
	}
	if(basej>1)
	{
		j=basej-1; 
		if (dfsCond(VAL_FUNC_EXPORT,kill,killOut))
			return 1;
		
	}
	j=basej;
	if(basei<size)
	{
		i=basei+1;	
		if (dfsCond(VAL_FUNC_EXPORT,kill,killOut))
			return 1;
	
	}
	if(basei>1)
	{
		i=basei-1;	
		if (dfsCond(VAL_FUNC_EXPORT,kill,killOut))
			return 1;
		
	}
	if(kill)
		return *killOut;
	else
		return 0;
}

char dfsCond(VAL_FUNC_IMPORT, char kill, int *killOut)
{
	
	int location=arrLoc(i,j,size);
	
	if(!logic[location]) 
	{
		if(board[location]==color)
		{
			if(kill)
			{
				board[location]=' ';
				++*killOut;
			}
			return dfs(VAL_FUNC_EXPORT,kill,killOut);
		}
		else if(board[location]==' ')
		{
			return 1;
		}
	}
	return 0;
}
