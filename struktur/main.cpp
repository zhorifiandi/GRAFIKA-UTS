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
#define MATSIZE 1000

using namespace std;

typedef struct {
	int r;
	int g;
	int b;
} color;

std::priority_queue<int> polygonMatrix[MATSIZE][MATSIZE];
std::vector<color> color_container;
std::vector<int> priority_container;


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
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}
			}
			else {
				//Border Pertama ketemu
				cout << "notempty ";
				if (priority_container.empty()){
					cout << "empty" << endl;
					priority_queue<int> dummy = polygonMatrix[i][j];
					while (!dummy.empty()){
						int iter = dummy.top();
						dummy.pop(); 
						cout << "push " << polygonMatrix[i][j].top() << endl;
						priority_container.push_back(iter);
						sort(priority_container.begin(),priority_container.end());
						reverse(priority_container.begin(),priority_container.end());
					}
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}
				//Border terluar same object, ga bentrok
				else if (priority_container.front() == polygonMatrix[i][j].top()){
					cout << "n1" << endl;
					int t; cin >> t;
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
					priority_queue<int> dummy = polygonMatrix[i][j];
					while (!dummy.empty()){
						auto idx = find(priority_container.begin(),priority_container.end(),dummy.top());
						if (idx != priority_container.end()){
							cout << "erase" << endl;
							priority_container.erase(idx);
						}
						dummy.pop();
					}
					
				}
				//Border pertama objek lain 
				else if (priority_container.front() > polygonMatrix[i][j].top()){
					cout << "n2 " << priority_container.front() << " " << polygonMatrix[i][j].top()<< endl;
					int t; cin >> t;
					priority_queue<int> dummy = polygonMatrix[i][j];
					while (!dummy.empty()){
						auto idx = find(priority_container.begin(),priority_container.end(),dummy.top());
						if (idx != priority_container.end()){
							cout << "erase" << endl;
							priority_container.erase(idx);
						}
						dummy.pop();
					};
					cout << "push " << polygonMatrix[i][j].top() << endl;
					priority_container.push_back(polygonMatrix[i][j].top());
					sort(priority_container.begin(),priority_container.end());
					reverse(priority_container.begin(),priority_container.end());
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}
				//Border pertama objek lain 
				else if (priority_container.front() < polygonMatrix[i][j].top()){
					cout << "n3 " << priority_container.front() << " " << polygonMatrix[i][j].top()<< endl;
					int t; cin >> t;
					// cout << "push " << polygonMatrix[i][j].top() << endl;
					// priority_container.push_back(polygonMatrix[i][j].top());
					// sort(priority_container.begin(),priority_container.end());
					// reverse(priority_container.begin(),priority_container.end());
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
				}

				//Hapus dari priority container apabila sudah menemui ujung kanannya
				// int k = 0;
				// for (auto iter= priority_container.begin(); iter != priority_container.end(); iter++){
				// 	priority_queue<int> dummy = polygonMatrix[i][j];
				// 	while (!dummy.empty()){
				// 		int dum = dummy.top();
				// 		dummy.pop(); 
				// 		if (dum == *iter){
				// 			priority_container.erase(priority_container.begin()+k);
				// 		}
				// 	}
				// 	k++;
				// }

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
	
	// KASUS INI ENGGAK
	c.r = 5;
	c.g = 32;
	c.b = 164;
	drawKotak(0,40,c,1);

	c.r = 45;
	c.g = 255;
	c.b = 64;
	drawKotak(-70,30,c,2);


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
	// drawKotak(60,80,c,2);


	
	showOverlap(); 
	return 0;
}
