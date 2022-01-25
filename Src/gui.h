#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "graphics.h"

typedef struct
{
    PIMAGE background;  //����ͼ��
    PIMAGE playercard;  //��������ͼ��
    PIMAGE bottom;  //����ͼ��
    PIMAGE selector;  //ѡ����ѡ��Ŀ��ͼ��
    PIMAGE tips;  //��ʾ����ͼ��
}gui_t;

void ProgramStart(void);
void GameGuiInit(void);

#endif // DRAW_H_INCLUDED
