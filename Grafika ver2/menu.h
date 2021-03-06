#ifndef MENU_H
#define MENU_H

#include "Line.h"
#include <ctime>
#include <cstdlib>

using namespace std;

class Menu : public Framebuffer {
	private:
		Line l;
		int lvl;
	public:
		int getLevel(){
			if (lvl==1) return 50000;
			if (lvl==2) return 20000;
			if (lvl==3) return 5000;	
		}
		void pilihan1(){
			system("clear");
			cout<<"CHOOSE THE DIFFICULTY LEVEL=============="<<endl;
			cout<<">Heaven"<<endl;
			cout<<" Easy"<<endl;
			cout<<" Hard"<<endl;
		}
		void pilihan2(){
			system("clear");
			cout<<"CHOOSE THE DIFFICULTY LEVEL=============="<<endl;
			cout<<" Heaven"<<endl;
			cout<<">Easy"<<endl;
			cout<<" Hard"<<endl;
		}
		void pilihan3(){
			system("clear");
			cout<<"CHOOSE THE DIFFICULTY LEVEL=============="<<endl;
			cout<<" Heaven"<<endl;
			cout<<" Easy"<<endl;
			cout<<">Hard"<<endl;
		}
		void writeTitle(int r, int g, int b){
			l.insertLine(50, 110, 300, 110, r,g,b);
			l.insertLine(125, 210, 725, 210, r,g,b);
			//U
			l.insertLine(100, 50, 50, 100, r,g,b);
			l.insertLine(50, 100, 100, 100, r,g,b);
			l.insertLine(100, 100, 150, 50, r,g,b);

			//F
			l.insertLine(200, 50, 150, 100, r,g,b);
			l.insertLine(200, 50, 250, 50, r,g,b);
			l.insertLine(175, 75, 225, 75, r,g,b);

			//O
			l.insertLine(300, 50, 250, 100, r,g,b);
			l.insertLine(300, 50, 350, 50, r,g,b);
			l.insertLine(250, 100, 300, 100, r,g,b);
			l.insertLine(300, 100, 350, 50, r,g,b);

			//S
			l.insertLine(150, 150, 125, 175, r,g,b);
			l.insertLine(150, 150, 200, 150, r,g,b);
			l.insertLine(125, 175, 175, 175, r,g,b);
			l.insertLine(175, 175, 150, 200, r,g,b);
			l.insertLine(150, 200, 100, 200, r,g,b);

			//H
			l.insertLine(250, 150, 200, 200, r,g,b);
			l.insertLine(225, 175, 275, 175, r,g,b);
			l.insertLine(300, 150, 250, 200, r,g,b);
			
			//O
			l.insertLine(350, 150, 300, 200, r,g,b);
			l.insertLine(350, 150, 400, 150, r,g,b);
			l.insertLine(300, 200, 350, 200, r,g,b);
			l.insertLine(350, 200, 400, 150, r,g,b);
			//O
			l.insertLine(450, 150, 400, 200, r,g,b);
			l.insertLine(450, 150, 500, 150, r,g,b);
			l.insertLine(400, 200, 450, 200, r,g,b);
			l.insertLine(450, 200, 500, 150, r,g,b);
			
			//T
			l.insertLine(525, 150, 575, 150, r,g,b);
			l.insertLine(550, 150, 500, 200, r,g,b);
			
			//E
			l.insertLine(600, 150, 550, 200, r,g,b);
			l.insertLine(600, 150, 650, 150, r,g,b);
			l.insertLine(575, 175, 625, 175, r,g,b);
			l.insertLine(550, 200, 600, 200, r,g,b);

			//R
			l.insertLine(675, 150, 725, 150, r,g,b);
			l.insertLine(675, 150, 625, 200, r,g,b);
			l.insertLine(650, 175, 700, 175, r,g,b);
			l.insertLine(725, 150, 700, 175, r,g,b);
			l.insertLine(700, 175, 710, 200, r,g,b);

		}

