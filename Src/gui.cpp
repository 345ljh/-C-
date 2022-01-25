#include "gui.h"
#include "graphics.h"

#include <time.h>

mouse_msg msg;
int mouse_x, mouse_y;//���ڻ�ȡ�������
gui_t gui = {newimage(), newimage(), newimage(), newimage(), newimage()};

//�����ĳ�����
void Rect(int left, int top, int right, int bottom, color_t color, PIMAGE img = NULL)
{
    setcolor(color, img);
    line(left, top, right, top, img);
    line(right, top, right, bottom, img);
    line(right, bottom, left, bottom, img);
    line(left, bottom, left, top, img);
}

//������������
void Tri(int x1, int y1, int x2, int y2, int x3, int y3, color_t color, PIMAGE img = NULL)
{
    setcolor(color, img);
    line(x1, y1, x2, y2, img);
    line(x2, y2, x3, y3, img);
    line(x3, y3, x1, y1, img);
}

//ͼ��ճ������
///����ʱ��·��תΪchar*,����warning
///�˺�������ʱ��mode���ݵ�TRANSPARENT�ѱ�����Ϊ1
void PasteImage(char* path, int x, int y, PIMAGE img = NULL, int mode = 0, color_t color = BLACK)
{
    PIMAGE temp = newimage();
    getimage(temp, path);
    switch(mode)
    {
    case 0:
    {
        putimage_alphablend(img, temp, x, y, 255);
        break;
    }
    case 1:
    {
        putimage_transparent(img, temp, x, y, color);
        break;
    }
    }
    delimage(temp);
}

void ProgramStart(void)
{
    srand(time(NULL));

    initgraph(1200, 600);

    PasteImage((char*)".\\Textures\\origin.png", 0, 0);

    setcolor(EGERGB(236, 95, 0));
    setbkmode(TRANSPARENT);
    setfont(120, 0, "����");
    outtextxy(420, 100, "����ɱ");

    setfillcolor(EGERGB(136, 0, 21));
    bar(500, 350, 700, 400);

    setcolor(EGERGB(234, 234, 255));
    setbkmode(TRANSPARENT);
    setfont(40, 0, "����");
    outtextxy(520, 355, "��ʼ��Ϸ");

    for(; is_run(); delay_fps(60))
    {
        msg = getmouse();

        mousepos(&mouse_x, &mouse_y);
        if(msg.is_down() && mouse_x >= 500 && mouse_x <= 700 && mouse_y >= 350 && mouse_y <= 400)
        {
            return;
        }
    }
}

void GameGuiInit(void)
{
    setbkcolor(BLACK);
    cleardevice();
    //ͼ���ʼ��
    getimage(gui.background, 0, 0, 1200, 600);
    PasteImage((char*)".\\Textures\\background.png", 0, 0, gui.background);
    getimage(gui.frame, 0, 0, 1200, 600);

    setfillcolor(EGERGB(190, 190, 126), gui.frame);
    bar(0, 450, 1200, 600, gui.frame);

    //���Ʊ߽�
    setcolor(FRAMELINE, gui.frame);
    line(150, 450, 150, 600, gui.frame);
    for(int i = 1; i <= 3; i++)  line(0, 450 + 37.5 * i, 150, 450 + 37.5 * i, gui.frame);
    for(int i = 1; i <= 8; i++)  line(150 + 100 * i, 450, 150 + 100 * i, 600, gui.frame);

    //��Ϣ��ʾͼ��
    PasteImage((char*)".\\Textures\\condition.png", 950, 450, gui.frame, TRANSPARENT, BLACK);

    //���ư�ť
    ///ʹ��������л���ť�������
    Rect(960, 510, 1050, 535, YELLOW, gui.frame);
    Rect(960, 540, 1050, 565, YELLOW, gui.frame);
    setbkmode(TRANSPARENT, gui.frame);
    setfont(25, 0, "����", gui.frame);
    outtextxy(980, 510, "ȷ��", gui.frame);
    outtextxy(980, 540, "ȡ��", gui.frame);
    setfillcolor(LIGHTGRAY, gui.frame);
    floodfill(981, 511, YELLOW, gui.frame);
    floodfill(981, 541, YELLOW, gui.frame);
    Tri(960, 585, 975, 575, 975, 595, EGERGB(44, 182, 14), gui.frame);
    Tri(1000, 585, 985, 575, 985, 595, EGERGB(44, 182, 14), gui.frame);
    setfillcolor(EGERGB(181, 230, 29), gui.frame);
    floodfill(970, 585, EGERGB(44, 182, 14), gui.frame);
    floodfill(990, 585, EGERGB(44, 182, 14), gui.frame);

    //�����ƾ���Ϣ
    setcolor(EGERGB(255, 127, 39), gui.frame);
    setfont(40, 0, "����", gui.frame);
    outtextxy(1000, 20, "��     ��", gui.frame);
    setcolor(WHITE, gui.frame);
    setfont(20, 0, "����", gui.frame);
    outtextxy(1000, 70, "�ƶ�ʣ��:", gui.frame);

    //���ͼ��
    putimage(0, 0, gui.background);
    putimage_alphatransparent(NULL, gui.frame, 0, 0, BLACK, 150);
}
