/**
 *  Class to save to and restore from EEPROM a data structure containing a configuration 
 *  that needs to be persisted across resets and power off
 *  Xavier Grosjean 2017
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License 
 */
#pragma once
#include <EEPROM.h>

#define NAME_MAX_LENGTH 20


// This structure needs to have a fixed size: no String, for instance
// First field needs to be an unsigned int to store the version.
// Second field needs to be char array to store a name
// All subclasses need to respect this.
typedef struct {
  unsigned int version;
  char name[NAME_MAX_LENGTH];
} configData; 
 
  
class EEPROMConfigClass {
public:
  EEPROMConfigClass(unsigned int version, const char* name, void* data, unsigned int dataSize);
  EEPROMConfigClass(unsigned int version, const char* name);
  
  void init(void);
  void initFromEeprom(void);
  void saveToEeprom(void);
  char* getName(void);
  void setName(const char*);
  unsigned int getVersion(void);
  void setVersion(unsigned int);
  void* getData(void);
  void setData(void*);
  unsigned int getDataSize(void);
  virtual void initFromDefault(void);
  
protected:
  void* _data;
  unsigned int _version;
  char _name[NAME_MAX_LENGTH];
  unsigned int _dataSize;
};

