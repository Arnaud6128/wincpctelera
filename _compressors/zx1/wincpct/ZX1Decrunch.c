/*
 * (c) Copyright 2015 by Einar Saukas. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of its author may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Adaptation for Wincpctelera by Arnaud BOUCHE 
 * Remove all file input/output functions 
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 65536  /* must be > MAX_OFFSET */
#define INITIAL_OFFSET 1

#define FALSE 0
#define TRUE 1

static const unsigned char* input_data;
static unsigned char* output_data;
static size_t input_index;
static size_t output_index;
static size_t input_size;
static size_t output_size;
static int bit_mask;
static int bit_value;
static int backtrack;
static int last_byte;
static int last_offset;

static int read_byte() 
{
    return input_data[input_index++];
}

static int read_bit() 
{
    bit_mask >>= 1;
    if (bit_mask == 0) 
    {
        bit_mask = 128;
        bit_value = read_byte();
    }
    return bit_value & bit_mask ? 1 : 0;
}

static int read_interlaced_elias_gamma() 
{
    int value = 1;
    while (read_bit()) 
    {
        value = value << 1 | read_bit();
    }
    return value;
}


static void write_byte(int value) 
{
    output_data[output_index++] = value;
    output_size++;
}

static void write_bytes(int offset, int length)
{
    while (length-- > 0)
    {
        int i = output_index - offset;
        write_byte(output_data[i]);
    }
}

static void decompress() 
{
    int i;
    int length;
    input_size = 0;
    input_index = 0;
    output_index = 0;
    output_size = 0;
    bit_mask = 0;
    backtrack = FALSE;
    last_offset = INITIAL_OFFSET;

COPY_LITERALS:
    length = read_interlaced_elias_gamma();
    for (i = 0; i < length; i++)
    {
        write_byte(read_byte());
    }

    if (read_bit()) 
    {
        goto COPY_FROM_NEW_OFFSET;
    }

    length = read_interlaced_elias_gamma();
    write_bytes(last_offset, length);
    if (!read_bit()) 
    {
        goto COPY_LITERALS;
    }

COPY_FROM_NEW_OFFSET:
    last_offset = read_byte();
    if (last_offset & 1) 
    {
        i = read_byte();
        last_offset = 32512 - (i & 254) * 128 - (last_offset & 254) - (i & 1);
    }
    else 
    {
        last_offset = 128 - last_offset / 2;
    }

    if (last_offset <= 0) 
    {
        return;
    }

    length = read_interlaced_elias_gamma() + 1;
    write_bytes(last_offset, length);
    if (read_bit()) 
    {
        goto COPY_FROM_NEW_OFFSET;
    }
    else 
    {
        goto COPY_LITERALS;
    }
}

void ZX1Decrunch(const unsigned char* input, unsigned char* output)
{
	input_data = input;
	output_data = output;

	decompress();
}