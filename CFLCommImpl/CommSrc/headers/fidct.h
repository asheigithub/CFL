#ifndef FIDCT_H
#define FIDCT_H

void fidct(short *const block);
void fidct_init();

/*****************************************************************************
*
*  XVID MPEG-4 VIDEO CODEC
*  - Inverse DCT  -
*
*  These routines are from Independent JPEG Group's free JPEG software
*  Copyright (C) 1991-1998, Thomas G. Lane (see the file README.IJG)
****************************************************************************/
/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

#define W1 2841     /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676     /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408     /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609     /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108     /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565       /* 2048*sqrt(2)*cos(7*pi/16) */
/* private data */
static short iclip[1024];  /* clipping table */
static short *iclp;
/* two dimensional inverse discrete cosine transform */
// fidct_init() MUST BE CALLED BEOFRE THE FIRST CALL TO THIS FUNCTION!
void fidct(short *const block)
{
	static short *blk;
	static long i;
	static long X0, X1, X2, X3, X4, X5, X6, X7, X8;

	for (i = 0; i < 8; i++)  /* idct rows */
	{
		blk = block + (i << 3);
		if (!((X1 = blk[4] << 11) | (X2 = blk[6]) | (X3 = blk[2]) | (X4 = blk[1]) |
			(X5 = blk[7]) | (X6 = blk[5]) | (X7 = blk[3])))
		{
			blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
			continue;
		}
		X0 = (blk[0] << 11) + 128; /* for proper rounding in the fourth stage  */
		/* first stage  */
		X8 = W7 * (X4 + X5);
		X4 = X8 + (W1 - W7) * X4;
		X5 = X8 - (W1 + W7) * X5;
		X8 = W3 * (X6 + X7);
		X6 = X8 - (W3 - W5) * X6;
		X7 = X8 - (W3 + W5) * X7;
		/* second stage  */
		X8 = X0 + X1;
		X0 -= X1;
		X1 = W6 * (X3 + X2);
		X2 = X1 - (W2 + W6) * X2;
		X3 = X1 + (W2 - W6) * X3;
		X1 = X4 + X6;
		X4 -= X6;
		X6 = X5 + X7;
		X5 -= X7;
		/* third stage  */
		X7 = X8 + X3;
		X8 -= X3;
		X3 = X0 + X2;
		X0 -= X2;
		X2 = (181 * (X4 + X5) + 128) >> 8;
		X4 = (181 * (X4 - X5) + 128) >> 8;
		/* fourth stage  */
		blk[0] = (short)((X7 + X1) >> 8);
		blk[1] = (short)((X3 + X2) >> 8);
		blk[2] = (short)((X0 + X4) >> 8);
		blk[3] = (short)((X8 + X6) >> 8);
		blk[4] = (short)((X8 - X6) >> 8);
		blk[5] = (short)((X0 - X4) >> 8);
		blk[6] = (short)((X3 - X2) >> 8);
		blk[7] = (short)((X7 - X1) >> 8);
	}       /* end for ( i = 0; i < 8; ++i ) IDCT-rows */
	for (i = 0; i < 8; i++)  /* idct columns */
	{
		blk = block + i;
		/* shortcut  */
		if (!
			((X1 = (blk[8 * 4] << 8)) | (X2 = blk[8 * 6]) | (X3 = blk[8 * 2]) | (X4 = blk[8 * 1])
			| (X5 = blk[8 * 7]) | (X6 = blk[8 * 5]) | (X7 = blk[8 * 3])))
		{
			blk[8 * 0] = blk[8 * 1] = blk[8 * 2] = blk[8 * 3] = blk[8 * 4] =
				blk[8 * 5] = blk[8 * 6] = blk[8 * 7] = iclp[(blk[8 * 0] + 32) >> 6];
			continue;
		}
		X0 = (blk[8 * 0] << 8) + 8192;
		/* first stage  */
		X8 = W7 * (X4 + X5) + 4;
		X4 = (X8 + (W1 - W7) * X4) >> 3;
		X5 = (X8 - (W1 + W7) * X5) >> 3;
		X8 = W3 * (X6 + X7) + 4;
		X6 = (X8 - (W3 - W5) * X6) >> 3;
		X7 = (X8 - (W3 + W5) * X7) >> 3;
		/* second stage  */
		X8 = X0 + X1;
		X0 -= X1;
		X1 = W6 * (X3 + X2) + 4;
		X2 = (X1 - (W2 + W6) * X2) >> 3;
		X3 = (X1 + (W2 - W6) * X3) >> 3;
		X1 = X4 + X6;
		X4 -= X6;
		X6 = X5 + X7;
		X5 -= X7;
		/* third stage  */
		X7 = X8 + X3;
		X8 -= X3;
		X3 = X0 + X2;
		X0 -= X2;
		X2 = (181 * (X4 + X5) + 128) >> 8;
		X4 = (181 * (X4 - X5) + 128) >> 8;
		/* fourth stage  */
		blk[8 * 0] = iclp[(X7 + X1) >> 14];
		blk[8 * 1] = iclp[(X3 + X2) >> 14];
		blk[8 * 2] = iclp[(X0 + X4) >> 14];
		blk[8 * 3] = iclp[(X8 + X6) >> 14];
		blk[8 * 4] = iclp[(X8 - X6) >> 14];
		blk[8 * 5] = iclp[(X0 - X4) >> 14];
		blk[8 * 6] = iclp[(X3 - X2) >> 14];
		blk[8 * 7] = iclp[(X7 - X1) >> 14];
	}
}

void fidct_init()
{
	int i;
	iclp = iclip + 512;
	for (i = -512; i < 512; i++)
		iclp[i] = (i < -256) ? -256 : ((i > 255) ? 255 : i);
}


#endif // !FIDCT_H
