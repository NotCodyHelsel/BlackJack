#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
using namespace std;

double win = 0;
int bet = 0;
double balance = 0;
int lose = 0;
bool won = true;
int ddc = 0;
bool dd = false;
int split = 0;
bool push = false;
int loseStreak = 0;
int ball = 0;
int lag = 0;
vector<int> decks;
vector<int> cards;
vector<vector<int>> splits;
char strategy[16][10] = {
    // 2   3    4    5    6    7    8    9    10   A
    {'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'}, // 3 to 8 + 4-4
    {'H', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'}, // 9
    {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H'}, // 10
    {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D'}, // 11
    {'H', 'H', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'}, // 12
    {'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'}, // 13 to 16
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'}, // 17 to 21
    {'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'}, // A-2 - A-6
    {'S', 'D', 'D', 'D', 'D', 'S', 'S', 'H', 'H', 'H'}, // A-7
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'}, // A-8 + A-9 + 10-10
    {'H', 'H', 'X', 'X', 'X', 'X', 'H', 'H', 'H', 'H'}, // 2-2 + 3-3
    {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H'}, // 5-5
    {'X', 'X', 'X', 'X', 'X', 'H', 'H', 'H', 'H', 'H'}, // 6-6
    {'X', 'X', 'X', 'X', 'X', 'X', 'H', 'H', 'H', 'H'}, // 7-7
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'}, // 8-8 + A-A
    {'X', 'X', 'X', 'X', 'X', 'S', 'X', 'X', 'S', 'S'}, // 9-9
};

class BlackJack
{
public:
    vector<int> playerCards;
    int playerTotal = 0;
    int dealerTotal = 0;
    vector<int> dealerCards;

    BlackJack()
    {
        fillCards();
        for (int p = 0; p < 1000; p++)
        {
            if (!won && !push)
            {
                lag++;
            }
            if ((!won) && (loseStreak < lag) && !push)
            {
                loseStreak = lag;
            }
            else if (won && !push)
            {
                lag = 0;
            }
            if (decks.size() <= 52)
            {
                shuffle();
            }
            if (!won && !push)
            {
                cout << "Streak: " << lag << " ";
            }
            dealerTotal = 0;
            playerTotal = 0;
            if (won && !push)
            {
                balance += bet;
                bet = 10;
            }
            /*else if (!won && lag == 1)
            {
                balance -= 15;
                bet = 15;
            }*/
            else if (lag > 5 && !push)
            {
                balance -= bet;
                ball++;
                if (ball == 4)
                {
                    bet = 10;
                }
            }
            else if (!won && !push)
            {
                balance -= bet;
                bet *= 2;
            }
            push = false;
            cout << "BAL: " << balance << " ";
            cout << "BET: " << bet << " ";
            deal();
            cout << endl;
        }
    }

    void fillCards()
    {
        for (int i = 2; i < 15; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (i > 11 && i <= 14)
                {
                    cards.push_back(10);
                }
                else
                {
                    cards.push_back(i);
                }
            }
        }
    }

    void playerHit(vector<int> &hand, int &handTotal)
    {
        if (playerCards.size() >= 2)
        {
            cout << "h>" << decks.back() << " ";
        }
        hand.push_back(decks.back());
        handTotal += decks.back();
        decks.pop_back();
        int cnt = count(hand.begin(), hand.end(), 11);
        if (cnt >= 1 && (handTotal > 21))
        {
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i] == 11)
                {
                    hand[i] = 1;
                    handTotal -= 10;
                    return;
                }
            }
        }
        if (handTotal > 21)
        {
            lose++;
            won = false;
            return;
        }
    }
    void dealerHit()
    {
        if (dealerCards.size() >= 2)
        {
            cout << "dh>" << decks.back() << " ";
        }
        dealerCards.push_back(decks.back());
        dealerTotal += decks.back();
        decks.pop_back();
        int cnt = count(dealerCards.begin(), dealerCards.end(), 11);
        if (cnt >= 1 && (dealerTotal > 21))
        {
            for (int i = 0; i < dealerCards.size(); i++)
            {
                if (dealerCards[i] == 11)
                {
                    dealerCards[i] = 1;
                    dealerTotal -= 10;
                    return;
                }
            }
        }
    }

    void deal()
    {
        playerCards.clear();
        dealerCards.clear();
        playerHit(playerCards, playerTotal);
        dealerHit();
        playerHit(playerCards, playerTotal);
        dealerHit();

        cout << " H(" << playerCards[0] << "," << playerCards[1] << ") ";
        cout << " D(" << dealerCards[0] << "," << dealerCards[1] << ") ";

        int cnt = count(dealerCards.begin(), dealerCards.end(), 11);
        if ((dealerCards[0] == 11) && (dealerTotal == 21) && (playerTotal == 21)) // even money
        {
            lose++;
            won = false;
        }
        else if ((dealerCards[0] == 10) && (dealerTotal == 21) && (playerTotal == 21)) // anxious bj
        {
            win += 1.5;
            won = true;
        }
        else if (playerTotal == 21) // player bj
        {
            win += 1.5;
            won = true;
        }
        else if (dealerTotal == 21) // dealer bj
        {
            lose += 1;
            won = false;
        }
        else
        {
            playerThink(playerCards, playerTotal);
        }
    }

    void playerThink(vector<int> &hand, int &handTotal)
    {
        bool pair;
        char decision;
        int cnt = count(hand.begin(), hand.end(), 11);
        if (dd == true)
        {
            decision = 'S';
            dd = false;
        }
        else if (handTotal == 21)
        {
            decision = 'S';
        }
        else if (hand[0] == hand[1] || (cnt >= 1 && handTotal == 12)) // pairs
        {
            pair = true;
            if (handTotal == 8)
            {
                decision = strategy[0][dealerCards[0] - 2];
            }
            else if (handTotal == 20)
            {
                decision = strategy[9][dealerCards[0] - 2];
            }
            else if (handTotal == 4 || handTotal == 6)
            {
                decision = strategy[10][dealerCards[0] - 2];
            }
            else if (handTotal == 10)
            {
                decision = strategy[11][dealerCards[0] - 2];
            }
            else if (handTotal == 12 && (cnt >= 1 && handTotal == 12))
            {
                decision = strategy[14][dealerCards[0] - 2];
            }
            else if (handTotal == 12)
            {
                decision = strategy[12][dealerCards[0] - 2];
            }
            else if (handTotal == 14)
            {
                decision = strategy[13][dealerCards[0] - 2];
            }
            else if (handTotal == 16)
            {
                decision = strategy[14][dealerCards[0] - 2];
            }
            else if (handTotal == 18)
            {
                decision = strategy[15][dealerCards[0] - 2];
            }
        }
        else if (cnt >= 1) // ace
        {
            if (handTotal >= 13 && handTotal <= 17)
            {
                decision = strategy[7][dealerCards[0] - 2];
            }
            else if (handTotal == 18)
            {
                decision = strategy[8][dealerCards[0] - 2];
            }
            else if (handTotal == 19 || handTotal == 20)
            {
                decision = strategy[9][dealerCards[0] - 2];
            }
        }
        else // normal
        {
            if (handTotal >= 3 && handTotal <= 8)
            {
                decision = strategy[0][dealerCards[0] - 2];
            }
            else if (handTotal == 9)
            {
                decision = strategy[1][dealerCards[0] - 2];
            }
            else if (handTotal == 10)
            {
                decision = strategy[2][dealerCards[0] - 2];
            }
            else if (handTotal == 11)
            {
                decision = strategy[3][dealerCards[0] - 2];
            }
            else if (handTotal == 12)
            {
                decision = strategy[4][dealerCards[0] - 2];
            }
            else if (handTotal >= 13 && handTotal <= 16)
            {
                decision = strategy[5][dealerCards[0] - 2];
            }
            else if (handTotal >= 17 && handTotal <= 20)
            {
                decision = strategy[6][dealerCards[0] - 2];
            }
        }
        switch (decision)
        {
        case 'H':
            playerHit(hand, handTotal);
            playerThink(hand, handTotal);
            break;
        case 'S':
            dealerThink();
            break;
        case 'X':
            split++;
            push = true;
            break;
        case 'D':
            ddc++;
            dd = true;
            playerHit(hand, handTotal);
            playerThink(hand, handTotal);
            break;
        }
    }
    void dealerThink()
    {
        while (dealerTotal < 17)
        {
            dealerHit();
            if (dealerTotal > 21)
            {
                win++;
                won = true;
                return;
            }
        }
        if (dealerTotal > playerTotal)
        {
            lose++;
            won = false;
        }
        else if (dealerTotal < playerTotal)
        {
            win++;
            won = true;
        }
        else if (dealerTotal == playerTotal)
        {
            push = true;
        }
    }
    void shuffle()
    {
        decks = cards;
        for (int k = 0; k < 104; k++)
        {
            int r = k + rand() % (104 - k);
            swap(decks[k], decks[r]);
        }
    }
};

int main()
{
    srand(time(NULL));
    BlackJack game;
    cout << endl;
    cout << "w:" << win << " l:" << lose << " s: " << loseStreak << " d: " << ddc << " sp: " << split << endl;
}