/**
 *  Class to save to and restore from EEPROM a data structure containing a configuration 
 *  that needs to be persisted across resets and power off
 *  Xavier Grosjean 2017
 *  MIT License  
 */
#pragma once
#include <EEPROM.h>
#include <Arduino.h>

//#define DEBUG_XEEPROM // Uncomment this to enable debug messages over serial port

#ifdef DEBUG_XEEPROM
#define Debug(...) Serial.printf(__VA_ARGS__)
#else
#define Debug(...)
#endif

// IMPORTANT WARNING: This structure (and its extensions) needs to have a fixed size:
// no 'String', for instance
// It will be serialialized to / deserialized from EEPROM, byte by byte
// First field needs to be an unsigned int to store the config version.
struct XEEPROMConfigDataStruct {
  unsigned int version;
}; 
  
class XEEPROMConfigClass {
public:
  XEEPROMConfigClass(unsigned int version, unsigned int dataSize);
  
  void init(void);
  void initFromEeprom(void);
  void saveToEeprom(void);
  void saveToEeprom(byte* to);
  void saveToEeprom(byte* from, byte* to);
  unsigned int getVersion(void);
  void setVersion(unsigned int);
  unsigned int getDataSize(void);
  virtual void initFromDefault(void);
  
protected:
  byte *_data;
  XEEPROMConfigDataStruct* _getDataPtr(void);
  unsigned int _version;
  unsigned int _dataSize;
};

