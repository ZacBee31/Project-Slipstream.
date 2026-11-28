//
// Created by zac on 11/17/26.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <span>
#include <spanstream>
#include <functional>
#include <array>
#include <string_view>
#include "csvParser.h"
#include "MappedFile.h"

static constexpr size_t fields = 9;

int main()
{
    std::size_t commas{0};
    std::array<std::string_view, fields> fieldOutputs;
    slipstream::MappedFile MarketDataFeed("QandT.csv");
    std::span<const char> data_ = MarketDataFeed.Data();
    std::string_view data(data_.data(), data_.size());
    auto newline = data.find('\n');
    std::string_view line = data.substr(0, newline);

    for (size_t i{0}; i < slipstream::preambleLength; i++)
    {
        auto newline = data.find('\n');
        std::cout << data.substr(0, newline) << std::endl;
        data = data.substr(newline +  1);
    }

    auto newline_ = data.find('\n');
    std::cout << data.substr(0, newline_) << std::endl;
    data = data.substr(newline_ +  1);
    return 0;
}





/*
    struct StringStringViewHasher {
        //allows for heterogeneous lookups.
        //so that std::string_view can be used to query for std::string key objects

        using is_transparent = int;

        size_t operator()(std::string_view str) const {
            return std::hash<std::string_view>{}(str);
        }

        size_t operator()(const std::string& str) const {
            return std::hash<std::string>{}(str);
        }
    };

    constexpr std::size_t Fields = 9;
    constexpr std::size_t PreambleLines = 21;

    template<std::size_t N>
    std::size_t SplitFields(std::string_view line, std::array<std::string_view, N>& fields)
    {
        std::size_t n{0};
        while (n < N)
        {
            const auto comma = line.find(',');
            fields[n++] = line.substr(0, comma);
            line = line.substr(comma + 1);
        }
    return n;
    }



    void MDFileReader(){
        MappedFile file{Filename};
        const auto data = file.Data();
        std::string_view text{data.data(), data.size()};
        size_t n{0};

        while (n < PreambleLines) //skip beginning of file.
        {
            auto endline = text.find('\n');

            std::string_view line = text.substr(0, endline);
            std::cout << line << std::endl;
            text = text.substr(endline + 1);
            n++;
        }

        while (!text.empty())
        {



        }









        auto view = file.Data();
        auto begin = view.begin();
        while(begin != view.end())
        {
            //ranges find is used because you can tell it
            //to not check for the end iterator.
            for (size_t i{0}; i < 10; i++)
            {
                const auto comma = std::ranges::find(begin, std::unreachable_sentinel, ',');
                std::string_view text = {begin, comma};
                begin = comma + 1;
            }

            const auto endLine = std::ranges::find(begin, std::unreachable_sentinel, '\n');
            float reading;
            std::from_chars(begin.base(), endLine.base(), reading);
            begin = endLine + 1;

            //finding the station
            if(auto iterator = records.find(station); iterator == records.end())
            {
                records.emplace(station,
                    Record{1, reading, reading, reading});
            }
            else
            {
                auto& existing = iterator->second;
                existing.Sum += reading;
                existing.Count += 1;
                existing.Max = std::max(reading, existing.Max);
                existing.Min = std::min(reading, existing.Min);
            }
        }


}*/
