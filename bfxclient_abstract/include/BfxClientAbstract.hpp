#include "quickfix/config.h"

#include "quickfix/FileStore.h"
#include "quickfix/SocketInitiator.h"
#ifdef HAVE_SSL
#include "quickfix/ThreadedSSLSocketInitiator.h"
#include "quickfix/SSLSocketInitiator.h"
#endif
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"
#include "quickfix/FileLog.h"
#include "quickfix/NullStore.h"

#include "BfxApplication.hpp" 
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <optional>


namespace FIX{

#ifdef HAVE_SSL
enum class SSLMode { None, SSL, SSL_ST};
#endif

/**
 * @brief Collection of all the things required for working QuickFix Initiator.
 */
template <class T = BfxApplication>
class BfxClient {
    
public:
//Constructors
        /** @brief Setups member components based on BFX_CLIENT_CONF */
        BfxClient();
        virtual ~BfxClient();
        #ifdef HAVE_SSL
        /** @brief Setups member components */
        BfxClient(std::string configFilePath, FIX::SSLMode mode = FIX::SSLMode::None);
        BfxClient(FIX::SSLMode mode);
        BfxClient(bool logging);
        #endif

//Components
        FIX::SessionSettings settings;
        T application;

        FIX::Initiator* initiator;
        std::optional<FIX::FileStoreFactory> storeFactory;
        std::optional<FIX::ScreenLogFactory> logFactory;
}; //End of BfxClient
}

// Constructors
template <class T>
FIX::BfxClient<T>::BfxClient() {
  const char* bfxconf{std::getenv("BFX_CLIENT_CONF")};
  if (bfxconf == nullptr) {
    throw FIX::ConfigError(
        "$BFX_CLIENT_CONF is not configured. Use different constructor or set "
        "env variable.");
  }

  settings = FIX::SessionSettings(bfxconf);
  application.settings = settings;
  storeFactory = FIX::FileStoreFactory(settings);
  logFactory = FIX::ScreenLogFactory(settings);

  initiator = new FIX::SocketInitiator(application, storeFactory.value(),
                                       settings, logFactory.value());
}

template <class T>
FIX::BfxClient<T>::BfxClient(bool logging) {
  const char* bfxconf{std::getenv("BFX_CLIENT_CONF")};
  if (bfxconf == nullptr) {
    throw FIX::ConfigError(
        "$BFX_CLIENT_CONF is not configured. Use different constructor or set "
        "env variable.");
  }

  settings = FIX::SessionSettings(bfxconf);
  application.settings = settings;

  if (logging) {
    storeFactory = FIX::FileStoreFactory(settings);
    logFactory = FIX::ScreenLogFactory(settings);

    initiator = new FIX::SocketInitiator(application, storeFactory.value(),
                                         settings, logFactory.value());
  } else {
    initiator = new FIX::SocketInitiator(application, storeFactory.value(),
                                         settings, logFactory.value());
  }
}

template <class T>
FIX::BfxClient<T>::~BfxClient() {}

#ifdef HAVE_SSL
template <class T>
FIX::BfxClient<T>::BfxClient::BfxClient(std::string configFilePath,
                                     FIX::SSLMode mode) {
  settings = FIX::SessionSettings(configFilePath);

  application.settings = settings;

  storeFactory = FIX::FileStoreFactory(settings);
  logFactory = FIX::ScreenLogFactory(settings);

  if (mode == FIX::SSLMode::SSL)
    initiator = new FIX::ThreadedSSLSocketInitiator(
        application, storeFactory.value(), settings, logFactory.value());
  else if (mode == FIX::SSLMode::SSL_ST)
    initiator = new FIX::SSLSocketInitiator(application, storeFactory.value(),
                                            settings, logFactory.value());
  else if (mode == FIX::SSLMode::None)
    initiator = new FIX::SocketInitiator(application, storeFactory.value(),
                                         settings, logFactory.value());
}

template <class T>
FIX::BfxClient<T>::BfxClient(FIX::SSLMode mode) {
  const char* bfxconf{std::getenv("BFX_CLIENT_CONF")};
  if (bfxconf == nullptr) {
    throw FIX::ConfigError(
        "$BFX_CLIENT_CONF is not configured. Use different constructor or set "
        "env variable.");
  }

  settings = FIX::SessionSettings(bfxconf);
  
  application.settings = settings;

  storeFactory = FIX::FileStoreFactory(settings);
  logFactory = FIX::ScreenLogFactory(settings);

  if (mode == FIX::SSLMode::SSL)
    initiator = new FIX::ThreadedSSLSocketInitiator(
        application, storeFactory.value(), settings, logFactory.value());
  else if (mode == FIX::SSLMode::SSL_ST)
    initiator = new FIX::SSLSocketInitiator(application, storeFactory.value(),
                                            settings, logFactory.value());
  else if (mode == FIX::SSLMode::None)
    initiator = new FIX::SocketInitiator(application, storeFactory.value(),
                                         settings, logFactory.value());
}
#endif