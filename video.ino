/*****************************************************************************

    File: "video.ino"
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

#include "video.h"
#include "fabgl.h"
#include "font.h"

int gm;
static  Color COL_HGR0  = Color::Black;
static  Color COL_HGR1  = Color::BrightGreen;
static  Color COL_HGR2  = Color::BrightMagenta;
static  Color COL_HGR3  = Color::BrightWhite;
static  Color COL_HGR4  = Color::Black;
static  Color COL_HGR5  = Color::BrightYellow;
static  Color COL_HGR6  = Color::BrightBlue;
static  Color COL_HGR7  = Color::BrightWhite;

/* even columns */
static Color colourtabe[8]  = {COL_HGR0, COL_HGR0, COL_HGR2, COL_HGR3, COL_HGR0, COL_HGR1, COL_HGR3, COL_HGR3};

/* odd columns */
static Color colourtabo[8]  = {COL_HGR0, COL_HGR0, COL_HGR1, COL_HGR3, COL_HGR0, COL_HGR2, COL_HGR3, COL_HGR3};

/* even columns */
static Color colourtabe2[8] = {COL_HGR4, COL_HGR4, COL_HGR6, COL_HGR7, COL_HGR4, COL_HGR5, COL_HGR7, COL_HGR7};

/* odd columns */
static Color colourtabo2[8] = {COL_HGR4, COL_HGR4, COL_HGR5, COL_HGR7, COL_HGR4, COL_HGR6, COL_HGR7, COL_HGR7};

// Low-Resolution Graphics
//
//  0	Black	      #000000	  0,   0,   0
//  1	Magenta	    #930B7C	147,  11, 124
//  2	Dark Blue	  #1F35D3	 31,  53, 211
//  3	Purple	    #BB36FF	187,  54, 255
//  4	Dark Green	#00760C	  0, 118,  12
//  5	Grey 1	    #7E7E7E	126, 126, 126
//  6	Medium Blue	#07A8E0	  7, 168, 224
//  7	Light Blue	#9DACFF	157, 172, 255
//  8	Brown	      #624C00	 98,  76,   0
//  9	Orange	    #F9561D	249,  86,  29
// 10	Grey 2	    #7E7E7E	126, 126, 126
// 11	Pink	      #FF81EC	255, 129, 236
// 12	Light Green	#43C800	 67, 200,   0
// 13	Yellow	    #DCCD16	220, 205,  22
// 14	Aqua	      #5DF784	 93, 247, 132
// 15	White	      #FFFFFF	255, 255, 255

static unsigned int textAddr[24] = {
		    0x0000,	0x0080,	0x0100,	0x0180,	0x0200,	0x0280,	0x0300,	0x0380,
		    0x0028,	0x00a8,	0x0128,	0x01a8,	0x0228,	0x02a8,	0x0328,	0x03a8,
		    0x0050,	0x00d0,	0x0150,	0x01d0,	0x0250,	0x02d0,	0x0350,	0x03d0,
};

// High-Resolution Graphics
//
// 0	Black 1	#000000	  0,   0,   0
// 1	Green	  #68E043	101, 226,  67
// 2	Purple	#D660EF	214,  96, 239
// 3	White 1	#FFFFFF	255, 255, 255
// 4	Black 2	#000000	  0,   0,   0
// 5	Orange	#E6792E	226, 114,  43
// 6	Blue	  #4BB8F1	 75, 184, 241
// 7	White 2	#FFFFFF	255, 255, 255

static unsigned int hreslineaddr[8] = {
        0x0000, 0x0400, 0x0800, 0x0c00, 0x1000, 0x1400, 0x1800, 0x1c00
};        

static unsigned int hresaddr[24] = {
        0x0000, 0x0080, 0x0100, 0x0180, 0x0200, 0x0280, 0x0300, 0x0380,
        0x0028, 0x00a8, 0x0128, 0x01a8, 0x0228, 0x02a8, 0x0328, 0x03a8,
        0x0050, 0x00d0, 0x0150, 0x01d0, 0x0250, 0x02d0, 0x0350, 0x03d0
};

