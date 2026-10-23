#!/usr/bin/env bash
# This file is AI generated.

set -euo pipefail

project_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
build_dir=${BUILD_DIR:-"$project_root/build-bench"}
frequency=${PERF_FREQUENCY:-999}
minimum_percent=${MIN_PERCENT:-0.10}
maximum_methods=${MAX_METHODS:-25}

if ! command -v perf >/dev/null 2>&1; then
  printf 'error: perf is required to profile bench\n' >&2
  exit 1
fi

if [[ ! $frequency =~ ^[1-9][0-9]*$ || ! $maximum_methods =~ ^[1-9][0-9]*$ ]]; then
  printf 'error: PERF_FREQUENCY and MAX_METHODS must be positive integers\n' >&2
  exit 1
fi

cmake -S "$project_root" -B "$build_dir" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS_RELEASE='-O3 -g -DNDEBUG -fno-omit-frame-pointer' \
  >/dev/null
cmake --build "$build_dir" --target bench --parallel >/dev/null

temporary_dir=$(mktemp -d "${TMPDIR:-/tmp}/chess-bench.XXXXXX")
trap 'rm -rf "$temporary_dir"' EXIT
perf_data="$temporary_dir/perf.data"
benchmark_output="$temporary_dir/benchmark.out"
perf_log="$temporary_dir/perf.log"
perf_report="$temporary_dir/perf-report.txt"

if ! perf record -q -F "$frequency" -e cycles:u --call-graph fp \
  -o "$perf_data" -- "$build_dir/bench" "$@" \
  >"$benchmark_output" 2>"$perf_log"; then
  printf 'error: perf could not profile bench\n' >&2
  cat "$perf_log" >&2
  exit 1
fi

perf report -i "$perf_data" --stdio --no-children --call-graph none \
  --percent-limit "$minimum_percent" --sort symbol \
  >"$perf_report" 2>>"$perf_log"

total_seconds=$(awk '
  /^  elapsed:/ { total += $2 }
  END { printf "%.6f", total }
' "$benchmark_output")

mapfile -t methods < <(
  awk -v maximum="$maximum_methods" '
    $1 ~ /^[0-9]+([.][0-9]+)?%$/ && $2 ~ /^\[.\]$/ {
      percent = $1
      sub(/%$/, "", percent)
      printf "%s\t%s\n", $3, percent
      if (++count >= maximum) {
        exit
      }
    }
  ' "$perf_report"
)

if ((${#methods[@]} == 0)); then
  printf 'error: perf produced no recognizable method samples\n' >&2
  cat "$perf_report" >&2
  exit 1
fi

printf 'Profiled benchmark runtime: %s s\n\n' "$total_seconds"
printf '+------------------------------------------+----------+-----------------+\n'
printf '| %-40s | %8s | %15s |\n' 'Method' 'Self CPU' 'Approx. self (s)'
printf '+------------------------------------------+----------+-----------------+\n'

for method in "${methods[@]}"; do
  IFS=$'\t' read -r symbol percent <<<"$method"
  self_seconds=$(awk -v total="$total_seconds" -v percent="$percent" \
    'BEGIN { printf "%.6f", total * percent / 100 }')
  printf '| %-40.40s | %7.2f%% | %15.6f |\n' \
    "$symbol" "$percent" "$self_seconds"
done

printf '+------------------------------------------+----------+-----------------+\n'
printf 'Self times are estimated from perf cycle samples and benchmark runtime.\n'
