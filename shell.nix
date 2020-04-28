with import <nixpkgs> {};

pkgs.mkShell {
  name = "hafos-shell";
  buildInputs = [
    pkgsCross.i686-embedded.buildPackages.gcc
    gdb
    grub2
    xorriso
    qemu
  ];
}
