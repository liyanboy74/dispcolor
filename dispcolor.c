//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-�����
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//
// Liyanboy74
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "dispcolor.h"
#include "font.h"

#define PI 3.14159265
static uint16_t _Width, _Height;

#if (DISPCOLOR_type == DISPTYPE_GC9A01)
#include "gc9a01.h"
#elif (DISPCOLOR_type == DISPTYPE_BMPC)
#include "bmpc.h"
bmpc_screen_s bmpcs;
#elif (DISPCOLOR_type == DISPTYPE_SIMLCD)
#include "simlcd.h"
#include "color.h"
color_rgb_s c;
#else
  #error Disp1color Can not find Screen Type!
#endif


static void SwapInt16Values(int16_t *pValue1, int16_t *pValue2) {
	int16_t TempValue = *pValue1;
	*pValue1 = *pValue2;
	*pValue2 = TempValue;
}

void dispcolor_Init(uint16_t Width, uint16_t Height) {
#if (DISPCOLOR_type == DISPTYPE_GC9A01)
	GC9A01_Init(Width, Height);
	_Width = GC9A01_GetWidth();
	_Height = GC9A01_GetHeight();
#elif (DISPCOLOR_type == DISPTYPE_BMPC)
    bmpc_init(&bmpcs,"Screen",Width,Height);
    _Width=bmpc_get_width(&bmpcs);
    _Height=bmpc_get_width(&bmpcs);
#elif(DISPCOLOR_type ==DISPTYPE_SIMLCD)
	LCD_BUFFER=simlcd_init(Height,Width,0);
	_Width=Width;
	_Height=Height;
#endif
	dispcolor_ClearScreen();
}

void dispcolor_Update() {
#if (DISPCOLOR_type == DISPTYPE_GC9A01)
	GC9A01_Update();
#elif (DISPCOLOR_type == DISPTYPE_BMPC)
    bmpc_update(&bmpcs);
#elif(DISPCOLOR_type ==DISPTYPE_SIMLCD)
	simlcd_display(&LCD_BUFFER);
#endif
}

void dispcolor_SetBrightness(uint8_t Value) {
	if (Value > 100)
		Value = 100;

#if (DISPCOLOR_type == DISPTYPE_GC9A01)
	GC9A01_SetBL(Value);
#elif (DISPCOLOR_type == DISPTYPE_BMPC)
    bmpc_set_brightness(&bmpcs,Value);
#elif(DISPCOLOR_type ==DISPTYPE_SIMLCD)

#endif
}

void dispcolor_DrawPixel(int16_t x, int16_t y, uint16_t color) {
#if (DISPCOLOR_type == DISPTYPE_GC9A01)
	GC9A01_DrawPixel(x, y, color);
#elif (DISPCOLOR_type == DISPTYPE_BMPC)
    bmpc_draw_pixel(&bmpcs,x,y,color_16_to_24_s(color));
#elif(DISPCOLOR_type ==DISPTYPE_SIMLCD)
	c=color_16_to_24_s(color);
	simlcd_set_color(&LCD_BUFFER,c.r,c.g,c.b);
	simlcd_draw_point(&LCD_BUFFER,x,y);
#endif
}

uint16_t dispcolor_GetPixel(int16_t x, int16_t y) {
#if (DISPCOLOR_type == DISPTYPE_GC9A01)
	return GC9A01_GetPixel(x, y);
#elif (DISPCOLOR_type == DISPTYPE_BMPC)
    return color_24_to_16_s(bmpc_read_pixel(&bmpcs,x,y));
#elif(DISPCOLOR_type ==DISPTYPE_SIMLCD)
	uint32_t c=simlcd_get_point(&LCD_BUFFER,x,y);
	return color_24_to_16(((c>>16)&0xff),((c>>8)&0xff),((c>>0)&0xff));
#endif
}

