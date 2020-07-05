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

COLORREF colors[16] = { RED, BLUE, GREEN, YELLOW, BLACK, LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, WHITE };
int times[5] = {50, 20, 5, 1, 0};

int mem[HANGSHU][LIESHU] = {0};
int shapeState = 1;
int colorState = 1;
int suduState = 1;
int timeDelay = 10;
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

void MID_line(int x1, int y1, int x2, int y2)
{
	int increX = 1, increY = 1, xi, yi;
	int a = y1 - y2, b = x2 - x1;
	if (a * a > b * b) {
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

void MID_eclipse(int xc, int yc, int a, int b)
{
	if (xc + a > LIESHU || xc - a < 0 || yc + b > HANGSHU || yc - b < 0)
		return;
	int xi, yi;
	int aa = a * a, bb = b * b;
	int d = (int)(bb + aa * (-b + 0.25)); 
	double x, y;
	for (xi = xc, yi = yc + b; bb * (xi - xc) <= aa * (yi - yc) + 10; xi++) {
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
	mem[yi][xi] = 1;
	mem[yc * 2 - yi][xi] = 1;
	mem[yi][xc * 2 - xi] = 1;
	mem[yc * 2 - yi][xc * 2 - xi] = 1;
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
	mem[yi][xi] = 1;
	mem[yc * 2 - yi][xi] = 1;
	mem[yi][xc * 2 - xi] = 1;
	mem[yc * 2 - yi][xc * 2 - xi] = 1;
}

void changeFunc()
{
	int a = -1;
	TCHAR s[50];
	USES_CONVERSION;
	while (a != 0 && a != 1 && a != 2 && a != 3 && a!=4)
	{
		InputBox(s, 50, _T("请选择形状(0：退出,1:三角形,2:四边形,3:圆,4:椭圆,右键重选):"));
		char* str = W2A(s);
		sscanf(str, "%d", &a);
	}
	shapeState = a;
	if (shapeState == 0)
		return;
	a = -1;
	InputBox(s, 40, _T("请选择颜色(0：红色,1:蓝色,2:绿色,3:黄色,4:黑色,5:灰色):"));
	char* str = W2A(s);
	sscanf(str, "%d", &a);
	colorState = a;
	InputBox(s, 40, _T("请选择填充速度(0：慢,1:较慢,2:快,3:很快,4:最佳):"));
	str = W2A(s);
	sscanf(str, "%d", &a);
	suduState = a;
	return;
}

void drawShape()
{
	for (int i = 0; i < HANGSHU; i++) {
		for (int j = 0; j < LIESHU; j++) {
			mem[i][j] = 0;
		}
	}
	if (shapeState == 1)
	{
		MID_line(48, 20, 58, 50);
		MID_line(48, 20, 28, 50);
		MID_line(28, 50, 58, 50);
	}
	else if (shapeState == 2)
	{
		MID_line(30, 20, 60, 20);
		MID_line(60, 20, 60, 50);
		MID_line(60, 50, 30, 50);
		MID_line(30, 50, 30, 20);
	}
	else if (shapeState == 3)
	{
		MID_circle(60, 40, 20);
	}
	else if (shapeState == 4)
	{
		MID_eclipse(60, 40, 30, 16);
	}
}

void seedFill(int x0, int y0)
{
	if (x0 < 0 || y0 < 0 || x0 >= LIESHU || y0 >= HANGSHU || mem[y0][x0] > 0)
		return;
	setfillcolor(colors[colorState]);
	Sleep(times[suduState]);
	mem[y0][x0] = 2;
	solidcircle(x0 * SHUMI, y0 * SHUMI, SHUMI/2);
	seedFill(x0 + 1, y0);
	seedFill(x0 - 1, y0);
	seedFill(x0, y0 + 1);
	seedFill(x0, y0 - 1);
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
	if (shapeState == 0)
		return 0;
	drawShape();
	drawRegion(buffer);
	int mouseState = 0;
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			mouseState = 1;
			startX = m.x / SHUMI;
			startY = m.y / SHUMI;
			if (m.x - startX * SHUMI > SHUMI / 2)
				startX ++;
			if (m.y - startY * SHUMI > SHUMI / 2)
				startY ++;
			seedFill(startX, startY);
		}

		else if (m.uMsg == WM_RBUTTONDOWN) {
			changeFunc();
			if (colorState == 0)
				return 0;
			drawShape();
			drawRegion(buffer);
		}
	}
	return 0;
}
