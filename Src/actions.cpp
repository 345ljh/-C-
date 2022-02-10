#include "actions.h"

///本文件中game均作为全局变量出现

//牌堆初始化
void PileInit(void)
{
    int temp[160];
    int pointer;

    memset(temp, 0, sizeof(temp));

    for(int i = 0; i <= 159; i++)
    {
        pointer = rand() % 160;
        while(temp[pointer]) ++pointer %= 160;
        game.card[i] = pointer;
        temp[pointer] = 1;
    }

    game.nowpile = 160;
}

//洗牌
void Shuffle(void)
{
    int pilecards = 0;
    int pointer;

    cleardevice(gui.throwcard);

    //统计弃牌堆内牌的数量
    for(int i = 0; i <= 150; i++)
    {
        if(card_inf[i].owner == -1) pilecards++;
    }

    for(int i = 0; i <= pilecards - 1; i++)
    {
        pointer = rand() % 160;
        while(card_inf[pointer].owner != -1) ++pointer %= 160;
        game.card[i] = pointer;
        card_inf[pointer].owner = -2;
    }

    game.nowpile = pilecards;
}

//从牌堆顶摸牌
void Takecard(player_t *player, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        if(!game.nowpile) Shuffle();
        card_inf[game.card[0]].owner = player->id;
        player->card[player->cardamount] = game.card[0];
        player->cardamount++;
        game.nowpile--;

        for(int j = 0; j <= game.nowpile - 1; j++)
        {
            game.card[j] = game.card[j + 1];
        }
        game.card[game.nowpile] = -1;
    }

    /* skills here */
}

