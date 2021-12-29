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
 * 
 * Class could be derived for different types of initiators like SSL, SSL-ST.
 * It's a class that it's aiming for fast implementation since all members are public.
 * TODO: Make it more safe.
 */
class BfxClient {
    
public:
//Constructors
        /** @brief Setups member components based on BFX_CLIENT_CONF */
        BfxClient();
        virtual ~BfxClient();
        #ifdef HAVE_SSL
        /** @brief Setups member components */
        BfxClient(std::string configFilePath, FIX::SSLMode mode = FIX::SSLMode::None);
        #endif

//Components
        FIX::SessionSettings settings;
        FIX::BfxApplication application;

        FIX::Initiator* initiator;
        std::optional<FIX::FileStoreFactory> storeFactory;
        std::optional<FIX::ScreenLogFactory> logFactory;

protected:
private:

};

}