## to use this HAL
(within a linux environment)

1. install nix:

```sh <(curl -L https://nixos.org/nix/install) --daemon```


2. enable flakes

Add the following to ~/.config/nix/nix.conf or /etc/nix/nix.conf:

`experimental-features = nix-command flakes`

3. `nix build` within the `hytech_hal` directory

4. to flash code onto a board using an stm-32's bootloader: `nix run .#dfu`

## getting your auto-completion to work

1. after following steps 1 and 2 from above, run `nix develop`

2. `mkdir build` in this directory

3. `cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && make -j` to build everything while generating the compile commands file that vscode will look for

## what this HAL contains

1. interface library for user libs to leverage implementations of the hytech_hal =
2. implementations of the hytech_hal library for each chip
3. the cmake toolchain(s) to be used in each library and application impl

TODO:
- a seperate repo / cmake package needs to be made that will include macros for creating a standard cmake package for hytech.  

## general flow
a user library uses the interface library (for say creating a driver for a screen) where as a user "application" implementation (where the `int main()` lives) will implement / use the individual implementations of the hytech_hal and pass it a pointer to it. (PIMPL pattern) https://en.cppreference.com/w/cpp/language/pimpl

