#include "quickfix/config.h"

#include "quickfix/FileStore.h"
#include "quickfix/SocketInitiator.h"
#ifdef HAVE_SSL
#include "quickfix/ThreadedSSLSocketInitiator.h"
#include "quickfix/SSLSocketInitiator.h"
#endif
#include "quickfix/SessionSettings.h"
#include "quickfix/SessionState.h"
#include "quickfix/Log.h"

#include "BfxClient.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

//Constructors
FIX::BfxClient::BfxClient() {
  const char* bfxconf{std::getenv("BFX_CLIENT_CONF")};
  if(bfxconf == nullptr){
  throw FIX::ConfigError("$BFX_CLIENT_CONF is not configured. Use different constructor or set env variable.");
  }

  settings = FIX::SessionSettings(bfxconf);
  application.settings = settings;
  storeFactory = FIX::FileStoreFactory(settings);
  logFactory = FIX::ScreenLogFactory(settings);

  initiator = new FIX::SocketInitiator(application, storeFactory.value(), settings, logFactory.value());
}

//ToDo: Better code
FIX::BfxClient::~BfxClient() {

}

#ifdef HAVE_SSL
FIX::BfxClient::BfxClient::BfxClient(std::string configFilePath, FIX::SSLMode mode) {
  settings = FIX::SessionSettings(configFilePath);

  application.settings = settings;

  storeFactory = FIX::FileStoreFactory(settings);
  logFactory = FIX::ScreenLogFactory(settings);
  
  if (mode == FIX::SSLMode::SSL)
    initiator = new FIX::ThreadedSSLSocketInitiator(application, storeFactory.value(), settings, logFactory.value()); 
  else if (mode == FIX::SSLMode::SSL_ST)
    initiator = new FIX::SSLSocketInitiator(application, storeFactory.value(), settings, logFactory.value()); 
  else if (mode == FIX::SSLMode::None)
    initiator = new FIX::SocketInitiator(application, storeFactory.value(), settings, logFactory.value());

}
#endif
