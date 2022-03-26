/**
 * @file    ai.cpp
 * @brief   AI对牌局的分析与判断
 * @note    所有函数以"Ai"结尾
 */


#include "ai.h"

//弃牌AI
///返回值用于白银狮子效果
int ThrowAi(player_t* recipient, int* state, int area, int suit, int type, int add)
{
    int maxstate = -1;
    int len = 1;
    int *tothrow = NULL;
    tothrow = (int*)malloc(sizeof(int));
    if(area & 1) for(int i = 0; i <= recipient->cardamount - 1; i++)
            if (suit & (1 << (int)card_inf[recipient->card[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->card[i]].type))))
            {
                /*优先级变高,重置tothrow,此时只有1个元素*/
                if(StateCompareAi(state, recipient->card[i]) > maxstate)
                {
                    maxstate = StateCompareAi(state, recipient->card[i]);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = recipient->card[i];
                    len = 1;
                }
                /*优先级相等,增加1个元素*/
                else if(StateCompareAi(state, recipient->card[i]) == maxstate)
                {
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, ++len * sizeof(int));
                    tothrow[len - 1] = recipient->card[i];
                }
            }
    if(area & 2) for(int i = (add == 0x30) ? 1 : (add == 0x50 ? 2 : 0); i <= 3; i++)
            if ((recipient->equips[i] != -1) && suit & (1 << (int)card_inf[recipient->equips[i]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->equips[i]].type))))
            {
                if(StateCompareAi(state, recipient->equips[i] | 0x100) > maxstate)
                {
                    maxstate = StateCompareAi(state, recipient->equips[i] | 0x100);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = recipient->equips[i] | 0x100;
                    len = 1;
                }
                else if(StateCompareAi(state, recipient->equips[i] | 0x100) == maxstate)
                {
                    realloc(tothrow, ++len * sizeof(int));
                    tothrow[len - 1] = recipient->equips[i] | 0x100;
                }
            }
    if(area & 4) for(int i = 0; i <= 2; i++)
            if (suit & (1 << (int)card_inf[recipient->judges[i][0]].suit) && (type & (1 << TypeIdentify(card_inf[recipient->judges[i][0]].type))))
            {
                if(StateCompareAi(state, recipient->judges[i][0] | 0x200) > maxstate)
                {
                    maxstate = StateCompareAi(state, recipient->judges[i][0] | 0x200);
                    int *ptemp = tothrow;
                    tothrow = (int*)realloc(ptemp, sizeof(int));
                    tothrow[0] = recipient->judges[i][0] | 0x200;
                    len = 1;
                }
                else if(StateCompareAi(state, recipient->judges[i][0] | 0x200) == maxstate)
                {
                    realloc(tothrow, ++len * sizeof(int));
                    tothrow[len - 1] = recipient->judges[i][0] | 0x200;
                }
            }

    int sel = rand() % len;
    int baiyin = 0;  //白银狮子效果

    switch(sel >> 16)
    {
    case 0:
    {
        card_inf[recipient->card[sel]].owner = -1;
        Putcard(recipient->card[sel]);
        Printcard(recipient->card[sel]);
        recipient->card[sel] = -1;
        recipient->cardamount--;
        IndexAlign(recipient->card, recipient->cardamount, 160);
        break;
    }
    case 1:
    {
        card_inf[recipient->equips[sel & 0xFF]].owner = -1;
        Putcard(recipient->equips[sel & 0xFF]);
        Printcard(recipient->equips[sel & 0xFF]);
        if( (type_e)recipient->equips[sel & 0xFF] == BAIYIN)  baiyin = 1;
        recipient->equips[sel & 0xFF] = -1;
        break;
    }
    case 2:
    {
        card_inf[recipient->judges[sel & 0xFF][0]].owner = -1;
        Putcard(recipient->judges[sel & 0xFF][0]);
        Printcard(recipient->judges[sel & 0xFF][0]);
        recipient->judges[sel & 0xFF][0] = -1;
        recipient->judges[sel & 0xFF][1] = -1;
        for(int j = 0; j <= 1; j++)
        {
            int temp[3];
            for(int k = 0; k <= 2; k++) temp[k] = recipient->judges[k][j];
            IndexAlign(temp, ArrayOccupied(temp, 3), 3);
            for(int k = 0; k <= 2; k++) recipient->judges[k][j] = temp[k];
        }
        break;
    }
    }
    free(tothrow);
    return baiyin;
}