void dispcolor_FillRect(int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
#if (DISPCOLOR_type == DISPTYPE_GC9A01)
	GC9A01_FillRect(x, y, w, h, color);
#elif (DISPCOLOR_type == DISPTYPE_BMPC)
    bmpc_fill_rect(&bmpcs,x,y,w,h,color_16_to_24_s(color));
#elif(DISPCOLOR_type ==DISPTYPE_SIMLCD)
	c=color_16_to_24_s(color);
	simlcd_set_color(&LCD_BUFFER,c.r,c.g,c.b);
	simlcd_draw_rect(&LCD_BUFFER,x,y,w,h);
#endif
}

void dispcolor_FillScreen(uint16_t color) {
	dispcolor_FillRect(0, 0, _Width, _Height, color);
}

void dispcolor_ClearScreen(void) {
	dispcolor_FillScreen(BLACK);
}

static void DrawLine_Slow(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		uint16_t color) {
	const int16_t deltaX = abs(x2 - x1);
	const int16_t deltaY = abs(y2 - y1);
	const int16_t signX = x1 < x2 ? 1 : -1;
	const int16_t signY = y1 < y2 ? 1 : -1;

	int16_t error = deltaX - deltaY;

	dispcolor_DrawPixel(x2, y2, color);

	while (x1 != x2 || y1 != y2) {
		dispcolor_DrawPixel(x1, y1, color);
		const int16_t error2 = error * 2;

		if (error2 > -deltaY) {
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX) {
			error += deltaX;
			y1 += signY;
		}
	}
}

void dispcolor_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		uint16_t color) {
	if (x1 == x2) {
		if (y1 > y2)
			dispcolor_FillRect(x1, y2, 1, y1 - y2 + 1, color);
		else
			dispcolor_FillRect(x1, y1, 1, y2 - y1 + 1, color);
		return;
	}

	if (y1 == y2) {
		if (x1 > x2)
			dispcolor_FillRect(x2, y1, x1 - x2 + 1, 1, color);
		else
			dispcolor_FillRect(x1, y1, x2 - x1 + 1, 1, color);
		return;
	}

	DrawLine_Slow(x1, y1, x2, y2, color);
}

void dispcolor_DrawLine2(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		uint16_t color, uint8_t thick) {
	const int16_t deltaX = abs(x2 - x1);
	const int16_t deltaY = abs(y2 - y1);
	const int16_t signX = x1 < x2 ? 1 : -1;
	const int16_t signY = y1 < y2 ? 1 : -1;

	int16_t error = deltaX - deltaY;

	if (thick > 1)
		dispcolor_FillCircle(x2, y2, thick >> 1, color);
	else
		dispcolor_DrawPixel(x2, y2, color);

	while (x1 != x2 || y1 != y2) {
		if (thick > 1)
			dispcolor_FillCircle(x1, y1, thick >> 1, color);
		else
			dispcolor_DrawPixel(x1, y1, color);

		const int16_t error2 = error * 2;
		if (error2 > -deltaY) {
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX) {
			error += deltaX;
			y1 += signY;
		}
	}
}

void DrawPixel(int16_t x, int16_t y, uint16_t color, float intensity) {
	uRGB565 colorRgb;
	uRGB565 oldColorRgb;

	colorRgb.word = color;
	oldColorRgb.word = dispcolor_GetPixel(x, y);

	float iintensity = 1 - intensity;
	uint16_t r = oldColorRgb.rgb.r * iintensity + colorRgb.rgb.r * intensity;
	uint16_t g = oldColorRgb.rgb.g * iintensity + colorRgb.rgb.g * intensity;
	uint16_t b = oldColorRgb.rgb.b * iintensity + colorRgb.rgb.b * intensity;

	uRGB565 newColorRgb;
	newColorRgb.rgb.r = r;
	newColorRgb.rgb.g = g;
	newColorRgb.rgb.b = b;
	dispcolor_DrawPixel(x, y, newColorRgb.word);
}