//弃牌,其中executor为弃牌者,player为被弃牌者
///area表示可选其牌区域,0位为1表示手牌区,1位为装备区,2位为判定区,默认值为1(即只可弃置手牌)
///此函数下弃手牌不能对所弃牌的种类进行限制,如火攻时的相同花色
void Throwcard(player_t *executor, player_t *player, int amount, int area)
{

    //玩家操作弃牌
    if(executor->controller == HUMAN)
    {
        //对玩家自己弃牌(如弃牌阶段)
        if(player->controller == HUMAN)
        {
            //判断对应区域有牌
            area |= 1 & (player->card[0] != -1);
            area |= 2 & (player->equips[0] != -1 || player->equips[1] != -1 || player->equips[2] != -1 || player->equips[3] != -1);
            area |= 4 & (player->judges[0][0] != -1 || player->judges[1][0] != -1 || player->judges[2][0] != -1);

            int *tothrow = NULL;
            tothrow = (int*)calloc(amount, sizeof(int));   //储存所弃牌在区域中的位置id
            memset(tothrow, 0xFF, amount * sizeof(int));

            //此部分不涉及判定区
            for(; is_run(); delay_fps(10))
            {
                while (mousemsg()) msg = getmouse();
                mousepos(&mouse_x, &mouse_y);

                DrawGui();
                cleardevice(gui.selector);

                //弃牌数提示
                setcolor(WHITE, gui.selector);
                setfont(30, 0, "仿宋", gui.selector);
                outtextxy(550, 415, Link( Link( Link( Link( (char*)"弃置", Myitoa(ArrayOccupied(tothrow, amount) )), (char*)"/"), MyitoaII(amount)), (char*)"张牌"), gui.selector);

                //全部绘制为未选定状态
                if(area & 1) for(int i = 0; i <= 7; i++)
                        if(player->card[game.page * 8 + i] != -1) LineRect(160 + 100 * i, 465, 240 + 100 * i, 585, EGERGB(255, 215, 77), gui.selector);
                if(area & 2) for(int i = 0; i <= 3; i++)
                        if(player->equips[i] != -1) LineRect(5, 453.75 + 37.5 * i, 145, 483.75 + 37.5 * i, EGERGB(255, 215, 77), gui.selector);

                //绘制已选定状态,tothrow低8位为选定牌在card/equips/judges的下标,高8位代表选定区域,0=手牌,1=装备区,2=判定区
                for(int i = 0; i <= amount - 1; i++)
                {
                    if(tothrow[i] != -1 && player->card[tothrow[i]] != -1 && tothrow[i] >> 8 == 0 && game.page == tothrow[i] / 8)
                        LineRect(160 + 100 * (tothrow[i] % 8), 465, 240 + 100 * (tothrow[i] % 8), 585, EGERGB(255, 57, 57), gui.selector);
                    if(tothrow[i] != -1 && tothrow[i] >> 8 == 1 && player->equips[tothrow[i] % 0x10] != -1)
                        LineRect(5, 453.75 + 37.5 * (tothrow[i] & 0xF), 145, 483.75 + 37.5 * (tothrow[i] & 0xF), EGERGB(255, 57, 57), gui.selector);
                }

                //确定键
                if(ArrayOccupied(tothrow, amount) == amount) LineRect(960, 510, 1050, 535, EGERGB(255, 215, 77), gui.selector);

                //检测按键
                //手牌区
                if( (area & 1) && msg.is_down() && mouse_x >= 150 && mouse_x <= 950 && mouse_y >= 450 && mouse_y <= 600)
                {
                    int sel = (mouse_x - 150) / 100;
                    if(player->cardamount > game.page * 8 + sel)  //确定对应位置有手牌
                    {
                        int found = 0;
                        for(int i = 0; i <= amount - 1; i++)
                        {
                            //若已选中则取消
                            if(tothrow[i] == game.page * 8 + sel)
                            {
                                tothrow[i] = -1;
                                found++;
                                break;
                            }
                        }

                        //若未选中则选定,将tothrow中目前下标最小的-1改为该牌id
                        if(!found && ArrayOccupied(tothrow, amount) <= amount)
                        {
                            for(int i = 0; i <= amount - 1; i++)
                            {
                                if(tothrow[i] == -1)
                                {
                                    tothrow[i] = game.page * 8 + sel;
                                    break;
                                }
                            }
                        }
                    }
                }

                //装备区
                if( (area & 2) && msg.is_down() && mouse_x >= 0 && mouse_x <= 150 && mouse_y >= 450 && mouse_y <= 600)
                {
                    int sel = (int)( (mouse_y - 450) / 37.5);
                    if(player->equips[sel] != -1)  //确定对应位置有装备
                    {
                        int found = 0;
                        for(int i = 0; i <= amount - 1; i++)
                        {
                            //若已选中则取消
                            if(tothrow[i] == (0x10 | sel))
                            {
                                tothrow[i] = -1;
                                found++;
                                break;
                            }
                        }

                        //若未选中则选定,将tothrow中目前下标最小的-1改为该牌id
                        if(ArrayOccupied(tothrow, amount) <= amount)
                        {
                            for(int i = 0; i <= amount - 1; i++)
                            {
                                if(tothrow[i] == -1)
                                {
                                    tothrow[i] = 0x10 | sel;
                                    break;
                                }
                            }
                        }
                    }
                }

                //翻页
                if(msg.is_down() && mouse_x >= 960 && mouse_x <= 970 && mouse_y >= 575 && mouse_y <= 595)
                {
                    if(game.page > 0) game.page--;
                }
                if(msg.is_down() && mouse_x >= 985 && mouse_x <= 1000 && mouse_y >= 575 && mouse_y <= 595)
                {
                    if(executor->cardamount > (game.page + 1) * 8) game.page++;
                }

                //确定键
                if(ArrayOccupied(tothrow, amount) == amount && msg.is_down() && mouse_x >= 960 && mouse_x <= 1050 && mouse_y >= 510 && mouse_y <= 535)
                {
                    for(int i = 0; i <= amount - 1; i++)
                    {
                        switch(tothrow[i] >> 8)
                        {
                        case 0:
                        {
                            card_inf[player->card[tothrow[i]]].owner = -1;
                            Putcard(player->card[tothrow[i]]);
                            player->card[tothrow[i]] = -1;
                            player->cardamount--;
                            break;
                        }
                        case 1:
                        {
                            card_inf[player->equips[tothrow[i] & 0xF]].owner = -1;
                            Putcard(player->equips[tothrow[i] & 0xF]);
                            player->equips[tothrow[i] & 0xF] = -1;
                            break;
                        }
                        }
                    }

                    //修改手牌下标
                    IndexAlign(player);
                    DrawGui();
                    return;
                }
                putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
            }
        }

        //对其他角色弃牌
        else
        {
            //绘制区域
            setfillcolor(EGERGB(83, 30, 0), gui.selector);
            bar(415, 165, 785, 435, gui.selector);

            setfont(12, 0, "仿宋", gui.selector);

            setcolor( area & 1 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
            Rect(425, 175, 505, 295, area & 1 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
            outtextxy(453, 229, (char*)"手牌", gui.selector);
            if( (area & 1) && player->cardamount) PasteCard(425, 175, -2 ,gui.selector);

            char str[] = "武器\0防具\0-1马\0+1马\0";
            setcolor( area & 2 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
            for(int i = 0; i <= 3; i++)
            {
                Rect(425 + 90 * i, 305, 505 + 90 * i, 425, area & 2 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                outtextxy(453 + 90 * i, 359, str + 5 * i, gui.selector);
                if( (area & 2) && player->equips[i] != -1)
                    PasteImage( Link( Link( (char*)".\\Textures\\Cards\\", Myitoa(card_inf[player->equips[i]].type) ), (char*)".png"), 425, 175, gui.selector, TRANSPARENT, BLACK);
            }

            setcolor( area & 4 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
            for(int i = 0; i <= 2; i++)
            {
                Rect(515 + 90 * i, 175, 595 + 90 * i, 295, area & 4 ? EGERGB(190, 183, 68) : LIGHTGRAY, gui.selector);
                outtextxy(543 + 90 * i, 229, (char*)"判定", gui.selector);
            }

            putimage_transparent(NULL, gui.selector, 0, 0, BLACK);
            getch();
        }
    }
}

//展示手牌
///amount=-1表示全部展示
void Showcard(player_t *executor, player_t *player, int amount)
{
    if(amount != -1)
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /* select cards */
        }
        for(int j = 0; j <= amount - 1; j++)
        {
            /* output thr information of player.card[selected[j]] */
        }
    }
}

//获得其他角色牌
///默认type=0表示executor获得player的牌,type=1表示executor交给player牌
//TODO: amount=-1 stands for getting or giving all cards
void Getcard(player_t *executor, player_t *player, int amount, int type = 0)
{
    if(type)
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /*
            card_inf[executor.card[selected[j]]] = player.id;
            player.card[player.cardamount] = executor.card[selected[j]];
            executor.card[selected[j]] = -1;
            */
            executor->cardamount--;
            player->cardamount++;
        }
        /* skills here */
    }
    else
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /*
            card_inf[player.card[selected[j]]] = executor.id;
            executor.card[executor.cardamount] = player.card[selected[j]];
            player.card[selected[j]] = -1;
            */
            player->cardamount--;
            executor->cardamount++;
        }
        /* skills here */
    }
}

