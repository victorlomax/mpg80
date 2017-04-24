/*
Example for MCP3008 - Library for communicating with MCP3008 Analog to digital converter.
Created by Uros Petrevski, Nodesign.net 2013
Released into the public domain.
*/

#ifndef MCP3008_h
#define MCP3008_h

#ifndef _H_ARDUINO
#include <arduino.h>
#endif

class MCP3008
{
  private:
    int _clockpin, _mosipin, _misopin, _cspin;
  public:
    MCP3008(int clockpin, int mosipin, int misopin, int cspin);
    int readADC(int adcnum);
}
#endif
