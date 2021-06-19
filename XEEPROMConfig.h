/**
 *  Class to save to and restore from EEPROM a data structure containing a configuration 
 *  that needs to be persisted across resets and power off
 *  Xavier Grosjean 2017
 *  MIT License  
 */
#pragma once
#include <EEPROM.h>
#include <Arduino.h>

#define TYPE_MAX_LENGTH 20

//#define DEBUG_XEEPROM // Uncomment this to enable debug messages over serial port


#ifdef DEBUG_XEEPROM
#define Debug(...) Serial.printf(__VA_ARGS__)
#else
#define Debug(...)
#endif

// IMPORTANT WARNING: This structure needs to have a fixed size: no 'String', for instance
// It will be serialialized to / deserialized from EEPROM.
// First field needs to be an unsigned int to store the config version.
// Second field needs to be char array to store a type that will not change
struct XEEPROMConfigDataStruct {
  unsigned int version;
  char type[TYPE_MAX_LENGTH + 1];
}; 
  
class XEEPROMConfigClass {
public:
  XEEPROMConfigClass(unsigned int version, const char* type, unsigned int dataSize);
  
  void init(void);
  void initFromEeprom(void);
  void saveToEeprom(void);
  char* getType(void);
  void setType(const char*);
  void saveToEeprom(unsigned char* to);
  void saveToEeprom(unsigned char* from, unsigned char* to); 
  unsigned int getVersion(void);
  void setVersion(unsigned int);
  unsigned int getDataSize(void);
  virtual void initFromDefault(void);
  
protected:
  unsigned char *_data;
  XEEPROMConfigDataStruct* _getDataPtr(void);
  unsigned int _version;
  char _type[TYPE_MAX_LENGTH + 1];
  unsigned int _dataSize;
};

