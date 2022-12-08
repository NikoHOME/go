#include "func.h"


int main()
{	


	/* Initialize curses */
	initscr();
    cbreak();
    noecho();
	curs_set(0);

	ESCDELAY = 25;

	WINDOW *window;

	int starty = (LINES - START_MENU_HEIGHT) / 2;	/* Calculating for a center placement */
	int startx = (COLS - START_MENU_WIDTH) / 2;	/* of the window		*/

	int maxy,maxx;
	getmaxyx(stdscr,maxy,maxx);

    window = createStartMenu(maxy,maxx);


	char *menuarr[] = {"9x9", "13x13", "17x17","Custom - keyboard"};
	char emptyarr[18];
	memset(emptyarr,' ',18);
	char customarr[CUSTOM_ARR_SIZE] = {'0','0','0','\0'};
	int customsiz=0;

	int key,keyg,i,j;
	int pos=0;

	printStartM(-3,"GO",maxy,maxx);
	printStartM(-1,"Pick the board size",maxy,maxx);
	printStartM(7,"q - exit",maxy,maxx);
	printStartM(6,"Enter - start",maxy,maxx);

	refresh();
	wrefresh(window);

	for(i=0;i<4;++i)
	{
		printStart(window, i, pos, emptyarr, menuarr[i]);
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
			case '\n':
				if(pos==3 && customsiz<1) break;
				
				werase(window);
				erase();
				refresh();

				int height;
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
				runtime(height,maxy,maxx);
				erase();
				refresh();
				return 0;

				break;
		}

		for(i=0;i<3;++i)
		{
			printStart(window, i, pos, emptyarr, menuarr[i]);
		}

		if(pos==3) wattron(window, A_STANDOUT);
		
		if(customsiz!=0)
		{
			if(customsiz==3 && customarrToInt(customarr)>256)
			{
				customarr[0]='2';	
				customarr[1]='5';	
				customarr[2]='6';
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



