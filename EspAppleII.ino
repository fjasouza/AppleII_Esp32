/*****************************************************************************

    File: "EspAppleII.ino"
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

#define DEBUG_PROG
#define SHOWTASKCORE

// Modo de video (definir apenas um) 
#define VGA_400x300_60Hz        // Tela full
//#define VGA_320x200_70Hz      // Tela menor

// Define o disco de boot(definir apenas um)
#define DOS_33
//#define RESCUE_RAIDERS
//#define GALAXIAN                        // Obs: demora alguns segundos para carregar
//#define CHOPLIFTER
//#define CANNONBALL_BLITZ                // Obs: demora quase 1 minuto para carregar
//#define CASTLE_WOLFENSTEIN                // Obs: demora quase 2 minutos e 20 segundos para carregar
//#define LOADRUNNER                      // Obs: não funcionou, belo desafio pra descobrir o que tem de diferente (eu aposto em bancos de memória que não foram implementados nesse emulador)

#include <Preferences.h>
#include "Timer.h"                    // https://github.com/JChristensen/Timer
#include "fabgl.h"
#include "fabui.h"
#include "common.h"

fabgl::PS2Controller    PS2Controller;          // Controlador de teclado PS2
fabgl::VGA16Controller  VGAController;          // Controlador de video VGA
fabgl::Canvas           canvas(&VGAController);

#define FATOR_TIMER_FLASH 250

#ifdef DOS_33
Timer timerFlash;                               //instantiate the timer object
#endif

/* videomodes */
#define modetext40	  0
#define modelres40  	1
#define modehres	    2

static unsigned char inverse;
static unsigned int virtmodedown;
static unsigned int virttextpage;       /* 0x0400 or 0x0800 */
static unsigned int virthrespage;       /* 0x2000 or 0x4000 */
static unsigned int virtsplit;

static unsigned char flashChar = 0xFF;

TaskHandle_t Task1;
TaskHandle_t Task2;

bool  flagPage1 = true;
Preferences   preferences;

/* virtual screen buffer */
unsigned char RAM[0xC000];
unsigned char RAMEXT[0x4000];
unsigned char RAM_TXT_BACK[0x400];
unsigned char RAM_HGR_BACK[0x2000];

#ifdef DEBUG_PROG
#define BAUD_RATE  115200
#define SERIAL_SIZE_RX  32    // used in Serial.setRxBufferSize()
#define SERIAL_SIZE_TX  32    // used in Serial.setTxBufferSize()
#endif

#ifdef DOS_33
/***************************************************************************************************************************************/

/***************************************************************************************************************************************/
void flashCaracter()
{
  flashChar ^= 0xFF;
}
#endif

#ifdef SHOWTASKCORE
/***************************************************************************************************************************************/
// showTaskCore
/***************************************************************************************************************************************/
void showTaskCore ()  {
  DEBUG_PRINT("Task running on core ");
  DEBUG_PRINTLN(xPortGetCoreID());

  setCpuFrequencyMhz (240);
  uint32_t Freq = getCpuFrequencyMhz();
  DEBUG_PRINT("CPU Freq = ");
  DEBUG_PRINT(Freq);
  DEBUG_PRINTLN(" MHz");
  Freq = getXtalFrequencyMhz();
  DEBUG_PRINT("XTAL Freq = ");

  DEBUG_PRINT(Freq);
  DEBUG_PRINTLN(" MHz");
  Freq = getApbFrequency();
  DEBUG_PRINT("APB Freq = ");
  DEBUG_PRINT(Freq);
  DEBUG_PRINTLN(" Hz");
}
#endif

/***************************************************************************************************************************************/
//initTasks
/***************************************************************************************************************************************/
void initTasks() {

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    2,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
  delay(500);
}

/***************************************************************************************************************************************/
// Task1code - 6502 Runtime
/***************************************************************************************************************************************/
void Task1code( void * pvParameters ){
  
#ifdef SHOWTASKCORE
  showTaskCore();
#endif

  initCode();  
  
  // Loop Task1
  for(;;) {
    execCode();
    // caso não seja disco de sistema operacional, acrescentar 2 microSegundos para delay de tela em jogos
#ifndef DOS_33    
    delayMicroseconds(2);
#endif    
  } 
}

/***************************************************************************************************************************************/
//Task2code - Video AppleII
/***************************************************************************************************************************************/
void Task2code( void * pvParameters ){
  auto keyboard = PS2Controller.keyboard();

#ifdef SHOWTASKCORE
  showTaskCore ();
#endif

  // Loop Task2
  for(;;) {

#ifdef DOS_33
    timerFlash.update();
#endif
//    if (cycles >= lastCycles) {
//		  lastCycles = lastCycles + lineCycles;
      // Refresh Imagem
      for (int rasterline = 0; rasterline<24; rasterline++)  
	      virtline(rasterline);
//	  }
    // Faz leitura do teclado
    if (keyboard->scancodeAvailable()) {
        keyboard_In (keyboard->getNextScancode());
    }
  }
}

/***************************************************************************************************************************************/
// Setup
/***************************************************************************************************************************************/
void setup()
{  
#ifdef DEBUG_PROG
  Serial.begin(BAUD_RATE);
  Serial.setRxBufferSize(SERIAL_SIZE_RX);
  Serial.setTxBufferSize(SERIAL_SIZE_TX);
  delay(1000); 
#endif
  
  DEBUG_PRINTLN( "EspApple II Emulator (ESP32) ");

  // Desabilita WatchDog dos dois Cores
  disableCore0WDT();
  delay(100); // experienced crashes without this delay!
  disableCore1WDT();
 
  VGAController.queueSize = 256;  // trade UI speed using less RAM and allow both WiFi
  VGAController.begin();
  
  #ifdef VGA_400x300_60Hz
  VGAController.setResolution(VGA_400x300_60Hz);
  #endif

  #ifdef VGA_320x200_70Hz
  VGAController.setResolution(VGA_320x200_70Hz);
  #endif

  // this speed-up display but may generate flickering
	VGAController.enableBackgroundPrimitiveExecution(false);
  VGAController.enableBackgroundPrimitiveTimeout(false);

  canvas.setBrushColor(Color::Black);
  canvas.clear();

  canvas.selectFont(&fabgl::FONT_8x8);
 
  canvas.setGlyphOptions(GlyphOptions().FillBackground(true));
  
  // Inicia teclado PS2
  PS2Controller.begin(PS2Preset::KeyboardPort0, KbdMode::NoVirtualKeys);

  // clear preferences
  preferences.clear();

  preferences.begin("EspAppleII", false);

  // Tela Splash
  canvas.setBrushColor(Color::Black);
  canvas.setPenColor(Color::BrightYellow );
  canvas.drawText(20, 25, " Apple II - ESP32 FABGL (2023) ");

  DEBUG_PRINTF("free heap:%.1fkb", (float)esp_get_free_heap_size() / 1024.0);
  DEBUG_PRINTLN();
	
  delay (2000);

#ifdef DOS_33
  // Instala timer de contagem de segundos
  timerFlash.every(FATOR_TIMER_FLASH, flashCaracter);
#endif

  speaker_begin();

  sei();

  initTasks();
}

/***************************************************************************************************************************************/
// Loop
/***************************************************************************************************************************************/
void loop()
{
}
