# rvcc
This repository is a learn-project of compiler and risc-v.

The tutorial from [sunshaoce](https://github.com/sunshaoce/rvcc), thanks sunshaoce!

## Dependencies
You must have a [RISC-V](https://en.wikipedia.org/wiki/RISC-V) environment to ensure that the program runs correctly.

I suggest use [docker](https://www.docker.com/).

## Quick start
### Configuration docker
```bash
docker run -u root --volume .:/rvcc --rm -it registry.cn-hangzhou.aliyuncs.com/dailycoding/rvcc
```
### Build 
```bash
# Build
make
# Run
make run
# Test
make test
# Clean
make clean
```
