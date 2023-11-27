/*****************************************************************************

    File: "common.h"
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

#ifdef DEBUG_PROG  //cool way to have debug serial ONLY when you want it, and replaced with effectively no statement when not needed. Supports basic formatting of vars.
  #define DEBUG_PRINTF(x,...) Serial.printf(x, ##__VA_ARGS__)
  #define DEBUG_PRINT(x,...) Serial.print(x, ##__VA_ARGS__)
  #define DEBUG_PRINTLN(x,...) Serial.println(x, ##__VA_ARGS__)
#else
  #define DEBUG_PRINTF(x,...)
  #define DEBUG_PRINT(x,...)
  #define DEBUG_PRINTLN(x,...)
#endif