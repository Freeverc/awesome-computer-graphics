
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
vector<vector<vector<int>>> points;

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
			if (mem[i][j] != 0) {
				setfillcolor(RED);
				fillcircle(j * SHUMI, i * SHUMI, SHUMI/4);
			}
		}
	}
	SetWorkingImage(NULL);
	putimage(0, 0, &buffer);
	return;
}

void drawShape()
{
	for (int i = 0; i < points.size(); i++) {
		for (int j = 0; j < points[i].size(); j++) {
			setfillcolor(GREEN);
			setcolor(BLACK);
			fillcircle(points[i][j][1] * SHUMI, points[i][j][0] * SHUMI, SHUMI / 4);
		}
		Sleep(timeDelay);
	}
	return;
}

void Simple_line(int x1, int y1, int x2, int y2)
{
	int incre = 1, xi, yi;
	if (x1 == x2 
		|| (double)(y1 - y2) / (double)(x1 - x2) < -1 
		|| (double)(y1 - y2) / (double)(x1 - x2) > 1) {
		double t = (double)(x1 - x2) / (double)(y1 - y2);
		double x = 0;
		if (y1 > y2)
			incre = -1;
		for (yi = y1; yi != y2; yi += incre) {
			x = t * (yi - y1) + x1;
			xi = (int)x;
			mem[yi][xi] = 1;
		}
	}
	else {
		double k = (double)(y1 - y2) / (double)(x1 - x2);
		double y = 0;
		if (x1 > x2)
			incre = -1;
		for (xi = x1; xi != x2; xi += incre) {
			y = k * (xi - x1) + y1;
			yi = (int)y;
			mem[yi][xi] = 1;
		}
	}
	return;
}

