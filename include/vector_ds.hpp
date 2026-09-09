#pragma once
#include <any>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

struct Metadata
{
    std::unordered_map<std::string, std::any> data;
};

template <typename T>
struct VectorRecord
{
    int id;
    std::vector<T> vector;
    Metadata meta;

    size_t dimension() const
    {
        return vector.size();
    }
};