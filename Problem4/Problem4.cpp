
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <graphics.h>
#include <math.h>
#include <conio.h>
#include <string>
#include <atlconv.h>
#include <Windows.h>
#include <vector>

using namespace std;

const int WIDTH = 960;
const int HEIGHT = 640;
const int SHUMI = 8;
const int HANGSHU = HEIGHT / SHUMI;
const int LIESHU = WIDTH / SHUMI;

int mem[HANGSHU][LIESHU] = {0};
int functionState = 1;
int methodState = 1;
int timeDelay = 100;
vector<vector<int>> points;

//画界面
void drawRegion(IMAGE & buffer)
{
	SetWorkingImage(&buffer);
	setfillcolor(WHITE);
	fillrectangle(0, 0, WIDTH - 1, WIDTH - 1);

	for (int i = 0; i < WIDTH; i += SHUMI) {
		for (int j = 0; j < WIDTH; j += SHUMI) {
			setlinecolor(BLACK);
			line(0, j, WIDTH, j);
			line(i, 0, i, WIDTH);
		}
	}
	for (int i = 0; i < HANGSHU; i++) {
		for (int j = 0; j < LIESHU; j++) {
			if (mem[i][j] == 1) {
				setfillcolor(RED);
				fillcircle(j * SHUMI, i * SHUMI, SHUMI/2);
			}
			if (mem[i][j] == 2) {
				setfillcolor(BLUE);
				fillcircle(j * SHUMI, i * SHUMI, SHUMI/2);
			}
		}
	}
	SetWorkingImage(NULL);
	putimage(0, 0, &buffer);
	return;
}


void MID_line(int x1, int y1, int x2, int y2)
{
	int increX = 1, increY = 1, xi, yi;
	int a = y1 - y2, b = x2 - x1;
	if (a * a > b * b)
	{
		if (y1 > y2) {
			int t = x1; x1 = x2; x2 = t;
			t = y1; y1 = y2; y2 = t;
		}
		a = y1 - y2, b = x2 - x1;
		if (x1 <= x2) {
			int d0 = 2 * b + a, d1 = 2 * b, d2 = 2 * (b + a);
			xi = x1;
			for (yi = y1; yi <= y2; yi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				if (d0 > 0) {
					xi++;
					d0 += d2;
				}
				else {
					d0 += d1;
				}
			}
		}
		else {
			int d0 = 2 * b - a, d1 = 2 * b, d2 = 2 * (b - a);
			xi = x1;
			for (yi = y1; yi <= y2; yi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				if (d0 < 0) {
					xi--;
					d0 += d2;
				}
				else {
					d0 += d1;
				}
			}
		}
	}
	else {
		if (x1 > x2) {
			int t = x1; x1 = x2; x2 = t;
			t = y1; y1 = y2; y2 = t;
		}
		a = y1 - y2, b = x2 - x1;
		if (y1 <= y2) {
			int d0 = 2 * a + b, d1 = 2 * a, d2 = 2 * (a + b);
			yi = y1;
			for (xi = x1; xi <= x2; xi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				if (d0 < 0) {
					yi++;
					d0 += d2;
				}
				else {
					d0 += d1;
				}
			}
		}
		else {
			int d0 = 2 * a - b, d1 = 2 * a, d2 = 2 * (a - b);
			yi = y1;
			for (xi = x1; xi <= x2; xi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				if (d0 > 0) {
					yi--;
					d0 += d2;
				}
				else {
					d0 += d1;
				}
			}
		}
	}
	return;
}

int fact(int n)
{
    if(n>1)
        return n*fact(n-1);
    else if(n>=0)
        return 1;
	return -1;
}

void bezier(vector<vector<int>> points)
{
    float u,x,y,b;
	int n = points.size();

    // print first point
	mem[points[0][1]][points[0][0]] = 2;

    // get factorial of n-1
    int nfact=fact(n-1);

    // Run loop from 0.0001 to 1
    for(u=0.0001;u<1;u+=0.0001)
    {
        // Initially x and y = 0
        x=0;
        y=0;

        // calculate wrt the formula for all n points
        for(int i=0;i<n;i++)
        {
            /*
                formula:

                b(u) =     n! * (1-u)^(n-i) * u^i
                        -------
                        i!(n-i)!
            */

            // here n-1 bcoz swag

            b=(nfact*pow(1-u,n-1-i)*pow(u,i))/(fact(n-1-i)*fact(i));

            // update x and y
			x += b * points[i][0];
			y += b * points[i][1];
        }

        // print x and y
		int xx = (int)x, yy = (int)y;
		mem[yy][xx] = 2;
    }

    // print last point
	mem[points[n-1][1]][points[n-1][0]] = 2;
}


int main()
{
	MOUSEMSG m;
	IMAGE buffer(WIDTH, HEIGHT);
	int xLeft, yUp, xRight, yDown;
	int x1, y1, x2, y2, len;
	initgraph(WIDTH , HEIGHT);
	drawRegion(buffer);
	setlinecolor(BLACK);
	setfillcolor(RED);
	functionState = 1;
	int state = 0;
	int startX = 0, startY = 0, endX, endY;
	while (1)
	{
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 32) {
				for (int i = 0; i < HANGSHU; i++) {
					for (int j = 0; j < LIESHU; j++) {
						mem[i][j] = 0;
					}
				}
				drawRegion(buffer);
				points.clear();
				state = 0;
			}
		}
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP) {
			endX = m.x / SHUMI;
			endY = m.y / SHUMI;
			if (m.x - endX * SHUMI > SHUMI / 2)
				endX ++;
			if (m.y - endY * SHUMI > SHUMI / 2)
				endY ++;
			vector<int> p = { {endX, endY} };
			points.push_back(p);
			if(state == 1)
				MID_line(startX, startY, endX, endY);
			startX = endX;
			startY = endY;
			state = 1;
		}
		else if (m.uMsg == WM_RBUTTONUP) {
			bezier(points);
			setlinecolor(BLACK);
			setfillcolor(RED);
			drawRegion(buffer);
		}
	}
	return 0;
}