//对自己弃牌AI
///此函数仅用于一张牌的计算,返回值为牌在区域中的id
///通过计算卡牌的可能收益(不考虑牌在手中未使用的收益),选择收益最小的一张弃置
int ThrowAiSelf(player_t *recipient, int area, int suit, int type, int add)
{
    int id = 0;  //卡牌在玩家区域的id
    double minprofit = 99;  //当前最小收益
    player_t &teammate = player[3 - recipient->id];
    player_t enemy[2] = {player[(2 + recipient->id) % 4], player[(5 - recipient->id) % 4]};

    if(area | 1)
    {
        for(int i = 0; i <= recipient->cardamount - 1; i++)
        {
            double profit = 99;
            if(recipient->card[i] != -1 && suit & (1 << (int)card_inf[recipient->card[i]].suit) && type & (1 << TypeIdentify(card_inf[recipient->card[i]].type) ))
            {
                //根据类别执行分析
                if(card_inf[recipient->card[i]].type == SHA || card_inf[recipient->card[i]].type == LEISHA || card_inf[recipient->card[i]].type == HUOSHA)
                {
                    double temp[2] = {0, 0};
                    for(int j = 0; j <= 1; j++)
                    {
                        //可用性计算,此步骤后temp为基础伤害*2
                        if(enemy[j].controller == DEAD) continue;
                        if(card_inf[enemy[j].equips[1]].type == RENWANG && card_inf[i].suit >> 1 == 0 && card_inf[recipient->equips[0]].type != QINGGANG)
                        {
                            temp[j] = 0;
                            continue;
                        }
                        if(card_inf[enemy[j].equips[1]].type == TENGJIA)
                        {
                            if(card_inf[recipient->equips[0]].type == ZHUQUE || card_inf[recipient->card[i]].type == HUOSHA) temp[j] = 4;
                            else if(card_inf[recipient->equips[0]].type == QINGGANG || card_inf[recipient->card[i]].type == LEISHA) temp[j] = 2;
                            else
                            {
                                temp[j] = 0;
                                continue;
                            }
                        }
                        //距离,其中enemy[0]处于对位
                        int distance = (j == 0 && player[(enemy[0].id + 1) % 4].controller != DEAD && player[(enemy[0].id + 3) % 4].controller != DEAD) ? 2 : 1;
                        //+1与-1马的计算
                        distance += enemy[j].equips[3] != -1;
                        distance -= recipient->equips[2] != -1;
                        //比较攻击范围
                        int range = recipient->equips[0] != -1 ? (int)card_inf[recipient->equips[0]].type >> 4 : 1;
                        if(distance > range)
                        {
                            temp[j] = 0;
                            continue;
                        }
                        //根据对方手牌计算有闪的期望,牌堆中闪共24张(15%)
                        ///此步后temp = p(有闪) + temp * (1 - p(有闪))
                        double p = 0.15 * enemy[j].cardamount + 0.5 * card_inf[enemy[j].equips[1]].type == BAGUA;
                        if(p > 1) p = 1;
                        temp[j] = p + temp[j] * (1 - p);
                        //回合外降低此部分收益
                        if(game.active != recipient->id || game.period > 3) profit *= 0.7;
                    }
                    //用于响应决斗(3),南蛮入侵(3),借刀杀人(2)时视为1收益,当体力更低时提高(0.2每损失体力)权重
                    for(int j = 0; j <= 1; j++)  temp[i] += 0.125 + (recipient->maxhealth - recipient->health) * 0.2;
                    //取较大者为理论收益
                    profit = temp[0] > temp[1] ? temp[0] : temp[1];
                }
                if(card_inf[recipient->card[i]].type == SHAN)
                {
                    //用于响应杀(44),万箭(1),当体力更低时提高(0.35每损失体力)权重
                    profit = 0.5625 + (recipient->maxhealth - recipient->health) * 0.35;
                    //回合外增加权重
                    if(game.active != recipient->id || game.period > 3) profit += 1;
                }
                if(card_inf[recipient->card[i]].type == TAO)  profit = 2;
                if(card_inf[recipient->card[i]].type == JIU)
                {
                    //回合内收益,算法与杀类似
                    if(game.active == recipient->id && game.period <= 3)
                    {
                        double temp[2] = {0, 0};
                        for(int j = 0; j <= 1; j++)
                        {
                            if(enemy[j].controller == DEAD) continue;
                            double p = 0.15 * enemy[j].cardamount + 0.5 * card_inf[enemy[j].equips[1]].type == BAGUA;
                            if(p > 1) p = 1;
                            temp[j] = p + 1.5 * (1 - p);
                        }
                        profit = (temp[0] > temp[1]) ? temp[0] : temp[1];
                    }
                    else profit = 0;
                    //回合外收益,当体力不大于2时提高权重
                    profit += recipient->health == 1 ? 1 : 0.5;
                }
                //计算装备,考虑当前该区域是否已装备
                if(TypeIdentify(card_inf[recipient->card[i]].type) == 1)
                {
                    //回合内
                    if(game.active == recipient->id && game.period <= 3)
                    {
                        if( (int)card_inf[recipient->card[i]].type >= 0x70 || card_inf[recipient->card[i]].type < 0x80)
                        {
                            if(recipient->equips[2] == -1) profit = 1.2;
                            else profit = 0.4;
                        }
                        if( (int)card_inf[recipient->card[i]].type >= 0x80 || card_inf[recipient->card[i]].type < 0x90)
                        {
                            if(recipient->equips[2] == -1) profit = 1.3;
                            else profit = 0.4;
                        }
                        if( (int)card_inf[recipient->card[i]].type >= 0x60 || card_inf[recipient->card[i]].type < 0x70)
                        {
                            if(card_inf[recipient->equips[1]].type == BAIYIN && recipient->health < recipient->maxhealth) profit = 1.9;
                            else if(recipient->equips[1] == -1 || (card_inf[recipient->equips[1]].type == TENGJIA && card_inf[recipient->card[i]].type != TENGJIA)) profit = 1.5;
                            else profit = 0.4;
                        }
                        if( (int)card_inf[recipient->card[i]].type >= 0x10 || card_inf[recipient->card[i]].type < 0x60)
                        {
                            if(recipient->equips[0] == -1) profit = 1.3;
                            else profit = 0.4;
                        }
                    }
                    else profit = 0.4;
                }
                //锦囊,当前代码不考虑回合内对方有无懈
                if( (int)card_inf[recipient->card[i]].type >= 0x90 || card_inf[recipient->card[i]].type < 0xA0)
                {
                    if(card_inf[recipient->card[i]].type == WUXIE)
                    {
                        if(game.active == recipient->id && game.period <= 3) profit = 0.45 + (recipient->maxhealth - recipient->health) * 0.3;
                        else profit = 0.5 + (recipient->maxhealth - recipient->health) * 0.35;
                    }
                    else
                    {
                        if(game.active == recipient->id && game.period <= 3)
                        {
                            if(card_inf[recipient->card[i]].type == JUEDOU)
                            {
                                double temp[2] = {0, 0};
                                for(int j = 0; j <= 1; j++)
                                {
                                    if(enemy[j].controller == DEAD) continue;
                                    temp[j] = (recipient->cardamount - 1 - enemy[j].cardamount) * 0.4;
                                    if(temp[j] > 2) temp[j] = 2;
                                    else if(temp[j] < -2) temp[j] = -2;
                                }
                                profit = (temp[0] > temp[1]) ? temp[0] : temp[1];
                            }
                            if(card_inf[recipient->card[i]].type == GUOCHAI)  profit = 1;
                            if(card_inf[recipient->card[i]].type == SHUNQIAN)  profit = 1.5;
                            if(card_inf[recipient->card[i]].type == WANJIAN)
                            {
                                profit = 0;
                                //计算对敌方
                                for(int j = 0; j <= 1; j++)
                                {
                                    if(enemy[j].controller == DEAD || card_inf[enemy[j].equips[1]].type == TENGJIA) continue;
                                    double p = 0.15 * enemy[j].cardamount + 0.5 * card_inf[enemy[j].equips[1]].type == BAGUA;
                                    if(p > 1) p = 1;
                                    profit += 2 - p;
                                }
                                //计算对队友
                                if(teammate.controller != DEAD && card_inf[teammate.equips[1]].type != TENGJIA)
                                {
                                    double p = 0.15 * teammate.cardamount + 0.5 * card_inf[teammate.equips[1]].type == BAGUA;
                                    if(p > 1) p = 1;
                                    profit -= 2 - p;
                                }
                            }
                            if(card_inf[recipient->card[i]].type == WANJIAN)
                            {
                                profit = 0;
                                //计算对敌方
                                for(int j = 0; j <= 1; i++)
                                {
                                    if(enemy[j].controller == DEAD || card_inf[enemy[j].equips[1]].type == TENGJIA) continue;
                                    double p = 0.2 * enemy[j].cardamount;
                                    if(p > 1) p = 1;
                                    profit += 2 - p;
                                }
                                //计算对队友
                                if(teammate.controller != DEAD && card_inf[teammate.equips[1]].type != TENGJIA)
                                {
                                    double p = 0.2 * teammate.cardamount;
                                    if(p > 1) p = 1;
                                    profit -= 2 - p;
                                }
                            }
                            if(card_inf[recipient->card[i]].type == TAOYUAN)
                            {
                                profit = 0.1;  //增加初始权重
                                for(int j = 0; j <= 1; j++)
                                {
                                    if(enemy[j].controller == DEAD) continue;
                                    else profit -= (1 - (float)enemy[j].health / enemy[j].maxhealth) * 3;  //体力越低,回复1体力带来的收益权重越高
                                }
                                if(teammate.controller != DEAD)  profit += (1 - (float)teammate.health / teammate.maxhealth) * 3;
                                if(recipient->controller != DEAD)  profit += (1 - (float)recipient->health / recipient->maxhealth) * 3;
                            }
                            if(card_inf[recipient->card[i]].type == WUZHONG)  profit = 2;
                            if(card_inf[recipient->card[i]].type == WUGU)
                            {
                                profit = 0.1;
                                for(int j = 0; j <= 1; j++)
                                {
                                    if(enemy[j].controller == DEAD) continue;
                                    else profit -= 1.4 - ((enemy[j].id - recipient->id + 4) % 4) * 0.2;  //距离使用者越远,权重越低
                                }
                                if(teammate.controller != DEAD)  profit += 1.4 - ((teammate.id - recipient->id + 4) % 4) * 0.2;
                                if(recipient->controller != DEAD)  profit += 1.4;
                            }
                            if(card_inf[recipient->card[i]].type == HUOGONG)
                            {
                                profit = 0;
                                //计算手牌花色数
                                int cardsuit[4] = {0, 0, 0, 0};
                                for(int j = 0; j <= recipient->cardamount - 1; i++) if(j != i) cardsuit[(int)card_inf[recipient->card[j]].suit] = 1;
                                for(int j = 0; j <= 3; j++) profit += cardsuit[j] * 0.5;
                                if( card_inf[enemy[0].equips[1]].type == TENGJIA || card_inf[enemy[0].equips[2]].type == TENGJIA || (enemy[0].chained && enemy[1].chained) ) profit *= 2;
                                profit += 0.1;
                            }
                            if(card_inf[recipient->card[i]].type == TIESUO) profit = 1;
                            if(card_inf[recipient->card[i]].type == JIEDAO)
                            {
                                if(enemy[0].equips[0] == -1 && enemy[1].equips[0] == -1) profit = 0;
                                else profit = 1;
                            }
                            if(card_inf[recipient->card[i]].type == LE)  profit = 3;
                            if(card_inf[recipient->card[i]].type == BING) profit = 1.5;
                            if(card_inf[recipient->card[i]].type == SHANDIAN)
                            {
                                profit = -0.4;
                                if(teammate.controller != DEAD) profit -= 0.4;
                                for(int j = 0; j <= 1; j++) if(enemy[j].controller != DEAD) profit += 0.4;
                            }
                        }
                        else profit = 0.4;
                    }
                }
            }
            if(profit < minprofit)
            {
                minprofit = profit;
                id = i;
            }
        }
    }
    if(area | 2)
    {
        double profit = 99;
        for(int i = 0; i <= 3; i++)
        {
            if(recipient->equips[i] != -1 &&  suit & (1 << (int)card_inf[recipient->equips[i]].suit) && type & (1 << TypeIdentify(card_inf[recipient->equips[i]].type) ))
            {
                profit = 2;
            }
            if(profit < minprofit)
            {
                minprofit = profit;
                id = i | 0x100;
            }
        }
    }
    if(area | 4)
    {
        double profit = 99;
        for(int i = 0; i <= 2; i++)
        {
            if(recipient->judges[i][0] != -1 &&  suit & (1 << (int)card_inf[recipient->judges[i][0]].suit) && type & (1 << TypeIdentify(card_inf[recipient->judges[i][0]].type) ))
            {
                if(card_inf[recipient->judges[i][0]].type == LE) profit = -3;
                if(card_inf[recipient->judges[i][0]].type == BING) profit = -1.5;
                if(card_inf[recipient->judges[i][0]].type == SHANDIAN)
                {
                    profit = -0.4;
                    if(teammate.controller != DEAD) profit -= 0.4;
                    for(int j = 0; j <= 1; j++) if(enemy[j].controller != DEAD) profit += 0.4;
                }
            }
            if(profit < minprofit)
            {
                minprofit = profit;
                id = i | 0x200;
            }
        }
    }
    return id;
}

