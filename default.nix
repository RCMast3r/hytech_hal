{ stdenv, cmake, gcc-arm-embedded, libopencm3_stm32f4 }:

stdenv.mkDerivation rec {
  pname = "hytech_hal";
  version = "0.1.0";
  src = ./.;
  nativeBuildInputs = [ cmake gcc-arm-embedded ];
  propagatedBuildInputs = [ libopencm3_stm32f4 ];
}
