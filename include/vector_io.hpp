#pragma once
#include "vector_ds.hpp"
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

class VectorStoreIO
{
public:
    template <typename T>
    static std::vector<VectorRecord<T>> read_vecs(const std::string &file_path)
    {
        std::ifstream file(file_path, std::ios::binary);
        std::vector<VectorRecord<T>> store;

        int id = 0;

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file.");
        }
        while (true)
        {
            int32_t dim{0};

            if (file.read(reinterpret_cast<char *>(&dim), sizeof(dim)))
            {
                VectorRecord<T> record;
                record.id = id;
                record.vector.resize(dim);
                file.read(reinterpret_cast<char *>(record.vector.data()), dim * sizeof(T));
                id++;
                store.emplace_back(record);
            }
            else
            {
                break;
            }
        }
        return store;
    }
};