// integer part of x
int ipart(float x) {
	return (int) x;
}

// fractional part of x
float fpart(float x) {
	return x - (int) x;
}

float rfpart(float x) {
	return 1 - fpart(x);
}

void swap(int16_t *a, int16_t *b) {
	int16_t temp = *a;
	*a = *b;
	*b = temp;
}

static void DrawLine_Slow_Wu(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		uint16_t color) {
	uint8_t steep = abs(y2 - y1) > abs(x2 - x1) ? 1 : 0;
	if (steep) {
		swap(&x1, &y1);
		swap(&x2, &y2);
	}
	if (x1 > x2) {
		swap(&x1, &x2);
		swap(&y1, &y2);
	}

	float dx = x2 - x1;
	float dy = y2 - y1;
	float gradient = dx == 0 ? 1 : dy / dx;

	// handle first endpoint
	float xend = round(x1);
	float yend = gradient * (xend - x1) + y1;
	float xgap = rfpart(x1 + 0.5);
	int16_t xpxl1 = xend; // this will be used in the main loop
	int16_t ypxl1 = ipart(yend);
	if (steep) {
		DrawPixel(ypxl1, xpxl1, color, rfpart(yend) * xgap);
		DrawPixel(ypxl1 + 1, xpxl1, color, fpart(yend) * xgap);
	} else {
		DrawPixel(xpxl1, ypxl1, color, rfpart(yend) * xgap);
		DrawPixel(xpxl1, ypxl1 + 1, color, fpart(yend) * xgap);
	}

	float intery = yend + gradient; // first y-intersection for the main loop

	// handle second endpoint
	xend = round(x2);
	yend = gradient * (xend - x2) + y2;
	xgap = fpart(x2 + 0.5);
	int16_t xpxl2 = xend; //this will be used in the main loop
	int16_t ypxl2 = ipart(yend);

	if (steep) {
		DrawPixel(ypxl2, xpxl2, color, rfpart(yend) * xgap);
		DrawPixel(ypxl2 + 1, xpxl2, color, fpart(yend) * xgap);
	} else {
		DrawPixel(xpxl2, ypxl2, color, rfpart(yend) * xgap);
		DrawPixel(xpxl2, ypxl2 + 1, color, fpart(yend) * xgap);
	}

	// main loop
	if (steep) {
		for (int16_t x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
			DrawPixel(ipart(intery), x, color, rfpart(intery));
			DrawPixel(ipart(intery) + 1, x, color, fpart(intery));
			intery += gradient;
		}
	} else {
		for (int16_t x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
			DrawPixel(x, ipart(intery), color, rfpart(intery));
			DrawPixel(x, ipart(intery) + 1, color, fpart(intery));
			intery += gradient;
		}
	}
}

void dispcolor_DrawLine_Wu(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		uint16_t color) {
	if (x1 == x2) {
		if (y1 > y2)
			dispcolor_FillRect(x1, y2, 1, y1 - y2 + 1, color);
		else
			dispcolor_FillRect(x1, y1, 1, y2 - y1 + 1, color);
		return;
	}

	if (y1 == y2) {
		if (x1 > x2)
			dispcolor_FillRect(x2, y1, x1 - x2 + 1, 1, color);
		else
			dispcolor_FillRect(x1, y1, x2 - x1 + 1, 1, color);
		return;
	}

	DrawLine_Slow_Wu(x1, y1, x2, y2, color);
}

void dispcolor_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		uint16_t color) {
	dispcolor_DrawLine(x1, y1, x1, y2, color);
	dispcolor_DrawLine(x2, y1, x2, y2, color);
	dispcolor_DrawLine(x1, y1, x2, y1, color);
	dispcolor_DrawLine(x1, y2, x2, y2, color);
}

void dispcolor_FillRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		uint16_t fillcolor) {
	if (x1 > x2)
		SwapInt16Values(&x1, &x2);
	if (y1 > y2)
		SwapInt16Values(&y1, &y2);

	dispcolor_FillRect(x1, y1, x2 - x1, y2 - y1, fillcolor);
}

