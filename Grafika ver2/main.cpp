#include "Screen.h"
#include "ncurses.h"
#include <fstream>
#include <iostream>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <cstdlib>

using namespace std;
const int RATIO_VALUE= 417;

list<LineDetails*> listPlane;
char ch;
// !!! width:height ratio = 4:3
int bigScreenWidth = 540;
int bigScreenHeight = 540;	
Screen bigScreen = Screen(0, 0, bigScreenWidth, bigScreenHeight);
	
// Small screen
// !!! its size is relative to bigScreen
int smallScreenWidth = bigScreenWidth/3;
int smallScreenHeight = bigScreenHeight/3;	
Screen smallScreen = Screen(bigScreenWidth, bigScreenHeight-smallScreenHeight, smallScreenWidth, smallScreenHeight);
	
//ZOOM Screen
// size relative to bigscreen
int zoomScreenWidth = bigScreenWidth/3;
int zoomScreenHeight = bigScreenHeight/3;	
Screen zoomScreen = Screen(0, 0, zoomScreenWidth, zoomScreenHeight);
int lastdX=1, lastdY=1;
bool shoot =false;

double getRatio(int x) {
	return (double) x/417;
}

void renderMove(int xa, int ya){
	zoomScreen.changeOrigin(xa,ya);
	zoomScreen.renderBorder();
	smallScreen.renderSmall(listPlane, zoomScreen, bigScreen,255,0,0);
}

int deltaX(double x1){
	if (x1*417<=50) {
		lastdX=1;
		return 1;
	}
	else if (x1*417>=350) {
		lastdX=-1;
		return -1;
	}
	else {
		int val = rand()+10;
		if (val%9) return lastdX;
		else return -lastdX;
	}
}

int deltaY(double y1){
	if (y1*417<=50) {
		lastdY=1;
		return 1;
	}
	else if (y1*417>=350) {
		lastdY=-1;
		return -1;
	}
	else {
		int val = rand()+10;
		if (val%9) return lastdY;
		else return -lastdY;
	}
}

void flyPlane(){
	int deltX, deltY;	
	while (!shoot){
		system("clear");
		for(list<LineDetails*>::iterator it = listPlane.begin(); it != listPlane.end(); it++) {
			if (it==listPlane.begin()){
				deltX = deltaX((*it)->x1);
				deltY = deltaY((*it)->y1);
			}
			
			(*it)->moveLine(getRatio(deltX),getRatio(deltY));
			bigScreen.renderLine((*it),255,0,0);
		}	
		zoomScreen.renderBorder();
		smallScreen.renderSmall(listPlane, zoomScreen, bigScreen,255,0,0);
		bigScreen.renderBorder();
		smallScreen.renderBorder();
		usleep(10000);
	}
}
void shootNoHit(){
	int oriX = zoomScreen.getOriginX();
	int oriY = zoomScreen.getOriginY();
	Line l;
	clock_t startTime = clock(); //Start timer
	zoomScreen.renderBorderCol(0,0,0);				
	while ( ch=='b')	{		   			
		l.insertLine(oriX,oriY,(oriX+zoomScreenWidth),(oriY+zoomScreenHeight), 250,0,0);
		l.insertLine((oriX+zoomScreenWidth),oriY,oriX,(oriY+zoomScreenHeight), 250,0,0);

	}
	zoomScreen.renderBorder();
	l.insertLine(oriX,oriY,(oriX+zoomScreenWidth),(oriY+zoomScreenHeight), 0,0,0);
	l.insertLine((oriX+zoomScreenWidth),oriY,oriX,(oriY+zoomScreenHeight), 0,0,0);
}

