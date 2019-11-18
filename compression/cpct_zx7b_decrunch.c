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

 /* 
 *  (c) 2018 dzx7b by Antonio Villena https://github.com/antoniovillena/zx7b/blob/master/dzx7b.c
 *  (c) 2018 Adaptation for cpct_zx7b_decrunch of WinCPCtelera by Arnaud Bouche (@Arnaud6128)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *input_data;
static unsigned char *output_data;

static size_t output_index;
static size_t output_size;

static int bit_mask;
static int bit_value;

static int read_byte()
{
	return *input_data--;
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

static int read_elias_gamma() 
{
	int value;

	value = 1;
	while (!read_bit())
	{
		value = value << 1 | read_bit();
	}

	if ((value & 255) == 255)
		value = -1;

	return value;
}

static int read_offset() 
{
	int value;
	int i;

	value = read_byte();
	if (value < 128) 
	{
		return value;
	}
	else 
	{
		i = read_bit();
		i = i << 1 | read_bit();
		i = i << 1 | read_bit();
		i = i << 1 | read_bit();
		return (value & 127 | i << 7) + 128;
	}
}

static void write_byte(int value)
{
	output_data[output_index++] = value;
}

static void write_bytes(int offset, int length)
{
	while (length-- > 0) 
	{
		write_byte(output_data[output_index - offset]);
	}
}

static void decompress() 
{
	int length;
	output_index = 0;
	bit_mask = 0;

	write_byte(read_byte());
	while (1) 
	{
		if (!read_bit())
		{
			write_byte(read_byte());
		}
		else 
		{
			length = read_elias_gamma() + 1;
			if (length == 0)
			{
				return;
			}
			write_bytes(read_offset() + 1, length);
		}
	}
}

static void computeOutputSize()
{
	int length;
	output_size = 1;
	bit_mask = 0; 

	read_byte();
	while (1)
	{
		if (!read_bit())
		{
			read_byte();
			output_size++;
		}
		else
		{
			length = read_elias_gamma() + 1;
			if (length == 0)
			{
				return;
			}
			read_offset();
			output_size += length;
		}
	}
}

static void reverseOutputData(unsigned char* output)
{
	size_t i;
	for (i = 0; i < output_size; i++)
		*output-- = output_data[i];
}

void cpct_zx7b_decrunch(void* dest_end, void* source_end)
{
	input_data = (unsigned char*)source_end;
	computeOutputSize();

	input_data = (unsigned char*)source_end;
	output_data = malloc(output_size);
	decompress();
	
	reverseOutputData(dest_end);
	free(output_data);
}

void cpct_zx7b_decrunch_s(void* dest_end, void* source_end)
{
	cpct_zx7b_decrunch(dest_end, source_end);
}

void cpct_zx7b_decrunch_f0(void* dest_end, void* source_end)
{
	cpct_zx7b_decrunch(dest_end, source_end);
}

void cpct_zx7b_decrunch_f1(void* dest_end, void* source_end)
{
	cpct_zx7b_decrunch(dest_end, source_end);
}

void cpct_zx7b_decrunch_f2(void* dest_end, void* source_end)
{
	cpct_zx7b_decrunch(dest_end, source_end);
}