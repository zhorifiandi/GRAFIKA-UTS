#include "Screen.h"
#include "ncurses.h"
#include <fstream>
#include <iostream>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <cstdlib>
#include "FrameBuffer2.h"

FrameBuffer2 *fb2;

using namespace std;
const int RATIO_VALUE= 417;

list<LineDetails*> listPlane;
list<LineDetails*> listExplosion;
char ch;
// !!! width:height ratio = 4:3
int bigScreenWidth = 540;
int bigScreenHeight = 540;	
Screen bigScreen = Screen(0, 0, bigScreenWidth, bigScreenHeight);

Screen bigScreen2 = Screen(0, 0, bigScreenWidth, bigScreenHeight);
	
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
			usleep(100);

			for (list<LineDetails*>::iterator it = listPlane.begin(); it != listPlane.end(); it++){
				int x = (*it)->x1*417;
				int y = (*it)->y1*417;
				int oriX = zoomScreen.getOriginX();
				int oriY = zoomScreen.getOriginY();
				if ((x>oriX && x<(oriX+zoomScreenWidth)) && (y>oriY && y<(oriY+zoomScreenHeight))) {
					shoot = true;
				} else {
					thread shootNH(shootNoHit);
					shootNH.detach();
				}
			}		
		break;
		}
		refresh();
	}
}

