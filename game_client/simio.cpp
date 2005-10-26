#include "simio.h"

#ifndef __MSDOS__

int auto_refresh = 1;
int last_key = 0;
int key_stored = 0;

void newline()
{
	cputch ( '\n' );
}

void initconio()
{
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr,1);
}

void deinitconio()
{
	endwin();
}


void setautorefresh ( int refr )
{
	auto_refresh = refr;
}


void autorefresh()
{
	if ( auto_refresh )	refresh();
}


void clrscr()
{
	clear();
	autorefresh();
}


void gotoxy ( int x, int y )
{
	move ( y-1, x-1 );
	autorefresh();
}


void cputch ( int ch )
{
	addch ( ch );
	autorefresh();
}


int kbhit()
{
	if ( !key_stored )
	{
		last_key = getch();
		if ( last_key != ERR )
		{
			key_stored = 1;
		}
	}

	return key_stored;
}

int waitkey()
{
	while ( !kbhit() )
	{
		// wait
	}
	key_stored = 0;

	return last_key;
}


#else

void initconio()
{
	textmode ( C4350 );
	_setcursortype ( _NOCURSOR );
}


void newline()
{
	cputch ( '\r' );
	cputch ( '\n' );
}


#endif
