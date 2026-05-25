#!/usr/bin/env bash
# Run every tracefile in both modes and diff against golden baselines.
# Exit non-zero if any diff is non-empty or any golden is missing.

set -u

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_DIR="$( cd "$SCRIPT_DIR/.." && pwd )"
BIN="$PROJECT_DIR/splitl1cache"
GOLDEN_DIR="$SCRIPT_DIR/golden"

if [[ ! -x "$BIN" ]]; then
    echo "ERROR: binary not found at $BIN. Run 'make' first." >&2
    exit 2
fi

if [[ ! -d "$GOLDEN_DIR" ]]; then
    echo "ERROR: golden directory missing: $GOLDEN_DIR" >&2
    exit 2
fi

shopt -s nullglob
TRACES=( "$SCRIPT_DIR"/*.txt )
if (( ${#TRACES[@]} == 0 )); then
    echo "ERROR: no tracefiles found in $SCRIPT_DIR" >&2
    exit 2
fi

pass=0
fail=0
missing=0
failed_names=()

for trace in "${TRACES[@]}"; do
    name="$( basename "$trace" )"
    for mode in 0 1; do
        golden="$GOLDEN_DIR/${name}.mode${mode}.out"
        if [[ ! -f "$golden" ]]; then
            echo "MISSING golden: ${name} mode ${mode}"
            missing=$((missing+1))
            continue
        fi
        actual="$( "$BIN" "$mode" "$trace" 2>&1 )"
        expected="$( cat "$golden" )"
        if [[ "$actual" == "$expected" ]]; then
            pass=$((pass+1))
        else
            fail=$((fail+1))
            failed_names+=( "${name} mode ${mode}" )
            echo "FAIL ${name} mode ${mode}:"
            diff <( echo "$expected" ) <( echo "$actual" ) | head -40
            echo
        fi
    done
done

echo "---"
echo "Pass: $pass   Fail: $fail   Missing-golden: $missing"
if (( fail > 0 )); then
    echo "Failed: ${failed_names[*]}"
fi
if (( fail > 0 || missing > 0 )); then
    exit 1
fi
exit 0
