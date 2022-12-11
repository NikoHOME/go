#include "func.h"


int main()
{	
	//Start ncurses and initialize modes 
	initscr();
    cbreak();
    noecho();
	curs_set(0);
	//Turn off ESC dealy
	ESCDELAY = 25;

	WINDOW *window;
	//maxBoardsize used in custom board size
	int maxy,maxx,maxBoardSize;
	getmaxyx(stdscr,maxy,maxx);

	if(MAX_BOARD_SIZE_X>MAX_BOARD_SIZE_Y)
		maxBoardSize=MAX_BOARD_SIZE_Y;
	else
		maxBoardSize=MAX_BOARD_SIZE_X;
    
	window = createMenu(maxy,maxx,START_MENU_HEIGHT,START_MENU_WIDTH);
	wrefresh(window);
	refresh();

	char *menuarr[] = {"9x9", "13x13", "19x19","Custom - keyboard"};
	char customarr[CUSTOM_ARR_SIZE] = {'0','0','0','\0'};
	//customsiz - size of custom board size array 
	//arrtoint - custom board size array to intiger
	int customsiz=0,arrtoint;

	int key,i,j;
	int pos=0;
	//Print outside the main window
	printStartM(-3,"GO",maxy,maxx);
	printStartM(-1,"Pick the board size",maxy,maxx);
	printStartM(7,"q - exit",maxy,maxx);
	printStartM(6,"Enter - start",maxy,maxx);
	//Print menu options
	for(i=0;i<4;++i)
	{
		printStart(window, i, pos, BLANK18, menuarr[i]);
	}
	box(window,0,0);
	wrefresh(window);
	refresh();
	while((key = wgetch(window)) != QUIT_KEY)
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
				if(customsiz!=0 && pos==3 && customsiz<3) //Do not add if there is not a digit other than 0
				{
					customarr[customsiz]=key;
					++customsiz;
					customarr[customsiz]='\0';
				}
				break;
			case CONFIRM_KEY:
				if(pos==3 && customsiz<1) break;
				
				werase(window);
				erase();
				refresh();
				//Ask if to start with handicap
				//return 2 = quit
				//return 1 = handicap
				//return 0 = no handicap
				char isHandi=handi(maxy,maxx);
				if(isHandi==2) 
				{
					erase();
					refresh();
					return 0;
				}
				int height;
				//Calculate board size
				switch(pos){
					case 0:
						height=9;
						if(height>maxBoardSize)
							height=maxBoardSize;
						break;
					case 1:
						height=13;
						if(height>maxBoardSize)
							height=maxBoardSize;
						break;
					case 2:
						height=19;
						if(height>maxBoardSize)
							height=maxBoardSize;
						break;
					case 3:
						char *conv;
						height=0;
						for(conv = customarr; *conv; ++conv)
						{
							height*=10;
							height+=(*conv-'0');
						}
						if(arrtoint<MIN_BOARD_SIZE) height=MIN_BOARD_SIZE;
						break;
				}
				customsiz=0;
				pos=0;
				erase();
				refresh();
				//Start the game
				//return 1 - new game
				//return 0 - quit
				if(!runtime(height,maxy,maxx,isHandi))
					return 0;
				//Redraw menu
    			window = createMenu(maxy,maxx,START_MENU_HEIGHT,START_MENU_WIDTH);
				printStartM(-3,"GO",maxy,maxx);
				printStartM(-1,"Pick the board size",maxy,maxx);
				printStartM(7,"q - exit",maxy,maxx);
				printStartM(6,"Enter - start",maxy,maxx);

				refresh();
				wrefresh(window);

				for(i=0;i<4;++i)
				{
					printStart(window, i, pos, BLANK18, menuarr[i]);
				}
				wrefresh(window);
				refresh();
				break;
		}
		//Print basic options
		for(i=0;i<3;++i)
		{
			printStart(window, i, pos, BLANK18, menuarr[i]);
		}
		//Print custom size
		if(pos==3) wattron(window, A_STANDOUT); //If on option 3 highlight
		
		if(customsiz!=0)
		{
			arrtoint=customarrToInt(customarr,customsiz); //check if not over the max size
			if(customsiz>1 && arrtoint>maxBoardSize)
				customsiz=customarrToMax(customarr,maxBoardSize);
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



