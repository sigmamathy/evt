# evt - CLI Event + TODO App

Only works on Linux machine. Written in C++20 with CMake.

### Build:

Clone the repo and navigate to the directory, then type:

```
cmake . -S out
make -C out
sudo mv out/evt /usr/local/bin/evt
```

then the executable will be placed in `/usr/local/bin`.

### Usage:

```
evt             # view calendar
evt ls          # list events and todos
evt +e          # add event
evt +t          # add todo
evt -e          # remove event
evt -t          # mark todo as done

```

In calendar mode, press `arrow keys` to move around.
Press `<` or `>` to navigate months, or `q` to quit.