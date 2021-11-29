#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <game_config.h>
#include <window.h>
#include <player.h>

/* Configura alguns dos prametros do ncurses  */
int init_ncurses_config(WINDOW *root, WINDOW *stdscr)
{
    /* Habilitar possibilidade de parar o programar com CTRL + c */
    if (cbreak() == ERR)
    {
        fprintf(stderr, "ERROR: cbreak()");
        return -1;
    }

    /* Habilitar F[1-12] e keypad */
    if (keypad(stdscr, TRUE) == ERR)
    {
        fprintf(stderr, "ERROR: keypad()");
        return -1;
    }

    /* Não mostrar o que foi digitado diretamente na tela */
    if (noecho() == ERR)
    {
        fprintf(stderr, "ERROR: noecho()");
        return -1;
    }

    /* getch() não bloqueia */
    if (nodelay(root, true) == ERR)
    {
        fprintf(stderr, "ERROR: nodelay()");
        return -1;
    }

    return 0;
}

/* Mostra o menu de inicio e espera por entrada do usuario */
int display_start_menu()
{
    bool done = false;

    int start_menu_y = LINES / 2 - 5;
    int start_menu_x = COLS / 2 - 3;

    int controls_menu_y = LINES / 2;
    int controls_menu_x = COLS / 2 - 3;

    /* Desenhar start menu */
    mvprintw(start_menu_y + 0, start_menu_x, ":: START MENU ::");
    mvprintw(start_menu_y + 1, start_menu_x, "ENTER: Start Game");
    mvprintw(start_menu_y + 2, start_menu_x, "q: Exit Game");

    /* Desenhar menu de controles */
    mvprintw(controls_menu_y + 0, controls_menu_x, "::  CONTROLS  ::");
    mvprintw(controls_menu_y + 1, controls_menu_x, "r: Reset game");
    mvprintw(controls_menu_y + 2, controls_menu_x, "w: Player up");
    mvprintw(controls_menu_y + 3, controls_menu_x, "s: Player down");


    while (!done)
    {
        int ch = getch();

        switch (ch)
        {
        case KEY_ENTER:
            done = true;
            break;
        case KEYBOARD_ENTER:
            done = true;
            break;
        case QUIT_GAME:
            done = true;
            endwin();
            exit(0);
            break;
        }
    }

    return 0;
}

/* Atualiza a struct timeval time para o tempo de agora */
int update_time(struct timeval *time)
{
    gettimeofday(time, NULL);

    return 0;
}

/* Retorna diferenca de tempo em milisegundos */
unsigned int time_difference(struct timeval *start, struct timeval *now)
{
    unsigned int difference;

    difference = ((now->tv_sec * 1000) + (now->tv_usec / 1000)) -
                 ((start->tv_sec * 1000) + (start->tv_usec / 1000));

    return difference;
}

int show_ingame_menu()
{
    mvprintw(0, 0, ":: GAME MENU ::");
    mvprintw(1, 0, "EXIT: q");
    mvprintw(2, 0, "RESET: r");
    mvprintw(3, 0, "UP: k");
    mvprintw(4, 0, "DOWN: j");

    return 0;
}

int show_ingame_scores()
{
    mvprintw(5, COLS / 3, "%i", player.score);
    mvprintw(5, COLS - COLS / 3, "%i", opponent.score);

    return 0;
}

int show_ingame_line()
{
    mvvline(0, COLS / 2, '|', LINES);

    return 0;
}