//判定
///返回值为卡牌id
int Judging(player_t *player)
{
    if(!game.nowpile) Shuffle();

    int res = game.card[0];
    card_inf[game.card[0]].owner = -1;
    game.nowpile--;

    for(int j = 0; j <= game.nowpile - 1; j++)
    {
        game.card[j] = game.card[j + 1];
    }
    game.card[game.nowpile] = -1;

    return res;
}
//造成伤害
//TODO: Add the card that causes the damage for the skill [奸雄]
///executor=NULL表示伤害无来源(如闪电),type=LOSS表示失去体力
void Damage(player_t *executor, player_t *player, int amount, damage_e type)
{
    /* skills here */
    player->health -= amount;
    if(player->health <= 0)
    {
        Neardeath(player);
    }
    /* skills here */
    /* chain here */
}

//恢复
void Recover(player_t *player, int amount)
{
    if(player->maxhealth - player->health < amount) amount = player->maxhealth - player->health;
    player->health += amount;
    /* skills here */
}

//濒死结算
void Neardeath(player_t *player)
{
    /* skills here */
    for(int i = 0; i <= PLAYERS - 1; i++)
    {
        do
        {
            /*  ask player that id = (player.id + i) % PLAYERS for a peach */
            if(player->health > 0) return;
        }
        while(0/* used a peach */);
    }

    if(player->health < 0) Death(player);
}

//死亡结算
void Death(player_t *player)
{
    player->controller = DEAD;
    /* VictoryJudge(); */

    for(int i = 0; i <= player->cardamount - 1; i++)
    {
        card_inf[player->card[i]].owner = -1;
    }

    Throwcard(player, player, player->cardamount);
}

//胜利条件判定
void VictoryJudge(void)
{
    if(player[0].controller == DEAD && player[3].controller == DEAD)  //1号与4号均死亡
    {
        if(game.humanid != 0 && game.humanid != 3)  /* victory */;  //1号与4号均为电脑
        else /* failed */;
    }
    else if( player[1].controller == DEAD && player[2].controller == DEAD)
    {
        if(game.humanid != 1 && game.humanid != 2)  /* victory */;
        else /* failed */;
    }
}

//无懈响应
///start为第一次询问无懈时开始的角色id,card为锦囊id,用于提示信息
///返回值0=此牌仍有效,1=被无懈抵消
int AskWuxie(int start, int card)
{
    int res = 0;
    int ans = 0;

    do
    {
        ans = 0;

        for(int i = 0; i <= 3; i++)
        {
            if(player[start + i].controller == HUMAN)
            {
                /* use here */
            }
            else if(player[start + i].controller == AI)
            {
                /* use here */
            }

            if(ans)
            {
                (start += i) %= PLAYERS;  //下一次询问从此次使用者开始
                res++;
                break;
            }
        }
    }
    while(ans);

    return res % 2;
}
