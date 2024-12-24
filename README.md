# ELF Parser

I'm not yet sure where exactly I want to go with this, but I'm currently just writing this to get a better grasp on the structure of an ELF executable.

## Building project
```
mkdir obj
make
```
P.S. If you're using a compiler other than `gcc`, make sure to modify the `Makefile` accordingly, as well as to replace the `__attribute__((packed))` directive present in some of the header files to whatever it corresponds to for your compiler.

## Running
```
Syntax: elfparse [file]
```

## Testing
The folder `samples` contains a couple of ELF binaries to test on. Each is named in such a way that it reflects the target architecture, ABI, and other relevant information.

## TODO List
[**x**] ELF header parsing\
[**x**] Program header parsing\
[**x**] Section header parsing\
[] Section names parsing\
[] Relating program headers to corresponding segments in the file image\
[] Relating section names, section headers, and corresponding segments in the file image

## Supported architectures
At the current moment, the only supported architectures (to be parsed) are `i386`, `arm` (32-bit) and `riscv`.
