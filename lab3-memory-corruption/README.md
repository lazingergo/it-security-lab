# Memory Corruption

2026-03-24

## Introduction

The goal of this lab is to introduce students to some important categories of software implementation bugs. In addition to recognizing these bugs, students will also learn to use several important tools during the lab, which can be used to exploit implementation flaws. Finally, students are asked to try different defenses against the attacks they have previously developed.

## Theoretical Summary

The theoretical parts of the material needed for the lab can be found in the Memory Corruption slides of the Software Security course. In addition to the foundations covered there, the following details may help with completing the lab.

## Compilation

During the lab it is often necessary to recompile the analyzed application with the appropriate flags. This can be done efficiently with make. Each task includes a makefile that contains all required configuration to make the work easier. At the start of the lab, it is recommended to review the makefile!

## Efficient Parameter Passing

The applications analyzed during the lab usually process command-line parameters. During an attack, it is often necessary to pass a large number of identical characters. This can be done efficiently by combining bash features with a scripting language. A simple example using Python:

```bash
./app "$(python3 -c 'print("A"*4 + "\x01\x02\x03\x04")')"
```

Passing raw bytes directly in bash:

```bash
./app $'AAAA\x01\x02\x03\x04'
```

## Useful GDB Commands

Using gdb is recommended when examining the applications during the lab. In addition to the more familiar instructions, the following commands may also be useful:

- `b *<address>`: Set a breakpoint at a given address.
- `print foo`: Print the address of the `foo` function in memory.
- `print 'malloc@plt'`: Print the address of the `malloc` function in the PLT table.^1 Functions called from external libraries are invoked indirectly through the `.plt` table.
- `disas`: Disassemble a given function or address.
- `x/[num]x <address>`: Print the contents of a memory region over `num*8` bytes.

## Function Call Process

During a function call, stack management is partly handled by the caller and partly by the callee.

### The Caller’s Responsibilities

Before execution can jump to a function, the caller has two tasks. First, it must place the parameters required for the callee on the stack, and their order may depend on the calling convention. Second, it must place a return address. This tells the callee which address execution should continue from after it finishes.

### The Callee’s Responsibilities

The callee continues building the stack frame. After the caller’s preparation, the EBP register is saved. Finally, only the space needed for local variables has to be allocated, and then the actual function logic can begin.

Based on this, the prepared stack looks like the following figure:

## Tasks

### 1. Introduction to Buffer Overflow

The goal of the first task is to implement a simple buffer overflow. The vulnerable application is located in the `task-1` folder. The goal of the attack is to divert execution so that the `not_called` function also runs.

Steps:

1. Review the makefile! Find which command will be executed when make is run.
2. Inspect the source code and find the vulnerability in the application!
3. Compile the application: `make`
4. Analyze the application’s behavior at runtime using gdb!
5. Draw the stack before the attack and immediately after it!
6. Provide the attacker input that achieves the goal!
7. Suggest a fix at the following levels: source code changes, compilation, operating system!
8. Check whether compiling the application with stack smashing protection enabled (`make withSSP`) protects against the attack. Explain the result!

### 2. Buffer Overflow with Parameters

This task aims to implement a buffer overflow attack extended with parameter passing. The vulnerable application is located in the `task-2` folder. The goal is to exploit the vulnerability to call the `now_called` function while placing the appropriate parameter on the stack as if a real function call were taking place.

Steps:

1. Compile the application: `make`
2. Analyze the application’s behavior at runtime using gdb!
3. Draw the stack before the attack and immediately after it!
4. Provide the attacker input that achieves the goal!
5. Check whether compiling with ASLR enabled (`make withASLR` and `make withASLRwithPIE`) protects against the attack. Explain the result!

### 3. Return to LibC

The goal of the third task is to implement a Return-to-LibC attack. The vulnerable application is located in the `task-3` folder. If an application uses a library, then an attacker can jump not only to functions written by the programmers, but also to any function found in the loaded libraries.

As a result, any function in a shared library can help an attacker, typically one from the LibC library. The goal of the attack is to divert execution so that the `system` function runs with the appropriate parameter.

Steps:

1. Compile the application: `make`
2. Analyze the application’s behavior at runtime using gdb!
3. Based on the LibC documentation, determine which parameter is required to achieve the goal.
4. Draw the stack before the attack and immediately after it!
5. Provide the attacker input that achieves the goal!
6. Check whether compiling with NX enabled (`make withNX`) protects against the attack. Explain the result!

### 4. ROP

The goal of the fourth task is to implement a ROP attack. The vulnerable application is located in the `task-4` folder. In a ROP attack, small code snippets found in the binary (gadgets) are reused to build an attack. The first step is to use the ROPgadget Python script to examine which gadgets are available. It is not at all certain that the gadgets needed for the simplest imagined solution are available, so some creativity may be required. By combining them appropriately, achieve a shell again! In a ROP attack, the input can be significantly longer than in previous tasks, so using the `exploit.py` script in the folder is recommended. This script includes several hints about which gadgets may be worth searching for, and it also serves as a sample showing how to assemble the attack.

Steps:

1. Inspect the source code and identify the vulnerability and the possible attack in the application!
2. Compile the application: `make`
3. Analyze the application’s behavior at runtime using gdb!
4. Draw the stack before the attack and immediately after it!
5. Use ROPgadget to find the available gadgets!
6. Use `exploit.py` to assemble the attack!
7. Check whether compiling with stack smashing protection enabled (`make withSSP`) protects against the attack. Explain the result!