void dispcolor_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color) {
	dispcolor_DrawLine(x0, y0, x1, y1, color);
	dispcolor_DrawLine(x1, y1, x2, y2, color);
	dispcolor_DrawLine(x2, y2, x0, y0, color);
}

void dispcolor_DrawTriangle_Wu(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color) {
	dispcolor_DrawLine_Wu(x0, y0, x1, y1, color);
	dispcolor_DrawLine_Wu(x1, y1, x2, y2, color);
	dispcolor_DrawLine_Wu(x2, y2, x0, y0, color);
}

void dispcolor_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color) {
	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		SwapInt16Values(&y0, &y1);
		SwapInt16Values(&x0, &x1);
	}
	if (y1 > y2) {
		SwapInt16Values(&y2, &y1);
		SwapInt16Values(&x2, &x1);
	}
	if (y0 > y1) {
		SwapInt16Values(&y0, &y1);
		SwapInt16Values(&x0, &x1);
	}

	if (y0 == y2)	  // Handle awkward all-on-same-line case as its own thing
			{
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;

		dispcolor_FillRect(a, y0, b - a + 1, 1, color);
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
			dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if (a > b)
			SwapInt16Values(&a, &b);

		dispcolor_FillRect(a, y, b - a + 1, 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t) dx12 * (y - y1);
	sb = (int32_t) dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if (a > b)
			SwapInt16Values(&a, &b);

		dispcolor_FillRect(a, y, b - a + 1, 1, color);
	}
}

void dispcolor_DrawCircle(int16_t x0, int16_t y0, int16_t radius,
		uint16_t color, uint8_t correction) {
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;

	if (correction)
		correction = 1;

	while (y >= 0) {
		dispcolor_DrawPixel(x0 + x, y0 + y + correction, color);
		dispcolor_DrawPixel(x0 + x, y0 - y, color);
		dispcolor_DrawPixel(x0 - x + correction, y0 + y + correction, color);
		dispcolor_DrawPixel(x0 - x + correction, y0 - y, color);
		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}

		error = 2 * (delta - x) - 1;

		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}

		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void dispcolor_DrawCircle_Wu(int16_t x0, int16_t y0, int16_t radius,
		uint16_t color) {
	int16_t i = 0;
	int16_t j = radius;
	float last_fade_amount = 0;

	while (i < j) {
		float height = sqrt(fmax(radius * radius - i * i, 0));
		float fade_amount = ceil(height) - height;

		if (fade_amount < last_fade_amount)
			j--;	// Opaqueness reset so drop down a row

		last_fade_amount = fade_amount;

		// We're fading out the current j row, and fading in the next one down.
		DrawPixel(x0 + i, y0 - j, color, 1 - fade_amount);
		DrawPixel(x0 + i, y0 - j + 1, color, fade_amount);

		DrawPixel(x0 + j, y0 - i, color, 1 - fade_amount);
		DrawPixel(x0 + j - 1, y0 - i, color, fade_amount);

		DrawPixel(x0 + j, y0 + i, color, 1 - fade_amount);
		DrawPixel(x0 + j - 1, y0 + i, color, fade_amount);

		DrawPixel(x0 + i, y0 + j, color, 1 - fade_amount);
		DrawPixel(x0 + i, y0 + j - 1, color, fade_amount);

		DrawPixel(x0 - i, y0 + j, color, 1 - fade_amount);
		DrawPixel(x0 - i, y0 + j - 1, color, fade_amount);

		DrawPixel(x0 - j, y0 + i, color, 1 - fade_amount);
		DrawPixel(x0 - j + 1, y0 + i, color, fade_amount);

		DrawPixel(x0 - j, y0 - i, color, 1 - fade_amount);
		DrawPixel(x0 - j + 1, y0 - i, color, fade_amount);

		DrawPixel(x0 - i, y0 - j, color, 1 - fade_amount);
		DrawPixel(x0 - i, y0 - j + 1, color, fade_amount);

		i++;
	}
}

