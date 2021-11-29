#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include <window.h>
#include <ball.h>
#include <player.h>

/* Rand para retornar -1 / 1  */
int rand_sign()
{
    return (float)rand() / ((float)INT32_MAX / 2.0f) <= 0.5 ? -1 : 1;
}

/* Retorna -1(x < 0) / 0(x == 0) / 1(x > 0)  */
int sign(const int x)
{

    if (x == 0)
    {
        return 0;
    }

    return x > 0 ? 1 : -1;
}

/* Atualiza posicao da bola baseado na sua posicao atual */
int auto_move_ball()
{
    /* Checa se a bola colidiu com a parte de baixo ou a parte de cima da tela */
    if (ball.window.posY + ball.window.height >= LINES || ball.window.posY <= 0)
    {
        /* Atualiza a velocidade y da bola (Faz ela quicar) */
        ball.vy = ball.vy * -1;
    }

    /* Checa se a bola colidiu com a parte da direita do "campo" */
    if (ball.window.posX + ball.window.width >= COLS)
    {
        /* Ponto do jogador da esquerda (player) */
        player.score++;
        reset_ball();
    }
    /* Checa se a bola colidiu com a parte da esquerda do "campo" */
    else if (ball.window.posX <= 0)
    {
        /* Pongo do jogador da direita (opponent) */
        opponent.score++;
        reset_ball();
    }

    /* Checa colisão com Player */
    if (ball.window.posY >= player.window.posY &&
        ball.window.posY <= player.window.posY + player.window.height &&
        ball.window.posX <= player.window.posX + player.window.width)
    {
        /* Bounce back */
        ball.vx = ball.vx * -1;
        ball.collided = true;
    }

    /* Checa colisao com o opponent */
    if (ball.window.posY >= opponent.window.posY &&
        ball.window.posY <= opponent.window.posY + opponent.window.height &&
        ball.window.posX >= opponent.window.posX - opponent.window.width)
    {
        /* Bounce back */
        ball.vx = ball.vx * -1;
        ball.collided = true;
    }

    /* Adiciona velocidade aleatória para a bola quando há colisao */
    if (ball.collided)
    {
        ball.vx += sign(ball.vx) * rand() % 3;
        ball.vy += sign(ball.vy) * rand() % 2;
        ball.collided = false;
        
        /* Atualizar players */
        update_pong_player();
        update_pong_opponent();
    }

    /* Atualiza posicao da bola */
    ball.window.posX += ball.vx;
    ball.window.posY += ball.vy;
    update_pong_ball();

    return 0;
}

// Resets the balls position to the middle of the screen
int reset_ball()
{
    ball.window.posX = COLS / 2 - 1;
    ball.window.posY = LINES / 2;
    ball.vx = rand_sign();
    ball.vy = rand_sign();

    return 0;
}

/* Inicia struct pong_ball com valores default */
int init_pong_ball()
{
    ball.vx = rand_sign();
    ball.vy = rand_sign();
    ball.collided = false;

    ball.window.height = 2;
    ball.window.width = 3;
    ball.window.posX = COLS / 2 - ball.window.width / 2;
    ball.window.posY = LINES / 2;
    ball.window.win = create_newwin(2, 3, ball.window.posY, ball.window.posX);
    return 0;
}

int destroy_pong_ball()
{
    destroy_win(ball.window.win);
    return 0;
}

int update_pong_ball()
{
    destroy_pong_ball();
    ball.window.win = create_newwin(
        ball.window.height,
        ball.window.width,
        ball.window.posY,
        ball.window.posX);
    return 0;
}
