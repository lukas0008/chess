# Chess

A terminal chess program and primitive depth-limited chess engine written in C.

## Features

- Terminal board with Unicode chess pieces
- Coordinate-based move input
- Material and piece-square evaluation
- Built-in primitive chess bot

## Requirements

- C compiler
- CMake 3.10 or newer
- GNU-compatible assembler on x86-64
- (optional for `bench.sh`) Bash, `awk`, and Linux `perf`

## Build

Out-of-source builds are required:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This produces:

- `build/chess` — interactive game
- `build/bench` — benchmark executable

## Play

```bash
./build/chess
```

Enter moves in long coordinate notation:

```text
e2e4
g1f3
```

A move can also be entered as two prompts by entering the source square and
then the destination square. Enter `q`, `quit`, or `exit` to stop the game.

## Current Limitations

The rules implementation does not yet support castling, en passant, or pawn
promotion. The interactive loop also does not automatically terminate on
checkmate or stalemate.

## Notes

The writing of this readme was assisted by ai.
