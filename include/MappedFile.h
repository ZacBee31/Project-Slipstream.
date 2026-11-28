//
// Created by zac on 11/24/26.
//

#ifndef SLIPSTREAM_MAPPEDFILE_H
#define SLIPSTREAM_MAPPEDFILE_H

#endif //SLIPSTREAM_MAPPEDFILE_H
#pragma once

namespace slipstream
{
    //const std::string Filename = "QandT.csv";
    struct MappedFile{
        struct MappedFileDetails{
            constexpr static int InvalidDescriptor = -1;
            int Descriptor{ InvalidDescriptor };
            char* Begin{ nullptr };
            size_t Size{ 0 };

            [[nodiscard]] bool IsOpen() const;
            void Close();
        };

        explicit MappedFile(const std::filesystem::path& path);
        [[nodiscard]] std::span<const char> Data() const;
        ~MappedFile();

    private:
        MappedFileDetails details_;
    };
}
