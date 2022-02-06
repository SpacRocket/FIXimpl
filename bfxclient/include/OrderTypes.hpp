namespace FIX{
    enum class OrderType : int{
      Market = 1,
      Limit = 2,
      Stop = 3,
      StopLimit = 4,
      //Trailing stop requrest tag(18) as R and, tag(211) as price, further info in bfxfixgw
      TrailingStop = 4 
    };
}