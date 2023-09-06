## to use this HAL
(within a linux environment)

1. install nix:

```sh <(curl -L https://nixos.org/nix/install) --daemon```


2. enable flakes

Add the following to ~/.config/nix/nix.conf or /etc/nix/nix.conf:

`experimental-features = nix-command flakes`

3. `nix build` within the `hytech_hal` directory