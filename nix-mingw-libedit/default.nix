let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/nixos-23.05.tar.gz";
  pkgs = import nixpkgs { overlays = [ overlay ]; };
  overlay = self: super: {
    foo = self.callPackage ./foo.nix {};
  };
in {
  host = {
    inherit (pkgs) foo;
  };
  win32 = {
    inherit (pkgs.pkgsCross.mingw32) foo;
  };
  win64 = {
    inherit (pkgs.pkgsCross.mingwW64) foo;
  };
}
