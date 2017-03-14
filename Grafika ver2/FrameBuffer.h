#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <bits/stdc++.h>
using namespace std;

class FrameBuffer2 {

	private:
		int fbfd;
	    struct fb_var_screeninfo vinfo;
	    struct fb_fix_screeninfo finfo;
	    long int screensize;
	    char *fbp;

	public:
		FrameBuffer() {
		    // Open the file for reading and writing
		    fbfd = open("/dev/fb0", O_RDWR);
		    if (fbfd == -1) {
		        perror("Error: cannot open FrameBuffer device");
		        exit(1);
		    }

		    // Get fixed screen information
		    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		        perror("Error reading fixed information");
		        exit(2);
		    }

		    // Get variable screen information
		    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		        perror("Error reading variable information");
		        exit(3);
		    }

		    // Figure out the size of the screen in bytes
		    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

		    // Map the device to memory
		    if (!(fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0))) {
		        perror("Error: failed to map FrameBuffer device to memory");
		        exit(4);
		    }

			//reset fb to true black value;
			for(int i = 0; i<vinfo.xres - 5; i++)
				for(int j = 0; j<vinfo.yres - 10; j++)
					put_pixel(i,j,0,0,0);

		}

		~FrameBuffer(){
			close(fbfd);
		}

		void put_pixel(int x, int y, int red, int green, int blue) {
            long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
			//printf("%d,%d\n", x,y);
			// printf("Before - R%i ", (char)*(fbp + location + 0));
			// printf("B%i ", (char)*(fbp + location + 1));
			// printf("R%i\n", (char)*(fbp + location + 2));
			if (vinfo.bits_per_pixel == 32) {
                *(fbp + location + 0) = (char) blue;
                *(fbp + location + 1) = (char) green;
                *(fbp + location + 2) = (char) red;
                *(fbp + location + 3) = (char) 0;
				// printf("After  - R%i ", (char)*(fbp + location + 0));
				// printf("B%i ", (char)*(fbp + location + 1));
				// printf("R%i\n\n", (char)*(fbp + location + 2));
            } else  {
                int b = blue;
                int g = green;
                int r = red;
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }
		}
		int is_black(int x, int y) {
			long int loc = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
			for (int i = 0; i < 3; i++) {

				//printf("v%i\n", *(fbp + loc + i));
				if ((char) *(fbp + loc + i) != 0)
					return 0;
				//*(fbp + loc + i) = 255;
			}
			return 1;
		}

		void draw_line(float x1, float y1,float x2, float y2, int r, int g, int b)
		{
			// first and last pixel
			put_pixel(x1,y1,r,g,b);
			put_pixel(x2,y2,r,g,b);

		    // Bresenham's line algorithm
			const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
			if(steep){
			  std::swap(x1, y1);
			  std::swap(x2, y2);
			}

			if(x1 > x2) {
			  std::swap(x1, x2);
			  std::swap(y1, y2);
			}

			const float dx = x2 - x1;
			const float dy = fabs(y2 - y1);

			float error = dx / 2.0f;
			const int ystep = (y1 < y2) ? 1 : -1;
			int y = (int)y1;

			const int maxX = (int)x2;

			for(int x=(int)x1; x<maxX; x++) {
			  if(steep) {
			      put_pixel(y,x,r,g,b);
			  }
			  else {
			      put_pixel(x,y,r,g,b);
			  }

			  error -= dy;
			  if(error < 0) {
			      y += ystep;
			      error += dx;
			  }
			}
		}

		void flood_fill(int x, int y, int r, int g, int b) {
			if ((is_black(x,y))) {
				put_pixel(x,y,r,g,b);
				//usleep(1000);
				flood_fill(x+1,y,r,g,b);
				flood_fill(x-1,y,r,g,b);
				flood_fill(x,y+1,r,g,b);
				flood_fill(x,y-1,r,g,b);
				
			}
			
		}

		void reset_fill(int x, int y, int r, int g, int b) {
			if (!(is_black(x,y))) {
				put_pixel(x,y,r,g,b);
				//usleep(1000);
				reset_fill(x+1,y,r,g,b);
				reset_fill(x-1,y,r,g,b);
				reset_fill(x,y+1,r,g,b);
				reset_fill(x,y-1,r,g,b);
				
			}
			
		}

		void draw_line3(float x1, float y1,float x2, float y2, int r, int g, int b)
		{
			draw_line(x1*2,y1*2,x2*2,y2*2,r,g,b);
		}

		void flood_fill3(int x, int y, int r, int g, int b) {
			flood_fill(x*2,y*2,r,g,b);
		}
		
		void reset_fill3(int x, int y, int r, int g, int b) {
			reset_fill(x*2,y*2,r,g,b);
		}
		

};



#endif
