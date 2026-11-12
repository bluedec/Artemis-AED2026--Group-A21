{ pkgs ? import <nixpkgs> {} }:

let
  libs = with pkgs; [
    libGL
    xorg.libX11
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXi
    xorg.libXext
  ];
in
pkgs.mkShell {
  packages = with pkgs; [ gcc gnumake pkg-config ] ++ libs;

  # GLFW carga algunas de estas librerías en tiempo de ejecución
  shellHook = ''
    export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath libs}:$LD_LIBRARY_PATH
  '';
}
