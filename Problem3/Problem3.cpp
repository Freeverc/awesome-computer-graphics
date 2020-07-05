
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
	SetWorkingImage(NULL);
	putimage(0, 0, &buffer);
	return;
}

void drawLine(int l, int r)
{
	for (int i = l; i < r; i++) {
		fillcircle(points[i][0] * SHUMI, points[i][1] * SHUMI, SHUMI / 2);
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
			for (yi = y1; yi <= y2; yi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				vector<int> p = { {xi, yi} };
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
			for (yi = y1; yi <= y2; yi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				vector<int> p = { {xi, yi} };
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
			for (xi = x1; xi <= x2; xi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				vector<int> p = { {xi, yi} };
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
			for (xi = x1; xi <= x2; xi ++) {
				mem[yi][xi] = 1;
				fillcircle(xi * SHUMI, yi * SHUMI, SHUMI/4);
				vector<int> p = { {xi, yi} };
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

int getCode(int xLeft, int yUp, int xRight, int yDown, int x1, int y1)
{
	int code = 0;
	if (x1 < xLeft)
		code |= 1;
	if (x1 > xRight)
		code |= 2;
	if (y1 < yDown)
		code |= 4;
	if (y1 > yUp)
		code |= 8;
	return code;
}

void cs(int xLeft, int yUp, int xRight, int yDown, int l, int r)
{
	if (l >= r)
		return;
	int x1 = points[l][0], y1 = points[l][1];
	int x2 = points[r-1][0], y2 = points[r-1][1];
	unsigned int code1 = getCode(xLeft, yUp, xRight, yDown, x1, y1);
	unsigned int code2 = getCode(xLeft, yUp, xRight, yDown, x2, y2);
	//setbkcolor(WHITE);
	//settextcolor(BLACK);
	//settextstyle(30, 0, _T("Consolas"));
	//setlinecolor(BLACK);
	//TCHAR s[20];
	//_stprintf(s, _T("%d %d"), code1, code2);
	//outtextxy(100, 100, s);
	if((code1 | code2) == 0)
	{
		setfillcolor(RED);
		drawLine(l, r);
		return;
	}
	if((code1 & code2) != 0)
	{
		setfillcolor(BLUE);
		drawLine(l, r);
		return;
	}
	if (x1 > x2) {
		int t = x1; x1 = x2; x2 = t;
		t = y1; y1 = y2; y2 = y1;
	}

	int m = (l + r + 1) / 2;
	cs(xLeft, yUp, xRight, yDown, l, m);
	cs(xLeft, yUp, xRight, yDown, m, r);
	return;
}


int main()
{
	MOUSEMSG m;
	IMAGE buffer(WIDTH, HEIGHT);
	int startX, startY, endX, endY;
	int xLeft, yUp, xRight, yDown;
	int x1, y1, x2, y2, len;
	initgraph(WIDTH , HEIGHT);
	drawRegion(buffer);
	setlinecolor(BLACK);
	setfillcolor(RED);
	functionState = 1;
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
		}

		else if (m.uMsg == WM_LBUTTONUP) {
			mouseState = 0;
			endX = m.x / SHUMI;
			endY = m.y / SHUMI;
			if (m.x - endX * SHUMI > SHUMI / 2)
				endX ++;
			if (m.y - endY * SHUMI > SHUMI / 2)
				endY ++;
			if (functionState == 1)
			{
				functionState++;
				MID_line(startX, startY, endX, endY);
				len = points.size();
				x1 = startX;
				x2 = endX;
				y1 = startY;
				y2 = endY;
			}
			else if (functionState == 2)
			{
				functionState++;
				MID_line(startX, startY, endX, startY);
				MID_line(endX, startY, endX, endY);
				MID_line(endX, endY, startX, endY);
				MID_line(startX, endY, startX, startY);
				xLeft = min(startX, endX);
				xRight = max(startX, endX);
				yUp = max(startY, endY);
				yDown = min(startY, endY);
				cs(xLeft, yUp, xRight, yDown, 0, len);
			}
		}
		else if (m.uMsg == WM_RBUTTONUP) {
			setlinecolor(BLACK);
			setfillcolor(RED);
			drawRegion(buffer);
			points.clear();
			functionState = 1;
		}
	}
	return 0;
}
