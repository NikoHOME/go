#include "func.h"



char runtime(int height, int maxy,int maxx,char isHandi)
{
	WINDOW *table,*side;
	int keyt=0,heights;
	heights = height * 2 + BOARD_BASE_PADDING;

	char *board = (char*)malloc((height)*(height)*sizeof(char));
	char *logic = (char*)malloc((height)*(height)*sizeof(char));
	memset(board,' ',(height)*(height));

	
	table = newwin(heights, heights+BOARD_X_PADDING,(maxy-heights)/2, (maxx/2));
	side = newwin(BOARD_MENU_HEIGHT, BOARD_MENU_WIDTH,(maxy-BOARD_MENU_HEIGHT)/2, (maxx/2-BOARD_MENU_WIDTH*3/2));
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
	char whiteSrcBase[SCORE_ARR_SIZE] = "White Score =";
	char blackSrcBase[SCORE_ARR_SIZE] = "Black Score =";
	char whiteSrcOut[20],blackSrcOut[20];
	int location,lastX=0,lastY=0,lastLoc,killCount;
	float blackScore=0,whiteScore=6.5;
	int killCond=0,killCondLast;

	snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);
	mvwprintw(side,18,1,positionOut);

	snprintf(whiteSrcOut, sizeof(whiteSrcOut), "%s %.1f",whiteSrcBase , whiteScore);
	snprintf(blackSrcOut, sizeof(blackSrcOut), "%s %.1f",blackSrcBase , blackScore);
	mvwprintw(side,13,1,blackSrcOut);
	mvwprintw(side,14,1,whiteSrcOut);

	wrefresh(table);
	wrefresh(side);
	refresh();

	if(isHandi)
	{
		int stoneCount=0;
		printBoardM(side,11,1,"Handicap Mode");
		whiteScore=0.5;
		snprintf(whiteSrcOut, sizeof(whiteSrcOut), "%s %.1f",whiteSrcBase , whiteScore);
		mvwprintw(side,14,1,whiteSrcOut);
		wrefresh(side);
		while((keyt = wgetch(table)) != QUIT_KEY && keyt!='\n' && keyt!='n')
		{
			switch(keyt)
			{		
				BOARD_MOVE_SWITCH	
				
				case PUT_KEY:
					if(stoneCount==height*height-1)
					{
						printBoardM(side,17,1,BLANK28);
						printBoardM(side,17,1,"Illegal move - 0 free spaces");
						break;
					}
					++stoneCount;
					printBoardM(side,17,1,BLANK28);
					location=arrLoc(posx,posy,height);
					board[location]=BLACK_CHAR;
					waddch(table,BLACK_CHAR);
					lastLoc=location;
					lastX=posx;
					lastY=posy;
					stonePlaced=1;
					break;
				case 27: //ESC
					if(stonePlaced)
					{
						stonePlaced=0;
						wmove(table, lastX*2, lastY*2+1);
						waddch(table,EMPTY_CHAR);
						wrefresh(table);
						refresh();
						--stoneCount;

						board[lastLoc]=EMPTY_CHAR;

						wmove(table, posx*2, posy*2+1);
					}
					break;
			}
			BOARD_MOVE_CURS
			printBoardM(side,11,1,"Handicap Mode");
			snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);
			printBoardM(side,18,1,BLANK8);
			mvwprintw(side,18,1,positionOut);
			wrefresh(side);



		}
		printBoardM(side,11,1,BLANK24);
		wrefresh(side);
		blackTurn=0;
		stonePlaced=0;
		if(keyt=='n') 
		{
			erase();
			refresh();
			free(board);
			free(logic);
			return 1;
		}
		if(keyt==QUIT_KEY) 
		{
			erase();
			refresh();
			free(board);
			free(logic);
			return 0;
		}
	}


	while((keyt = wgetch(table)) != QUIT_KEY && keyt!='n')
	{
		switch(keyt)
		{
			BOARD_MOVE_SWITCH

			case PUT_KEY:
				location=arrLoc(posx,posy,height);
				if(board[location]==EMPTY_CHAR && !stonePlaced)
				{
					if(blackTurn)
					{	
						board[location]=BLACK_CHAR;
						killCondLast=killCond;
						killCond=isValid(table,posx,posy,height,WHITE_CHAR,logic,board,2);
						if(isKo(killCond,killCondLast,posx,posy,lastX,lastY))
						{
							board[location]=EMPTY_CHAR;
							printBoardM(side,17,1,BLANK24);
							printBoardM(side,17,1,"Illegal move - ko");
							break;
						}
						if( isValid(table,posx,posy,height,BLACK_CHAR,logic,board,0) || killCond)
						{
							waddch(table,BLACK_CHAR);
							lastLoc=location;
							lastX=posx;
							lastY=posy;
							stonePlaced=1;

						}
						else 
						{
							board[location]=EMPTY_CHAR;
							printBoardM(side,17,1,BLANK24);
							printBoardM(side,17,1,"Illegal move - suicide");
						}
					}
					else
					{
						board[location]=WHITE_CHAR;
						killCondLast=killCond;
						killCond=isValid(table,posx,posy,height,BLACK_CHAR,logic,board,2);
						if(isKo(killCond,killCondLast,posx,posy,lastX,lastY))
						{
							board[location]=EMPTY_CHAR;
							printBoardM(side,17,1,BLANK24);
							printBoardM(side,17,1,"Illegal move - ko");
							break;
						}
						if(	isValid(table,posx,posy,height,WHITE_CHAR,logic,board,0) || 
							killCond)
						{
							waddch(table,WHITE_CHAR);
							lastLoc=location;
							lastX=posx;
							lastY=posy;
							stonePlaced=1;
						}
						else 
						{
							board[location]=EMPTY_CHAR;
							printBoardM(side,17,1,BLANK24);
							printBoardM(side,17,1,"Illegal move - suicide");
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
					killCount=isValid(table,lastX,lastY,height,WHITE_CHAR,logic,board,1);
					blackScore+=killCount;
					printBoardM(side,16,1,BLANK16);
					printBoardM(side,16,1,"White Turn");

				}
				else
				{
					blackTurn=1;
					killCount=isValid(table,lastX,lastY,height,BLACK_CHAR,logic,board,1);
					whiteScore+=killCount;
					printBoardM(side,16,1,BLANK16);
					printBoardM(side,16,1,"Black Turn");
				}
				stonePlaced=0;
				//snprintf(killOut, sizeof(killOut), "%d",killCount);
				//printBoardM(side,14,1,BLANK8);
				//printBoardM(side,14,1,killOut);	
				printBoardM(side,17,1,BLANK24);
				break;
			case 27: //ESC
				if(stonePlaced)
				{
					stonePlaced=0;
					wmove(table, lastX*2, lastY*2+1);
					waddch(table,EMPTY_CHAR);
					wrefresh(table);
					refresh();

					board[lastLoc]=EMPTY_CHAR;

					wmove(table, posx*2, posy*2+1);
				}
				break;
				
			case 's':
				erase();
				refresh();
				if(!save(maxy,maxx,board,SAVE_FUNC_EXPORT))
				{
					erase();
					refresh();
					return 0;
				}
				free(board);
				free(logic);
				return 1;
				break;
			case 'l':
				erase();
				refresh();
				board = load(maxy,maxx,LOAD_FUNC_EXPORT);
				FILE *ptr = fopen("debug.txt","a");
		    	int temp1,temp2;
    			for(temp1=0;temp1<height;++temp1)
    			{
    			    for(temp2=0;temp2<height;++temp2)
    			    {
    			        fprintf(ptr,"%d ",board[temp1*height+temp2]);
    			    }
    			    fprintf(ptr,"\n");
    			}
    			fprintf(ptr,"\n");
    			fclose(ptr);

				REDRAW
		
		
		}

		BOARD_MOVE_CURS

		snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);
		printBoardM(side,18,1,BLANK8);
		mvwprintw(side,18,1,positionOut);

		snprintf(whiteSrcOut, sizeof(whiteSrcOut), "%s %.1f",whiteSrcBase , whiteScore);
		snprintf(blackSrcOut, sizeof(blackSrcOut), "%s %.1f",blackSrcBase , blackScore);
		printBoardM(side,13,SCORE_ARR_SIZE,BLANK8);
		printBoardM(side,14,SCORE_ARR_SIZE,BLANK8);
		mvwprintw(side,13,1,blackSrcOut);
		mvwprintw(side,14,1,whiteSrcOut);


		
		wrefresh(side);
		wrefresh(table);
		refresh();
	}
	free(board);
	free(logic);

	erase();
	refresh();
	if(keyt=='n') return 1;
	return 0;
}