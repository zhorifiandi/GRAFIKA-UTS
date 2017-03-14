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

		void writeMenu(){
			initscr();
	keypad(stdscr,TRUE);
	noecho();
	char ch;
			
			int r=250,g=0,b=0;
			while (r<=260){
				r++;
				writeTitle(250,0,0);
				usleep(100000);
				system("clear");
				usleep(50000);
			}
				
	
			system("clear");
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