void dispcolor_FillCircle(int16_t x0, int16_t y0, int16_t radius,
		uint16_t color) {
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;

	while (y >= 0) {
		dispcolor_DrawLine(x0 + x, y0 - y, x0 + x, y0 + y, color);
		dispcolor_DrawLine(x0 - x, y0 - y, x0 - x, y0 + y, color);
		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}

		error = 2 * (delta - x) - 1;

		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}

		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void dispcolor_FillCircleWu(int16_t x0, int16_t y0, int16_t radius,
		uint16_t color) {
	if (radius > 1)
		dispcolor_FillCircle(x0, y0, radius - 1, color);
	dispcolor_DrawCircle_Wu(x0, y0, radius, color);
}

void dispcolor_DrawArc(int16_t x0, int16_t y0, int16_t radius,
		int16_t startAngle, int16_t endAngle, uint16_t color, uint8_t thick) {
	int16_t xLast = -1, yLast = -1;
	startAngle -= 90;
	endAngle -= 90;

	for (int16_t angle = startAngle; angle <= endAngle; angle += 2) {
		float angleRad = (float) angle * PI / 180;
		int x = cos(angleRad) * radius + x0;
		int y = sin(angleRad) * radius + y0;

		if (xLast == -1 || yLast == -1) {
			xLast = x;
			yLast = y;
			continue;
		}

		if (thick > 1)
			dispcolor_DrawLine2(xLast, yLast, x, y, color, thick);
		else
			dispcolor_DrawLine(xLast, yLast, x, y, color);

		xLast = x;
		yLast = y;
	}
}

void dispcolor_DrawArc_Wu(int16_t x0, int16_t y0, int16_t radius,
		int16_t startAngle, int16_t endAngle, uint16_t color, uint8_t thick) {
	int16_t xLast = -1, yLast = -1;
	startAngle -= 90;
	endAngle -= 90;

	for (int16_t angle = startAngle; angle <= endAngle; angle += 1)	//1)//6)
			{
		float angleRad = (float) angle * PI / 180;
		int x = cos(angleRad) * radius + x0;
		int y = sin(angleRad) * radius + y0;

		if (xLast == -1 || yLast == -1) {
			xLast = x;
			yLast = y;
			continue;
		}

		if (thick > 1)
			dispcolor_DrawCircle_Wu(x, y, thick / 2, color);
		else
			dispcolor_DrawLine_Wu(xLast, yLast, x, y, color);

		xLast = x;
		yLast = y;
	}
}

static uint8_t dispcolor_DrawChar_General(int16_t X, int16_t Y, uint8_t FontID,
		char Char, uint16_t TextColor, uint16_t BgColor,
		uint8_t TransparentBg) {

	uint8_t *pCharTable = font_GetFontStruct(FontID, Char);
	uint8_t CharWidth = font_GetCharWidth(pCharTable);
	uint8_t CharHeight = font_GetCharHeight(pCharTable);
	pCharTable += 2;

	if (FontID == FONTID_6X8M) {
		for (uint8_t row = 0; row < CharHeight; row++) {
			for (uint8_t col = 0; col < CharWidth; col++) {
				if (pCharTable[row] & (1 << (7 - col)))
					dispcolor_DrawPixel(X + col, Y + row, TextColor);
				else if (!TransparentBg)
					dispcolor_DrawPixel(X + col, Y + row, BgColor);
			}
		}
	} else {
		for (uint8_t row = 0; row < CharHeight; row++) {
			for (uint8_t col = 0; col < CharWidth; col++) {
				if (col < 8) {
					if (pCharTable[row * 2] & (1 << (7 - col)))
						dispcolor_DrawPixel(X + col, Y + row, TextColor);
					else if (!TransparentBg)
						dispcolor_DrawPixel(X + col, Y + row, BgColor);
				} else {
					if (pCharTable[(row * 2) + 1] & (1 << (15 - col)))
						dispcolor_DrawPixel(X + col, Y + row, TextColor);
					else if (!TransparentBg)
						dispcolor_DrawPixel(X + col, Y + row, BgColor);
				}
			}
		}
	}

	return CharWidth;
}

