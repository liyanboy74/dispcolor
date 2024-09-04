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
#ifndef _DISPCOLOR_H
#define _DISPCOLOR_H

#include <stdint.h>

// �������������� �������
#define DISPTYPE_BMPC       1
#define DISPTYPE_GC9A01     2
#define DISPTYPE_SIMLCD		3

// ��� ������� (���������� �������)
#ifndef DISPCOLOR_type
	#define DISPCOLOR_type      DISPTYPE_SIMLCD
#endif

typedef struct
{
	uint16_t r:5;
	uint16_t g:6;
	uint16_t b:5;
} sRGB565;

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} sRGB888;

typedef union
{
	sRGB565 rgb;
	uint16_t word;
} uRGB565;


#define RGB565(r, g, b)         (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

// �������� �����
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

#ifdef __cplusplus
extern "C" {
#endif

void dispcolor_Init(uint16_t Width, uint16_t Height);
void dispcolor_Update();
void dispcolor_ClearScreen(void);
void dispcolor_SetBrightness(uint8_t Value);
void dispcolor_FillScreen(uint16_t color);
void dispcolor_DrawPixel(int16_t X, int16_t Y, uint16_t color);
void dispcolor_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void dispcolor_DrawLine2(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, uint8_t thick);
void dispcolor_DrawLine_Wu(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void dispcolor_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void dispcolor_FillRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t fillcolor);
void dispcolor_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void dispcolor_DrawTriangle_Wu(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void dispcolor_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void dispcolor_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color, uint8_t correction);
void dispcolor_DrawCircle_Wu(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
void dispcolor_FillCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
void dispcolor_FillCircleWu(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
void dispcolor_DrawArc(int16_t x0, int16_t y0, int16_t radius, int16_t startAngle, int16_t endAngle, uint16_t color, uint8_t correction);
int16_t dispcolor_DrawString(int16_t X, int16_t Y, uint8_t FontID, char *Str, uint16_t TextColor);
int16_t dispcolor_DrawString_Bg(int16_t X, int16_t Y, uint8_t FontID, char *Str, uint16_t TextColor, uint16_t BgColor);
int16_t dispcolor_printf(int16_t X, int16_t Y, uint8_t FontID, uint16_t TextColor, const char *args, ...);
int16_t dispcolor_printf_Bg(int16_t X, int16_t Y, uint8_t FontID, uint16_t TextColor, uint16_t BgColor, const char *args, ...);

#ifdef __cplusplus
}
#endif

#endif
