#ifndef EXO_HELPER_ALREADY_INCLUDED
#define EXO_HELPER_ALREADY_INCLUDED

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

#define CRUNCH_OPTIONS_DEFAULT {NULL, 65535, 65535, 65535, 1, 0, 1}

struct common_flags
{
    struct crunch_options *options;
    const char *outfile;
};


struct crunch_options
{
    const char *exported_encoding;
    int max_passes;
    int max_len;
    int max_offset;
    int use_literal_sequences;
    int use_imprecise_rle;
    int output_header;
};

struct crunch_info
{
    int literal_sequences_used;
    int needed_safety_offset;
};

void decrunch(struct membuf *inbuf,
              struct membuf *outbuf);

void decrunch_backwards(struct membuf *inbuf,
                        struct membuf *outbuf);

void reverse_buffer(char *start, int len);
#endif
