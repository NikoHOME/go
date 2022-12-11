#include "func.h"


char runtime(int height, int maxy,int maxx,char isHandi)
{
	WINDOW *table,*side;
	int keyt=0,heights;
	//heights - size of the board display window (has decorations)
	heights = height * 2 + BOARD_BASE_PADDING;

	char *board = (char*)malloc((height)*(height)*sizeof(char));
	//Board with stones
	char *logic = (char*)malloc((height)*(height)*sizeof(char));
	//Array for dfs function
	memset(board,' ',(height)*(height));

	
	table = newwin(heights, heights+BOARD_X_PADDING,BOARD_POS_X, BOARD_POX_Y);
	//Display for board
	side = newwin(BOARD_MENU_HEIGHT, BOARD_MENU_WIDTH, BOARD_MENU_POS_X, BOARD_MENU_POS_Y);
	//Side menu 
	keypad(table, true);
	printBoard(table,board,height);
	//Add border
	box(table, 0, 0);
	box(side, 0, 0);

	int posx=1,posy=1,pposx=1,pposy=1;
	//Highlight cursor
	mvwchgat(table,posx*2,posy*2+1,1,A_REVERSE,0,NULL);

	printBoardM(side,1,1,"Wiktor Gawronski 193285");
	printBoardM(side,2,1,"q - quit");
	printBoardM(side,3,1,"n - start a new game");
	printBoardM(side,5,1,"Arrows - movement");
	printBoardM(side,6,1,"Enter - end turn");
	printBoardM(side,7,1,"Esc - undo");
	printBoardM(side,8,1,"i - place a stone");
	printBoardM(side,9,1,"s - save the game");
	printBoardM(side,10,1,"l - load a game");


	printBoardM(side,16,1,"Black Turn");
	wrefresh(table);
	wrefresh(side);


	char blackTurn=1,stonePlaced=0;
	char positionOut[7]; //posx and posy output string
	char whiteSrcBase[SCORE_ARR_SIZE] = "White Score =";
	char blackSrcBase[SCORE_ARR_SIZE] = "Black Score =";
	char whiteSrcOut[20],blackSrcOut[20];//score output strings

	int location,lastX=0,lastY=0,lastLoc,killCount;

	float blackScore=0,whiteScore=6.5;
	int killCond=0,killCondLast;
	//killCond - number of stones you will capture (for ko rule)

	snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);
	mvwprintw(side,18,1,positionOut);
	//Print score
	snprintf(whiteSrcOut, sizeof(whiteSrcOut), "%s %.1f",whiteSrcBase , whiteScore);
	snprintf(blackSrcOut, sizeof(blackSrcOut), "%s %.1f",blackSrcBase , blackScore);
	mvwprintw(side,13,1,blackSrcOut);
	mvwprintw(side,14,1,whiteSrcOut);

	wrefresh(table);
	wrefresh(side);

	//Handicap mode
	if(isHandi)
	{
		int stoneCount=0; //Number of stones placed
		printBoardM(side,11,1,"Handicap Mode");
		whiteScore=0.5;
		//Print score
		snprintf(whiteSrcOut, sizeof(whiteSrcOut), "%s %.1f",whiteSrcBase , whiteScore);
		mvwprintw(side,14,1,whiteSrcOut);
		wrefresh(side);
		while((keyt = wgetch(table)) != QUIT_KEY && keyt!='\n' && keyt!='n')
		{
			switch(keyt)
			{		
				//Basic movement
				BOARD_MOVE_SWITCH	
				
				case PUT_KEY:
					if(stoneCount==height*height-1) //If board full stop
					{
						printBoardM(side,17,1,BLANK28);
						printBoardM(side,17,1,"Illegal move - 0 free spaces");
						break;
					}
					printBoardM(side,17,1,BLANK28);
					location=arrLoc(posx,posy,height);
					board[location]=BLACK_CHAR;
					waddch(table,BLACK_CHAR);
					lastLoc=location;
					lastX=posx;
					lastY=posy;
					stonePlaced=1;
					++stoneCount;
					break;
				case UNDO_KEY: //Move to one key before and erase it
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
			//Move cursor and add highlight
			BOARD_MOVE_CURS

			printBoardM(side,11,1,"Handicap Mode");
			snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);
			printBoardM(side,18,1,BLANK8);
			mvwprintw(side,18,1,positionOut);
			wrefresh(side);
		}
		printBoardM(side,11,1,BLANK24);
		wrefresh(side);
		if(!stoneCount) //If nothing changed start with default 6.5 points
		{
			whiteScore=6.5;
			snprintf(whiteSrcOut, sizeof(whiteSrcOut), "%s %.1f",whiteSrcBase , whiteScore);
			printBoardM(side,14,SCORE_ARR_SIZE,BLANK8);
			mvwprintw(side,14,1,whiteSrcOut);	
			wrefresh(side);
		}
		blackTurn=0;
		stonePlaced=0;
		if(keyt==NEW_GAME_KEY) 
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

	//Main funtion
	while((keyt = wgetch(table)) != QUIT_KEY && keyt!=NEW_GAME_KEY)
	{
		switch(keyt)
		{
			//Basic movement
			BOARD_MOVE_SWITCH

			case PUT_KEY:
				location=arrLoc(posx,posy,height);
				if(board[location]==EMPTY_CHAR && !stonePlaced)
				{
					if(blackTurn)
					{	
						board[location]=BLACK_CHAR;
						//Count how many stones this move will capture
						killCondLast=killCond;
						killCond=isValid(table,posx,posy,height,WHITE_CHAR,logic,board,2);
						if(isKo(killCond,killCondLast,posx,posy,lastX,lastY)) 
						{
							board[location]=EMPTY_CHAR;
							printBoardM(side,17,1,BLANK24);
							printBoardM(side,17,1,"Illegal move - ko");
							break;
						}
						//if is not ko and has breath or kills place
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
						//Count how many stones this move will capture
						killCondLast=killCond;
						killCond=isValid(table,posx,posy,height,BLACK_CHAR,logic,board,2);
						if(isKo(killCond,killCondLast,posx,posy,lastX,lastY))
						{
							board[location]=EMPTY_CHAR;
							printBoardM(side,17,1,BLANK24);
							printBoardM(side,17,1,"Illegal move - ko");
							break;
						}
						//if is not ko and has breath or kills place
						if(	isValid(table,posx,posy,height,WHITE_CHAR,logic,board,0) || killCond)
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
					//Reset cursor
					wmove(table, posx*2, posy*2+1);
					wrefresh(side);
				}

				break;
			case CONFIRM_KEY:
				if(blackTurn)
				{
					blackTurn=0;
					//Kill if the move kills
					killCount=isValid(table,lastX,lastY,height,WHITE_CHAR,logic,board,1);
					blackScore+=killCount;
					printBoardM(side,16,1,BLANK16);
					printBoardM(side,16,1,"White Turn");

				}
				else
				{
					blackTurn=1;
					//Kill if the move kills
					killCount=isValid(table,lastX,lastY,height,BLACK_CHAR,logic,board,1);
					whiteScore+=killCount;
					printBoardM(side,16,1,BLANK16);
					printBoardM(side,16,1,"Black Turn");
				}
				stonePlaced=0;
				printBoardM(side,17,1,BLANK24);
				break;
			case UNDO_KEY:
				//If stone was placed in this turn move cursor and erase it
				if(stonePlaced)
				{
					stonePlaced=0;
					wmove(table, lastX*2, lastY*2+1);
					waddch(table,EMPTY_CHAR);
					board[lastLoc]=EMPTY_CHAR;
					wrefresh(table);
					refresh();
					//Reset cursor
					wmove(table, posx*2, posy*2+1);
				}
				break;
				
			case SAVE_KEY:
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
			case LOAD_KEY:
				erase();
				refresh();
				board = load(maxy,maxx,LOAD_FUNC_EXPORT);
				REDRAW
		}
		//Move cursor and add highlight
		BOARD_MOVE_CURS

		//Print position
		snprintf(positionOut, sizeof(positionOut), "%dx%d",posy , posx);
		printBoardM(side,18,1,BLANK8);
		mvwprintw(side,18,1,positionOut);
		//Print score
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
	if(keyt==NEW_GAME_KEY) return 1;
	return 0;
}