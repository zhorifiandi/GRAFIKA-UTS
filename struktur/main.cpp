#include "Screen.h"
#include "ncurses.h"
#include <fstream>
#include <iostream>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <cstdlib>
#include "Framebuffer.h"

using namespace std;

int bigScreenWidth = 540;
int bigScreenHeight = 540;	
Screen bigScreen = Screen(0, 0, bigScreenWidth, bigScreenHeight);

int main(){
	Line l;
	int r,g,b = 255;
	cout << "smtg" << endl;
	l.insertLine(100, 50, 50, 100, r,g,b);
	l.insertLine(50, 100, 100, 100, r,g,b);
	l.insertLine(100, 100, 150, 50, r,g,b);
	cout << "smtg" << endl; 
	return 0;
}