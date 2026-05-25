# Split L1 Cache Simulator

A C++ simulator for a 32-bit split-L1 cache with MESI coherence and pseudo-LRU replacement. Reads a trace file, applies cache operations one at a time, and prints either summary statistics or per-operation bus messages.

## Quick start

```
cd splitL1cache
make
./splitl1cache 1 tests/tracefile1.txt
```

Two arguments: `<mode> <trace_file>`. Mode `0` prints only the stats blocks emitted by the `9` operation. Mode `1` also prints L2 bus messages as each operation runs.

## What's modeled

| Component       | Dimension                  | Total capacity        |
|-----------------|----------------------------|-----------------------|
| L1 data cache   | 8-way, 16K sets, 64B line  | 8 MiB                 |
| L1 instruction  | 4-way, 16K sets, 64B line  | 4 MiB                 |
| Coherence       | MESI                       |                       |
| Replacement     | Pseudo-LRU per set         | 3-bit (D), 2-bit (I)  |
| Write policy    | Write-back, write-allocate |                       |
| Address space   | 32-bit                     |                       |

Dimensions are large by modern L1 standards. They live as `constexpr` constants in `splitL1cache/header.h:15-21` and every `.cpp` references those constants, so retuning is a one-file change.

## Address decoding

A 32-bit address splits into three fields:

```
 31                    20 19              6 5         0
+-----------------------+------------------+-----------+
|        tag (12)       |   set index (14) | offset (6)|
+-----------------------+------------------+-----------+
```

Worked example for `0x984DE132`:

| Field      | Computation                          | Value    |
|------------|--------------------------------------|----------|
| byte offset| `addr & 0x3F`                        | `0x32`   |
| set index  | `(addr & 0x000FFFFF) >> 6`           | `0x3784` |
| tag        | `addr >> 20`                         | `0x984`  |

This decode lives at the top of every operation function (for example `read.cpp:5-6`).

## MESI transitions

Data cache on a `read` tag match:

| Prior state | Counted as | New state |
|-------------|------------|-----------|
| M           | hit        | M         |
| E           | hit        | S         |
| S           | hit        | S         |
| I           | miss       | S         |

Data cache on a `write` tag match:

| Prior state | Counted as | New state |
|-------------|------------|-----------|
| M           | hit        | M         |
| E           | hit        | M         |
| S           | hit        | M         |
| I           | miss       | M         |

Instruction cache on a `fetch_inst` tag match:

| Prior state | Counted as | New state |
|-------------|------------|-----------|
| M           | hit        | M         |
| E           | hit        | S         |
| S           | hit        | S         |
| I           | miss       | S         |

Allocation on miss (no tag match in the set):

| Operation   | New line installed as |
|-------------|-----------------------|
| read        | E (Exclusive)         |
| write       | M (Modified, write-allocate) |
| fetch_inst  | E (Exclusive)         |

External bus operations on the data cache:

| Operation         | Effect on a matching line |
|-------------------|---------------------------|
| invalidate (3)    | force state to I          |
| snoop_invalidate (4) | force state to I       |

Only the bus-invalidate flavor of snoop is modeled. A real MESI implementation would also handle a bus-read snoop (M to S with writeback, E to S without). If your traces need that, you'll need to extend `snoop.cpp` and the trace format.

## Pseudo-LRU

Every line carries one unsigned counter:

| Cache | Counter width | Range | MRU value | LRU value |
|-------|---------------|-------|-----------|-----------|
| Data  | 3 bits        | 0..7  | 7         | 0         |
| Inst  | 2 bits        | 0..3  | 3         | 0         |

On every promotion (`L1_LRU` in `L1_LRU.cpp`):
1. The promoted way's counter is forced to MRU.
2. Every other way whose counter is greater than or equal to the promoted way's previous counter is decremented by one.

This keeps the counters as a permutation of `[0..N-1]` across the N occupied ways, so `find_LRU` just scans for the way whose counter is `0`. The decrement is guarded against unsigned underflow so a transiently broken invariant (e.g., during warm-up when several ways share counter `0`) cannot wrap to `UINT_MAX` and silently break eviction.

