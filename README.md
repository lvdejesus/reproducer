# Reproducer

Logs input to the terminal to be played back later.

## How to use

---

Concatenate your program to `flush.c` then compile. Run with `tester`.

```bash
gcc tester.c -o tester

cat flush.c main.c > temp.c
gcc temp.c -o temp
./tester ./temp
```

## Example

---

Place `main.c` in the repository folder.

```c
// main.c
#include <stdio.h>

int main() {
  char text[16];
  scanf("%[^\n]s", text);
  printf("%s\n", text);
}
```

Run the commands in the "How to use" section. The makefile can also be used.

When running, three options are shown.

- **Save** asks for the path for the log file then starts the program normally.
- **Load** asks for the path for the log file then starts the program and plays back the inputs.
- **Save and Load** asks for the path for the log file to save to and the path for the log file to load from then starts the program and plays back the inputs. The new log file will contain both the contents of the loaded log file and the new input.
  Sending `Ctrl+X` will stop the program anywhere. Use it instead of `Ctrl+C`.

1. Save to `test`. Enter `Hello world!`.

```bash
$ ./tester ./temp
[1] Save
[2] Load
[3] Save and Load
1
Path to save to: test
Hello world!
Hello world!
^X
```

2. Load from `test`. `Hello world!` will be printed immediately.

```bash
$ ./tester ./temp
[1] Save
[2] Load
[3] Save and Load
2
Path to load from: test
Hello world!
^X
```
