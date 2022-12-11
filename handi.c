#include "func.h"

char handi(int maxy,int maxx)
{
    WINDOW *handi;
    handi = createMenu(maxy,maxx,HANDI_MENU_HEIGHT,HANDI_MENU_WIDTH);
    
    int key,pos=0,i;


    char *menuarr[] = {"Yes", "No"};

    printStartM(-1,"Start with handicap?",maxy,maxx);
    
    for(i=0;i<2;++i)
	{
		printStart(handi, i, pos, BLANK18, menuarr[i]);
	}
    refresh();
    while((key = wgetch(handi)) != QUIT_KEY && key!='\n')
	{	
		switch(key)
		{
			case KEY_UP:
				if(pos>0) --pos;
				break;
			case KEY_DOWN:
				if(pos<1) ++pos;
				break;
        }
        for(i=0;i<2;++i)
	    {
		    printStart(handi, i, pos, BLANK18, menuarr[i]);
	    }
        wrefresh(handi);
    }
    erase();
    refresh();
    if(key==QUIT_KEY) 
        return 2;
    if(!pos)
        return 1;
    return 0;
}

char save(int maxy,int maxx,SAVE_FUNC_IMPORT)
{

	WINDOW *win;
    FILE *file;
    int key;
    win = createMenu(maxy,maxx,SAVE_MENU_HEIGHT,SAVE_MENU_WIDTH);
    printStartM(-1,"Enter file name letters/numbers",maxy,maxx);
    refresh();

    char input[MAX_FILENAME_SIZE+1],inputf[MAX_FILENAME_SIZE+1];
    input[0]=' ';
    input[1]='\0';
    int namesize=0;
    char write=0;
    while(!write && (key = wgetch(win)) != QUIT_KEY)
	{	
		switch(key)
		{
			case 'a' ... 'z': case 'A' ... 'Z': case '0' ... '9':
                if(namesize<32)
                {
				    input[namesize]=key;
                    ++namesize;
                    input[namesize]='\0';
                    printStartM(-2,BLANK24,maxy,maxx);
                }
                else
                {
                    printStartM(-2,"Reached max filename",maxy,maxx);
                }
                break;
			case KEY_BACKSPACE:
				if(namesize>0)
				{
					namesize--;
					input[namesize]='\0';
                    printStartM(-2,BLANK24,maxy,maxx);
				}
				break;
            case '\n':
                snprintf(inputf,sizeof(inputf),"save/%s.save",input);
                if(file=fopen(inputf, "r"))
                {
                    printStartM(-2,"File exists",maxy,maxx);
                    break;
                }
                write=1;
                break;
        }
        mvwprintw(win, 2, 1, BLANK32);
        mvwprintw(win, 2, 1, input);
        refresh();

    }
        erase();
        refresh();
        if(!write)
        {
            return 0;
        }
        struct data data={SAVE_FUNC_EXPORT};
        file=fopen(inputf,"wb");
        fwrite(&data,sizeof(struct data),1,file);
        fwrite(board,sizeof(char),(height)*(height),file);
        fclose(file);
        return 1;
}

char *load(int maxy,int maxx,LOAD_FUNC_IMPORT)
{

    WINDOW *win;
    FILE *file;
    int key;
    win = createMenu(maxy,maxx,SAVE_MENU_HEIGHT,SAVE_MENU_WIDTH);
    printStartM(-1,"Enter file name letters/numbers",maxy,maxx);
    refresh();

    char input[MAX_FILENAME_SIZE+1],inputf[MAX_FILENAME_SIZE+1];
    input[0]=' ';
    input[1]='\0';
    int namesize=0;
    char write=0;
    while(!write && (key = wgetch(win)) != QUIT_KEY)
	{	
		switch(key)
		{
			case 'a' ... 'z': case 'A' ... 'Z': case '0' ... '9':
                if(namesize<32)
                {
				    input[namesize]=key;
                    ++namesize;
                    input[namesize]='\0';
                    printStartM(-2,BLANK24,maxy,maxx);
                }
                else
                {
                    printStartM(-2,"Reached max filename",maxy,maxx);
                }
                break;
			case KEY_BACKSPACE:
				if(namesize>0)
				{
					namesize--;
					input[namesize]='\0';
                    printStartM(-2,BLANK24,maxy,maxx);
				}
				break;
            case '\n':
                snprintf(inputf,sizeof(inputf),"save/%s.save",input);
                if(!(file=fopen(inputf, "r")))
                {                
                    printStartM(-2,"File does not exist",maxy,maxx);
                    break;
                }
                write=1;
                break;
        }
        mvwprintw(win, 2, 1, BLANK32);
        mvwprintw(win, 2, 1, input);
        refresh();

    }
    erase();
    refresh();
    if(!write)
    {
        return 0;
    }
    struct data data;
    file=fopen(inputf,"rb");
    fread(&data,sizeof(struct data),1,file);
	char *boardNew = (char*)malloc((data.height)*(data.height)*sizeof(char));
    
    
    memset(boardNew,' ',data.height*data.height);
    fread(boardNew,sizeof(char),(data.height)*(data.height),file);
    
    fclose(file);

    *height=data.height;
    *posx=data.posx;
    *posy=data.posy;
    *pposx=data.pposx;
    *pposy=data.pposy;
    *blackTurn=data.blackTurn;
    *stonePlaced=data.stonePlaced;
    *location=data.location;
    *lastX=data.lastX;
    *lastY=data.lastY;
    *lastLoc=data.lastLoc;
    *killCount=data.killCount;
    *blackScore=data.blackScore;
    *whiteScore=data.whiteScore;
    *killCond=data.killCond;
    *killCondLast=data.killCondLast;
    return boardNew;
}