void buttonPress(){
	int varZoom = 3;	
	int xa = 0; 
	int ya = 0;
	
	while (!shoot){
		ch = getch();
		switch(ch){
			case 'a':
				if (xa>0) { 
					xa-=2;
					renderMove(xa, ya);
				}
			break;
			case 'd':
				if (xa+zoomScreenWidth<bigScreenWidth) {
					xa+=2;
					renderMove(xa, ya);
					
				}
			break;
			case 'w':
				if (ya>0) {
					ya-=2;
					renderMove(xa, ya);
				}
			break;
			case 's':
				if (ya+zoomScreenHeight<bigScreenHeight) {
					ya+=2;
					renderMove(xa, ya);
				}		
			break;
		case 'i':
			  if (varZoom == 3) {
					varZoom = 6;
			  }
			  else if (varZoom == 6) {
					varZoom = 9;
			  }
			  else if (varZoom == 9) {
					varZoom = 18;
			  }
			  else if (varZoom == 18) {
					varZoom = 60;
			  }
			  else if (varZoom == 60) {
					varZoom = 105;
			  }
			  else {
			  }
			  //system("clear");
			  zoomScreenWidth = bigScreenWidth/varZoom;
			  zoomScreenHeight = bigScreenHeight/varZoom;	
			  zoomScreen = Screen(xa, ya, zoomScreenWidth, zoomScreenHeight);
			  smallScreen.renderSmall(listPlane, zoomScreen, bigScreen,255,0,0);
		break;
		case 'o':
			  if (varZoom == 105) {
					varZoom = 60;
			  }
			  else if (varZoom == 60) {
					varZoom = 18;
			  }
			  else if (varZoom == 18) {
					varZoom = 9;
			  }
			  else if (varZoom == 9) {
					varZoom = 6;
			  }
			  else if (varZoom == 6) {
					varZoom = 3;
			  }
			  else {
			  }
			  //system("clear");
			  zoomScreenWidth = bigScreenWidth/varZoom;
			  zoomScreenHeight = bigScreenHeight/varZoom;
			  zoomScreen = Screen(xa, ya, zoomScreenWidth, zoomScreenHeight);	
			  smallScreen.renderSmall(listPlane, zoomScreen, bigScreen,255,0,0);	
		break;
		case 'b':
			// list<LineDetails*>::iterator it = listPlane.begin();

			
			for (auto it = listPlane.begin(); it != listPlane.end(); it++){
				int x = (*it)->x1*417;
				int y = (*it)->y1*417;
				int oriX = zoomScreen.getOriginX();
				int oriY = zoomScreen.getOriginY();
				if ((x>oriX && x<(oriX+zoomScreenWidth)) && (y>oriY && y<(oriY+zoomScreenHeight))) shoot = true;
				else{
					thread shootNH(shootNoHit);
					shootNH.detach();
				}
			}		
		break;
		}
		refresh();
	}
}



int main() {
	//List of LineDetails
	int plus=0;
	list<int> intList;
	int loop = 0;
	int e;
	int* temp = new int[4];
   
	//Draw Plane
	fstream planefile("pesawat.txt", std::ios_base::in);
	list<int> intListPesawat;
	
    while (planefile >> e)
    {
        intListPesawat.push_back(e);
    }
    
	loop = 0;
    for (std::list<int>::iterator it = intListPesawat.begin(); it != intListPesawat.end(); ++it) {
		temp[loop] = *it;
		loop++;
		if (loop == 4) {
			listPlane.push_back(new LineDetails(getRatio(temp[0]), getRatio(temp[1]), getRatio(temp[2]), getRatio(temp[3])));
			loop = 0;
		}
	}

	planefile.close();
	
	
//----------Next Step----------------
	system("clear"); 
	initscr();
	keypad(stdscr,TRUE);
	noecho();
	char ch;
	zoomScreenWidth = bigScreenWidth/9;
			  zoomScreenHeight = bigScreenHeight/9;	
			  zoomScreen = Screen(0, 0, zoomScreenWidth, zoomScreenHeight);
			  smallScreen.renderSmall(listPlane, zoomScreen, bigScreen,255,0,0);
	zoomScreen.renderBorder();
	
	thread fly(flyPlane);
	thread button(buttonPress);
	
	while(!shoot) {
		
	}
	fly.detach();
	button.detach();
	
	
	//Abis ditembak-> animasi ledakan sama dilatasi
	if (shoot){
	//shootEffect();
	}
	
	//Credit?
	
	
		
	return 0;
}
