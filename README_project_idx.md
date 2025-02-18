## ./autogen.sh

From within the `vice` top level dir
`nix-shell -p autoconf automake --run ./autogen.sh`

## ./configure

From within the `vice` top level dir:
`nix-shell -p gcc gnumake byacc flex xa gawk dos2unix perl pkg-config glib gtk3 glew libevdev curl libpulseaudio alsaLib --command 'LDFLAGS="-Wl,--copy-dt-needed-entries" ./configure'`

Notes on the above: the `LDFLAGS="-Wl,--copy-dt-needed-entries"` is needed to avoid an error about 'DSO missing from command line', although I forgot to take note of the exact missing symbol and compilation command that caused it - it was something to do with a missing symbol for 'glX.........'. Should be easy to recreate by replacing `--command '....'` with `--run ./configure` in the above.

## make

Use the same command as for `./configure` above, but run `make` instead of `./configure`.
