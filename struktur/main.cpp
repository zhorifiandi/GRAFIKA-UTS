#include "Screen.h"
#include "ncurses.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <cstdlib>
#include "Framebuffer.h"
#include "Draw.h"
#define MATSIZE 1000

using namespace std;

typedef struct {
	int r;
	int g;
	int b;
} color;

std::vector<int> polygonMatrix[MATSIZE][MATSIZE];
std::vector<color> color_container;
std::vector<int> priority_container;
list<int> colorMode;


void insertPriority(int j,int i, int p){
		polygonMatrix[i][j].push_back(p);
		sort(polygonMatrix[i][j].begin(),polygonMatrix[i][j].end());
}

void insertLine_Matrix(int x1, int y1,int x2, int y2, int priority)
{
	//Bresenham's line algorithm
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

void deleteFromPrio(int p){
	std::vector<int> delidx;
	for (auto it = priority_container.begin(); it != priority_container.end(); it++){
		if (*it == p){
			delidx.push_back(it-priority_container.begin());
		}
	}
	int k = 0;
	for (auto it = delidx.begin(); it != delidx.end(); it++){
		priority_container.erase(priority_container.begin()+*it-k);
		k++;
	}
}

void showOverlap(){
	int current_priority = -1;
	Framebuffer fb;
	bool isNeedCheck = false;
	int checker = -1;

	for (int i=0;i<MATSIZE;i++){//y
		for (int j=0;j<MATSIZE;j++){//x


			if (polygonMatrix[i][j].empty()){
				if (priority_container.empty()){
					//kosong
				}
				else {
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
					if (j == MATSIZE -1){
						cout << "Kena ujung" << endl;
					}
				}
			}
			else {
				//Border Pertama ketemu
				cout << "notempty ";
				if (priority_container.empty()){
					cout << "empty" << endl;
					int temp; cin >> temp;
					if (checker == polygonMatrix[i][j].front()){
							//Skip
					}
					else {
						priority_container.push_back(polygonMatrix[i][j].front());
						auto elmt = color_container.at(priority_container.front()-1);
						fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
						checker = -1;
					}
					// cout << "ini?" << endl;
				}
				//Ujung dari bordernya
				else if (priority_container.front() == polygonMatrix[i][j].front()){
					cout << "n1" << endl;
					int temp; cin >> temp;
					temp = priority_container.front();
					for (auto it = polygonMatrix[i][j].begin(); it != polygonMatrix[i][j].end(); it++){
						if (*it != temp){
							priority_container.push_back(*it);
							// cout <<"push "<< *it << endl;
						}
						
					}
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
					// cout <<"len "<< priority_container.size() << endl;
					
					// cout <<"len "<< priority_container.size() << endl;
					isNeedCheck = true;
					checker =priority_container.front();
					deleteFromPrio(temp);
				}
				//Belom sampe ujungnya, tapi nemu ujung prioritas lain
				else {
					//Awal border dari suatu objek
					cout << "n2" << endl;
					bool isLastBorderExist = false;
					std::vector<int> lastborders;
					if (!polygonMatrix[i][j].empty()){
						for (auto it = polygonMatrix[i][j].begin(); it != polygonMatrix[i][j].end(); it++){
							if (!binary_search(priority_container.begin(),priority_container.end(),*it)){
								priority_container.push_back(*it);
							}
							else {
								isLastBorderExist = true;
								lastborders.push_back(*it);
							}
						}
					}
					sort(priority_container.begin(),priority_container.end());
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
					if (!isLastBorderExist){
						
					}
					else {
						for (auto it = lastborders.begin(); it != lastborders.end(); it++)
							deleteFromPrio(*it);
					}
				}
			}
			if (j == MATSIZE -1){
				checker = -1;
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

void drawSegitiga(int deltax, int deltay, color c, int priority){
	insertLine_Matrix(deltax+100+50 , deltay+100 , deltax+200+50 , deltay+200 ,priority);
	//insertLine_Matrix(deltax+200+50 , deltay+200 , deltax+410-50 , deltay+200 ,2);
//	insertLine_Matrix(deltax+200+50 , deltay+100 , deltax+410-51 , deltay+100 ,2);
	insertLine_Matrix(deltax+201+50 , deltay+100 , deltax+410-50 , deltay+200 ,priority);
	color_container.push_back(c);
}


int main(){
	Line l;
	int r,g,b = 255;
	// l.insertLine(100, 50, 50, 100, r,g,b);
	// l.insertLine(50, 100, 100, 100, r,g,b);
	// l.insertLine(100, 100, 150, 50, r,g,b);
	color c;
	// KASUS INI ENGGAK
	// c.r = 5;
	// c.g = 32;
	// c.b = 164;
	// drawKotak(0,40,c,1);

	// c.r = 45;
	// c.g = 255;
	// c.b = 64;
	// drawKotak(-70,30,c,2);

	// c.r = 255;
	// c.g = 255;
	// c.b = 255;
	// drawKotak(0,40,c,1);


	// KASUS INI BUG
	// c.r = 45;
	// c.g = 255;
	// c.b = 64;
	// drawKotak(-70,30,c,1);

	// c.r = 5;
	// c.g = 32;
	// c.b = 164;
	// drawKotak(0,40,c,2);


//Abaikan ini
	// c.r = 58;
	// c.g = 132;
	// c.b = 94;
	// drawKotak(10,80,c,3);

	c.r = 158;
	c.g = 13;
	c.b = 4;
	drawSegitiga(110,80,c,1);


	
	showOverlap(); 
	return 0;
}