//获得牌AI
int GetAi(player_t *executor, player_t* recipient, int* state, int area)
{
    int maxstate = -1;
    int len = 1;
    int *tothrow = NULL;
    tothrow = (int*)malloc(sizeof(int));

    if(area & 1) for(int i = 0; i <= recipient->cardamount - 1; i++)
    {
        /*优先级变高,重置tothrow,此时只有1个元素*/
        if(StateCompareAi(state, recipient->card[i]) > maxstate)
        {
            maxstate = StateCompareAi(state, recipient->card[i]);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = recipient->card[i];
            len = 1;
        }
        /*优先级相等,增加1个元素*/
        else if(StateCompareAi(state, recipient->card[i]) == maxstate)
        {
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, ++len * sizeof(int));
            tothrow[len - 1] = recipient->card[i];
        }
    }
    if(area & 2) for(int i = 0; i <= 3; i++)
    {
        if(StateCompareAi(state, recipient->equips[i] | 0x100) > maxstate)
        {
            maxstate = StateCompareAi(state, recipient->equips[i] | 0x100);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = recipient->equips[i] | 0x100;
            len = 1;
        }
        else if(StateCompareAi(state, recipient->equips[i] | 0x100) == maxstate)
        {
            realloc(tothrow, ++len * sizeof(int));
            tothrow[len - 1] = recipient->equips[i] | 0x100;
        }
    }
    if(area & 4) for(int i = 0; i <= 2; i++)
    {
        if(StateCompareAi(state, recipient->judges[i][0] | 0x200) > maxstate)
        {
            maxstate = StateCompareAi(state, recipient->judges[i][0] | 0x200);
            int *ptemp = tothrow;
            tothrow = (int*)realloc(ptemp, sizeof(int));
            tothrow[0] = recipient->judges[i][0] | 0x200;
            len = 1;
        }
        else if(StateCompareAi(state, recipient->judges[i][0] | 0x200) == maxstate)
        {
            realloc(tothrow, ++len * sizeof(int));
            tothrow[len - 1] = recipient->judges[i][0] | 0x200;
        }
    }

    int sel = rand() % len;
    int baiyin = 0;

    switch(sel >> 16)
    {
    case 0:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = recipient->card[sel];
        card_inf[recipient->card[sel]].owner = executor->id;

        if(executor->controller == HUMAN || recipient->controller == HUMAN) Printcard(recipient->card[sel]);

        recipient->card[sel] = -1;
        recipient->cardamount--;
        IndexAlign(recipient->card, recipient->cardamount, 160);
        break;
    }
    case 1:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = recipient->equips[sel & 0xFF];
        card_inf[recipient->equips[sel & 0xFF]].owner = executor->id;
        if(executor->controller == HUMAN || recipient->controller == HUMAN) Printcard(recipient->equips[sel & 0xFF]);
        if( (type_e)recipient->equips[sel & 0xFF] == BAIYIN)  baiyin = 1;
        recipient->equips[sel & 0xFF] = -1;
        break;
    }
    case 2:
    {
        executor->cardamount++;
        executor->card[executor->cardamount - 1] = recipient->judges[sel & 0xFF][0];
        card_inf[recipient->judges[sel & 0xFF][0]].owner = executor->id;

        if(executor->controller == HUMAN || recipient->controller == HUMAN) Printcard(recipient->judges[sel & 0xFF][0]);

        recipient->judges[sel & 0xFF][0] = -1;
        recipient->judges[sel & 0xFF][1] = -1;
        for(int j = 0; j <= 1; j++)
        {
            int temp[3];
            for(int k = 0; k <= 2; k++) temp[k] = recipient->judges[k][j];
            IndexAlign(temp, ArrayOccupied(temp, 3), 3);
            for(int k = 0; k <= 2; k++) recipient->judges[k][j] = temp[k];
        }
        break;
    }
    }
    free(tothrow);
    return baiyin;
}

