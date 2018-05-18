/**
 *  Class to save to and restore from EEPROM a data structure containing a configuration 
 *  that needs to be persisted across resets and power off
 *  Xavier Grosjean 2017
 *  MIT License 
 *
 */
 
#include <Arduino.h>
#include "XEEPROMConfig.h"

XEEPROMConfigClass::XEEPROMConfigClass(unsigned int version, unsigned int dataSize) {
  Debug("XEEPROMConfigClass::XEEPROMConfigClass %d, %d\n", version, dataSize);
  _version = version;
  _dataSize = dataSize;
  _data = (byte*)malloc(dataSize);
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
  EEPROM.begin(sizeConfig);
  for(unsigned int i = 0; i < sizeConfig; i++) {
    *(ptrConfig ++) = EEPROM.read(i);
  }
}

/**
 * Save the whole data structure to EEPROM byte by byte  
 *
 */
void XEEPROMConfigClass::saveToEeprom(void) {
  Debug("XEEPROMConfigClass::saveToEeprom\n");
  unsigned int sizeConfig = getDataSize();
  byte* ptrConfig = (byte *)_getDataPtr();
  for(unsigned int i = 0; i < sizeConfig; i++) {
    EEPROM.write(i, *ptrConfig++);
  }
  EEPROM.commit();
}

/**
 * Save data structure to EEPROM byte by byte up to given address  
 *
 */
void XEEPROMConfigClass::saveToEeprom(byte* to) {
  int count;
  Debug("XEEPROMConfigClass::saveToEeprom\n");
  unsigned int sizeConfig = getDataSize();
  byte* ptrConfig = (byte *)_getDataPtr();
  count = min(sizeConfig, to - ptrConfig);
  for(int i = 0; i < sizeConfig; i++) {
    EEPROM.write(i, *ptrConfig++);
  }
  EEPROM.commit();
}

/**
 * Save data structure to EEPROM byte by byte 
 * from given address up to given address  
 *
 */
void XEEPROMConfigClass::saveToEeprom(byte* from, byte* to) {
  Serial.println("XEEPROMConfigClass::saveToEeprom(from, to) is not implemented yet.");
// TODO : implement !!
//  int count;
//  Debug("XEEPROMConfigClass::saveToEeprom\n");
//  unsigned int sizeConfig = getDataSize();
//  byte* ptrConfig = (byte *)_getDataPtr();
//  count = min(sizeConfig, to - ptrConfig);
//  for(int i = 0; i < sizeConfig; i++) {
//    EEPROM.write(i, *ptrConfig++);
//  }
//  EEPROM.commit();
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
}

/**
 * Return the pointer to the data structure object
 *
 */
XEEPROMConfigDataStruct* XEEPROMConfigClass::_getDataPtr(void) {
  Debug("XEEPROMConfigClass::_getDataPtr\n");
  return (XEEPROMConfigDataStruct*)_data;
}