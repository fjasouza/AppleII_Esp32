/*****************************************************************************

    File: "video.h"
    Date:  20/07/2023
    Copyright (C) 2023, Francisco J A Souza

    This file is part of EspAppleII Project.

    EspAppleII is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    EspAppleII is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*****************************************************************************/

//---------------------------------------------------------------------------

#ifndef Unit_VideoH
#define Unit_VideoH
//---------------------------------------------------------------------------

typedef enum {USA, France, Germany, UK, Denmark1, Sweden, Italy, Spain, Japan, Norway, Denmark2} Charset;

/* BINMANIP */
int bsave(char *filename);
int bload(char *filename);

/* Graphics flags */
#define HRG 1 /* 0001 */
#define PG2 2 /* 0010 */
#define GRX 4 /* 0100 */
#define SPL 8 /* 1000 */

#define T40 1
#define T80 3
#define G1 0x4
#define G2 0x6
#define G7 0xD
#define G8 0xE

/* VIDEO */
void virtinit ();
void virtline (unsigned int rastline);
void opengraph(void);

#ifdef SAFE
#define GetAttrib(x) 0
#endif

/* Color definitions */

/* Lo-res graphics */
#define COL_LGR0 0x0
#define COL_LGR1 0x1
#define COL_LGR2 0x2
#define COL_LGR3 0x3
#define COL_LGR4 0x4
#define COL_LGR5 0x5
#define COL_LGR6 0x6
#define COL_LGR7 0x7
#define COL_LGR8 0x8
#define COL_LGR9 0x9
#define COL_LGRA 0xA
#define COL_LGRB 0xB
#define COL_LGRC 0xC
#define COL_LGRD 0xD
#define COL_LGRE 0xE
#define COL_LGRF 0xF

#define COL_TXT_WHT0 0x18
#define COL_TXT_WHT1 0x19
#define COL_TXT_WHT2 0x1A
#define COL_TXT_WHT3 0x1B

#define COL_TXT_GRN0 0x1C
#define COL_TXT_GRN1 0x1D
#define COL_TXT_GRN2 0x1E
#define COL_TXT_GRN3 0x1F

#define COL_TXT_AMB0 0x20
#define COL_TXT_AMB1 0x21
#define COL_TXT_AMB2 0x22
#define COL_TXT_AMB3 0x23

#define COL_DRV_OFF  0x24
#define COL_DRV_ON   0x25

#define COL_LPT_OFF  0x26
#define COL_LPT_ON   0x27

#define COL_DASM     0x28
#define COL_FLAG     0x29
#define COL_DASM80   0x2A
#define COL_FLAG80   0x2B

#define COL_HDV_OFF  0x2C
#define COL_HDV_ON   0x2D

#define COL_DISKFLAG 0x2E
#define COL_LPTERR   0x2F

#endif