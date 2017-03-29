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

std::vector<int> polygonMatrix[MATSIZE][MATSIZE];
std::vector<color> color_container;
std::vector<int> priority_container;
list<int> colorMode;


void insertPriority(int j,int i, int p){
		polygonMatrix[i][j].push_back(p);
		sort(polygonMatrix[i][j].begin(),polygonMatrix[i][j].end());
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

// void showOverlap(){
// 	int current_priority = -1;
// 	Framebuffer fb;
// 	for (int i=0;i<MATSIZE;i++){//y
// 		for (int j=0;j<MATSIZE;j++){//x
// 			if (polygonMatrix[i][j].empty()){
// 				if (priority_container.empty()){
// 					//kosong
// 				}
// 				else {
// 					auto elmt = color_container.at(priority_container.front()-1);
// 					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
// 				}
// 			}
// 			else {
// 				//Border Pertama ketemu
// 				cout << "notempty ";
// 				if (priority_container.empty()){
// 					cout << "empty" << endl;
// 					priority_queue<int> dummy = polygonMatrix[i][j];
// 					while (!dummy.empty()){
// 						int iter = dummy.top();
// 						dummy.pop(); 
// 						cout << "push " << polygonMatrix[i][j].top() << endl;
// 						priority_container.push_back(iter);
// 						sort(priority_container.begin(),priority_container.end());
// 						reverse(priority_container.begin(),priority_container.end());
// 					}
// 					auto elmt = color_container.at(priority_container.front()-1);
// 					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
// 				}
// 				//Border terluar same object, ga bentrok
// 				else if (priority_container.front() == polygonMatrix[i][j].top()){
// 					cout << "n1" << endl;
// 					int t; cin >> t;
// 					auto elmt = color_container.at(priority_container.front()-1);
// 					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
// 					priority_queue<int> dummy = polygonMatrix[i][j];
// 					while (!dummy.empty()){
// 						auto idx = find(priority_container.begin(),priority_container.end(),dummy.top());
// 						if (idx != priority_container.end()){
// 							cout << "erase" << endl;
// 							priority_container.erase(idx);
// 						}
// 						dummy.pop();
// 					}
					
// 				}
// 				//Border pertama objek lain 
// 				else if (priority_container.front() > polygonMatrix[i][j].top()){
// 					cout << "n2 " << priority_container.front() << " " << polygonMatrix[i][j].top()<< endl;
// 					int t; cin >> t;
// 					priority_queue<int> dummy = polygonMatrix[i][j];
// 					while (!dummy.empty()){
// 						auto idx = find(priority_container.begin(),priority_container.end(),dummy.top());
// 						if (idx != priority_container.end()){
// 							cout << "erase" << endl;
// 							priority_container.erase(idx);
// 						}
// 						dummy.pop();
// 					};
// 					cout << "push " << polygonMatrix[i][j].top() << endl;
// 					priority_container.push_back(polygonMatrix[i][j].top());
// 					sort(priority_container.begin(),priority_container.end());
// 					reverse(priority_container.begin(),priority_container.end());
// 					auto elmt = color_container.at(priority_container.front()-1);
// 					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
// 				}
// 				//Border pertama objek lain 
// 				else if (priority_container.front() < polygonMatrix[i][j].top()){
// 					cout << "n3 " << priority_container.front() << " " << polygonMatrix[i][j].top()<< endl;
// 					int t; cin >> t;
// 					// cout << "push " << polygonMatrix[i][j].top() << endl;
// 					// priority_container.push_back(polygonMatrix[i][j].top());
// 					// sort(priority_container.begin(),priority_container.end());
// 					// reverse(priority_container.begin(),priority_container.end());
// 					auto elmt = color_container.at(priority_container.front()-1);
// 					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
// 				}

// 				//Hapus dari priority container apabila sudah menemui ujung kanannya
// 				// int k = 0;
// 				// for (auto iter= priority_container.begin(); iter != priority_container.end(); iter++){
// 				// 	priority_queue<int> dummy = polygonMatrix[i][j];
// 				// 	while (!dummy.empty()){
// 				// 		int dum = dummy.top();
// 				// 		dummy.pop(); 
// 				// 		if (dum == *iter){
// 				// 			priority_container.erase(priority_container.begin()+k);
// 				// 		}
// 				// 	}
// 				// 	k++;
// 				// }

// 			}
// 		}
// 	}

// }

bool isExist(int p){
	for (std::list<int>::iterator it = colorMode.begin(); it != colorMode.end(); ++it){
		if (p==*it) return true;		
	}
	return false;
}

bool isExistPList(int row, int col, int p){
	for (int i =0; i<polygonMatrix[col][row].size();i++){
		if (polygonMatrix[col][row][i]==p) return true;
		}
	return false;
}

bool isExistRow(int row,int col,int p){
		for (int j = row;j<MATSIZE;j++)
			if (isExistPList(j,col,p)) return true;
		return false;
	}

void showOverlap(){
	Framebuffer fb1;
	colorMode.push_front(0);
	for (int i=0;i<MATSIZE;i++){//y
		for (int j=0;j<MATSIZE;j++){//x
			if (colorMode.front()==0 && polygonMatrix[i][j][0]==9) int p=0; //Tidak ada mode pewarnaan & idx matrix kosong(tidak di area gambar)
			
			else if (colorMode.front()==0 && polygonMatrix[i][j][0]!=9){//border terluar
				int k =0;
				while (k<polygonMatrix[i][j].size()){
					if (!isExist(polygonMatrix[i][j][k]) && polygonMatrix[i][j][k]!=9 &&  isExistRow(j,i,polygonMatrix[i][j][k])) 
						colorMode.push_front(polygonMatrix[i][j][k]);
					k++;
				}
				colorMode.remove(0);
				colorMode.sort();
				fb1.putPixel(j, i, color_container[colorMode.front()].r,color_container[colorMode.front()].g,color_container[colorMode.front()].b);
				colorMode.push_back(0);
			}
			
			else if (colorMode.front()!=0){//ada mode warna
				if (polygonMatrix[i][j][0]==9) fb1.putPixel(j, i, color_container[colorMode.front()].r,color_container[colorMode.front()].g,color_container[colorMode.front()].b);//warna sel kosong
				else {//sel punya warna
					int priority = polygonMatrix[j][i][0];
					if (priority>colorMode.front()) priority = colorMode.front();
					fb1.putPixel(j, i, color_container[colorMode.front()].r,color_container[colorMode.front()].g,color_container[colorMode.front()].b);
					
					//usleep(50000);
					int k =0;
					while (k<polygonMatrix[i][j].size()){
						if (!isExist(polygonMatrix[i][j][k])) 
							colorMode.push_back(polygonMatrix[i][j][k]);
						else if (isExist(polygonMatrix[i][j][k])) 
							colorMode.remove(polygonMatrix[i][j][k]);
						k++;
					}
					
					colorMode.remove(9);
					colorMode.remove(0);
					colorMode.sort();
					colorMode.push_back(0);				
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

void initializeMatrix(){
	for (int i=0;i<MATSIZE;i++){
		for (int j=0;j<MATSIZE;j++){
			for (int k=0;k<4;k++) polygonMatrix[i][j].push_back(9);	
		}	
	}
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
	initializeMatrix();
	// KASUS INI ENGGAK
	// c.r = 5;
	// c.g = 32;
	// c.b = 164;
	// drawKotak(0,40,c,1);

	// c.r = 45;
	// c.g = 255;
	// c.b = 64;
	// drawKotak(-70,30,c,2);

	c.r = 255;
	c.g = 255;
	c.b = 255;
	drawKotak(50,120,c,1);


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
