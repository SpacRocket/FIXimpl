#include "quickfix/config.h"

#include "quickfix/FileStore.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"
#include "BfxClient.hpp"
#include <string>
#include <iostream>
#include <fstream>

int main( int argc, char** argv )
{
  try
  {
    FIX::BfxClient Client;
    Client.initiator->start();
    
    Client.initiator->stop();

    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
    return 1;
  }
}