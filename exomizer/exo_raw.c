/*
* Copyright (c) 2002 - 2005 Magnus Lind.
*
* This software is provided 'as-is', without any express or implied warranty.
* In no event will the authors be held liable for any damages arising from
* the use of this software.
*
* Permission is granted to anyone to use this software, alter it and re-
* distribute it freely for any non-commercial, non-profit purpose subject to
* the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not
*   claim that you wrote the original software. If you use this software in a
*   product, an acknowledgment in the product documentation would be
*   appreciated but is not required.
*
*   2. Altered source versions must be plainly marked as such, and must not
*   be misrepresented as being the original software.
*
*   3. This notice may not be removed or altered from any distribution.
*
*   4. The names of this software and/or it's copyright holders may not be
*   used to endorse or promote products derived from this software without
*   specific prior written permission.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "membuf.h"
#include "exo_helper.h"

void read_file(const char *name, struct membuf *buf)
{
    char block[1024];
    FILE *in;
    int len;

    in = fopen(name, "rb");
    if(in == NULL)
    {
        exit(-1);
    }
    do
    {
        len = fread(block, 1, 1024, in);
        membuf_append(buf, block, len);
    }
    while(len == 1024);
    fclose(in);
}

/*void write_file(const char *name, struct membuf *buf)
{
    FILE *out;
    out = fopen(name, "wb");
    if(out == NULL)
    {
        exit(-1);
    }
    fwrite(membuf_get(buf), 1, membuf_memlen(buf), out);
    fclose(out);
}*/

int unexo(char* in, char* out)
{
	struct membuf inbuf[1];
	struct membuf outbuf[1];

	//unsigned short size = *(unsigned short*)(in + 3) + 2;

	//char block[1024];
	//memcpy(block, in, size);

	membuf_init(inbuf);

	//read_file("compress.exo", inbuf);
	//membuf_append(inbuf, in, size);

	membuf_init(outbuf);

	int backwards_mode = 0;
	int seems_backward = 0;
	int seems_forward = 0;
	unsigned char *p;
	
	p = membuf_get(inbuf);
	p = in;

	/*if (p[0] == 0x80 && p[1] == 0x0)
	{
		seems_backward = 1;
	}
	p += membuf_memlen(inbuf);
	if (p[-1] == 0x80 && p[-2] == 0x0)
	{
		seems_forward = 1;
	}*/

	/* do we know what way it was crunched? */
	/*if ((seems_backward ^ seems_forward) != 0)
	{*/
		/* yes, override option. */
/*		backwards_mode = seems_backward;
	}*/

//	if (backwards_mode)
	{
		decrunch_backwards(inbuf, outbuf);
	}
/*	else
	{
		decrunch(inbuf, outbuf);
	}*/
	
	//write_file("icon.c", outbuf);

	membuf_free(outbuf);
	membuf_free(inbuf);

	return 0;
}