		void writeTitle2(int r, int g, int b){
			int x = 50 ;
			int y = 5 ; 
			r = 0;
			g = 255;
			b= 0;
			l.insertLine(x+50, y+110, x+300, y+110, r,g,b);
			l.insertLine(x+125, y+210, x+725, y+210, r,g,b);
			
			x = x + 50;
			y = y + 5;
			l.insertLine(x+50, y+110, x+300, y+110, 0,0,255);
			l.insertLine(x+125, y+210, x+725, y+210, 0,0,255);
			/*
			//U
			l.insertLine(x+100, y+50, x+50, y+100, r,g,b);
			l.insertLine(x+50, y+100, x+100, y+100, r,g,b);
			l.insertLine(x+100, y+100, x+150, y+50, r,g,b);

			//F
			l.insertLine(x+200, y+50, x+150, y+100, r,g,b);
			l.insertLine(x+200, y+50, x+250, y+50, r,g,b);
			l.insertLine(x+175, y+75, x+225, y+75, r,g,b);

			//O
			l.insertLine(x+300, y+50, x+250, y+100, r,g,b);
			l.insertLine(x+300, y+50, x+350, y+50, r,g,b);
			l.insertLine(x+250, y+100, x+300, y+100, r,g,b);
			l.insertLine(x+300, y+100, x+350, y+50, r,g,b);

			//S
			l.insertLine(x+150, y+150, x+125, y+175, r,g,b);
			l.insertLine(x+150, y+150, x+200, y+150, r,g,b);
			l.insertLine(x+125, y+175, x+175, y+175, r,g,b);
			l.insertLine(x+175, y+175, x+150, y+200, r,g,b);
			l.insertLine(x+150, y+200, x+100, y+200, r,g,b);

			//H
			l.insertLine(x+250, y+150, x+200, y+200, r,g,b);
			l.insertLine(x+225, y+175, x+275, y+175, r,g,b);
			l.insertLine(x+300, y+150, x+250, y+200, r,g,b);
			
			//O
			l.insertLine(x+350, y+150, x+300, y+200, r,g,b);
			l.insertLine(x+350, y+150, x+400, y+150, r,g,b);
			l.insertLine(x+300, y+200, x+350, y+200, r,g,b);
			l.insertLine(x+350, y+200, x+400, y+150, r,g,b);
			//O
			l.insertLine(x+450, y+150, x+400, y+200, r,g,b);
			l.insertLine(x+450, y+150, x+500, y+150, r,g,b);
			l.insertLine(x+400, y+200, x+450, y+200, r,g,b);
			l.insertLine(x+450, y+200, x+500, y+150, r,g,b);
			
			//T
			l.insertLine(x+525, y+150, x+575, y+150, r,g,b);
			l.insertLine(x+550, y+150, x+500, y+200, r,g,b);
			
			//E
			l.insertLine(x+600, y+150, x+550, y+200, r,g,b);
			l.insertLine(x+600, y+150, x+650, y+150, r,g,b);
			l.insertLine(x+575, y+175, x+625, y+175, r,g,b);
			l.insertLine(x+550, y+200, x+600, y+200, r,g,b);

			//R
			l.insertLine(x+675, y+150, x+725, y+150, r,g,b);
			l.insertLine(x+675, y+150, x+625, y+200, r,g,b);
			l.insertLine(x+650, y+175, x+700, y+175, r,g,b);
			l.insertLine(x+725, y+150, x+700, y+175, r,g,b);
			l.insertLine(x+700, y+175, x+710, y+200, r,g,b);
			*/
		}

	void writeMenu(){
		initscr();
		keypad(stdscr,TRUE);
		noecho();
		char ch;
			
		int r=250,g=0,b=0;
		while (r<=280){
			r++;
			writeTitle(250,0,0);
			writeTitle2(0,250,0);
			usleep(100000);
			system("clear");
			usleep(50000);
			cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<"Press S or W to select dificulty"<<endl;
		}
				
	
		system("clear");
		
		/*
		printf("\n");
		printf("*************************************************************\n");
		printf("*                UFO SHOOTER - UTS GRAFIKA                  *\n");
		printf("*               ____________________________                *\n");
		printf("*                                                           *\n");
		printf("*                     3 different stage                     *\n");
		printf("*                                                           *\n");
		printf("*************************************************************\n");
		printf("\n");
		printf("\n");
		*/		
		printf(" CHOOSE THE DIFFICULTY LEVEL");
		printf("\n>Heaven\n");
		printf(" Easy\n");
		printf(" Hard\n");
			
		int pil=1;
		ch=getch();
		while (ch!='b'){
			if (ch=='s'){//bawah
				if(pil==1) {pil=2;pilihan2();}
				else if (pil==2) {pil=3;pilihan3();}			
			}else if (ch='w'){
				if(pil==3) {pil=2;pilihan2();}
				else if (pil==2) {pil=1;pilihan1();}
			}

			ch=getch();
		}
		lvl=pil;		
	}
};

#endif
