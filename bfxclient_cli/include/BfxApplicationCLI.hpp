#include "BfxApplication.hpp"

namespace FIX {

    class BfxApplicationCLI : public BfxApplication{
public:
  void run();
private:
  char queryAction();
  bool queryConfirm( const std::string& query );
  void queryHeader( FIX::Header& header );

  void queryEnterOrder(); 
  FIX44::NewOrderSingle queryNewOrderSingle44();
  FIX44::OrderCancelRequest queryOrderCancelRequest44();
  FIX44::OrderCancelReplaceRequest queryCancelReplaceRequest44();

  void queryCancelOrder();
  void queryReplaceOrder();
  void queryMarketDataRequest();

  FIX::SenderCompID querySenderCompID();
  FIX::TargetCompID queryTargetCompID();
  FIX::TargetSubID queryTargetSubID();
  FIX::ClOrdID queryClOrdID();
  FIX::OrigClOrdID queryOrigClOrdID();
  FIX::Symbol querySymbol();
  FIX::Side querySide();
  FIX::OrderQty queryOrderQty();
  FIX::OrdType queryOrdType();
  FIX::Price queryPrice();
  FIX::StopPx queryStopPx();
  FIX::TimeInForce queryTimeInForce();
    };

};