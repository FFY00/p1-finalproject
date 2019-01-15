# FinalProject

### Build

Requirements:
  - meson

Instructions:
```
mkdir build && cd build
meson ..
ninja
```

### Debug

I reccomend using `valgrind` and `gdb`, usualy `valgrind` is enough.

If you do use `valgrin`, invoke it with the following arguments:
```
valgrind --leak-check=full --show-leak-kinds=all ./finalproject
```

### Documention

Documention is generated trough `doxygen`. Simple run it against the distributed `Doxyfile`.
```
doxygen Doxyfile
```

Documentation should be in `doc/{html,latex}`.

### License

GNU General Public License v3
