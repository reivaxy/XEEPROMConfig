/**
 *  Class to save to and restore from EEPROM a data structure containing a configuration 
 *  that needs to be persisted across resets and power off
 *  Xavier Grosjean 2017
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 *
 */
 
#include <Arduino.h>
#include "EEPROMConfig.h"

EEPROMConfigClass::EEPROMConfigClass(unsigned int version, const char* name) {
  //Serial.println("EEPROMConfigClass::EEPROMConfigClass");
  configData data;  // Default data object, made of a version number and a name.
  EEPROMConfigClass(version, name, (void*)&data, sizeof(data));
}

EEPROMConfigClass::EEPROMConfigClass(unsigned int version, const char* name, void* data, unsigned int dataSize) {
  //Serial.println("EEPROMConfigClass::EEPROMConfigClass");
  _version = version;
  // Copy name making sure there is no overflow
  strncpy(_name, name, NAME_MAX_LENGTH);
  _name[NAME_MAX_LENGTH] = 0;
  _data = data;
  _dataSize = dataSize;
}

/**
 * Initialize the data structure from the EEPROM content.
 * If the version is not the one expected (EEPROM content is obsolete, or was never initialized), initialize the data object
 * from the default values, then save it to EEPROM
 *
 */
void EEPROMConfigClass::init(void) {
  //Serial.println("EEPROMConfigClass::init");
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
void EEPROMConfigClass::initFromEeprom(void) {
  //Serial.println("EEPROMConfigClass::initFromEeprom");
  unsigned int sizeConfig = getDataSize();
  uint8_t* ptrConfig = (uint8_t *)_data;
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
void EEPROMConfigClass::saveToEeprom(void) {
  //Serial.println("EEPROMConfigClass::saveToEeprom");
  unsigned int sizeConfig = getDataSize();
  byte* ptrConfig = (byte *)_data;
  for(int i = 0; i < sizeConfig; i++) {
    EEPROM.write(i, *ptrConfig++);
  }
  EEPROM.commit();
}

/**
 * Set the version in the data structure
 *
 */
void EEPROMConfigClass::setVersion(unsigned int version) {
  //Serial.println("EEPROMConfigClass::setVersion");
  // Version is an unsigned it at the begining of the data structure
  *(unsigned int *)_data = version;
}

/**
 * Get version from the data structure
 *
 */
unsigned int EEPROMConfigClass::getVersion(void) {
  //Serial.println("EEPROMConfigClass::getVersion");
  // Version is an unsigned it at the begining of the data structure
  unsigned int* versionPtr = (unsigned int *)_data;
  //Serial.println(*versionPtr);
  return *versionPtr;
}

/**
 * Set the name in the data structure
 *
 */
void EEPROMConfigClass::setName(const char* name) {
  //Serial.println("EEPROMConfigClass::setName");
  char* namePtr = (char*)_data + sizeof(_version);   // is there any padding ?
  strncpy(namePtr, name, NAME_MAX_LENGTH);
  *(namePtr + NAME_MAX_LENGTH) = 0;
}

/**
 * Get the name from the data structure
 *
 */
char* EEPROMConfigClass::getName(void) {
  //Serial.println("EEPROMConfigClass::getName");
  char* namePtr = (char*)_data + sizeof(_version);
  return namePtr;
}

/**
 * Get the data structure size (it was provided at init)
 *
 */
unsigned int EEPROMConfigClass::getDataSize(void) {
  //Serial.println("EEPROMConfigClass::getDataSize");
  //Serial.println(_dataSize);
  return _dataSize;
}

/**
 * Get the data structure
 *
 */
void* EEPROMConfigClass::getData(void) {
  //Serial.println("EEPROMConfigClass::getData");
  return _data;
}

/**
 * Set the data structure
 *
 */
void EEPROMConfigClass::setData(void* data) {
  //Serial.println("EEPROMConfigClass::setData");
  _data = data;
}

/**
 * Init the data structure from the default values.
 * Subclass need to take care of initializing fields other than version and name
 *
 */
void EEPROMConfigClass::initFromDefault(void) {
  //Serial.println("EEPROMConfigClass::initFromDefault");
  setVersion(_version);
  setName(_name);
}
