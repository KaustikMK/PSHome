# PSHome

This repository contains a simple PS4 homebrew example. The code can be compiled using the **orbisdev** toolchain.

## Building locally

Install the [orbisdev](https://github.com/orbisdev/orbisdev) toolchain and run:

```sh
orbis-clang main.c -o fake_shell.elf
```

## GitHub Actions

A GitHub Actions workflow is provided to automatically build the project. It uses the `orbisdev/orbisdev` Docker image to compile `main.c` and uploads the resulting `fake_shell.elf` artifact.
