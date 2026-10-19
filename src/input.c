#include "input.h"
#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_input(char *buf, size_t buf_size) {
  buf[buf_size - 1] = '\0';
  fgets(buf, buf_size - 1, stdin);
  for (size_t i = 0; i < buf_size; i++) {
    if (buf[i] == '\0') {
      break;
    }
    if (buf[i] == 10) {
      buf[i] = '\0';
    }
  }
}

char is_abc_move_worthy(char c) { return c >= 'a' && c <= 'h'; }
char is_num_move_worthy(char c) { return c >= '1' && c <= '8'; }

char is_move_worthy(char c[2]) {
  return is_abc_move_worthy(c[0]) && is_num_move_worthy(c[1]);
}

char parsemove(ChessMove *move, char *str_move) {
  size_t str_len = strlen(str_move);
  if (str_len < 2 || str_len > 5) {
    return -1;
  }

  if (!is_move_worthy(str_move)) {
    return -2;
  }

  move->start = str_move[0] - 'a' + (str_move[1] - '1') * 8;
  if (str_len < 4) {
    return 1;
  }

  char offset = str_len == 5;
  if (!is_move_worthy(&str_move[2 + offset])) {
    return -3;
  }

  move->end = str_move[2 + offset] - 'a' + (str_move[3 + offset] - '1') * 8;
  return 0;
}

static char get_single_move(ChessMove *move) {
  char input[20];
  get_input(input, 20);
  if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0 ||
      strcmp(input, "exit") == 0) {
    printf("exiting\n");
    exit(0);
  }
  return parsemove(move, input);
}

char getmove(ChessMove *move, ChessState *board, char color) {
  char move_result = get_single_move(move);
  if (move_result <= 0) {
    return move_result;
  }

  char start = move->start;
  printboard(board, (color * COLOR_FLAG) | start);

  move_result = get_single_move(move);
  if (move_result <= 0) {
    return move_result;
  }

  move->end = move->start;
  move->start = start;
  return 0;
}

