#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#define PLAYERS 4  //玩家数量
#define GENERALS 4 //当前已加入武将数量


typedef enum
{
    SHA = 0x00, HUOSHA, LEISHA, SHAN, TAO, JIU,
    //基本牌:杀,火杀,雷杀,闪,桃,酒
    ZHUGE = 0x10,
    QINGGANG = 0x20, CIXIONG, GUDING, HANBING,
    GUANSHI = 0x30, QINGLONG, ZHANGBA,
    FANGTIAN = 0x40, ZHUQUE,
    QILIN = 0x50,
    //武器,十六进制下第2位表示范围
    RENWANG = 0x60, BAGUA, TENGJIA, BAIYIN,
    //防具
    CHITU = 0x70, DAWAN, ZIXIN,
    //-1马
    JUEYING = 0x80, FEIDIAN, DILU, HUALIU,
    //+1马
    JUEDOU = 0x90, GUOCHAI, SHUNQIAN, WANJIAN, NANMAN, TAOYUAN, WUZHONG, WUGU,
    WUXIE, HUOGONG, TIESUO, JIEDAO,
    //基本锦囊:决斗,过拆,顺牵,万箭,南蛮,桃园,无中,五谷,无懈,火攻,铁锁,借刀
    LE = 0xA0, BING, SHANDIAN,
    //延时锦囊:乐,兵,闪电
}type_e; //牌类型
///本枚举均为牌名的拼音

typedef enum
{
    SPADE = 0x00, CLUB, HEART = 0x10, DIAMOND, NONE = 0x20
}suit_e; //花色

typedef enum
{
    WEI, SHU, WU, QUN, SHEN
}nation_e; //势力

typedef enum
{
    HUMAN, AI, DEAD
}controller_e; //控制者


typedef enum
{
    COMMON, FIRE, THUNDER
}damage_e; //伤害属性

typedef enum
{
    MALE, FEMALE
}gender_e; //性别

typedef struct
{
    char name[31];  //武将名
    int id;  //对应id
    gender_e gender;  //性别
    char skill[21];  //技能名,多个技能用空格分开
    int maxhealth;  //体力上限
    nation_e nation;//势力
    int limit;  //限定技是否拥有,0=无,1=有
    int selected;  //是否已被选中(用于选将防止重复),0=未选,1=已选,2=待玩家选择
} general_t;//武将信息

typedef struct
{
    int num;  //点数
    suit_e suit; //花色
    type_e type; //类型
    char name[17];  //牌名
    int owner;  //当前所有者,-2=牌堆,-1=弃牌堆
} card_t;

typedef struct
{
    int id;  //位次
    int general;  //武将id
    controller_e controller;  //玩家或AI,以及存活情况

    int health;  //当前体力
    int cardamount;  //当前手牌数
    int maxhealth; //体力上限
    int maxcard;  //当前手牌上限
    int nowslash;  //当前回合已使用杀次数
    int maxslash;  //每回合使用杀的最大次数
    int slashlimit;  //是否限制杀的使用次数
    int spirits;  //回合内使用酒的状态:0=未使用,1=已使用且存在伤害+1效果,2=已使用

    int card[100]; //当前拥有手牌,储存id=0代表为空
    int equips[4];  //装备区,分别为武器,防具,+1,-1
    int judges[3]; //判定区,判定阶段由后向前结算
    int range; //当前攻击距离

    int chained; //是否横置
    int turned;//是否翻面

    int limitused;  //1=存在未使用的限定技=1,否则为0
    int awaken;  //1=存在觉醒技且已发动,否则为0

    int temp[5];  //临时用变量,用于技能相关计算
}player_t;

typedef struct
{
    int humanid;  //玩家id
    int card[160];  //牌堆,其中储存0~159,分别对应card_inf中的排列
    ///一般情况下摸牌时将card[0]写入用户结构体，之后将所有牌前移1下标，并在末尾写入-1表示无牌
    int nowpile;  //当前牌堆剩余量
    int turn;  //当前轮数
    int active;  //当前行动者
    int period;  //当前阶段: 0=准备, 1=判定, 2=摸牌, 3=出牌, 4=弃牌, 5=结束
}game_t;

extern game_t game;
extern general_t general_inf[GENERALS];
extern card_t card_inf[160];
extern player_t player[PLAYERS];

#endif // ITEMS_H_INCLUDED
