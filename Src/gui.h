#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"
#include "items.h"

#include <string.h>
#include <stdlib.h>

typedef struct
{
    PIMAGE background;  //����ͼ��
    PIMAGE frame;  //������Ϣ��߽硢��ťͼ��
    PIMAGE general;  //�佫������ͼ��
    PIMAGE playercard;  //��������ͼ��
    PIMAGE selector;  //ѡ����ѡ��Ŀ��ͼ��
    PIMAGE tips;  //��ʾ����ͼ��
}gui_t;

char* Link(char* str1, char* str2);
void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img);
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img);

void PasteImage(char* path, int x, int y, PIMAGE img, int mode, color_t color);
void ProgramStart(void);
void GameGuiInit(void);
void GeneralSelect(void);

#endif // DRAW_H_INCLUDED
