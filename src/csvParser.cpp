//
// Created by zac on 11/26/26.
//
#include <cstddef>
#include <string_view>
#include "csvParser.h"


/*namespace slipstream
{

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

}*/


