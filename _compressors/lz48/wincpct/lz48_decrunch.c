/* LZ48 simple C source file / crappy version by roudoudou - Flower Corp. 2016 */
/* Modified by Arnaud Bouche for WinCpctelera */

/*************** System includes ***************/
#include <cpctelera.h>

#ifndef WINCPCTELERA
	
	void LZ48Decrunch(const u8* source, u8* target) __z88dk_callee
	{
		source;
		target;
		__asm
			pop af
			pop hl
			pop de

			push af
			push ix

			call _LZ48_decrunch

			pop ix
		__endasm;
	}

#else

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <malloc.h>

	static void LZ48_decode(const unsigned char *data, unsigned char *odata)
	{
		int HL = 0, DE = 0;
		int literallength, matchlength, HLmatch;

		odata[DE++] = data[HL++];

		while (1) {
			literallength = (data[HL] & 0xF0) >> 4;
			matchlength = (data[HL++] & 0xF);

			if (literallength == 15) {
				while (data[HL] == 255) {
					literallength += 255;
					HL++;
				}
				literallength += data[HL++];
			}

			while (literallength > 0) {
				odata[DE++] = data[HL++];
				literallength--;
			}

			/* matchkey */
			if (matchlength == 15) {
				while (data[HL] == 255) {
					matchlength += 255;
					HL++;
				}
				matchlength += data[HL++];
			}
			matchlength += 3;
			if (data[HL] == 0xFF) return; else HLmatch = DE - (data[HL++] + 1);

			while (matchlength) {
				odata[DE++] = odata[HLmatch++];
				matchlength--;
			}
		}
	}

	static unsigned char* LZ48_decrunch(const unsigned char *data, int *osize)
	{
		int literallength, matchlength;
		int HL = 1, DE = 1;
		unsigned char *odata = NULL;

		while (1) {
			literallength = (data[HL] & 0xF0) >> 4;
			matchlength = (data[HL++] & 0xF);
			if (literallength == 15) {
				while (data[HL] == 255) {
					literallength += 255;
					HL++;
				}
				literallength += data[HL++];
			}

			DE += literallength;
			HL += literallength;

			/* matchkey */
			if (matchlength == 15) {
				while (data[HL] == 255) {
					matchlength += 255;
					HL++;
				}
				matchlength += data[HL++];
			}
			matchlength += 3;
			if (data[HL] == 0xFF) break; else HL++;

			DE += matchlength;
		}
		*osize = DE;
		odata = malloc(*osize);
		if (!odata) {
			fprintf(stderr, "memory full\n");
			exit(-1);
		}
		LZ48_decode(data, odata);
		return odata;
	}

	void LZ48Decrunch(const unsigned char* pSrc, unsigned char* pDest)
	{
		int deflateSize;
		unsigned char* deflateData = LZ48_decrunch(pSrc, &deflateSize);
		memcpy(pDest, deflateData, deflateSize);
		free(deflateData);
	}

#endif