//出牌阶段AI
///若为出牌,返回值低8位为牌在手牌区的id,8~11位为目标
int PlayAi(player_t* executor)
{
    player_t &teammate = player[3 - executor->id];
    player_t enemy[2] = {player[(2 + executor->id) % 4], player[(5 - executor->id) % 4]};

    int card = 0xFF;  //所出牌的id,若弃牌则为0xFF
    int target = 0;  //目标
    float maxprofit = 0;  //当前最大收益
    //判断出牌
    ///此函数考虑牌留在手中用于响应,从而出牌造成的"负收益"
    for(int i = 0; i <= executor->cardamount - 1; i++)
    {
        float nowprofit = 0;  //当前收益
        int nowtarget = 0;  //当前目标
        //三种杀
        ///此AI暂不考虑对队友出杀以触发技能的操作
        if( (int)card_inf[executor->card[i]].type == SHA || (int)card_inf[executor->card[i]].type == HUOSHA || (int)card_inf[executor->card[i]].type == LEISHA)
        {
            //次数限制
            if(executor->slashlimit && (type_e)card_inf[executor->equips[0]].type != ZHUGE && executor->nowslash >= executor->maxslash) continue;

            double temp[2];  //分别计算对两目标收益
            for(int j = 0; j <= 1; j++)
            {
                temp[j] = -0.2 * ((enemy[0].controller != DEAD) + (enemy[1].controller != DEAD));  //保留收益,若无法使用,continue后结果<0,判定为不使用

                //可用性判定
                if(enemy[j].controller == DEAD) continue;
                if(card_inf[enemy[j].equips[1]].type == RENWANG && card_inf[i].suit >> 1 == 0 && card_inf[executor->equips[0]].type != QINGGANG)  continue;
                //距离,其中enemy[0]处于对位
                int distance = (j == 0 && player[(enemy[0].id + 1) % 4].controller != DEAD && player[(enemy[0].id + 3) % 4].controller != DEAD) ? 2 : 1;
                //+1与-1马的计算
                distance += enemy[j].equips[3] != -1;
                distance -= executor->equips[2] != -1;
                //比较攻击范围
                int range = executor->equips[0] != -1 ? (int)card_inf[executor->equips[0]].type >> 4 : 1;
                if(distance > range)  continue;
                //藤甲
                if(card_inf[enemy[j].equips[1]].type == TENGJIA)
                {
                    if(card_inf[executor->equips[0]].type == ZHUQUE || card_inf[executor->card[i]].type == HUOSHA) temp[j] += 4;
                    else if(card_inf[executor->equips[0]].type == QINGGANG || card_inf[executor->card[i]].type == LEISHA) temp[j] += 2;
                    else continue;
                }

                //装备麒麟弓且目标有马
                if(card_inf[enemy[j].equips[0]].type == QILIN && (enemy[j].equips[2] != -1 || enemy[j].equips[3] != -1)) temp[j] += 1.2;

                //计算出闪期望
                double p = 0.15 * enemy[j].cardamount + 0.5 * card_inf[enemy[j].equips[1]].type == BAGUA;
                if(p > 1) p = 1;
                temp[j] = p + temp[j] * (1 - p);
            }
            //选择目标
            if(executor->targets >= 2 && temp[0] > 0 && temp[1] > 0)  //选多个目标
            {
                nowtarget = (1 << enemy[0].id) | (1 << enemy[1].id);
                nowprofit = temp[0] + temp[1];
            }
            else
            {
                nowtarget = temp[0] > temp[1] ? (1 << enemy[0].id) : (1 << enemy[1].id);
                nowprofit = temp[0] > temp[1] ? temp[0] : temp[1];
            }
        }
        if(nowprofit > maxprofit)
        {
            maxprofit = nowprofit;
            card = i;
            target = nowtarget;
        }
    }

    return (target << 8) + card;
}

