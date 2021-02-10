# Compiling

Requires make and GCC or Clang

Run `make ijvm` to build the ijvm binary

You can enable the debug print (`dprintf`) found in `include/util.h` by
setting the `-DDEBUG` compiler flag (e.g., `make clean && make testbasic CFLAGS=-DDEBUG`).

# Running a binary

Run an IJVM program using `./ijvm binary`. For example `./ijvm files/advanced/Tanenbaum.ijvm`.

## Adding header files

Add your header files to the folder `include`.

# Compatibility

You need a valid C11 compiler, such as clang or gcc, as well as glibc. Do not
use any non-standard libraries.
