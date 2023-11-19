let
    nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/nixos-23.05.tar.gz";
    host = import nixpkgs {};

    common = target:
        target.stdenv.mkDerivation {
            name = "ncurses5";

            nativeBuildInputs = [
                host.pkg-config
            ];

            buildInputs = [
                target.ncurses5
           ];
        };
in {
    mingw64 = common host.pkgsCross.mingwW64;
}
