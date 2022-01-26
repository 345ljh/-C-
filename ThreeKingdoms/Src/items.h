#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#define PLAYERS 4  //�������
#define GENERALS 4 //��ǰ�Ѽ����佫����


typedef enum
{
    SHA = 0x00, HUOSHA, LEISHA, SHAN, TAO, JIU,
    //������:ɱ,��ɱ,��ɱ,��,��,��
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
    WEI, SHU, WU, QUN, SHEN
}nation_e; //����

typedef enum
{
    HUMAN, AI, DEAD
}controller_e; //������


typedef enum
{
    COMMON, FIRE, THUNDER
}damage_e; //�˺�����

typedef enum
{
    MALE, FEMALE
}gender_e; //�Ա�

typedef struct
{
    char name[21];  //�佫��
    int id;  //��Ӧid
    gender_e gender;  //�Ա�
    char skill[21];  //������,��������ÿո�ֿ�
    int maxhealth;  //��������
    nation_e nation;//����
    int limit;  //�޶����Ƿ�ӵ��,0=��,1=��
    int selected;  //�Ƿ��ѱ�ѡ��(����ѡ����ֹ�ظ�),0=δѡ,1=��ѡ,2=�����ѡ��
} general_t;//�佫��Ϣ

typedef struct
{
    int num;  //����
    suit_e suit; //��ɫ
    type_e type; //����
    char name[17];  //����
    int owner;  //��ǰ������,-2=�ƶ�,-1=���ƶ�
} card_t;

typedef struct
{
    int id;  //λ��
    general_t general;  //�佫��Ϣ
    controller_e controller;  //��һ�AI,�Լ�������

    int health;  //��ǰ����
    int cardamount;  //��ǰ������
    int maxhealth; //��������
    int maxcard;  //��ǰ��������
    int nowslash;  //��ǰ�غ���ʹ��ɱ����
    int maxslash;  //ÿ�غ�ʹ��ɱ��������
    int spirits;  //�غ���ʹ�þƵ�״̬:0=δʹ��,1=��ʹ���Ҵ����˺�+1Ч��,2=��ʹ��

    int card[100]; //��ǰӵ������,����id=0����Ϊ��
    int equips[4];  //װ����,�ֱ�Ϊ����,����,+1,-1
    int judges[3]; //�ж���,�ж��׶��ɺ���ǰ����
    int range; //��ǰ��������

    int chained; //�Ƿ����
    int turned;//�Ƿ���

    int limitused;  //1=����δʹ�õ��޶���=1,����Ϊ0
    int awaken;  //1=���ھ��Ѽ����ѷ���,����Ϊ0
}player_t;

typedef struct
{
    player_t player[PLAYERS];  //4����ɫ�ṹ��
    int humanid;  //���id
    int card[160];  //�ƶ�,���д���0~159,�ֱ��Ӧcard_inf�е�����
    ///һ�����������ʱ��card[0]д���û��ṹ�壬֮��������ǰ��1�±꣬����ĩβд��-1��ʾ����
    int nowpile;  //��ǰ�ƶ�ʣ����
    int turn;  //��ǰ����
    int active;  //��ǰ�ж���
    int period;  //��ǰ�׶�: 0=׼��, 1=�ж�, 2=����, 3=����, 4=����, 5=����
}game_t;

#endif // ITEMS_H_INCLUDED