void game_over(){

	int deltay = -100;
	int deltax = -100;
	while (deltay<=300) {
		//G
		fb2->draw_line3(deltax+232 , deltay+129 , deltax+258 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+258 , deltay+129 , deltax+258 , deltay+138 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+258 , deltay+138 , deltax+242 , deltay+138 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+242 , deltay+138 , deltax+242 , deltay+165 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+242 , deltay+165 , deltax+250 , deltay+165 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+250 , deltay+165 , deltax+250 , deltay+159 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+250 , deltay+159 , deltax+246 , deltay+159 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+246 , deltay+159 , deltax+246 , deltay+150 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+246 , deltay+150 , deltax+259 , deltay+150 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+259 , deltay+150 , deltax+259 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+259 , deltay+174 , deltax+232 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+232 , deltay+174 , deltax+232 , deltay+129 , 150 , 0 , 0  );

		fb2->flood_fill3(deltax+253 , deltay+155 , 120 , 30 , 0);

		//A
		fb2->draw_line3(deltax+264 , deltay+136 ,deltax+271 , deltay+129 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+271 , deltay+129 ,deltax+291 , deltay+129 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+291 , deltay+129 ,deltax+291 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+291 , deltay+174 ,deltax+282 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+282 , deltay+174 ,deltax+282 , deltay+159 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+282 , deltay+159 ,deltax+274 , deltay+159 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+274 , deltay+159 ,deltax+274 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+274 , deltay+174 ,deltax+264 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+264 , deltay+174 ,deltax+264 , deltay+136 , 150 , 0 , 0   );

		fb2->draw_line3(deltax+274 , deltay+150 , deltax+274 , deltay+138 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+274 , deltay+138 , deltax+282 , deltay+138 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+282 , deltay+138 , deltax+282 , deltay+150 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+282 , deltay+150 , deltax+274 , deltay+150 , 150 , 0 , 0   );

		fb2->flood_fill3(deltax+285 , deltay+154 , 120 , 30 , 0);

		//M
		fb2->draw_line3(deltax+296 , deltay+129 , deltax+308 , deltay+129 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+308 , deltay+129 , deltax+313 , deltay+146 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+313 , deltay+146 , deltax+319 , deltay+129 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+319 , deltay+129 , deltax+331 , deltay+129 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+331 , deltay+129 , deltax+331 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+331 , deltay+174 , deltax+321 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+321 , deltay+174 , deltax+321 , deltay+157 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+321 , deltay+157 , deltax+319 , deltay+157 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+319 , deltay+157 , deltax+313 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+313 , deltay+174 , deltax+308 , deltay+157 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+308 , deltay+157 , deltax+305 , deltay+157 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+305 , deltay+157 , deltax+305 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+305 , deltay+174 , deltax+296 , deltay+174 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+296 , deltay+174 , deltax+296 , deltay+129 , 150 , 0 , 0   );

		fb2->flood_fill3(deltax+313 , deltay+153 , 120 , 30 , 0);

		//E
		fb2->draw_line3(deltax+336 , deltay+129 , deltax+358 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+358 , deltay+129 , deltax+358 , deltay+138 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+358 , deltay+138 , deltax+346 , deltay+138 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+346 , deltay+138 , deltax+346 , deltay+146 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+346 , deltay+146 , deltax+358 , deltay+146 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+358 , deltay+146 , deltax+358 , deltay+155 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+358 , deltay+155 , deltax+346 , deltay+155 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+346 , deltay+155 , deltax+346 , deltay+165 , 150 , 0 , 0   );
		fb2->draw_line3(deltax+346 , deltay+165 , deltax+359 , deltay+165 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+359 , deltay+165 , deltax+359 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+359 , deltay+174 , deltax+336 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+336 , deltay+174 , deltax+336 , deltay+129 , 150 , 0 , 0  );

		fb2->flood_fill3(deltax+342 , deltay+151 , 120 , 30 , 0);

		//O
		fb2->draw_line3(deltax+378 , deltay+129 , deltax+406 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+406 , deltay+129 , deltax+406 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+406 , deltay+174 , deltax+378 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+378 , deltay+174 , deltax+378 , deltay+129 , 150 , 0 , 0  );

		fb2->draw_line3(deltax+388 , deltay+138 , deltax+396 , deltay+138 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+396 , deltay+138 , deltax+396 , deltay+165 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+396 , deltay+165 , deltax+388 , deltay+165 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+388 , deltay+165 , deltax+388 , deltay+138 , 150 , 0 , 0  );

		fb2->flood_fill3(deltax+399 , deltay+152 , 120 , 30 , 0);


		//V
		fb2->draw_line3(deltax+410 , deltay+129 , deltax+421 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+421 , deltay+129 , deltax+424 , deltay+153 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+424 , deltay+153 , deltax+428 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+428 , deltay+129 , deltax+438 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+438 , deltay+129 , deltax+432 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+432 , deltay+174 , deltax+416 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+416 , deltay+174 , deltax+410 , deltay+129 , 150 , 0 , 0  );

		fb2->flood_fill3(deltax+424 , deltay+157 , 120 , 30 , 0); 

		//E
		fb2->draw_line3(deltax+336+106 , deltay+129 , deltax+358+106 , deltay+129 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+358+106 , deltay+129 , deltax+358+106 , deltay+138 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+358+106 , deltay+138 , deltax+346+106 , deltay+138 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+346+106 , deltay+138 , deltax+346+106 , deltay+146 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+346+106 , deltay+146 , deltax+358+106 , deltay+146 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+358+106 , deltay+146 , deltax+358+106 , deltay+155 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+358+106 , deltay+155 , deltax+346+106 , deltay+155 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+346+106 , deltay+155 , deltax+346+106 , deltay+165 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+346+106 , deltay+165 , deltax+359+106 , deltay+165 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+359+106 , deltay+165 , deltax+359+106 , deltay+174 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+359+106 , deltay+174 , deltax+336+106 , deltay+174 , 150 , 0 , 0 );
		fb2->draw_line3(deltax+336+106 , deltay+174 , deltax+336+106 , deltay+129 , 150 , 0 , 0 );

		fb2->flood_fill3(deltax+448 , deltay+151 , 120 , 30 , 0);

		//R
		fb2->draw_line3(deltax+468 , deltay+135 , deltax+474 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+474 , deltay+129 , deltax+495 , deltay+129 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+495 , deltay+129 , deltax+495 , deltay+158 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+495 , deltay+158 , deltax+490 , deltay+158 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+490 , deltay+158 , deltax+496 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+496 , deltay+174 , deltax+485 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+485 , deltay+174 , deltax+480 , deltay+158 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+480 , deltay+158 , deltax+478 , deltay+158 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+478 , deltay+158 , deltax+478 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+478 , deltay+174 , deltax+468 , deltay+174 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+468 , deltay+174 , deltax+468 , deltay+135 , 150 , 0 , 0  );

		fb2->draw_line3(deltax+478 , deltay+138 , deltax+485 , deltay+138 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+485 , deltay+138 , deltax+485 , deltay+149 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+485 , deltay+149 , deltax+478 , deltay+149 , 150 , 0 , 0  );
		fb2->draw_line3(deltax+478 , deltay+149 , deltax+478 , deltay+138 , 150 , 0 , 0  );

		fb2->flood_fill3(deltax+482 , deltay+153 , 120 , 30 , 0);

		usleep(20000);

		fb2->reset_fill3(deltax+253 , deltay+155 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+285 , deltay+154 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+313 , deltay+153 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+342 , deltay+151 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+399 , deltay+152 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+424 , deltay+157 , 0 , 0 , 0); 
		fb2->reset_fill3(deltax+448 , deltay+151 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+482 , deltay+153 , 0 , 0 , 0);	
		
		deltay=deltay + 2;	
	}
}

