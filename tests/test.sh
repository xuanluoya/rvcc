#!/usr/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./build/rvcc "$input" > ./tests/tmp.s || exit
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
assert 4 '4-1+1'
assert 41 ' 12 + 34 - 5 '
assert 6 '2 * 3'
assert 15 '5*(2+1)'
assert 2 '6/3'
assert 10 '-10+20'
assert 10 '- -10'
assert 10 '- - +10'
assert 48 '------12*+++++----++++++++++4'

echo "OK"