//弃牌权重比对,返回值较大者优先弃置
///state中元素按顺序代表:[手牌区]杀,闪,桃/酒,无懈,其他普通锦囊,延时锦囊,装备,[判定区],[装备区]武器,防具,-1,+1
///id低16位为card_inf对应的id,高8位表示区域
int StateCompareAi(int state[13], int id)
{
    int area = id >> 16;
    id &= 0xFF;
    return
    ( (int)card_inf[id].type <= 2 && !area) ? state[0] :
    (card_inf[id].type == SHAN && !area) ? state[1] :
    ((card_inf[id].type == TAO || card_inf[id].type == JIU) && !area) ? state[2] :
    (card_inf[id].type == WUXIE && !area) ? state[3] :
    ( ((int)card_inf[id].type & 0xF0) == 0x90 && !area) ? state[4] :
    ( ((int)card_inf[id].type & 0xF0) == 0xA0 && !area) ? state[5] :
    !area ? state[6] :
    area == 2 ? state[7] :
    ((int)card_inf[id].type >= 0x10 && (int)card_inf[id].type < 0x60) ? state[8] :
    ( ((int)card_inf[id].type & 0xF0) == 0x60) ? state[9] :
    ( ((int)card_inf[id].type & 0xF0) == 0x70) ? state[10] : state[11];
}

