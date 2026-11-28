//
// Created by Zac B on 11/13/26.
//
#include <iostream>
#include "libslipstream_codec.h"
#include "orderbook.h"
#include "types.h"


int main()
{
    std::cout << "In main" << std::endl;

    OrderBook ob;
    const OrderID orderid = 1;
    ob.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, orderid, Side::Buy, 100, 10));
    std::cout << ob.Size() << std::endl;

    ob.CancelOrder(orderid);
    std::cout << ob.Size() << std::endl;
    return 0;
}
