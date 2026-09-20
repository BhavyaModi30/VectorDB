#pragma once
#include "vector_ds.hpp"
#include <algorithm>
#include <queue>
#include <utility>
#include <vector>

float distance(const std::vector<float> &u, const std::vector<float> &v)
{
    float dist = 0;
    for (size_t i = 0; i < u.size(); i++)
    {
        float diff = u[i] - v[i];
        dist += diff * diff;
    }
    return dist;
}

std::vector<int> top_k_search(const std::vector<float> &query, const std::vector<VectorRecord<float>> &base, int k)
{
    std::priority_queue<std::pair<float, int>> pq;
    for (size_t i = 0; i < base.size(); i++)
    {
        float dist = distance(query, base[i].vector);

        if ((int)pq.size() < k)
        {
            pq.push({dist, base[i].id});
        }
        else if (dist < pq.top().first)
        {
            pq.pop();
            pq.push({dist, base[i].id});
        }
    }

    std::vector<int> indices;
    indices.reserve(pq.size());
    while (!pq.empty())
    {
        indices.push_back(pq.top().second);
        pq.pop();
    }
    std::reverse(indices.begin(), indices.end());
    return indices;
}