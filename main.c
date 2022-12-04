#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define CUSTOM_ARR_SIZE 4

void printMenu(WINDOW *win, int i, int pos, char *def, char *out);
void printBoard(WINDOW *win, char *board, int size);
int main()
{	


	/* Initialize curses */
	initscr();
	//start_color();
    cbreak();
    noecho();
	curs_set(0);
	//keypad(stdscr, TRUE);
	//init_pair(1, COLOR_RED, COLOR_BLACK);

	WINDOW *window;

	int height = 6;
	int width = 20;
	int starty = (LINES - height) / 2;	/* Calculating for a center placement */
	int startx = (COLS - width) / 2;	/* of the window		*/

	int maxy,maxx;
	getmaxyx(stdscr,maxy,maxx);

    window= newwin(height,width,(maxy-height)/2,(maxx-width)/2);
	box(window, 0, 0);
	keypad(window,true);


	char *menuarr[] = {"9x9", "13x13", "17x17","Custom - keyboard"};
	char emptyarr[18];
	memset(emptyarr,' ',18);
	char customarr[CUSTOM_ARR_SIZE] = {'0','0','0','\0'};
	int customsiz=0;

	int key,keyg,i,j;
	int pos=0;

	mvprintw((maxy-height)/2-3, (maxx-width)/2, "GO");

	mvprintw((maxy-height)/2+7, (maxx-width)/2, "q - exit");
	mvprintw((maxy-height)/2+6, (maxx-width)/2, "Enter - start");

	mvprintw((maxy-height)/2-1, (maxx-width)/2, "Pick the board size");

	refresh();
	wrefresh(window);

	for(i=0;i<4;++i)
	{
		printMenu(window, i, pos, emptyarr, menuarr[i]);
	}

	while((key = wgetch(window)) != 'q')
	{	
		switch(key)
		{
			case KEY_UP:
				if(pos>0) --pos;
				customsiz=0;
				break;
			case KEY_DOWN:
				if(pos<3) ++pos;
				customsiz=0;
				break;
			case KEY_BACKSPACE:
				if(customsiz>0)
				{
					customsiz--;
					customarr[customsiz]='\0';
				}
				break;
			case '1' ... '9':
				if(pos==3 && customsiz<3)
				{
					customarr[customsiz]=key;
					++customsiz;
					customarr[customsiz]='\0';
				}
				break;
			case '0':
				if(customsiz!=0 && pos==3 && customsiz<3)
				{
					customarr[customsiz]=key;
					++customsiz;
					customarr[customsiz]='\0';
				}
				break;
			case 10:
				if(pos==3 && customsiz<1) break;
				werase(window);
				erase();
				WINDOW *table;
				int keyt=0;;
				refresh();
				switch(pos){
					case 0:
						height=9;
						break;
					case 1:
						height=13;
						break;
					case 2:
						height=17;
						break;
					case 3:
						char *conv;
						height=0;
						for(conv = customarr; *conv; ++conv)
						{
							height*=10;
							height+=(*conv-'0');
						}
						break;
				}
				int heights = height * 2 + 3;
				table = newwin(heights, heights,(maxy-heights)/2, (maxx-heights)/3*2);
				keypad(table, true);
				char *board = (char*)malloc(height*height*sizeof(char));
				memset(board,' ',height*height);
				printBoard(table,board,height);
				box(table, 0, 0);
				wrefresh(table);
				refresh();

				int posx=1,posy=1,pposx=1,pposy=1;

				//wmove(table, posx*2, posy*2);
				//waddch(table,'X');
				mvwchgat(table,posx*2,posy*2,1,A_REVERSE,0,NULL);
				wrefresh(table);

				while((keyt = wgetch(table)) != 'q')
				{
					switch(keyt)
					{
						case KEY_UP:
							if(posx>1)
							{
								--posx;
							}
							break;
						case KEY_LEFT:
							if(posy>1)
							{
								--posy;
							}
							break;
						case KEY_DOWN:
							if(posx<height)
							{
								++posx;
							}
							break;
						case KEY_RIGHT:
							if(posy<height)
							{
								++posy;
							}
							break;
					}
					//wattroff(table, A_BOLD);
					mvwchgat(table,pposx*2,pposy*2,1,A_NORMAL,0,NULL);
					pposx=posx;
					pposy=posy;
					mvwchgat(table,posx*2,posy*2,1,A_REVERSE,0,NULL);
					wmove(table, posx*2, posy*2);
					wrefresh(table);
					refresh();
				}

				break;
		}

		for(i=0;i<3;++i)
		{
			printMenu(window, i, pos, emptyarr, menuarr[i]);
		}

		if(pos==3) wattron(window, A_STANDOUT);
		
		if(customsiz!=0)
		{
			if(customsiz==3 && (((customarr[0]-48)*100)+((customarr[1]-48)*10)+customarr[2]-48>256))
			{
				customarr[0]='2';	customarr[1]='5';	customarr[2]='6';
			}
			char prefix[8];
			snprintf(prefix, sizeof(prefix), "%sx%s", customarr, customarr);
			mvwprintw(window, 4, 1, prefix);
		}	
		else 
			mvwprintw(window, 4, 1, menuarr[3]);
		wattroff(window, A_STANDOUT);

		refresh();
		
	}

	endwin();
}


void printMenu(WINDOW *win, int i, int pos, char *def, char *out)
{
	mvwprintw(win, 4, 1, def);
	if(i==pos) wattron(win, A_STANDOUT);
	mvwprintw(win, i+1, 1, out);
	wattroff(win, A_STANDOUT);
}

void printBoard(WINDOW *win, char *board, int size)
{	
	int i,j;
	char *baseRow = {"  "};
	char *baseRowEnd = {" "};
	char *baseRowS = {"|"};
	for(i=1;i<=size*2+1;i+=2)
	{
		for(j=1;j<=size*2+1;j+=2)
		{
			mvwprintw(win, i, j, baseRow);	
		}
		mvwprintw(win, i, size*2+1, baseRowEnd);
		for(j=1;j<=size*2+1;j+=2)
		{
			mvwprintw(win, i+1, j, baseRowS);
			mvwprintw(win, i+1, j+1, &board[size*(((i-1)/2))+(j-1)/2]);	
		}
		mvwprintw(win, i+1, size*2+1, baseRowS);
	}
}
