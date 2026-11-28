//
// Created by zac on 11/25/26.
//
#include "types.h"
#include <optional>
#include <vector>
#include <memory>
#include <list>
#include <map>
#include <unordered_map>




struct LevelInfo{
    Price price;
    Quantity quantity;
};

using LevelInfos = std::vector<LevelInfo>;
/*
doing namespaces like this forces
them into the global scope of every
single file that includes it. Should
investigate wrapping all of this in
a namespace.
*/


class OrderbookLevelInfos{
public:
    OrderbookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
    : bids_{bids}
    , asks_{asks}
    { }

private:
    LevelInfos bids_;
    LevelInfos asks_;
};

class Order {
public:
    Order(OrderType orderType, OrderID orderID, Side side, Price price, Quantity quantity)
    : orderType_{ orderType }
    , orderID_{ orderID }
    , side_{ side }
    , price_{ price }
    , remainingQuantity_ { quantity }
    , initialQuantity_ { quantity }
    { }



    OrderID getOrderID() const { return orderID_; }
    Price GetPrice() const { return price_; }
    OrderType GetOrderType() const { return orderType_; }
    Side GetSide() const { return side_; }
    Quantity getRemainingQuantity() const { return remainingQuantity_; }
    void Fill(Quantity quantity);
    bool IsFilled() const { return getRemainingQuantity() == 0; }

private:
    Side side_{};
    Quantity remainingQuantity_{};
    Quantity initialQuantity_{};
    OrderID orderID_{};
    Price price_{};
    OrderType orderType_{};
};

/*
we're gonna be storing orders
in several different structures
so using reference semantics
is important
*/

/*
list gives an iterator that can't
be invalidated even after reallocation
*/

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

/*
need to add tradeinfo and trade objects
the actual trade object is an aggregate
of two separate trade objects, one for
each side.
*/

class OrderModify{
public:
    OrderModify(OrderID orderId, Side side, Price price, Quantity quantity)
    : orderId_{ orderId }
    , price_{ price }
    , side_{ side }
    , quantity_{ quantity }
    { }

    /*
    it's ok to include trivial getters and setters
    because i probably wont change them later
    in fact, might be worth deleting the getters
    and setters later
    the pointer getter is not as trivial so that will
    be in an implementation file instead
    */
    OrderID getOrderID() const { return orderId_; }
    Price GetPrice() const { return price_; }
    Side GetSide() const { return side_; }
    Quantity GetQuantity() const { return quantity_; }

    OrderPointer ToOrderPointer(OrderType type) const;

private:
    OrderID orderId_;
    Price price_;
    Side side_;
    Quantity quantity_;

};


struct TradeInfo{
    OrderID OrderID_;
    Price Price_;
    Quantity quantity_;

};

class Trade{
public:
    Trade(const TradeInfo& bidTrade, const TradeInfo& askTrade)
    : bidTrade_{ bidTrade }
    , askTrade_{ askTrade }
    { }

    const TradeInfo& GetBidTrade() const { return bidTrade_; }
    const TradeInfo& GetAskTrade() const { return askTrade_; }

private:
    TradeInfo bidTrade_;
    TradeInfo askTrade_;

};

using Trades = std::vector<Trade>;

class OrderBook{
private:
    struct OrderEntry{
        OrderPointer order_{ nullptr };
        OrderPointers::iterator location;
    };

    std::map<Price, OrderPointers, std::greater<Price>> bids_;
    std::map<Price, OrderPointers, std::less<Price>> asks_;
    std::unordered_map<OrderID, OrderEntry> orders_;

    bool CanMatch(Side side, Price price) const;
    Trades MatchOrders();

public:
    Trades AddOrder(OrderPointer order);
    Trades MatchOrder(OrderModify order);
    void CancelOrder(OrderID orderId);
    std::size_t Size() const { return orders_.size(); }

    OrderbookLevelInfos getOrderInfos() const;
};



