#ifndef CHESS_H
#define CHESS_H

#define PIECE_EMPTY 0
#define PIECE_BISHOP_WHITE 2
#define PIECE_BISHOP_BLACK 3
#define PIECE_KNIGHT_WHITE 4
#define PIECE_KNIGHT_BLACK 5
#define PIECE_ROOK_WHITE 6
#define PIECE_ROOK_BLACK 7
#define PIECE_QUEEN_WHITE 8
#define PIECE_QUEEN_BLACK 9
#define PIECE_KING_WHITE 10
#define PIECE_KING_BLACK 11
#define PIECE_PAWN_WHITE 12
#define PIECE_PAWN_BLACK 13

#define COLOR_FLAG (1 << 6)

#define VALID_MOVES 288

#ifndef CHESS_FLIP_DISPLAY_COLORS
#define CHESS_FLIP_DISPLAY_COLORS 1
#endif

typedef struct ChessState {
  char pieces[64];
} ChessState;

typedef struct ChessMove {
  char start;
  char end;
} ChessMove;

char piece_from_alphabetic(char piece);
ChessState initboard();
void printboard(ChessState *state, int show_moves_from);
void makemove(ChessState *state, ChessMove move);
char validatemove(ChessState *state, ChessMove move);
int list_valid_moves(ChessState *state, char color, ChessMove *moves);
char check2mate(ChessState *state, char color);
char checkking(ChessState *state, char color);

#endif