static unsigned char dhrescol[16] = {
        COL_LGR0, COL_LGR2, COL_LGR4, COL_LGR6,
        COL_LGR8, COL_LGRA, COL_LGRC, COL_LGRE,
        COL_LGR1, COL_LGR3, COL_LGR5, COL_LGR7,
        COL_LGR9, COL_LGRB, COL_LGRD, COL_LGRF
};

/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void virtreset() {

  gm = 0;
  virtsetmode();
} /* virtreset */

/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void virtinit() {
  virtreset(); /* reset softswitches */
  virtsetmode(); /* set mode and page for rasterline */
} /* virtinit */

/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void virtline(unsigned int rastline)
{
	int x, y;
	unsigned int addr;
	unsigned int val, val1, valBits;
	unsigned char bit, bytes;

	switch (virtmodedown)
	{
		case modetext40:
			/*text 40 column */
			addr = textAddr[rastline] + virttextpage;
			for (bytes = 0; bytes < 40; bytes++)
			{
				val = RAM[addr];
				if (val != RAM_TXT_BACK[addr - virttextpage] || (val & 0xC0) == 0x40)
				{ 
					RAM_TXT_BACK[addr - virttextpage] = val;
					// imprime oito linhas do caracter
					for (int line = 0; line < 8; line++)
					{
						if (val >= 128)   
						{
              // Caracteres normais
							valBits = AppleFont[(((val + (val - 128<' ' ? 64 : 0)) & 0x7f) << 3) | line];
						}
						else
						{
							if (val >= 64) {
                // Caracteres normais
                val1 = val - 0x40;  inverse = flashChar;
              }
							else
							//if (val < 64) 
              {
                // Caracteres inverse
                val1 = val + 0x40; inverse = 0xFF;
              } 
							valBits = AppleFont[(val1 << 3) | line] ^ inverse;
						}

						// imprime sete pixels do caracter
						x = bytes * 7;
						y = (rastline *8) + line;
						for (bit = 128; bit > 1; bit = bit >> 1)
						{
							if (valBits & bit)
                canvas.setPenColor(COL_HGR7); // White
							else
								canvas.setPenColor(COL_HGR0); // Black
							canvas.setPixel(x, y);
							x++; /*next pixel */
						}	// for bit 
					}	// for line
				}	// for bytes
				addr++;
			}
			return;

		case modelres40:
			/*lores */
			addr = textAddr[rastline] + virttextpage;
			for (bytes = 0; bytes < 40; bytes++)
			{
			  val = RAM[addr];
					// imprime oito linhas do caracter
				for (int line = 0; line < 8; line++)
				{
					if (line < 4)
						val1 = (val & 0xf) + COL_LGR0;
					else
						val1 = (val >> 4) + COL_LGR0;
					// imprime sete pixels do caracter
					x = bytes * 7;
					y = (rastline *8) + line;
					for (bit = 0; bit < 7; bit++)
					{
						if (val1 != 0)
							canvas.setPenColor(255, 255, 255);
						else
							canvas.setPenColor(0, 0, 0);
						canvas.setPixel(x, y);
						x++; /*next pixel */
					}	// for bit 
				}	// for line
				addr++;
			}
			return;			

		case modehres:
			/*hires */
			for (int line = 0; line < 8; line++)
			{
				addr = hresaddr[rastline] + hreslineaddr[line] + virthrespage;
				for (bytes = 0; bytes < 40; bytes++)
				{
					x = bytes * 7;
					y = (rastline *8) + line;
					val = (((unsigned char) RAM[addr - 1] & 0x60) >> 5) |
						    (((unsigned char) RAM[addr] & 0x7f) << 2) |
						    (((unsigned char) RAM[addr + 1] & 0x3) << 9);
					if ((bytes & 1) == 0)
					{ /*even column */
						if (bytes == 0)
						{
							val &= 0x7fc; /*drop surrounding bits at left border */
						}

						if (RAM[addr] & 0x80)
						{
							canvas.setPenColor(colourtabe2[(val >> 1) & 7]);	// colourtabe2
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo2[(val >> 2) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe2[(val >> 3) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo2[(val >> 4) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe2[(val >> 5) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo2[(val >> 6) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe2[(val >> 7) & 7]);
							canvas.setPixel(x++, y);
						}
						else
						{
							canvas.setPenColor(colourtabe[(val >> 1) & 7]);	// colourtabe
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo[(val >> 2) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe[(val >> 3) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo[(val >> 4) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe[(val >> 5) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo[(val >> 6) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe[(val >> 7) & 7]);
							canvas.setPixel(x++, y);
						}
					}
					else
					{ /*odd column */
						if (bytes == 39)
						{
							val &= 0x1ff; /*drop surrounding bits at right border */
						}

						if (RAM[addr] & 0x80)
						{
							canvas.setPenColor(colourtabo2[(val >> 1) & 7]);	// colourtabo2
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe2[(val >> 2) & 7]);	// colourtabe2
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo2[(val >> 3) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe2[(val >> 4) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo2[(val >> 5) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe2[(val >> 6) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo2[(val >> 7) & 7]);
							canvas.setPixel(x++, y);
						}
						else
						{
							canvas.setPenColor(colourtabo[(val >> 1) & 7]);	// colourtabo
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe[(val >> 2) & 7]);	// colourtabe
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo[(val >> 3) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe[(val >> 4) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo[(val >> 5) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabe[(val >> 6) & 7]);
							canvas.setPixel(x++, y);
							canvas.setPenColor(colourtabo[(val >> 7) & 7]);
							canvas.setPixel(x++, y);
						}
					}
					addr++;
				} /*for bytes */
			} /*for lines */
			return;
	} /*switch */
} /*virtline */ 

/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void CheckVideoIO(word Address) {
  switch (Address) {
    //  CLRTEXT =  $C050 ;display graphics 
    case 0xc050 : {
      if (!(gm&GRX)) {
        gm|=GRX;
        virtsetmode();
      }
      return;
    }
    //  SETTEXT =  $C051 ;display text
    case 0xc051 : { 
      if (gm&GRX) {
        gm&=~GRX;
        virtsetmode();
      }
      return;
    }
    //  CLRMIXED = $C052 ;clear mixed mode- enable full graphics 
    case 0xc052 : { 
      if (gm&SPL) {
        gm&=~SPL;
        virtsetmode();
      }
      return;
    }
    //  SETMIXED = $C053 ;enable graphics/text mixed mode
    case 0xc053 : { 
      if (!(gm&SPL)) {
        gm|=SPL;
        virtsetmode();
      }
      return;
    }
    //  PAGE1 =    $C054 ;select text/graphics page1 
    case 0xc054 : { 
      if (gm&PG2) {
        gm &= ~PG2;
        virtsetmode();
      }
      return;
    }
    //  PAGE2 =    $C055 ;select text/graphics page2
    case 0xc055 : { 
      if (!(gm&PG2)) {
        gm |= PG2;
        virtsetmode();
      }
      return;
    }
    //  CLRHIRES = $C056 ;select Lo-res 
    case 0xc056 : { 
      if (gm&HRG) {
         gm&=~HRG;
         virtsetmode();
      }
      return;
    }
    //  SETHIRES = $C057 ;select Hi-res 
    case 0xc057 : { 
      if (!(gm&HRG)) {
         gm|=HRG;
         virtsetmode();
      }
      return;
    }
 }
}

/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void virtsetmode() {

  if (gm & GRX) {
    /* set the display modes for both parts of the screen */
    if (gm & HRG) {
        if (gm & SPL) {
          virtmodedown = modetext40;
          virtsplit = 160;
        } else {
          virtmodedown = modehres;
          virtsplit = 192;
        }
    } else {
      virtsplit = 0;
      if (gm & SPL) {
        virtmodedown = modetext40;
      } else {
        virtmodedown = modelres40;
      }
    }
  } else {
    virtsplit = 0;
    virtmodedown = modetext40;
  }

  if (!(gm & PG2)) {
    /* set the visible page */
    flagPage1 = true;
    virttextpage = 0x400;
    virthrespage = 0x2000;
  } else {
    flagPage1 = false;
    virttextpage = 0x800;
    virthrespage = 0x4000;
  }


} /* virtsetmode */