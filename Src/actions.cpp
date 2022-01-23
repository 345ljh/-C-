#include "actions.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>

///���ļ���game����Ϊȫ�ֱ�������

//�ƶѳ�ʼ��
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

//ϴ��
void Shuffle(void)
{
    int pilecards = 0;
    int pointer;
    int* temp = NULL;
    temp = (int*)calloc(pilecards, sizeof(int));
    memset(temp, 0, sizeof(int) * pilecards);

    //ͳ�����ƶ����Ƶ�����
    for(int i = 0; i <= 150; i++)
    {
        if(card_inf[i].owner == -1) pilecards++;
    }

    //�˲�����Pileinit����
    for(int i = 0; i <= pilecards - 1; i++)
    {
        pointer = rand() % 160;
        while(temp[pointer]) ++pointer %= pilecards;
        game.card[i] = pointer;
        temp[pointer] = 1;

    }

    game.nowpile = pilecards;
    free(temp);
}

//����
void Takecard(player_t player, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        if(card_inf[0].owner == -1) Shuffle();
        card_inf[game.card[0]].owner = player.id;
        player.card[player.cardamount] = game.card[0];
        player.cardamount++;
        game.nowpile--;

        for(int j = 0; j <= game.nowpile - 1; j++)
        {
            game.card[j] = game.card[j + 1];
        }
        game.card[game.nowpile] = -1;

        /* skills here */
    }
}

//����,����executorΪ�����ߵ�id,playerΪ��������
void Throwcard(int executor, player_t player, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        int select = 0;
        //TODO: a function that the executor choose a card's id of player to throw
        card_inf[player.card[select]].owner = -1;
        player.cardamount--;

        for(int j = select; j <= player.cardamount - 1; j++)
        {
            player.card[j] = player.card[j + 1];
        }
        player.card[player.cardamount] = -1;

        /* skills here */
    }
}

//�˺�
///executor=0��ʾ�˺�����Դ(������),=-1��ʾʧȥ����
void Damage(int executor, player_t player, int amount)
{
    player.health -= amount;
    if(player.health <= 0)
    {
        Neardeath(player)
    }
    /* skills here */
}

//��������
void Neardeath(player_t player)
{
    /* skills here */
    for(int i = 0; i <= PLAYERS; i++)
    {
        do
        {
            /*  ask player that id = (player.id + i) % PLAYERS for a peach */
            if(player.health > 0) return;
        }while(/* used a peach */)
    }

    if(player.health < 0) /* death */
}
