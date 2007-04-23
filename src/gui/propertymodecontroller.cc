/*                                                                              
 * PDFedit - free program for PDF document manipulation.                        
 * Copyright (C) 2006, 2007  PDFedit team:      Michal Hocko, 
 *                                              Miroslav Jahoda,       
 *                                              Jozef Misutka, 
 *                                              Martin Petricek                                             
 *
 * Project is hosted on http://sourceforge.net/projects/pdfedit                                                                      
 */ 
/** @file
 PropertyModeController - class providing wrapper around single ModeController
 instance and managing its configuration
 @author Martin Petricek
*/

#include "propertymodecontroller.h"
#include "qtcompat.h"
#include "settings.h"
#include <qstring.h>
#include <stdlib.h>
#include <modecontroller.h>
#include <confparser.h>
#include "util.h"

namespace gui {

using namespace configuration;

/** Single property mode instance for entire application */
PropertyModeController *propertyModeControllerInstance=NULL;

//Internal private data for single PropertyModeController instance

/** Mode Controller instance */
ModeController modeController;

/**
 Low-level parser for simple configuration files (one line = one setting).
 This base parser is used by ConfParser,
 which is used by ModeController in loadFromFile 
*/
StringConfigurationParser baseParser(NULL);

/** Configuration parser */
ModeController::ConfParser parser(baseParser);

/** atexit handler that will delete the single instance if it was allocated */
void removePropertyModeController() {
 if (!propertyModeControllerInstance) return;
 delete propertyModeControllerInstance;
}

/**
 Constructor
 Class will be initialized and also configuration will be read, if specified in the settings
*/
PropertyModeController::PropertyModeController(){
 //Register to be destroyed at exit
 atexit(removePropertyModeController);
 QString confFile=globalSettings->getFullPathName("config",QString::null,"modecontroller/");
 if (confFile.isNull()) {
  guiPrintDbg(debug::DBG_WARN,"Mode controller config not found");
 }
 guiPrintDbg(debug::DBG_DBG,"Mode controller config file: " << Q_OUT(confFile));
 int result=modeController.loadFromFile(util::convertFromUnicode(confFile,util::NAME),parser);
 if(result==-1) {
  guiPrintDbg(debug::DBG_WARN,"Mode controller failed to parse file: " << Q_OUT(confFile));
  //TODO alert user
 }
};

/**
 Return Instance of PropertyModeController.
 Ensures only one instance of PropertyModeController exists at any time (singleton)
 If no instance exists, it is created.
 @return existing PropertyModeController object
*/
PropertyModeController* PropertyModeController::getInstance() {
 if (!propertyModeControllerInstance) propertyModeControllerInstance=new PropertyModeController();
 return propertyModeControllerInstance;
}

/**
 For given type and name return mode of the property
 @param type Type field value of complex type
 @param name Name of property
 @return property mode
*/
PropertyMode PropertyModeController::mode(const QString &type,const QString &name) {
 return modeController.getMode(util::convertFromUnicode(type,util::PDF),util::convertFromUnicode(name,util::PDF));
}

/**
 Return instance of ModeController that is being used by this class
 @return ModeController instance
*/
ModeController* PropertyModeController::get() {
 return &modeController;
}

/** Destructor */
PropertyModeController::~PropertyModeController(){
 propertyModeControllerInstance=NULL;
};

} // namespace gui
