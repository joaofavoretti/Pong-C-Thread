#include <ncurses.h>
#include <window.h>
#include <game_config.h>
#include <player.h>
#include <ball.h>

/* Atribuir configuracoes iniciais dos jogadores*/
int init_players_config(void)
{
    /* Atribuindo parametros defaults dos players */
    player_default_height = 10;
    player_default_width = 2;
    player_left_start_y = (LINES - player_default_height) / 2;
    player_right_start_y = (LINES - player_default_height) / 2;
    player_left_start_x = 2;
    player_right_start_x = COLS - player_default_width - 2;

    return 0;
}

// Makes the opponent follow the Y-coord of the ball, with a delay
int auto_move_opponent()
{
    opponent.window.posY += (ball.window.posY - opponent.window.posY) / 10;
    update_pong_opponent();

    return 0;
}

/* Seta o opponent com todos os atributos default */
int init_pong_opponent()
{
    opponent.window.win = create_newwin(
        player_default_height,
        player_default_width,
        player_right_start_y,
        player_right_start_x);

    opponent.window.height = player_default_height;
    opponent.window.width = player_default_width;
    opponent.window.posY = player_right_start_y;
    opponent.window.posX = player_right_start_x;

    opponent.score = 0;

    return 0;
}

/* Remove o opponent do cenário */
int destroy_pong_opponent()
{
    destroy_win(opponent.window.win);
    return 0;
}

/* Atualiza posicao do opponent no cenário baseado nos seus atributos */
int update_pong_opponent()
{
    destroy_pong_opponent();

    opponent.window.win = create_newwin(
        opponent.window.height,
        opponent.window.width,
        opponent.window.posY,
        opponent.window.posX);

    return 0;
}

/* Seta o player com todos os atributos default*/
int init_pong_player()
{
    player.window.win = create_newwin(
        player_default_height,
        player_default_width,
        player_left_start_y,
        player_left_start_x);
    player.window.height = player_default_height;
    player.window.width = player_default_width;
    player.window.posY = player_left_start_y;
    player.window.posX = player_left_start_x;
    player.score = 0;

    return 0;
}

/* Remove o player do cenário */
int destroy_pong_player()
{
    destroy_win(player.window.win);
    return 0;
}

/* Atualiza posicao do player no cenário baseado nos seus atributos */
int update_pong_player()
{
    destroy_pong_player();

    player.window.win = create_newwin(
        player.window.height,
        player.window.width,
        player.window.posY,
        player.window.posX);

    return 0;
}
