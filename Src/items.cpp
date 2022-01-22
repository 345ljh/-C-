#include "items.h"

class player
{
public:
    PlayerInit(general_t general, int health, int maxhealth, int maxcard)
    {
        maxhealth = general.maxhealth;
        health = maxhealth;
        maxcard = maxhealth;
        n
    }
private:

    int number;  //λ��
    general_t general;  //�佫��Ϣ

    int health;  //��ǰ����
    int maxhealth; //��������
    int maxcard;  //��ǰ��������
    int nowslash = 0;  //��ǰ�غ���ʹ��ɱ����
    int maxslash = 0;  //ÿ�غ�ʹ��ɱ��������
    int spirits = 0;  //�غ���ʹ�þƵ�״̬:0=δʹ��,1=��ʹ���Ҵ����˺�+1Ч��,2=��ʹ��

    int card[100]; //��ǰӵ������,����id=0����Ϊ��
    int equips[4];  //װ����,�ֱ�Ϊ����,����,+1,-1
    int judges[3]; //�ж���,�ж��׶��ɺ���ǰ����
    int range; //��ǰ��������

    int limitused;  //����δʹ�õ��޶���=1,����Ϊ0
    int awaken;  //���ھ��Ѽ����ѷ���=1,����Ϊ0
};

card_t pile[160] =
{
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, LIGHTNING, "����", 0}, {1, SPADE, GUDING, "�Ŷ���", 0},
    {2, SPADE, DUEL, "����˫�ɽ�", 0}, {2, SPADE, DUEL, "������", 0}, {2, SPADE, DUEL, "�ټ�", 0}, {2, SPADE, DUEL, "������", 0},
    {3, SPADE, DUEL, "���Ӳ���", 0}, {3, SPADE, DUEL, "˳��ǣ��", 0}, {3, SPADE, DUEL, "��", 0},
    {4, SPADE, DUEL, "���Ӳ���", 0}, {4, SPADE, DUEL, "˳��ǣ��", 0}, {4, SPADE, DUEL, "��ɱ", 0},
    {5, SPADE, DUEL, "�������µ�", 0}, {5, SPADE, DUEL, "��Ӱ", 0}, {5, SPADE, DUEL, "��ɱ", 0},
    {6, SPADE, DUEL, "�ֲ�˼��", 0}, {6, SPADE, DUEL, "���G��", 0}, {6, SPADE, DUEL, "��ɱ", 0},
    {7, SPADE, DUEL, "ɱ", 0}, {7, SPADE, DUEL, "��������", 0}, {7, SPADE, DUEL, "��ɱ", 0},
    {8, SPADE, DUEL, "ɱ", 0}, {8, SPADE, DUEL, "ɱ", 0}, {8, SPADE, DUEL, "��ɱ", 0},
    {9, SPADE, DUEL, "ɱ", 0}, {9, SPADE, DUEL, "ɱ", 0}, {9, SPADE, DUEL, "��", 0},
    {10, SPADE, DUEL, "ɱ", 0}, {10, SPADE, DUEL, "ɱ", 0}, {10, SPADE, DUEL, "����", 0},
    {11, SPADE, DUEL, "˳��ǣ��", 0}, {11, SPADE, DUEL, "��и�ɻ�", 0}, {11, SPADE, DUEL, "��������", 0},
    {12, SPADE, DUEL, "���Ӳ���", 0}, {12, SPADE, DUEL, "�ɰ���ì", 0}, {12, SPADE, DUEL, "��������", 0},
    {13, SPADE, DUEL, "��������", 0}, {13, SPADE, DUEL, "����", 0}, {13, SPADE, DUEL, "��и�ɻ�", 0},

    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},

    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},

    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},
    {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0}, {1, SPADE, DUEL, "����", 0},

};
