
{
  description = "Hytech HAL";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    utils.url = "github:numtide/flake-utils";
    libopencm3_cmake.url = "github:RCMast3r/libopencm3_cmake";
  };
  outputs = { self, nixpkgs, utils, libopencm3_cmake }:
    let
      hytech_hal_overlay = final: prev: {
        hytech_hal = final.callPackage ./default.nix { };
      };
      my_overlays = [libopencm3_cmake.overlays.default hytech_hal_overlay ]; 
      pkgs = import nixpkgs {
            system = "x86_64-linux";
            overlays = [ self.overlays.default ];
      };
    in
    {
      overlays.default = nixpkgs.lib.composeManyExtensions my_overlays;
      
      packages.x86_64-linux =
        rec {
          hytech_hal = pkgs.hytech_hal;
          default = hytech_hal;
        };

        devShells.x86_64-linux.default = 
        pkgs.mkShell rec {
            # Update the name to something that suites your project.
            name = "nix-devshell";

            packages = with pkgs; [
              # Development Tools
              gcc-arm-embedded
              cmake
            ];

            # Setting up the environment variables you need during
            # development.
            shellHook = let
              icon = "f121";
            in ''
              export PS1="$(echo -e '\u${icon}') {\[$(tput sgr0)\]\[\033[38;5;228m\]\w\[$(tput sgr0)\]\[\033[38;5;15m\]} (${name}) \\$ \[$(tput sgr0)\]"
            '';
          };
      
    };
}
