
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
    in
    {
      overlays.default = nixpkgs.lib.composeManyExtensions my_overlays;
      packages.x86_64-linux =
        let
          pkgs = import nixpkgs {
            system = "x86_64-linux";
            overlays = [ self.overlays.default ];
          };
        in
        rec {
          hytech_hal = pkgs.hytech_hal;
          default = hytech_hal;
        };

    };
}
