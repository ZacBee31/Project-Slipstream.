//
// Created by Zac B on 11/13/26.
//
#include <cstdint>
#pragma once

struct FrameHeader
{
  uint16_t body_len;
  uint8_t msg_type;
  uint8_t version;
};


struct QuoteBody
{
    char symbol[12];
    uint64_t ts_ns;
    uint32_t bid_qty;
    uint64_t bid_px;
    uint32_t ask_qty;
    uint64_t ask_px;
};


struct TradeBody
{
    char symbol[12];
    uint64_t ts_ns;
    uint32_t qty;
    uint64_t px;
    char aggressor;
    int64_t id;
};

struct HeartbeatBody
{
    uint64_t ts_ns;
};


struct SessionControlBody
{
    uint64_t ts_ns;
    uint8_t state; //0=OPEN, 1=HALT, 2=CLOSE
};

struct NewOrder //msg_type = 10
{
    uint64_t client_order_id;
    char symbol[12];
    char status;
    uint64_t ts_ns;
    int64_t trade_id;
    char side;
    uint32_t qty;
    int64_t limit_px;


};



enum class OrderType{
    GoodTillCancel,
    FillAndKill
};

enum class Side {
    Buy,
    Sell
};


struct ExecReport
{
    uint64_t client_order_id;
    uint64_t ts_ns;
    uint8_t status;
    uint32_t filled_qty;
    int64_t avg_px;
    int8_t reason_code; //0=none,1=risk,2=price,3=size,4=throttle


};

using OrderID = uint32_t;   //32 bits should be able to store enough orders
using Price = int32_t;      //price can be negative, so it's signed.
using Quantity = uint32_t;  //quantity can't be negative, so its unsigned.


