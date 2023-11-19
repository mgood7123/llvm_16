{ stdenv, pkg-config, ncurses5, libedit }:

stdenv.mkDerivation {
  name = "foo";
  nativeBuildInputs = [ pkg-config ];
  buildInputs = [
    ncurses5
    #libedit
  ];
}
