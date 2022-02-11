#include "quickfix/SessionID.h"

#include "string"

namespace FIX{

    struct Order{

     SessionID sessionID = null;
     std::string symbol = null;
     int quantity = 0;
     int open = 0;
     int executed = 0;
     int side = 0;
     int type = 0;
     int tif = 0;
     double limit = null;
     double stop = null;
     double avgPx = 0.0;
     bool rejected = false;
     bool canceled = false;
     bool isNew = true;
     std::string message = null;
     std::string ID = null;
     std::string originalID = null;
     int nextID = 1;

    };

};