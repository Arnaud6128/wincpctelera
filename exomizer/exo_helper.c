/*
 * Copyright (c) 2005, 2013, 2015 Magnus Lind.
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

#include "membuf.h"
#include "exodec.h"
#include "exo_helper.h"

void reverse_buffer(char *start, int len)
{
    char *end = start + len - 1;
    char tmp;

    while (start < end)
    {
        tmp = *start;
        *start = *end;
        *end = tmp;

        ++start;
        --end;
    }
}

void decrunch(struct membuf *inbuf,
              struct membuf *outbuf)
{
    struct dec_ctx ctx[1];
    dec_ctx_init(ctx, inbuf, outbuf);
    dec_ctx_decrunch(ctx);
    dec_ctx_free(ctx);
}

void decrunch_backwards(struct membuf *inbuf,
                        struct membuf *outbuf)
{
    int outpos;
    reverse_buffer(membuf_get(inbuf), membuf_memlen(inbuf));
    outpos = membuf_memlen(outbuf);

    decrunch(inbuf, outbuf);

    reverse_buffer(membuf_get(inbuf), membuf_memlen(inbuf));
    reverse_buffer((char*)membuf_get(outbuf) + outpos,
                   membuf_memlen(outbuf) - outpos);
}
