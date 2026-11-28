//
// Created by zac on 11/25/26.
//

#include <optional>
#include <vector>
#include <memory>
#include <list>
#include <map>
#include <unordered_map>
#include <iostream>
#include <numeric>
#include <format>
#include <stdexcept>
#include "orderbook.h"
#include "types.h"





void Order::Fill(Quantity quantity)
{
    if(quantity > getRemainingQuantity())
    throw std::logic_error(std::format("Order ({}) cannot be filled for more than its remaining quantity",
    getOrderID()));

    remainingQuantity_ -= quantity;

}

OrderPointer OrderModify::ToOrderPointer(OrderType type) const
{
    return std::make_shared<Order>(type, getOrderID(), GetSide(), GetPrice(), GetQuantity());
}

bool OrderBook::CanMatch(Side side, Price price) const
{
    if(side == Side::Buy)
    {
        if(asks_.empty()) return false;
        const auto& [bestAsk, _] = *asks_.begin();
        return price >= bestAsk;
    }
    else
    {
        if(bids_.empty()) return false;
        const auto& [bestBid, _] = *bids_.begin();
        return price <= bestBid;
    }
}

Trades OrderBook::MatchOrders()
{
    Trades trades;
    trades.reserve(orders_.size());

    while(true)
    {
        if(bids_.empty() || asks_.empty()) break;

        auto& [bidPrice, bids] = *bids_.begin();
        auto& [askPrice, asks] = *asks_.begin();

        if(bidPrice <= askPrice) break;

        while(bids.size() || asks.size())
        {
            auto& bid = bids.front();
            auto& ask = asks.front();

            Quantity quantity = std::min(bid->getRemainingQuantity(), ask->getRemainingQuantity());

            if (bid->IsFilled())
            {
                bids.pop_front();
                orders_.erase(bid->getOrderID());
            }

            if (ask->IsFilled())
            {
                asks.pop_front();
                orders_.erase(bid->getOrderID());
            }

            if(bids.empty())
            {
                bids_.erase(bidPrice);
            }

            if(asks.empty())
            {
                asks_.erase(askPrice);
            }

            trades.push_back(Trade{TradeInfo{bid->getOrderID(), bid->GetPrice(), quantity},
                                   TradeInfo{ask->getOrderID(), ask->GetPrice(), quantity}});

        }
    }
    if (!bids_.empty())
    {
        auto& [_, bids] = *bids_.begin();
        auto& order = bids.front();
        if(order->GetOrderType() == OrderType::FillAndKill)
        {
            CancelOrder(order->getOrderID());
        }

        if(order->GetOrderType() == OrderType::FillAndKill)
        {
            CancelOrder(order->getOrderID());
        }



    }
        return trades;
}

Trades OrderBook::AddOrder(OrderPointer order)
{
    if(orders_.contains(order->getOrderID())) return { };

    if (order->GetOrderType() == OrderType::FillAndKill &&
    !CanMatch(order->GetSide(), order->GetPrice())) return { };
        //not super readable

    OrderPointers::iterator iterator;

    if(order->GetSide() == Side::Buy)
    {
        auto& orders = bids_[order->GetPrice()];
        orders.push_back(order);
        iterator = std::next(orders.begin(), orders.size() - 1);
    }
    else
    {
        auto& orders = asks_[order->GetPrice()];
        orders.push_back(order);
        iterator = std::next(orders.begin(), orders.size() - 1);
    }
    orders_.insert({order->getOrderID(), OrderEntry{order, iterator}});
    return MatchOrders();
}

void OrderBook::CancelOrder(OrderID orderId)
{
    if(!orders_.contains(orderId)) return;

    const auto& [order, iterator] = orders_.at(orderId);
    orders_.erase(orderId);

    if (order->GetSide() == Side::Sell)
    {
        auto price = order->GetPrice();
        auto& orders = asks_.at(price);
        orders.erase(iterator);
        if(orders.empty()) asks_.erase(price);
    }
    else
    {
        auto price = order->GetPrice();
        auto& orders = bids_.at(price);
        orders.erase(iterator);
        if(orders.empty()) bids_.erase(price);
    }


}

Trades OrderBook::MatchOrder(OrderModify order)
{
    if(!orders_.contains(order.getOrderID())) return { }; //would std::optional be better?

    const auto& [existingOrder, _] = orders_.at(order.getOrderID());
    CancelOrder(order.getOrderID());
    return AddOrder(order.ToOrderPointer(existingOrder->GetOrderType()));
}

OrderbookLevelInfos OrderBook::getOrderInfos() const
{
    LevelInfos bidInfos, askInfos;
    bidInfos.reserve(orders_.size());
    askInfos.reserve(orders_.size());

    auto CreateLevelInfos = [](Price price, const OrderPointers& orders)
    {
        return LevelInfo{price, std::accumulate(orders.begin(), orders.end(), (Quantity)0,
        [](std::size_t runningSum, const OrderPointer& order)
        { return runningSum + order->getRemainingQuantity(); })};
    };

        for(const auto& [price, orders] : bids_)
        bidInfos.push_back(CreateLevelInfos(price, orders));

        for(const auto& [price, orders] : asks_)
        askInfos.push_back(CreateLevelInfos(price, orders));

        return OrderbookLevelInfos{ bidInfos, askInfos };


}




