#include "gui.h"

mouse_msg msg;
int mouse_x, mouse_y;//���ڻ�ȡ�������
gui_t gui = {newimage(), newimage(), newimage(), newimage(), newimage(), newimage()};

//����ƴ��,����ֵΪƴ�Ӻ���ַ���
char* Link(char* str1, char* str2)
{
    static char p[101];
    strcpy(p, str1);
    strcat(p, str2);
    return p;
}

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

//��ʼ����
void ProgramStart(void)
{
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

//��ʼ��Ϸʱ�������
void GameGuiInit(void)
{
    setbkcolor(BLACK);
    cleardevice();
    //ͼ���ʼ��
    getimage(gui.background, 0, 0, 1200, 600);
    getimage(gui.frame, 0, 0, 1200, 600);
    getimage(gui.general, 0, 0, 1200, 600);
    getimage(gui.playercard, 0, 0, 1200, 600);
    getimage(gui.selector, 0, 0, 1200, 600);
    getimage(gui.tips, 0, 0, 1200, 600);

    PasteImage((char*)".\\Textures\\background.png", 0, 0, gui.background);

    setfillcolor(EGERGB(71, 98, 32), gui.frame);
    bar(0, 450, 1070, 600, gui.frame);

    //���Ʊ߽�
    setcolor(YELLOW, gui.frame);
    line(150, 450, 150, 600, gui.frame);
    for(int i = 1; i <= 3; i++)  line(0, 450 + 37.5 * i, 150, 450 + 37.5 * i, gui.frame);
    for(int i = 1; i <= 8; i++)  line(150 + 100 * i, 450, 150 + 100 * i, 600, gui.frame);

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
    Rect(1070, 430, 1200, 600, YELLOW, gui.frame);

    //����������ɫ
    Rect(30, 150, 160, 320, YELLOW, gui.frame);
    Rect(1040, 150, 1170, 320, YELLOW, gui.frame);
    Rect(520, 20, 650, 190, YELLOW, gui.frame);

    //��Ϣ��ʾͼ��
    PasteImage((char*)".\\Textures\\condition.png", 950, 450, gui.frame, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 30, 340, gui.frame, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 1040, 340, gui.frame, TRANSPARENT, BLACK);
    PasteImage((char*)".\\Textures\\condition.png", 650, 130, gui.frame, TRANSPARENT, BLACK);

    //ȷ��λ��
    game.humanid = rand() % PLAYERS;
    char str[] = "һ\0��\0��\0��\0";
    setfillcolor(EGERGB(238, 89, 0), gui.frame);
    setcolor(RED, gui.frame);
    fillellipse(1050, 585, 15, 15, gui.frame);
    setcolor(EGERGB(253, 228, 140), gui.frame);
    setfont(20, 0, "����", gui.frame);
    outtextxy(1040, 575, str + game.humanid * 3, gui.frame);
    setcolor(YELLOW, gui.frame);
    setfont(20, 0, "����", gui.frame);
    outtextxy(1080, 320, Link(str + ((game.humanid + 1) % 4) * 3, (char*)"��λ"), gui.frame);
    outtextxy(560, 190, Link(str + ((game.humanid + 2) % 4) * 3, (char*)"��λ"), gui.frame);
    outtextxy(70, 320, Link(str + ((game.humanid + 3) % 4) * 3, (char*)"��λ"), gui.frame);
    //���ͼ��
    putimage(0, 0, gui.background);
    putimage_alphatransparent(NULL, gui.frame, 0, 0, BLACK, 150);
}

//ѡ��
void GeneralSelect(void)
{
    int forselect[4];
    int pointer;

    //ȷ����ҿ�ѡ��4��
    for(int i = 0; i <= 3; i++)
    {
        pointer = rand() % GENERALS;
        while(!general_inf[pointer].selected) ++pointer %= GENERALS;
        general_inf[pointer].selected = 2;
        forselect[i] = pointer;
    }

    setcolor(EGERGB(102, 0, 15), gui.selector);
    bar(300, 200, 900, 450, gui.selector);
    Rect(300, 200, 900, 450, YELLOW, gui.selector);
    putimage_alphatransparent(NULL, gui.selector, 0, 0, BLACK, 150);
}
