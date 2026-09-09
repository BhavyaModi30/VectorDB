#include "flat_search.hpp"
#include "vector_io.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

int main()
{
    auto base_vectors = VectorStoreIO::read_vecs<float>("siftsmall_base.fvecs");
    auto query_vectors = VectorStoreIO::read_vecs<float>("siftsmall_query.fvecs");
    auto ground_truth = VectorStoreIO::read_vecs<int>("siftsmall_groundtruth.ivecs");

    if (base_vectors.empty() || query_vectors.empty() || ground_truth.empty())
    {
        return 1;
    }

    int k = 100;
    int total_matches = 0;
    int total_elements = query_vectors.size() * k;

    for (size_t q = 0; q < query_vectors.size(); q++)
    {
        std::vector<int> top_k_indices = top_k_search(query_vectors[q].vector, base_vectors, k);

        for (int idx : top_k_indices)
        {
            if (std::find(ground_truth[q].vector.begin(), ground_truth[q].vector.begin() + k, idx) != ground_truth[q].vector.begin() + k)
            {
                total_matches++;
            }
        }
    }

    float recall = (float)(total_matches) / total_elements;
    std::cout << "Recall@" << k << ": " << recall << std::endl;

    return 0;
}