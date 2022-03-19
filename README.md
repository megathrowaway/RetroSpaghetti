# RetroSpaghetti
executes spaghetti format ROMs and displays recipe.

# compiling and running
just use gcc, simple as can be.

```
gcc -o spaghetti main.c
./spaghetti bolognese_spaghetti.rom
```

# format
rom:
```c
struct rom {
  char magic[5]; // SPGHT
  uint8_t entry_size;
  struct entry entries[entry_size];
};
```

entry:
```c
struct entry {
  uint8_t msg_len;
  enum opt; // 0 - continue; 1 - ask (y or n)
  char msg[msg_len];
};
```
