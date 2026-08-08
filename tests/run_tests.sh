#!/usr/bin/env bash
#
# Builds the compiler and runs it on every .psc file in examples/,
# comparing the output against the matching file in tests/expected/.
#
# Usage: ./tests/run_tests.sh
# Exit code: 0 if all tests pass, 1 if any test fails.

set -u

# Resolve paths relative to this script, so it can be run from anywhere.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
EXAMPLES_DIR="$ROOT_DIR/examples"
EXPECTED_DIR="$SCRIPT_DIR/expected"
BINARY="$ROOT_DIR/compiler"

echo "Building compiler..."
g++ -std=c++17 -I"$ROOT_DIR/include" \
    "$ROOT_DIR/src/main.cpp" \
    "$ROOT_DIR/src/Tokenizer.cpp" \
    "$ROOT_DIR/src/Parser.cpp" \
    "$ROOT_DIR/src/Utils.cpp" \
    -o "$BINARY"

if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

pass_count=0
fail_count=0

for psc_file in "$EXAMPLES_DIR"/*.psc; do
    name="$(basename "$psc_file" .psc)"
    expected_file="$EXPECTED_DIR/${name}.expected"

    if [ ! -f "$expected_file" ]; then
        echo "SKIP  $name (no expected output found at $expected_file)"
        continue
    fi

    actual_output="$("$BINARY" "$psc_file")"
    expected_output="$(cat "$expected_file")"

    if [ "$actual_output" == "$expected_output" ]; then
        echo "PASS  $name"
        pass_count=$((pass_count + 1))
    else
        echo "FAIL  $name"
        diff <(echo "$expected_output") <(echo "$actual_output")
        fail_count=$((fail_count + 1))
    fi
done

echo ""
echo "$pass_count passed, $fail_count failed"

if [ "$fail_count" -ne 0 ]; then
    exit 1
fi
