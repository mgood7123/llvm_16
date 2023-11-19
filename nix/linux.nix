#
# nix cross compilation targets
#
# pkgs.pkgsCross.aarch64-android             pkgs.pkgsCross.loongarch64-linux           pkgs.pkgsCross.ppcle-embedded
# pkgs.pkgsCross.aarch64-android-prebuilt    pkgs.pkgsCross.m68k                        pkgs.pkgsCross.raspberryPi
# pkgs.pkgsCross.aarch64-darwin              pkgs.pkgsCross.mingw32                     pkgs.pkgsCross.remarkable1
# pkgs.pkgsCross.aarch64-embedded            pkgs.pkgsCross.mingwW64                    pkgs.pkgsCross.remarkable2
# pkgs.pkgsCross.aarch64-multiplatform       pkgs.pkgsCross.mips-embedded               pkgs.pkgsCross.riscv32
# pkgs.pkgsCross.aarch64-multiplatform-musl  pkgs.pkgsCross.mips-linux-gnu              pkgs.pkgsCross.riscv32-embedded
# pkgs.pkgsCross.aarch64be-embedded          pkgs.pkgsCross.mips64-embedded             pkgs.pkgsCross.riscv64
# pkgs.pkgsCross.arm-embedded                pkgs.pkgsCross.mips64-linux-gnuabi64       pkgs.pkgsCross.riscv64-embedded
# pkgs.pkgsCross.armhf-embedded              pkgs.pkgsCross.mips64-linux-gnuabin32      pkgs.pkgsCross.rx-embedded
# pkgs.pkgsCross.armv7a-android-prebuilt     pkgs.pkgsCross.mips64el-linux-gnuabi64     pkgs.pkgsCross.s390
# pkgs.pkgsCross.armv7l-hf-multiplatform     pkgs.pkgsCross.mips64el-linux-gnuabin32    pkgs.pkgsCross.s390x
# pkgs.pkgsCross.avr                         pkgs.pkgsCross.mipsel-linux-gnu            pkgs.pkgsCross.sheevaplug
# pkgs.pkgsCross.ben-nanonote                pkgs.pkgsCross.mmix                        pkgs.pkgsCross.ucrt64
# pkgs.pkgsCross.bluefield2                  pkgs.pkgsCross.msp430                      pkgs.pkgsCross.vc4
# pkgs.pkgsCross.fuloongminipc               pkgs.pkgsCross.musl-power                  pkgs.pkgsCross.wasi32
# pkgs.pkgsCross.ghcjs                       pkgs.pkgsCross.musl32                      pkgs.pkgsCross.x86_64-darwin
# pkgs.pkgsCross.gnu32                       pkgs.pkgsCross.musl64                      pkgs.pkgsCross.x86_64-embedded
# pkgs.pkgsCross.gnu64                       pkgs.pkgsCross.muslpi                      pkgs.pkgsCross.x86_64-freebsd
# pkgs.pkgsCross.gnu64_simplekernel          pkgs.pkgsCross.or1k                        pkgs.pkgsCross.x86_64-netbsd
# pkgs.pkgsCross.i686-embedded               pkgs.pkgsCross.pogoplug4                   pkgs.pkgsCross.x86_64-netbsd-llvm
# pkgs.pkgsCross.iphone32                    pkgs.pkgsCross.powernv                     pkgs.pkgsCross.x86_64-unknown-redox
# pkgs.pkgsCross.iphone32-simulator          pkgs.pkgsCross.ppc-embedded
# pkgs.pkgsCross.iphone64                    pkgs.pkgsCross.ppc64
# pkgs.pkgsCross.iphone64-simulator          pkgs.pkgsCross.ppc64-musl
# 

let
    nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarballs/release-22.11";
    host = import <nixpkgs> {};
    common = target:
        target.stdenv.mkDerivation {
            name = "llvm 18";
            src = ./. ;

            nativeBuildInputs = [
                host.cmake
                host.pkg-config
            ];

            buildInputs = [
                target.libedit
                target.lzma
                target.zlib
                target.zstd
                target.lz4
                target.xxHash
                target.bzip2
                target.libb2
                target.libxml2
                target.openssl_3_0
                target.expat
                target.libarchive
            ];
        };
in {
    host = common host.pkgsCross.gnu64;
}