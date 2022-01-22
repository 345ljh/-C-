#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED


/**************************************************************************************
*                                       enums                                         *
**************************************************************************************/

typedef enum
{
    SHA = 0x01, SHAN, TAO, JIU, HUOSHA, LEISHA,
    //������:ɱ,��,��,��,��ɱ,��ɱ
    ZHUGE = 0x10,
    QINGGANG = 0x20, CIXIONG, GUDING, HANBING,
    GUANSHI = 0x30, QINGLONG, ZHANGBA,
    FANGTIAN = 0x40, ZHUQUE,
    QILIN = 0x50,
    //����,ʮ�������µ�2λ��ʾ��Χ
    RENWANG = 0x60, BAGUA, TENGJIA, BAIYIN,
    //����
    CHITU = 0x70, DAWAN, ZIXIN,
    //-1��
    JUEYING = 0x80, FEIDIAN, DILU, HUALIU,
    //+1��
    JUEDOU = 0x90, GUOCHAI, SHUNQIAN, WANJIAN, NANMAN, TAOYUAN, WUZHONG, WUGU,
    WUXIE, HUOGONG, TIESUO, JIEDAO,
    //��������:����,����,˳ǣ,���,����,��԰,����,���,��и,��,����,�赶
    LE = 0xA0, BING, SHANDIAN,
    //��ʱ����:��,��,����
}type_e; //������
///��ö�پ�Ϊ������ƴ��

typedef enum
{
    SPADE = 0x00, CLUB, HEART = 0x10, DIAMOND, NONE = 0x20
}suit_e; //��ɫ

typedef enum
{
    WEI, SHU, WU, QUN
} nation_e; //����


/**************************************************************************************
*                                     structs                                         *
**************************************************************************************/

typedef struct
{
    char name[11];  //�佫��
    int id;  //��Ӧid
    char skill[21];  //������,��������ÿո�ֿ�
    int maxhealth;  //��������
    nation_e nation;//����
    int limit;  //�޶����Ƿ�ӵ��
} general_t;//�佫��Ϣ

typedef struct
{
    int num;  //����
    suit_e suit; //��ɫ
    type_e type; //����
    char name[17];  //����
    int owner;  //��ǰ������,0��ʾ�ƶ�,-1��ʾ���ƶ�

} card_t;

/**************************************************************************************
*                                  consts&variables                                   *
**************************************************************************************/
extern card_t pile[160];


#endif // ITEMS_H_INCLUDED