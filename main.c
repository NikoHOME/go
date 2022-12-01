#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>



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
	refresh();
	wrefresh(window);
	keypad(window,true);


	char *menuarr[] = {"7x7", "9x9", "13x13","Custom"};

	char customarr[4] = {'0','0','0','\0'};
	int customsiz=0;

	int key,i,j;
	int pos=0;

	for(i=0;i<4;++i)
	{
		if(i==pos) wattron(window, A_STANDOUT);
		mvwprintw(window, i+1, 1, menuarr[i]);
		wattroff(window, A_STANDOUT);
	}

	while((key = wgetch(window)) != 'q')
	{	
		switch(key)
		{
			case KEY_UP:
				if(pos>0) --pos;
				customsiz=0;
				mvwprintw(window, 0, 0, "1");
				break;
			case KEY_DOWN:
				if(pos<3) ++pos;
				customsiz=0;
				mvwprintw(window, 0, 0, "2");
				break;
			default:
				mvwprintw(window, 0, 0, customarr);
				if(key>='0' && key<='9' && pos==3 && customsiz<3)
				{
					if(customsiz==0)
					{
						if(key!='0')
						{
							customarr[customsiz]=key;
							++customsiz;
							customarr[customsiz]='\0';
						}
					}
					else
					{
						customarr[customsiz]=key;
						++customsiz;
						customarr[customsiz]='\0';
					}
				}
				break;
		}

		for(i=0;i<3;++i)
		{
			if(i==pos) wattron(window, A_STANDOUT);
			mvwprintw(window, i+1, 1, menuarr[i]);
			wattroff(window, A_STANDOUT);
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

