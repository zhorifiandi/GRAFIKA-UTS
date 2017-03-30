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
				// cout << "notempty ";
				if (priority_container.empty()){
					// cout << "empty" << endl;
					priority_container.push_back(polygonMatrix[i][j].front());
					cout << polygonMatrix[i][j].front() << endl;
					auto elmt = color_container.at(priority_container.front()-1);
					fb.putPixel(j,i,elmt.r,elmt.g,elmt.b);
					// cout << "ini?" << endl;
				}
				//Ujung dari bordernya
				else if (priority_container.front() == polygonMatrix[i][j].front()){
					// cout << "n1" << endl;
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
					deleteFromPrio(temp);
					// cout <<"len "<< priority_container.size() << endl;
				}
				//Belom sampe ujungnya, tapi nemu ujung prioritas lain
				else {
					//Awal border dari suatu objek
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
		}
	}

}

// bool isExist(int p){
// 	for (std::list<int>::iterator it = colorMode.begin(); it != colorMode.end(); ++it){
// 		if (p==*it) return true;		
// 	}
// 	return false;
// }

// bool isExistPList(int row, int col, int p){
// 	for (int i =0; i<polygonMatrix[col][row].size();i++){
// 		if (polygonMatrix[col][row][i]==p) return true;
// 		}
// 	return false;
// }

// bool isExistRow(int row,int col,int p){
// 		for (int j = row;j<MATSIZE;j++)
// 			if (isExistPList(j,col,p)) return true;
// 		return false;
// 	}

// void showOverlap(){
// 	Framebuffer fb1;
// 	colorMode.push_front(0);
// 	for (int i=0;i<MATSIZE;i++){//y
// 		for (int j=0;j<MATSIZE;j++){//x
// 			if (colorMode.front()==0 && polygonMatrix[i][j][0]==9) int p=0; //Tidak ada mode pewarnaan & idx matrix kosong(tidak di area gambar)
			
// 			else if (colorMode.front()==0 && polygonMatrix[i][j][0]!=9){//border terluar
// 				int k =0;
// 				while (k<polygonMatrix[i][j].size()){
// 					if (!isExist(polygonMatrix[i][j][k]) && polygonMatrix[i][j][k]!=9 &&  isExistRow(j,i,polygonMatrix[i][j][k])) 
// 						colorMode.push_front(polygonMatrix[i][j][k]);
// 					k++;
// 				}
// 				colorMode.remove(0);
// 				colorMode.sort();
// 				fb1.putPixel(j, i, color_container[colorMode.front()].r,color_container[colorMode.front()].g,color_container[colorMode.front()].b);
// 				colorMode.push_back(0);
// 			}
			
// 			else if (colorMode.front()!=0){//ada mode warna
// 				if (polygonMatrix[i][j][0]==9) fb1.putPixel(j, i, color_container[colorMode.front()].r,color_container[colorMode.front()].g,color_container[colorMode.front()].b);//warna sel kosong
// 				else {//sel punya warna
// 					int priority = polygonMatrix[j][i][0];
// 					if (priority>colorMode.front()) priority = colorMode.front();
// 					fb1.putPixel(j, i, color_container[colorMode.front()].r,color_container[colorMode.front()].g,color_container[colorMode.front()].b);
					
// 					//usleep(50000);
// 					int k =0;
// 					while (k<polygonMatrix[i][j].size()){
// 						if (!isExist(polygonMatrix[i][j][k])) 
// 							colorMode.push_back(polygonMatrix[i][j][k]);
// 						else if (isExist(polygonMatrix[i][j][k])) 
// 							colorMode.remove(polygonMatrix[i][j][k]);
// 						k++;
// 					}
					
// 					colorMode.remove(9);
// 					colorMode.remove(0);
// 					colorMode.sort();
// 					colorMode.push_back(0);				
// 				}						
// 			}
// 		}
// 	}
// }

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
	c.r = 58;
	c.g = 132;
	c.b = 94;
	drawKotak(10,80,c,3);


	
	showOverlap(); 
	return 0;
}
