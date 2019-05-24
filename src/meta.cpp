#include "main.h"
#include "meta.h"

Meta::Meta()
{
    this->lives = 5;
    this->coins = 0;
}

void Meta::coin_collect(int val)
{
    this->coins+=val;
}

void Meta::powerup_collect(int type)
{
    if(type == 1 && this->lives<9)
        this->lives++;
    else
        this->coins+=10;
}

void Meta::hit(GLFWwindow *window)
{
    this->lives--;
    if(this->lives==0)
    {
        printf("\nCoins collected: %d\n======================Game Over!!==========================\n", this->coins);
        quit(window);
    }
}

int Meta::get_coins()
{
    return this->coins;
}

int Meta::get_lives()
{
    return this->lives;
}