#include "func.h"

void printStart(WINDOW *win, int i, int pos, const char def[],char *out)
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
	static const char *baseRowCen = BASE_ROW_CEN;
	static const char *baseRowCor = BASE_ROW_COR;
	static const char *baseRowEnd = BASE_ROW_END;
	static const char *baseRowSep = BASE_ROW_SEP;
	//FILE *ptr = fopen("debug.txt","a");
	mvwprintw(win, 1, 1, BLANK32);
	for(i=1;i<=size*2-1;i+=2)
	{
		mvwprintw(win, i, 1, BLANK32);
		mvwprintw(win, i+1, 1, BLANK32);
		for(j=1;j<=size*2-1;j+=2)
		{
			mvwprintw(win, i, j+1, baseRowCen);
			mvwprintw(win, i, j+2, baseRowCor);	
		}
		for(j=1;j<=size*2-1;j+=2)
		{
			mvwprintw(win, i+1, j+1, baseRowSep);
			mvwprintw(win, i+1, j+2, &board[size*(((i-1)/2))+(j-1)/2]);	
			//fprintf(ptr,"%c %d",board[size*(((i-1)/2))+(j-1)/2],size*(((i-1)/2))+(j-1)/2);
		}
		wrefresh(win);
		refresh();
		mvwprintw(win, i+1, size*2+2, baseRowSep);
		mvwprintw(win, i+1, size*2+3, BLANK1);
		//fprintf(ptr,"\n");
	}
	mvwprintw(win, size*2+1, 1, BLANK32);
	box(win, 0, 0);
	//fprintf(ptr,"\n");
	//fclose(ptr);
}


void printBoardM(WINDOW *win, int offsety, int offsetx,const char tab[])
{
    mvwprintw(win,offsety,offsetx,tab);

}


WINDOW* createMenu(int maxy,int maxx,int height,int width)
{
    WINDOW *window = newwin(height,width,(maxy-height)/2,(maxx-width)/2);
    box(window, 0, 0);
    keypad(window,true);
    return window;
}

int customarrToInt(char tab[CUSTOM_ARR_SIZE],int customsiz)
{
	if(customsiz==3) return ((tab[0]-'0')*100)+((tab[1]-'0')*10)+tab[2]-'0';
	if(customsiz==2) return ((tab[0]-'0')*10)+tab[1]-'0';
	return tab[0]-'0';
}
int customarrToMax(char *tab,int maxsize)
{
	int val=maxsize,i,div=1,out=1;
	for(div=1;div<=maxsize/10;div*=10,++out);
	for(i=0; div>0; ++i)
	{
		tab[i]=(val/div)+'0';
		val%=div;
		div/=10;
	}
	tab[i]='\0';
	return out;
}
int arrLoc(int i,int j,int offset)
{
	return (i-1)*offset+j-1;
}

char isKo(int killCond,int killCondLast,int posx,int posy,int lastX, int lastY)
{
	if(killCond!=killCondLast) return 0;
	if(killCond!=1) return 0;
	if(abs(posx-lastX)==1 && posy==lastY) return 1;
	if(abs(posy-lastY)==1 && posx==lastX) return 1;
	return 0;
}


int isValid(VAL_FUNC_IMPORT, char kill)
{
	memset(logic,0,size*size);
	int location=arrLoc(i,j,size);
	int killOut=0;
	if(kill) 
	{
		int basei=i,basej=j;
		
		if(basej<size)
		{
			j=basej+1;
			KILL_DFS_FUNC
		}
		if(basej>1)
		{
			j=basej-1; 
			KILL_DFS_FUNC
		}
		j=basej;
		if(basei<size)
		{
			i=basei+1;	
			KILL_DFS_FUNC
		}
		if(basei>1)
		{
			i=basei-1;	
			KILL_DFS_FUNC
		}	
		return killOut;	
	}
	else 
		return dfs(VAL_FUNC_EXPORT, kill,&killOut);
}

int dfs(VAL_FUNC_IMPORT,char kill, int *killOut)
{
	int location=arrLoc(i,j,size);
	int basei=i,basej=j;
	logic[location]=1;
	if(kill)
		dfsCond(VAL_FUNC_EXPORT,kill,killOut);
	if(basej<size)
	{
		j=basej+1;
		DFS_COND_FUNC
	}
	if(basej>1)
	{
		j=basej-1; 
		DFS_COND_FUNC
	}
	j=basej;
	if(basei<size)
	{
		i=basei+1;	
		DFS_COND_FUNC
	}
	if(basei>1)
	{
		i=basei-1;	
		DFS_COND_FUNC
	}
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
				if(kill==1)
				{
					board[location]=EMPTY_CHAR;
					wmove(win, i*2, j*2+1);
					waddch(win,EMPTY_CHAR);
					wrefresh(win);
				}
				++*killOut;
			}
			return dfs(VAL_FUNC_EXPORT,kill,killOut);
		}
		else if(board[location]==EMPTY_CHAR)
		{
			return 1;
		}
	}
	return 0;
}
