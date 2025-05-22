# rvcc
This repository is a learn-project of compiler and risc-v.

The tutorial from [sunshaoce](https://github.com/sunshaoce/rvcc), thanks sunshaoce!

## Dependencies
You must have a [RISC-V](https://en.wikipedia.org/wiki/RISC-V) environment to ensure that the program runs correctly.

I suggest use [docker](https://www.docker.com/).

If you want query document you need [doxygen](https://github.com/doxygen/doxygen) to build document, or you can look comment in code.

## Quick start
### Configuration docker
```bash
docker run -u root --volume .:/rvcc --rm -it registry.cn-hangzhou.aliyuncs.com/dailycoding/rvcc
```
### Quick start docker
```bash
make docker
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
# Build document
make docs
```