uint8_t dispcolor_DrawChar(int16_t X, int16_t Y, uint8_t FontID, char Char,
		uint16_t TextColor) {
	return dispcolor_DrawChar_General(X, Y, FontID, Char, TextColor, 0, 1);
}

uint8_t dispcolor_DrawChar_Bg(int16_t X, int16_t Y, uint8_t FontID,
		char Char, uint16_t TextColor, uint16_t BgColor) {
	return dispcolor_DrawChar_General(X, Y, FontID, Char, TextColor, BgColor, 0);
}

static int16_t dispcolor_DrawString_General(int16_t X, int16_t Y,
		uint8_t FontID, char *Str, uint16_t TextColor, uint16_t BgColor,
		uint8_t TransparentBg) {
	uint8_t done = 0;
	int16_t Xstart = X;
	uint8_t StrHeight = 8;

	while (!done) {
		switch (*Str) {
		case '\0':
			done = 1;
			break;
		case '\n':
			Y += StrHeight;
			break;
		case '\r':
			X = Xstart;
			break;
		default:
			if (TransparentBg)
				X += dispcolor_DrawChar(X, Y, FontID, *Str, TextColor);
			else
				X += dispcolor_DrawChar_Bg(X, Y, FontID, *Str, TextColor,
						BgColor);

			StrHeight = font_GetCharHeight(font_GetFontStruct(FontID, *Str));
			break;
		}
		Str++;
	}
	return X;
}

int16_t dispcolor_DrawString(int16_t X, int16_t Y, uint8_t FontID, char *Str,
		uint16_t TextColor) {
	return dispcolor_DrawString_General(X, Y, FontID, Str, TextColor, 0, 1);
}

int16_t dispcolor_DrawString_Bg(int16_t X, int16_t Y, uint8_t FontID, char *Str,
		uint16_t TextColor, uint16_t BgColor) {
	return dispcolor_DrawString_General(X, Y, FontID, Str, TextColor, BgColor,
			0);
}

int16_t dispcolor_printf(int16_t X, int16_t Y, uint8_t FontID,
		uint16_t TextColor, const char *args, ...) {
	char StrBuff[100];

	va_list ap;
	va_start(ap, args);
	vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
	va_end(ap);

	return dispcolor_DrawString(X, Y, FontID, StrBuff, TextColor);
}

int16_t dispcolor_printf_Bg(int16_t X, int16_t Y, uint8_t FontID,
		uint16_t TextColor, uint16_t BgColor, const char *args, ...) {
	char StrBuff[100];

	va_list ap;
	va_start(ap, args);
	vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
	va_end(ap);

	return dispcolor_DrawString_Bg(X, Y, FontID, StrBuff, TextColor,
			BgColor);
}

int16_t dispcolor_GetTextWidth(uint8_t FontID, char *Str) {
	uint8_t done = 0;
	int16_t width = 0;

	while (!done) {
		switch (*Str) {
		case '\0':
			done = 1;
			break;
		case '\n':
		case '\r':
			break;
		default:
			width += font_GetCharWidth(font_GetFontStruct(FontID, *Str));
			break;
		}
		Str++;
	}
	return width;
}

int16_t dispcolor_GetFormatedTextWidth(uint8_t FontID, char *Str,
		const char *args, ...) {
	char StrBuff[100];

	va_list ap;
	va_start(ap, args);
	vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
	va_end(ap);

	return dispcolor_GetTextWidth(FontID, Str);
}
