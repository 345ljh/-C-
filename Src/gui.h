#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"

#define FRAMELINE EGERGB(255, 255, 145)

typedef struct
{
    PIMAGE background;  //����ͼ��
    PIMAGE frame;  //������Ϣ��߽硢��ťͼ��
    PIMAGE playercard;  //��������ͼ��
    PIMAGE selector;  //ѡ����ѡ��Ŀ��ͼ��
    PIMAGE tips;  //��ʾ����ͼ��
}gui_t;

void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img);
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img);

void PasteImage(char* path, int x, int y, PIMAGE img, int mode, color_t color);
void ProgramStart(void);
void GameGuiInit(void);

#endif // DRAW_H_INCLUDED
