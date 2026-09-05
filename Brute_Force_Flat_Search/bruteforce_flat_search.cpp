#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <unordered_set>
using namespace std;

// Reads .fvecs file for data
vector<vector<float>> read_fvecs(string filename)
{
    ifstream file(filename, ios::binary);
    vector<vector<float>> data;

    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return data;
    }

    int dim = 0;
    while (file.read(reinterpret_cast<char *>(&dim), sizeof(int)))
    {
        vector<float> vec(dim);
        file.read(reinterpret_cast<char *>(vec.data()), dim * sizeof(float));
        data.push_back(move(vec)); // Move saves deepcopy cost unlike direct pushback
    }

    return data;
}

// Reads .ivecs file for groundtruth verification thingy
vector<vector<int>> read_ivecs(string filename)
{
    ifstream file(filename, ios::binary);
    vector<vector<int>> data;

    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return data;
    }

    int dim = 0;
    while (file.read(reinterpret_cast<char *>(&dim), sizeof(int)))
    {
        vector<int> vec(dim);
        file.read(reinterpret_cast<char *>(vec.data()), dim * sizeof(int));
        data.push_back(move(vec)); // Move saves deepcopy cost unlike direct pushback
    }

    return data;
}

// Calculating squared distance without sqrt
float distance(vector<float> &u, vector<float> &v) // Might overflow for big values but isnt overflowing for the file provided
{
    float dist = 0;
    for (int i = 0; i < (int)u.size(); i++)
    {
        float diff = u[i] - v[i];
        dist += diff * diff;
    }
    return dist;
}

// Top-k flat search
vector<int> top_k_search(vector<float> &query, vector<vector<float>> &base, int k)
{
    priority_queue<pair<float, int>> pq;
    for (int i = 0; i < (int)base.size(); i++)
    {
        float dist = distance(query, base[i]);

        if (pq.size() < k)
        {
            pq.push({dist, i});
        }
        else if (dist < pq.top().first)
        {
            pq.pop();
            pq.push({dist, i});
        }
    }

    vector<int> indices;
    indices.reserve(pq.size()); // To save the cost of reallocating blocks by direct pushbacks
    while (!pq.empty())
    {
        indices.push_back(pq.top().second);
        pq.pop();
    }
    reverse(indices.begin(), indices.end());
    return indices;
}

int main()
{
    vector<vector<float>> base_vectors = read_fvecs("siftsmall_base.fvecs");
    vector<vector<float>> query_vectors = read_fvecs("siftsmall_query.fvecs");
    vector<vector<int>> ground_truth = read_ivecs("siftsmall_groundtruth.ivecs");

    if (base_vectors.empty() || query_vectors.empty())
    {
        return 1; // File wasnt read properly... Or improperly coz like... Not read at all...
    }

    int k = 100;
    int total_matches = 0;
    int total_elements = query_vectors.size() * k;
    for (int q = 0; q < (int)query_vectors.size(); q++)
    {
        vector<int> top_k_indices = top_k_search(query_vectors[q], base_vectors, k);

        // for(int i = 0; i < k; i++){
        //     if(retrieved_indices[i] == ground_truth[q][i]){
        //         total_matches++;
        //     }
        // }
        // This induces error due to floating point precision.

        // unordered_set<int> truth_set(ground_truth[q].begin(), ground_truth[q].begin() + k);
        // for (int idx : retrieved_indices)
        // {
        //     if (truth_set.count(idx))
        //     {
        //         total_matches++;        //Uses O(n) extra memory but doesnt need O(k) searches each check.
        //     }
        // }

        for (int idx : top_k_indices)
        {
            if (find(ground_truth[q].begin(), ground_truth[q].begin() + k, idx) != ground_truth[q].begin() + k)
            {
                total_matches++; // No extra memory used, O(k) time per check tho.
            }
        }
    }

    float recall = (float)(total_matches) / total_elements;
    cout << "Recall@" << k << ": " << recall << endl;

    return 0;
}