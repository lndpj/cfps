# cfps
Alarm based FPS counter and date in C

## Depends

- [C23](https://www.open-std.org/JTC1/SC22/WG14/)
- [CMake](https://www.open-std.org/JTC1/SC22/WG14/)

## Compile

```sh
cmake . --install-prefix=/usr -DCMAKE_BUILD_TYPE=ReleaseStrip -DCMAKE_C_COMPILER=clang
make install
```

## Usage

```sh
./cfps
```

### Keyboard Layout
- `ESC `       to exit sucessful
- `CTRL` + `C` to exit failure with cleanup

## Example

```c
#include "sys.h"

int main(int argc, char** argv)
{
	if(!sys.init()) exit(EXIT_FAILURE);

	while(term_getc() != KEY_ESC && sys.step() && sys.print());

	exit(EXIT_SUCCESS);
}
```