void DDA_line(int x1, int y1, int x2, int y2)
{
	int a = y1 - y2, b = x1 - x2;
	int incre = 1, xi, yi;
	if (a * a > b * b) {
		if (y1 > y2) {
			int t = x1; x1 = x2; x2 = t;
			t = y1; y1 = y2; y2 = t;
		}
		double t = (double)(x1 - x2) / (double)(y1 - y2);
		double x = 0;
		for (x = x1, yi = y1; yi != y2; yi ++, x += t) {
			xi = (int)x;
			mem[yi][xi] = 1;
		}
	}
	else {
		if (x1 > x2) {
			int t = x1; x1 = x2; x2 = t;
			t = y1; y1 = y2; y2 = t;
		}
		double k = (double)(y1 - y2) / (double)(x1 - x2);
		double y;
		for (xi = x1, y = y1; xi != x2; xi ++, y+=k) {
			yi = (int)y;
			mem[yi][xi] = 1;
		}
	}
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
			for (yi = y1; yi != y2; yi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
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
			for (yi = y1; yi != y2; yi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
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
			for (xi = x1; xi != x2; xi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
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
			for (xi = x1; xi != x2; xi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
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

void Bresenham_line(int x1, int y1, int x2, int y2)
{
	int increX = 1, increY = 1, xi, yi;
	int dx = x2 - x1, dy = y2 - y1;
	if (dx * dx <= dy * dy) {
		if (y1 > y2) {
			int t = x1; x1 = x2; x2 = t;
			t = y1; y1 = y2; y2 = t;
		}
		dx = x2 - x1, dy = y2 - y1;
		if (x1 <= x2) {
			int d = dy - 2 * dx;
			for (yi = y1, xi = x1; yi <= y2; yi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
				if (d > 0) {
					d -= 2 * dx;
				}
				else {
					xi++;
					d += 2 * (dy - dx);
				}
			}
		}
		else {
			int d = dy + 2 * dx;
			for (xi = x1, yi = y1; yi <= y2; yi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
				if (d < 0) {
					d -= 2 * dx;
				}
				else {
					xi--;
					d -= 2 * (dy + dx);
				}
			}
		}
	}
	else {
		if (x1 > x2) {
			int t = x1; x1 = x2; x2 = t;
			t = y1; y1 = y2; y2 = t;
		}
		dx = x2 - x1, dy = y2 - y1;
		if (y1 <= y2) {
			int d = 2 * dy - dx;
			for (xi = x1, yi = y1; xi != x2; xi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
				if (d < 0) {
					d += 2 * dy;
				}
				else {
					yi++;
					d += 2 * (dy - dx);
				}
			}
		}
		else {
			int d = 2 * dy + dx;
			for (xi = x1, yi = y1; xi <= x2; xi ++) {
				mem[yi][xi] = 1;
				vector<vector<int>> p = { {yi, xi} };
				points.push_back(p);
				if (d > 0) {
					d += 2 * dy;
				}
				else {
					yi--;
					d += 2 * (dy + dx);
				}
			}
		}
	}
	return;
}

void DDA_circle(int xc, int yc, int r)
{
	if (xc + r > LIESHU || xc - r < 0 || yc + r > HANGSHU || yc - r < 0)
		return;
	int xi, yi;
	int rr = r * r;
	double x, y;

	for (xi = xc, yi =  yc + r, y = yc + r; xi - xc <= yi - yc; xi++) {
		mem[yi][xi] = 1;
		mem[yc * 2 - yi][xi] = 1;
		mem[yi][xc * 2 - xi] = 1;
		mem[yc * 2 - yi][xc * 2 - xi] = 1;
		mem[xi - xc + yc][yi - yc + xc] = 1;
		mem[xi-xc+yc][yc  - yi+xc] = 1;
		mem[xc - xi + yc][yi-yc +xc] = 1;
		mem[xc - xi+yc][yc - yi + xc] = 1;
		vector<vector<int>> p = { {yi, xi},{yc * 2 - yi,xi}, {yi, xc * 2 - xi}, {yc * 2 - yi, xc * 2 - xi},
			{xi - xc + yc,yi - yc + xc}, {xi - xc + yc, yc - yi + xc}, {xc - xi + yc, yi - yc + xc}, {xc - xi + yc, yc - yi + xc} };
		points.push_back(p);
		y = sqrt(rr - (xi - xc) * (xi - xc));
		yi = (int)(y+0.5) + yc;
	}
}

void MID_circle(int xc, int yc, int r)
{
	if (xc + r > LIESHU || xc - r < 0 || yc + r > HANGSHU || yc - r < 0)
		return;
	int xi, yi;
	int rr = r * r;
	int d =  5 - 4 * r;
	double x, y;

	for (xi = xc, yi =  yc + r, y = yc + r; xi - xc <= yi - yc; xi++) {
		mem[yi][xi] = 1;
		mem[yc * 2 - yi][xi] = 1;
		mem[yi][xc * 2 - xi] = 1;
		mem[yc * 2 - yi][xc * 2 - xi] = 1;
		mem[xi - xc + yc][yi - yc + xc] = 1;
		mem[xi-xc+yc][yc  - yi+xc] = 1;
		mem[xc - xi + yc][yi-yc +xc] = 1;
		mem[xc - xi+yc][yc - yi + xc] = 1;
		vector<vector<int>> p = { {yi, xi},{yc * 2 - yi,xi}, {yi, xc * 2 - xi}, {yc * 2 - yi, xc * 2 - xi},
			{xi - xc + yc,yi - yc + xc}, {xi - xc + yc, yc - yi + xc}, {xc - xi + yc, yi - yc + xc}, {xc - xi + yc, yc - yi + xc} };
		points.push_back(p);
		if (d < 0)
			d += 8 * (xi - xc)+ 12;
		else {
			d += 8 * (xi - xc) + 12 + 8 - 8 * (yi - yc);
			yi--;
		}
	}
}

void DDA_eclipse(int xc, int yc, int a, int b)
{
	if (xc + a > LIESHU || xc - a < 0 || yc + b > HANGSHU || yc - b < 0)
		return;
	int xi, yi;
	int aa = a * a, bb = b * b;
	double x, y;
	for (xi = xc, yi = yc + b; bb * (xi - xc) <= aa * (yi - yc); xi++) {
		y = sqrt(1 - (double)(xi - xc) * (xi - xc) / a / a) * b;
		yi = (int)(y+0.5) + yc;
		mem[yi][xi] = 1;
		mem[yc * 2 - yi][xi] = 1;
		mem[yi][xc * 2 - xi] = 1;
		mem[yc * 2 - yi][xc * 2 - xi] = 1;
		vector<vector<int>> p = { {yi, xi},{yc * 2 - yi,xi}, {yi, xc * 2 - xi}, {yc * 2 - yi, xc * 2 - xi} };
		points.push_back(p);
	}
	for (; yi - yc >= 0; yi--) {
		x = sqrt(1 - (double)(yi - yc) * (yi - yc) / b / b) * a;
		xi = (int)(x+0.5) + xc;
		mem[yi][xi] = 1;
		mem[yc * 2 - yi][xi] = 1;
		mem[yi][xc * 2 - xi] = 1;
		mem[yc * 2 - yi][xc * 2 - xi] = 1;
		vector<vector<int>> p = { {yi, xi},{yc * 2 - yi,xi}, {yi, xc * 2 - xi}, {yc * 2 - yi, xc * 2 - xi} };
		points.push_back(p);
	}
}

void MID_eclipse(int xc, int yc, int a, int b)
{
	if (xc + a > LIESHU || xc - a < 0 || yc + b > HANGSHU || yc - b < 0)
		return;
	int xi, yi;
	int aa = a * a, bb = b * b;
	int d = (int)(bb + aa * (-b + 0.25)); 
	double x, y;
	for (xi = xc, yi = yc + b; bb * (xi - xc) <= aa * (yi - yc); xi++) {
		mem[yi][xi] = 1;
		mem[yc * 2 - yi][xi] = 1;
		mem[yi][xc * 2 - xi] = 1;
		mem[yc * 2 - yi][xc * 2 - xi] = 1;
		vector<vector<int>> p = { {yi, xi},{yc * 2 - yi,xi}, {yi, xc * 2 - xi}, {yc * 2 - yi, xc * 2 - xi} };
		points.push_back(p);
		if (d < 0)
			d += bb * (2 * (xi - xc) + 3);
		else {
			yi--;
			d += bb * (2 * (xi - xc) + 3) + aa * (2 - 2 * (yi - yc));
		}
	}
	d = (int)(aa + bb * (-a + 0.25)); 
	for (xi = xc + a, yi = yc;bb * (xi - xc) >= aa * (yi - yc); yi++) {
		mem[yi][xi] = 1;
		mem[yc * 2 - yi][xi] = 1;
		mem[yi][xc * 2 - xi] = 1;
		mem[yc * 2 - yi][xc * 2 - xi] = 1;
		vector<vector<int>> p = { {yi, xi},{yc * 2 - yi,xi}, {yi, xc * 2 - xi}, {yc * 2 - yi, xc * 2 - xi} };
		points.push_back(p);
		if (d < 0)
			d += aa * (2 * (yi - yc) + 3);
		else {
			xi--;
			d += aa * (2 * (yi - yc) + 3) + bb * (2 - 2 * (xi - xc));
		}
	}
}

void changeFunc()
{
	int a = -1;
	TCHAR s[40];
	USES_CONVERSION;
	while (a != 0 && a != 1 && a != 2 && a != 3)
	{
		InputBox(s, 40, _T("请选择功能(0：退出,1:直线,2:圆,3:椭圆,右键重选):"));
		char* str = W2A(s);
		sscanf(str, "%d", &a);
	}
	functionState = a;
	if (functionState == 0)
		return;
	a = -1;
	while (a != 0 && a != 1 && a != 2 && a != 3)
	{
		InputBox(s, 40, _T("请选择算法(0：暴力绘制法,1:DDA,2:中点法,3:Bresenham):"));
		char* str = W2A(s);
		sscanf(str, "%d", &a);
	}
	methodState = a;
	return;
}

int main()
{
	MOUSEMSG m;
	IMAGE buffer(WIDTH, HEIGHT);
	int heiBai = 1, gameState = 0;
	int startX, startY, endX, endY;
	initgraph(WIDTH , HEIGHT);
	drawRegion(buffer);
	changeFunc();
	if (functionState == 0)
		return 0;
	int mouseState = 0;
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
				drawShape();
			}
		}
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			mouseState = 1;
			startX = m.x / SHUMI;
			startY = m.y / SHUMI;
			if (m.x - startX * SHUMI > SHUMI / 2)
				startX ++;
			if (m.y - startY * SHUMI > SHUMI / 2)
				startY ++;
			mem[startY][startX] = 1;
			setfillcolor(RED);
			fillcircle(startX * SHUMI, startY * SHUMI, SHUMI/4);
		}

		else if (m.uMsg == WM_LBUTTONUP) {
			mouseState = 0;
			points.clear();
			endX = m.x / SHUMI;
			endY = m.y / SHUMI;
			if (m.x - endX * SHUMI > SHUMI / 2)
				endX ++;
			if (m.y - endY * SHUMI > SHUMI / 2)
				endY ++;
			mem[endY][endX] = heiBai;
			setfillcolor(RED);
			fillcircle(endX * SHUMI, endY * SHUMI, SHUMI/4);
			if (functionState == 1)
			{
				if(methodState == 0)
					Simple_line(startX, startY, endX, endY);
				if(methodState == 1)
					DDA_line(startX, startY, endX, endY);
				if(methodState == 2)
					MID_line(startX, startY, endX, endY);
				if(methodState == 3)
					Bresenham_line(startX, startY, endX, endY);
			}
			if (functionState == 2)
			{
				int r;
				TCHAR s[10];
				InputBox(s, 10, _T("请输入半径"));
				USES_CONVERSION;
				char* str = W2A(s);
				sscanf(str, "%d", &r);
				if(methodState == 0 || methodState == 1)
					DDA_circle(startX, startY, r);
				if(methodState == 2 || methodState == 3)
					MID_circle(startX, startY, r);
			}
			if (functionState == 3)
			{
				int a, b;
				TCHAR s[10];
				USES_CONVERSION;
				InputBox(s, 10, _T("请输入a:"));
				char* str = W2A(s);
				sscanf(str, "%d", &a);
				InputBox(s, 10, _T("请输入b:"));
				str = W2A(s);
				sscanf(str, "%d", &b);
				if(methodState == 0 || methodState == 1)
					DDA_eclipse(startX, startY, a, b);
				if(methodState == 2 || methodState == 3)
					MID_eclipse(startX, startY, a, b);
			}
			drawRegion(buffer);
		}
		else if (m.uMsg == WM_MOUSEMOVE)
		{
			if (mouseState == 1)
			{
				int currX = m.x / SHUMI;
				int currY = m.y / SHUMI;
				if (m.x - currX * SHUMI > SHUMI / 2)
					currX++;
				if (m.y - currY * SHUMI > SHUMI / 2)
					currY++;
				setfillcolor(BLUE);
				setcolor(BLACK);
				fillcircle(currX * SHUMI, currY * SHUMI, SHUMI/4);
				
			}
		}
		else if (m.uMsg == WM_RBUTTONUP) {
			changeFunc();
			if (functionState == 0)
				return 0;
		}
	}
	return 0;
}
