# WinCpctelera is a port of cpctelera API to Windows
https://github.com/lronaldo/cpctelera

WinCpctelera is a Windows implementation of the API of CPCTelera (latest v1.5 beta) and use GDI or SDL.
The objective is to code and debug easily under Windows environment (tested with Visual C++ Community) and use the same code to compile CPCTelera project.

Some functions are also provided to help debug or to have a better code compatibility with CPC, 
they are in directory **_helper**.

Cross compressors are also provided in the directory **_compressor** :
- Exomizer (not the optimized version) from Magnus Lind
- Lz48 from Roudoudou
- Zx7 from Einar Saukas

There are some limitations :
- Changing mode (0,1,2) on interrupt corrupt display
- No sound
- No direct memory access :
```
u8* memVideo = (u8*)0xC000;
*memVideo = 0x00; // Crash under Windows
```

But you can use this function helper :
```
u8* memVideo = WinCpcTelera_GetMemPtr(0xC000);
*memVideo = 0x00; // Works both for Windows and CPC
```

- SDCC assembly is not supported but you can use the macro WINCPCTELERA to make an alternate C version :

```
#ifndef WINCPCTELERA
  _asm
    ...
  _endasm
#else
  Same code but coded in C
#endif
```


