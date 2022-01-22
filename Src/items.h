#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED
/********************************************
*            consts&variables               *
********************************************/

/********************************************
*                  enums                    *
********************************************/

typedef enum
{
    SLASH = 0x01, DODGE, PEACH, SPIRITS, FIRESLASH, THUNDERSLASH,
    //������:ɱ,��,��,��,��ɱ,��ɱ
    ZHUGE = 0x10,
    QINGGANG = 0x20, CIXIONG, GUDING, HANBING,
    GUANSHI = 0x30, QINGLONG, ZHANGBA,
    FANGTIAN = 0x40, ZHUQUE,
    QILIN = 0x50,
    //����,ʮ�������µ�2λ��ʾ��Χ
    RENWANG = 0x60, BAGUA, TENGJIA,
    //����
    CHITU = 0x70, DAWAN, ZIXIN,
    //-1��
    JUEYING = 0x80, FEIDIAN, DILU, HUALIU,
    //+1��
    DUEL = 0x90, SABOTAGE, THEFT, ARROWRAIN, SAVAGE, FRATERNITY, OUTOFNONE, HARVEST,
    DEFENSE, FIREATTACK, CHAIN, BORROWKNIFE,
    //��������:����,����,˳ǣ,���,����,��԰,����,���,��и,��,����,�赶
    DROWN = 0xA0, SUPPLYSHORT, LIGHTNING,
    //��ʱ����:��,��,����
}type_e; //������

typedef enum
{
    SPADE = 0x00, CLUB, HEART = 0x10, DIAMOND, NONE = 0x20
}suit_e; //��ɫ

typedef enum
{
    WEI, SHU, WU, QUN
} nation_e; //����


/********************************************
*                 structs                   *
********************************************/

typedef struct
{
    char name[11];  //�佫��
    int id;  //��Ӧid
    char skill[21];  //������,��������ÿո�ֿ�
    int maxhealth;  //��������
    nation_e nation;//����
    int limit;  //�޶����Ƿ����
    int awake;  //���Ѽ��Ƿ����
} general_t;//�佫��Ϣ

typedef struct
{
    int num;  //����
    suit_e suit; //��ɫ
    type_e type; //����
    char name[17];  //����
    int owner;  //��ǰ������,0��ʾ�ƶ�,-1��ʾ���ƶ�

} card_t;
#endif // ITEMS_H_INCLUDED
