#!/usr/bin/env bash
# Regenerate every golden baseline from the current binary.
# Use after an intentional output change; review the resulting diff
# before committing.

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_DIR="$( cd "$SCRIPT_DIR/.." && pwd )"
BIN="$PROJECT_DIR/splitl1cache"
GOLDEN_DIR="$SCRIPT_DIR/golden"

if [[ ! -x "$BIN" ]]; then
    echo "ERROR: binary not found at $BIN. Run 'make' first." >&2
    exit 2
fi

mkdir -p "$GOLDEN_DIR"

shopt -s nullglob
count=0
for trace in "$SCRIPT_DIR"/*.txt; do
    name="$( basename "$trace" )"
    for mode in 0 1; do
        out="$GOLDEN_DIR/${name}.mode${mode}.out"
        "$BIN" "$mode" "$trace" > "$out" 2>&1
        count=$((count+1))
    done
done

echo "Regenerated $count golden file(s) in $GOLDEN_DIR"
