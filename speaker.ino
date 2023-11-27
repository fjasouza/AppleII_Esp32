/*****************************************************************************

    File: "speaker.ino"
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

#define SPEAKER_PIN 25

/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void speaker_begin() {
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN, LOW);
}

/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void speaker_toggle() {
  static boolean state = false;
  state = (state)?false:true;
  digitalWrite(SPEAKER_PIN, state);
}
