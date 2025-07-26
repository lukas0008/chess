#ifndef CHESS_H
#define CHESS_H

#define PIECE_BISHOP_WHITE 'b'
#define PIECE_BISHOP_BLACK 'B'
#define PIECE_KNIGHT_WHITE 'n'
#define PIECE_KNIGHT_BLACK 'N'
#define PIECE_ROOK_WHITE 'r'
#define PIECE_ROOK_BLACK 'R'
#define PIECE_QUEEN_WHITE 'q'
#define PIECE_QUEEN_BLACK 'Q'
#define PIECE_KING_WHITE 'k'
#define PIECE_KING_BLACK 'K'
#define PIECE_PAWN_WHITE 'p'
#define PIECE_PAWN_BLACK 'P'

typedef struct ChessState {
  char pieces[64];
} ChessState;

typedef struct ChessMove {
  char start;
  char end;
} ChessMove;

ChessState initboard();
void printboard(ChessState *state, char show_moves_from);
char parsemove(ChessMove *move, char *str_move);
void makemove(ChessState *state, ChessMove move);
char validatemove(ChessState *state, ChessMove move);

#endif
