#ifndef INPUT_H
#define INPUT_H

#include "chess.h"
#include <stddef.h>

void get_input(char *buf, size_t buf_size);
char getmove(ChessMove *move, ChessState *board, char color);
char parsemove(ChessMove *move, char *str_move);

#endif
