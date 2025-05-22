#!/usr/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./build/rvcc $input > ./tests/tmp.s || exit
  riscv64-unknown-linux-gnu-gcc -static ./tests/tmp.s -o ./tests/tmp
  qemu-riscv64 -L $RISCV/sysroot ./tests/tmp

  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 242 242

echo "OK"
