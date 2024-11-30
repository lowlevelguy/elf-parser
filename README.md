# ELF Parser

I'm not yet sure where exactly I want to go with this, but I'm currently just writing this to get a better grasp on the structure of an ELF executable.

## Building project
```
mkdir obj
make
```

## Running
```
Syntax: elfparse [file]
```

## TODO List
[*x*] ELF header parsing\
[*x*] Program header parsing\
[] Section header parsing

## Supported architectures
At the current moment, the only supported architectures are `i386`, `arm` (32-bit) and `riscv`.
