# WinCpctelera
is a port of cpctelera API to Windows :
https://github.com/lronaldo/cpctelera

WinCpctelera is a Windows implementation of the API of CPCTelera and use the GDI or SDL.
The goal is to code and debug easily under Windows environment (i use visual c++) and use the same code to compile for CPC with Cywin and cpctelera.

Some functions are also provided to help debugging or to have a better compatibility with CPC, they are in directory **_helper**

There are some limitations :
- No sound
- No direct memory access
```
u8* memVideo = 0xC000;
*memVideo = 0x00; // Crash under Windows
```

But you can use this function helper :
```
u8* memVideo = WinCpcTelera_GetMemPtr(u0xC000);
*memVideo = 0x00; // Works both for Windows and CPC
```

- SDCC assembly is not supported but you can use the macro WINCPCTELERA to make an alternate C version.

```
#ifndef WINCPCTELERA
  _asm
    ...
  _endasm
#else
  Same code but coded in C
#endif
```


