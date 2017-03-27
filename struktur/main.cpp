#include "Screen.h"
#include "ncurses.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <cstdlib>
#include "Framebuffer.h"
#define MATSIZE 400

using namespace std;

typedef struct {
	int r;
	int g;
	int b;
} color;

std::priority_queue<int> polygonMatrix[MATSIZE][MATSIZE];
std::vector<color> color_container;
std::priority_queue<int> priority_container;


void insertPriority(int j,int i, int p){
		polygonMatrix[i][j].push(p);
}

void insertLine_Matrix(float x1, float y1,float x2, float y2, float priority)
{
	// Bresenham's line algorithm
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep)
	{
	  std::swap(x1, y1);
	  std::swap(x2, y2);
	}
	 
	if(x1 > x2)
	{
	  std::swap(x1, x2);
	  std::swap(y1, y2);
	}
	 
	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);
	 
	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;
	 
	const int maxX = (int)x2;
	
	for(int x=(int)x1; x<maxX; x++)
	{
	  if(steep)
	  {
	      insertPriority(y,x,priority);
		//insertPriority(y+1,x,priority);
	  }
	  else
	  {
	      insertPriority(x,y,priority);//insertPriority(x,y+1,priority);
	  }
	 
	  error -= dy;
	  if(error < 0)
	  {
	      y += ystep;
	      error += dx;
	  }
	}	
	

}

void showOverlap(){
	int current_priority = -1;
	Framebuffer fb;
	for (int i=0;i<MATSIZE;i++){//y
		for (int j=0;j<MATSIZE;j++){//x
			if (polygonMatrix[i][j].empty()){
				if (priority_container.empty()){
					//kosong
				}
				else {
					auto elmt = color_container.at(priority_container.top()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}
			}
			else {
				//Border Pertama ketemu
				if (priority_container.empty()){
					priority_queue<int> dummy = polygonMatrix[i][j];
					while (!dummy.empty()){
						int iter = dummy.top();
						dummy.pop(); 
						priority_container.push(iter);
					}
					auto elmt = color_container.at(priority_container.top()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}
				//Border terluar same object, ga bentrok
				else if (priority_container.top() == polygonMatrix[i][j].top()){
					auto elmt = color_container.at(priority_container.top()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}
				//Border pertama objek lain 
				else if (priority_container.top() >= polygonMatrix[i][j].top()){
					priority_container.pop();
					auto elmt = color_container.at(priority_container.top()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}
				//Border pertama objek lain 
				else if (priority_container.top() < polygonMatrix[i][j].top()){
					priority_container.push(polygonMatrix[i][j].top());
					auto elmt = color_container.at(priority_container.top()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}

			}
		}
	}

}

void drawKotak(int deltax, int deltay, color c, int priority){
	insertLine_Matrix(deltax+200+50 , deltay+100 , deltax+200+50 , deltay+200 ,priority);
	//insertLine_Matrix(deltax+200+50 , deltay+200 , deltax+410-50 , deltay+200 ,2);
//	insertLine_Matrix(deltax+200+50 , deltay+100 , deltax+410-51 , deltay+100 ,2);
	insertLine_Matrix(deltax+410-50 , deltay+100 , deltax+410-50 , deltay+200 ,priority);
	color_container.push_back(c);
}


int main(){
	Line l;
	int r,g,b = 255;
	cout << "smtg" << endl;
	// l.insertLine(100, 50, 50, 100, r,g,b);
	// l.insertLine(50, 100, 100, 100, r,g,b);
	// l.insertLine(100, 100, 150, 50, r,g,b);
	cout << "smtg" << endl;
	color c;
	c.r = 45;
	c.g = 255;
	c.b = 64;
	drawKotak(-70,30,c,1);

	c.r = 5;
	c.g = 32;
	c.b = 164;
	drawKotak(0,30,c,2);
	showOverlap(); 
	return 0;
}
