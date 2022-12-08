#include "func.h"

void runtime(int height, int maxy,int maxx)
{
	WINDOW *table,*side;
	int keyt=0,heights;
	heights = height * 2 + BOARD_BASE_PADDING;

	char *board = (char*)malloc((height+1)*(height+1)*sizeof(char));
	char *logic = (char*)malloc((height+1)*(height+1)*sizeof(char));
	memset(board,' ',height*height);

	
	table = newwin(heights, heights+BOARD_X_PADDING,(maxy-heights)/2, (maxx-heights)/3*2);
	side = newwin(BOARD_MENU_HEIGHT, BOARD_MENU_WIDTH,(maxy-BOARD_MENU_HEIGHT)/2, (maxx-BOARD_MENU_WIDTH)/3);
	keypad(table, true);
	printBoard(table,board,height);
	box(table, 0, 0);
	box(side, 0, 0);

	int posx=1,posy=1,pposx=1,pposy=1;

	mvwchgat(table,posx*2,posy*2+1,1,A_REVERSE,0,NULL);



	printBoardM(side,1,1,"Wiktor Gawronski 193285");
	printBoardM(side,2,1,"q - quit");
	printBoardM(side,3,1,"n - restart");
	printBoardM(side,5,1,"Arrows - movement");
	printBoardM(side,6,1,"Enter - end turn");
	printBoardM(side,7,1,"Esc - undo");
	printBoardM(side,8,1,"i - place a stone");

	printBoardM(side,16,1,"Black Turn");

	wrefresh(table);
	wrefresh(side);
	refresh();

	char blackTurn=1,stonePlaced=0;
	char positionOut[7];
	int location,lastX,lastY,lastLoc;

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
			case 'i':
				location=arrLoc(posx,posy,height);
				if(board[location]==' ' && !stonePlaced)
				{
					if(blackTurn)
					{
						if(isValid(posx,posy,height,'@',logic,board,0))
						{
							board[location]='@';
							waddch(table,'@');
							lastLoc=location;
							lastX=posx;
							lastY=posy;
							stonePlaced=1;
						}
						else 
						{
							printBoardM(side,16,1,BLANK16);
							printBoardM(side,16,1,"Illegal move");
						}
					}
					else
					{
						if(isValid(posx,posy,height,'#',logic,board,0))
						{
							board[location]='#';
							waddch(table,'#');
							lastLoc=location;
							lastX=posx;
							lastY=posy;
							stonePlaced=1;
						}
						else 
						{
							printBoardM(side,16,1,BLANK16);
							printBoardM(side,16,1,"Illegal move");
						}
					}
					
					wmove(table, posx*2, posy*2+1);
					wrefresh(side);
				}

				break;
			case '\n':
				if(blackTurn)
				{
					blackTurn=0;
					stonePlaced=0;
					printBoardM(side,16,1,BLANK16);
					printBoardM(side,16,1,"White Turn");
				}
				else
				{
					blackTurn=1;
					stonePlaced=0;
					printBoardM(side,16,1,BLANK16);
					printBoardM(side,16,1,"Black Turn");
				}
				break;
			case 27: //ESC
				if(stonePlaced)
				{
					stonePlaced=0;
					wmove(table, lastX*2, lastY*2+1);
					waddch(table,' ');
					wrefresh(table);
					refresh();

					board[lastLoc]=' ';

					wmove(table, posx*2, posy*2+1);
				}
				break;
			case 'n':
				free(board);
				board = (char*)malloc(height*height*sizeof(char));
				memset(board,' ',height*height);
				printBoard(table,board,height);
				blackTurn=1;
				stonePlaced=0;
				break;
		}
		mvwchgat(table,pposx*2,pposy*2+1,1,A_NORMAL,0,NULL);
		pposx=posx;
		pposy=posy;
		mvwchgat(table,posx*2,posy*2+1,1,A_REVERSE,0,NULL);
		wmove(table, posx*2, posy*2+1);



		snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);
		printBoardM(side,18,1,BLANK8);
		mvwprintw(side,18,1,positionOut);



		//FILE *ptr = fopen("debug.txt","a");
		//fprintf(ptr,"%d %d %d .%c.\n",posx,posy,arrLoc(posx,posy,height),board[arrLoc(posx,posy,height)]);
		//fclose(ptr);
		
		wrefresh(side);
		wrefresh(table);
		refresh();
	}
}