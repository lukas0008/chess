/* This file is AI generated. */
#define _POSIX_C_SOURCE 200809L

#include "chess.h"
#include "engine.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_MOVEGEN_ITERATIONS 10000ULL
#define DEFAULT_SEARCH_ITERATIONS 1ULL

static double seconds_between(struct timespec start, struct timespec end) {
  return (double)(end.tv_sec - start.tv_sec) +
         (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

static uint64_t parse_iterations(const char *text, const char *name) {
  char *end = NULL;
  errno = 0;
  unsigned long long value = strtoull(text, &end, 10);
  if (errno != 0 || end == text || *end != '\0' || value == 0) {
    fprintf(stderr, "invalid %s iteration count: %s\n", name, text);
    exit(EXIT_FAILURE);
  }
  return (uint64_t)value;
}

static void print_move(ChessMove move) {
  printf("%c%c%c%c", 'a' + move.start % 8, '1' + move.start / 8,
         'a' + move.end % 8, '1' + move.end / 8);
}

static void benchmark_move_generation(uint64_t iterations) {
  ChessState board = initboard();
  ChessMove moves[VALID_MOVES];
  uint64_t total_moves = 0;
  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (uint64_t i = 0; i < iterations; ++i) {
    total_moves += (uint64_t)list_valid_moves(&board, 0, moves);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed = seconds_between(start, end);
  printf("move generation\n");
  printf("  iterations:     %" PRIu64 "\n", iterations);
  printf("  generated:      %" PRIu64 " moves\n", total_moves);
  printf("  elapsed:        %.6f s\n", elapsed);
  printf("  iterations/sec: %.2f\n", (double)iterations / elapsed);
  printf("  ns/iteration:   %.2f\n\n",
         elapsed * 1000000000.0 / (double)iterations);
}

static void benchmark_search(uint64_t iterations) {
  ChessState position = initboard();
  ChessMove e2e4 = {.start = 12, .end = 28};
  makemove(&position, e2e4);

  ChessMove best = {0};
  uint64_t checksum = 0;
  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (uint64_t i = 0; i < iterations; ++i) {
    struct Engine engine = {
        .current = position,
        .engine_color = 1,
        .current_color = 1,
    };
    engine_init(&engine);
    best = engine_guess(&engine);
    checksum += (uint64_t)(unsigned char)best.start * 64u +
                (uint64_t)(unsigned char)best.end;
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed = seconds_between(start, end);
  printf("engine search after e2e4\n");
  printf("  iterations:     %" PRIu64 "\n", iterations);
  printf("  elapsed:        %.6f s\n", elapsed);
  printf("  searches/sec:   %.4f\n", (double)iterations / elapsed);
  printf("  ms/search:      %.3f\n", elapsed * 1000.0 / (double)iterations);
  printf("  best move:      ");
  print_move(best);
  printf("\n  checksum:       %" PRIu64 "\n", checksum);
}

int main(int argc, char **argv) {
  if (argc > 3) {
    fprintf(stderr, "usage: %s [movegen-iterations] [search-iterations]\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  uint64_t movegen_iterations = DEFAULT_MOVEGEN_ITERATIONS;
  uint64_t search_iterations = DEFAULT_SEARCH_ITERATIONS;
  if (argc >= 2) {
    movegen_iterations = parse_iterations(argv[1], "move generation");
  }
  if (argc == 3) {
    search_iterations = parse_iterations(argv[2], "search");
  }

  printf("chess benchmark\n===============\n\n");
  benchmark_move_generation(movegen_iterations);
  benchmark_search(search_iterations);
  return EXIT_SUCCESS;
}
