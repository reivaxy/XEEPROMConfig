/**
 *  Class to save to and restore from EEPROM a data structure containing a configuration 
 *  that needs to be persisted across resets and power off
 *  Xavier Grosjean 2017
 *  MIT License 
 *
 */
 
#include <Arduino.h>
#include "XEEPROMConfig.h"

XEEPROMConfigClass::XEEPROMConfigClass(unsigned int version, const char* name, unsigned int dataSize) {
  Debug("XEEPROMConfigClass::XEEPROMConfigClass %d, %s, %d\n", version, name, dataSize);
  _version = version;
  _dataSize = dataSize;
  _data = (byte*)malloc(dataSize);
  // Copy name making sure there is no overflow
  strncpy(_name, name, NAME_MAX_LENGTH);
  _name[NAME_MAX_LENGTH] = 0;
}

/**
 * Initialize the data structure from the EEPROM content.
 * If the version is not the one expected (EEPROM content is obsolete, or was never initialized), initialize the data object
 * from the default values, then save it to EEPROM
 *
 */
void XEEPROMConfigClass::init(void) {
  Debug("XEEPROMConfigClass::init\n");
  initFromEeprom();
  if (_version != getVersion()) {
    Serial.println("EEprom not up to date");
    initFromDefault();
    saveToEeprom();    
  } else {
    Serial.println("EEprom is up to date.");
  }
} 

/**
 * Read EEPROM byte by byte to initialize the data structure 
 *
 */
void XEEPROMConfigClass::initFromEeprom(void) {
  Debug("XEEPROMConfigClass::initFromEeprom\n");
  unsigned int sizeConfig = getDataSize();
  uint8_t* ptrConfig = (uint8_t *)_getDataPtr();
  int i;
  EEPROM.begin(sizeConfig);
  for(i = 0; i < sizeConfig; i++) {
    *(ptrConfig ++) = EEPROM.read(i);
  }
}

/**
 * Save the data structure to EEPROM byte by byte  
 *
 */
void XEEPROMConfigClass::saveToEeprom(void) {
  Debug("XEEPROMConfigClass::saveToEeprom\n");
  unsigned int sizeConfig = getDataSize();
  byte* ptrConfig = (byte *)_getDataPtr();
  for(int i = 0; i < sizeConfig; i++) {
    EEPROM.write(i, *ptrConfig++);
  }
  EEPROM.commit();
}

/**
 * Set the version in the data structure
 *
 */
void XEEPROMConfigClass::setVersion(unsigned int version) {
  Debug("XEEPROMConfigClass::setVersion\n");
  // Version is an unsigned int at the begining of the data structure
  *(unsigned int *)_getDataPtr() = version;
}

/**
 * Get version from the data structure
 *
 */
unsigned int XEEPROMConfigClass::getVersion(void) {
  // Version is an unsigned int at the begining of the data structure
  unsigned int* versionPtr = (unsigned int *)_getDataPtr();
  Debug("XEEPROMConfigClass::getVersion: %d\n", *versionPtr);
  return *versionPtr;
}

/**
 * Set the name in the data structure
 *
 */
void XEEPROMConfigClass::setName(const char* name) {
  Debug("XEEPROMConfigClass::setName\n");
  char* namePtr = (char*)_getDataPtr() + sizeof(_version);   // is there any padding ?
  strncpy(namePtr, name, NAME_MAX_LENGTH);
  *(namePtr + NAME_MAX_LENGTH) = 0;
}

/**
 * Get the name from the data structure
 *
 */
char* XEEPROMConfigClass::getName(void) {
  Debug("XEEPROMConfigClass::getName\n");
  char* namePtr = (char*)_getDataPtr() + sizeof(_version);
  return namePtr;
}

/**
 * Get the data structure size (it was provided at init)
 *
 */
unsigned int XEEPROMConfigClass::getDataSize(void) {
  Debug("XEEPROMConfigClass::getDataSize: %d\n", _dataSize);
  return _dataSize;
}

/**
 * Init the data structure from the default values.
 * Subclass need to take care of initializing fields other than version and name
 *
 */
void XEEPROMConfigClass::initFromDefault(void) {
  Debug("XEEPROMConfigClass::initFromDefault\n");
  setVersion(_version);
  setName(_name);
}

/**
 * Return the pointer to the data structure object
 *
 */
XEEPROMConfigDataStruct* XEEPROMConfigClass::_getDataPtr(void) {
  Debug("XEEPROMConfigClass::_getDataPtr\n");
  return (XEEPROMConfigDataStruct*)_data;
}