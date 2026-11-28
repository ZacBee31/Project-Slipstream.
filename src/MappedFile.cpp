//
// Created by zac on 11/24/26.
//
#include <unordered_map>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <system_error>
#include <sys/stat.h>
#include <span>
#include <spanstream>
#include <functional>
#include <iterator>
#include <algorithm>
#include <array>
#include <string_view>
#include <charconv>
#include "MappedFile.h"

namespace slipstream
{
    //const std::string Filename = "QandT.csv";


            [[nodiscard]] bool MappedFile::MappedFileDetails::IsOpen() const { return Descriptor != InvalidDescriptor; }

            void MappedFile::MappedFileDetails::Close()
            {
                if (Descriptor == InvalidDescriptor){
                    return;
                }
                //could close in destructor

                close(Descriptor);
            }

        [[nodiscard]] std::span<const char> MappedFile::Data() const {
            return {details_.Begin, details_.Size};
        }

        MappedFile::~MappedFile(){
            if(!details_.IsOpen()){
                return;
            }

            details_.Close();

            if(details_.Begin == nullptr){
                return;
            }

            munmap(details_.Begin, details_.Size);
        }





    MappedFile::MappedFile(const std::filesystem::path& path){
        const auto descriptor = open(path.c_str(), O_RDONLY);

        if(descriptor == -1)
        {
            throw std::system_error(errno, std::system_category(), "Cannot open file!");
        }

        struct stat sb; //getting size
        if(fstat(descriptor, &sb) == -1)
        {
            throw std::system_error(errno, std::system_category(), "Cannot stat file!");
        }


        //opening a handle here
        const auto start = static_cast<char*>(
            mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, descriptor, 0));
        if(start == MAP_FAILED){
            throw std::system_error(errno, std::system_category(), "Cannot mmap file");
        }

        madvise(details_.Begin, details_.Size, MADV_SEQUENTIAL);

        details_ = {
            .Descriptor = descriptor,
            .Begin = start,
            .Size = static_cast<size_t>(sb.st_size),
        };
    }
}