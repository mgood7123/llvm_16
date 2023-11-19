let
    nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarballs/release-22.11";
    host = import <nixpkgs> {};
    common = target:
        target.stdenv.mkDerivation {
            name = "ncurses5";

            nativeBuildInputs = [
                host.pkg-config
            ];

            buildInputs = [
                target.ncurses5
                target.libedit
           ];
        };
in {
    host = common host.pkgsCross.mingwW64;
}