void win(){

	int deltay = -100;
	int deltax = -100;
	while (deltay<=300) {
		//W
		fb2->draw_line3(deltax+278 , deltay+110 , deltax+288 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+288 , deltay+110 , deltax+292 , deltay+130 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+292 , deltay+130 , deltax+296 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+296 , deltay+110 , deltax+303 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+303 , deltay+110 , deltax+308 , deltay+130 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+308 , deltay+130 , deltax+311 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+311 , deltay+110 , deltax+322 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+322 , deltay+110 , deltax+314 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+314 , deltay+155 , deltax+304 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+304 , deltay+155 , deltax+300 , deltay+133 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+300 , deltay+133 , deltax+295 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+295 , deltay+155 , deltax+286 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+286 , deltay+155 , deltax+278 , deltay+110 , 0 , 150 , 0  );

		fb2->flood_fill3(deltax+299 , deltay+125 , 0 , 130 , 20);

		//I
		fb2->draw_line3(deltax+324 , deltay+110 , deltax+342 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+342 , deltay+110 , deltax+342 , deltay+119 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+342 , deltay+119 , deltax+338 , deltay+119 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+338 , deltay+119 , deltax+338 , deltay+146 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+338 , deltay+146 , deltax+342 , deltay+146 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+342 , deltay+146 , deltax+342 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+342 , deltay+155 , deltax+324 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+324 , deltay+155 , deltax+324 , deltay+146 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+324 , deltay+146 , deltax+328 , deltay+146 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+328 , deltay+146 , deltax+328 , deltay+119 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+328 , deltay+119 , deltax+324 , deltay+119 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+324 , deltay+119 , deltax+324 , deltay+110 , 0 , 150 , 0  );

		fb2->flood_fill3(deltax+332 , deltay+132 , 0 , 130 , 20);

		//N
		fb2->draw_line3(deltax+346 , deltay+110 , deltax+356 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+356 , deltay+110 , deltax+363 , deltay+128 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+363 , deltay+128 , deltax+365 , deltay+128 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+365 , deltay+128 , deltax+365 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+365 , deltay+110 , deltax+374 , deltay+110 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+374 , deltay+110 , deltax+374 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+374 , deltay+155 , deltax+364 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+364 , deltay+155 , deltax+358 , deltay+137 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+358 , deltay+137 , deltax+356 , deltay+137 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+356 , deltay+137 , deltax+356 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+356 , deltay+155 , deltax+346 , deltay+155 , 0 , 150 , 0  );
		fb2->draw_line3(deltax+346 , deltay+155 , deltax+346 , deltay+110 , 0 , 150 , 0  );

		fb2->flood_fill3(deltax+363 , deltay+135 , 0 , 130 , 20);
		
		usleep(20000);

		fb2->reset_fill3(deltax+299 , deltay+125 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+332 , deltay+132 , 0 , 0 , 0);
		fb2->reset_fill3(deltax+363 , deltay+135 , 0 , 0 , 0);

		deltay=deltay + 2;	
	}
}

void shootEffect() {
	
	// Load explosion coordinates
	int idx;
	int elmt;
	int* table = new int[4];
   
	fstream explosion("explosion1.txt", std::ios_base::in);
	list<int> intListExplosion;
	
    while (explosion >> elmt)
    {
        intListExplosion.push_back(elmt);
    }
    
	idx = 0;
    for (std::list<int>::iterator it = intListExplosion.begin(); it != intListExplosion.end(); ++it) {
		table[idx] = *it;
		idx++;
		if (idx == 4) {
			listExplosion.push_back(new LineDetails(getRatio(table[0]), getRatio(table[1]), getRatio(table[2]), getRatio(table[3])));
			idx = 0;
		}
	}

	explosion.close();

	// Draw explosion
	system("clear");
	int initx, inity;
	initx = (*listPlane.begin())->x1*417;
	inity = (*listPlane.begin())->y1*417;
	for(list<LineDetails*>::iterator it = listExplosion.begin(); it != listExplosion.end(); it++) {
		(*it)->moveLine(getRatio(initx),getRatio(inity));
		bigScreen2.renderLine((*it),255,0,0);
	}
	zoomScreen.renderBorder();
	smallScreen.renderSmall(listExplosion, zoomScreen, bigScreen2, 255,0,0);
	bigScreen2.renderBorder();
	smallScreen.renderBorder();
	usleep(10000);
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
		shootEffect();
	}

	//Credit?
	/*
	fb2 = new FrameBuffer2;
	
	game_over();
	//win();
	delete fb2;
	*/
		
	return 0;
}