//出牌响应AI
///返回值为手牌中的位置,若没有符合条件的牌则返回-1
///若type为SHA则3种类型的杀均可响应
int AnswerAi(player_t *recipient, type_e type, int add)
{
        int *buf = NULL;
        int amount = 0;
        buf = (int*)calloc(0, sizeof(int));

        //判断手牌中是否有对应类型,若有则随机出一张
        for(int i = 0; i <= recipient->cardamount - 1; i++)
        {
            if( (type == SHA && (card_inf[recipient->card[i]].type == SHA || card_inf[recipient->card[i]].type == HUOSHA || card_inf[recipient->card[i]].type == LEISHA)) ||
                card_inf[recipient->card[i]].type == type ||
                (add == 2 && card_inf[recipient->card[i]].type == JIU) )
               {
                   int *ptemp = buf;
                   buf = (int*)realloc(ptemp, ++amount * sizeof(int));
                   buf[amount - 1] = i;
               }
        }

        if(amount) return buf[rand() % amount];
        else return -1;
}

//五谷AI
int WuguAi(player_t* recipient, int* buf, int len)
{
    int *state = NULL;
    state = (int*)calloc(len, sizeof(int));

    //分别计算剩余牌的权重
    for(int i = 0; i <= len - 1; i++)
    {
        if(buf[i] == -1)
        {
            state[i] = 100;
            continue;
        }
        if(card_inf[buf[i]].type == SHA || card_inf[buf[i]].type == HUOSHA || card_inf[buf[i]].type == LEISHA)
        {
            if(game.active == recipient->id) state[i] = 3;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == SHAN)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == TAO)
        {
            state[i] = 0;
        }
        if(card_inf[buf[i]].type == JIU)
        {
            if(recipient->health == 1) state[i] = 0;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == JUEDOU)
        {
            if(game.active == recipient->id) state[i] = 3;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == GUOCHAI)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == SHUNQIAN)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == WANJIAN)
        {
            if(game.active == recipient->id &&
               (player[3 - recipient->id].health > 1 || player[(recipient->id + 2) % 4].health == 1 || player[(recipient->id + 2) % 4].health == 1)) state[i] = 2;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == NANMAN)
        {
            if(game.active == recipient->id &&
               (player[3 - recipient->id].health > 1 || player[(recipient->id + 2) % 4].health == 1 || player[(recipient->id + 2) % 4].health == 1)) state[i] = 2;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == TAOYUAN)
        {
            if(game.active == recipient->id &&
               (recipient->health < recipient->maxhealth && player[3 - recipient->id].health < player[3 - recipient->id].maxhealth)) state[i] = 2;
            else state[i] = 4;
        }
        if(card_inf[buf[i]].type == WUZHONG)
        {
            if(game.active == recipient->id) state[i] = 0;
            else state[i] = 1;
        }
        if(card_inf[buf[i]].type == WUGU)
        {
            if(game.active == recipient->id) state[i] = 4;
            else state[i] = 5;
        }
        if(card_inf[buf[i]].type == WUXIE)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == HUOGONG)
        {
            if(game.active == recipient->id && recipient->cardamount >= 3) state[i] = 3;
            else state[i] = 3;
        }
        if(card_inf[buf[i]].type == TIESUO)
        {
            state[i] = 4;
        }
        if(card_inf[buf[i]].type == JIEDAO)
        {
            if(game.active == recipient->id && (player[(recipient->id + 2) % 4].equips[0] != -1 || player[(recipient->id + 3) % 4].equips[0] != -1)) state[i] = 2;
            else state[i] = 4;
        }
        if(card_inf[buf[i]].type == LE)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == BING)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if(card_inf[buf[i]].type == SHANDIAN)
        {
            if(game.active == recipient->id) state[i] = 5;
            else state[i] = 5;
        }
        if( (int)card_inf[buf[i]].type >= 0x10 && (int)card_inf[buf[i]].type <= 0x60)
        {
            if(game.active == recipient->id) state[i] = 2;
            else state[i] = 4;
        }
        if( (int)card_inf[buf[i]].type >= 0x60 && (int)card_inf[buf[i]].type <= 0x70)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 2;
        }
        if( (int)card_inf[buf[i]].type >= 0x70 && (int)card_inf[buf[i]].type <= 0x90)
        {
            if(game.active == recipient->id) state[i] = 1;
            else state[i] = 3;
        }
    }

    int res = 0;
    for(int i = 1; i <= len - 1; i++) if(state[i] < state[res]) res = i;
    return res;
}
