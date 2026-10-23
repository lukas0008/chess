# Chess

A terminal chess program and depth-limited chess engine written in C. The human
plays White and the engine plays Black.

## Features

- Terminal board with Unicode chess pieces
- Coordinate-based move input
- Piece-specific pseudo-legal move generation
- King-safety filtering for engine move generation
- Depth-4 minimax search with alpha-beta pruning
- Material and piece-square evaluation
- Built-in move-generation and search benchmark
- `perf`-based per-function CPU profiling

## Requirements

- C compiler
- CMake 3.10 or newer
- GNU-compatible assembler on x86-64
- Bash, `awk`, and Linux `perf` for `bench.sh`

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

## Benchmark

Run the benchmark directly:

```bash
./build/bench
```

Optional arguments control the move-generation and engine-search iteration
counts:

```bash
./build/bench [movegen-iterations] [search-iterations]
```

The defaults are 10,000 move-generation iterations and one engine search after
`e2e4`.

## Profile Methods

`bench.sh` builds an optimized binary with debug symbols and frame pointers,
runs it under `perf`, and prints the hottest methods with self-CPU percentages
and approximate self time:

```bash
./bench.sh
```

The script accepts the same optional iteration arguments as `bench`:

```bash
./bench.sh 20000 2
```

Profiling can be configured through environment variables:

| Variable | Default | Purpose |
|---|---:|---|
| `BUILD_DIR` | `build-bench` | Profiling build directory |
| `PERF_FREQUENCY` | `999` | Samples per second |
| `MIN_PERCENT` | `0.10` | Minimum self-CPU percentage shown |
| `MAX_METHODS` | `25` | Maximum methods shown |

## Project Layout

```text
include/          Public headers
src/chess.c       Board representation and move generation
src/engine.c      Evaluation and alpha-beta search
src/input.c       Terminal move parsing
src/main.c        Interactive game loop
src/benchmark.c   Benchmark executable
bench.sh          Per-method profiler
```

## Current Limitations

The rules implementation does not yet support castling, en passant, or pawn
promotion. The interactive loop also does not automatically terminate on
checkmate or stalemate.