## Trace file format

One operation per line. Blank lines are skipped. Format:

```
<op> <hex_address>
```

Op codes:

| Code | Operation                            |
|------|--------------------------------------|
| 0    | Data read                            |
| 1    | Data write                           |
| 2    | Instruction fetch                    |
| 3    | Invalidate (data cache)              |
| 4    | Snoop invalidate (data cache)        |
| 8    | Clear all cache state and statistics |
| 9    | Print current statistics             |

Example trace:

```
0 984DE132
1 984DE140
9
```

Reads `0x984DE132`, writes `0x984DE140`, then dumps current stats.

## Output modes

Mode 0 emits stats blocks only at each `9` operation:

```
$ ./splitl1cache 0 tests/cache_set_fill_hits_misses.txt

	 ** KEY CACHE USAGE STATISTICS **

	 -- DATA CACHE STATISTICS --
 number of Cache Reads:      32
 number of Cache Writes:     0
 number of Cache Hits:       16
 number of Cache Misses:     16
 Cache Hit Ratio:            0.5
 Cache Hit Percentage:       50 %
```

Mode 1 adds one bus message per operation in addition to the stats blocks:

```
 [Data-Operation] Read from L2: L1 cache miss, obtain data from L2 984de132
 [Data-Operation] Read for Ownership from L2 984de140
 [Data Write_Through] Write to L2: we have a data Cache Miss 984de140
```

Mode-1 messages reset the stream back to decimal after each hex address, so subsequent stat blocks keep printing in base 10.

## Testing

```
make test     # diff every trace output against tests/golden/
make bless    # regenerate tests/golden/ after intentional changes
```

The harness runs all 14 traces in both modes (28 outputs) and exits non-zero on any difference. Treat `make bless` as a destructive action: review the diff first, because the goldens capture correct simulator behavior, and an unexplained diff usually means a regression.

## Source layout

```
splitL1cache/
  header.h           cache layouts, MESI states, externs, helpers
  globals.cpp        single-definition home for the two cache arrays
  splitl1cache.cpp   main: arg parsing, mode select
  parser.cpp         trace loader and operation dispatch
  read.cpp           data-cache read
  write.cpp          data-cache write
  fetch_inst.cpp     instruction-cache fetch
  invalidate.cpp     local invalidate
  snoop.cpp          external bus-invalidate snoop
  matching_tag.cpp   tag lookup within a set
  find_LRU.cpp       which way to evict
  L1_LRU.cpp         counter update on promotion
  clear_cache.cpp    reset all state and stats
  print_stats.cpp    formatted hit/miss output
  tests/
    *.txt            trace files
    golden/          captured baselines (28)
    run_all.sh       diff harness
    bless.sh         baseline regenerator
```

## Modeling limits worth knowing

- **Single processor.** Coherence states like Exclusive vs Shared get triggered by the trace's snoop and invalidate operations rather than by other CPUs. The `read` path conservatively demotes `E` to `S` on hit, modeling another reader appearing on the bus.
- **Data only on the bus.** The instruction cache isn't snooped, matching a Harvard-style L1 where instructions are read-only and don't participate in coherence.
- **No miss-type breakdown.** Compulsory, capacity, and coherence misses are all just "miss" in the stats output.
- **The "L2" is a text label.** There's no second-level cache structure; mode-1 messages document what a real L2 controller would see across the bus.
- **No multi-level invariant checks at runtime** beyond the LRU `assert`. If you change `L1_LRU.cpp`, run the golden harness; the LRU invariant break is subtle and silent.

## Build flags

`CXXFLAGS = -Wall -Wextra -Wshadow -std=c++20`. `-Wshadow` is load-bearing: it catches the variable-shadowing bug class that silently corrupted instruction-cache state in earlier versions. Leave it on.

## License

MIT. See `LICENSE` when added.
