#include <bits/stdc++.h>


struct Metadata {
    std::unordered_map<std::string, std::any> data;
};



template <typename T>
struct VectorRecord{
    int id;
    std::vector<T> vector;
    Metadata meta;    


};

class VectorStoreIO{
    public:
    
    template <typename T>
    static std::vector<VectorRecord<T>> read_vecs(const std::string& file_path){
        std::ifstream file(file_path, std::ios::binary);
        std::vector<VectorRecord<T>> store;
        
        long long id = 0;
        
        if(!file.is_open()){throw std::runtime_error("Failed too open file.");}
        while(true){
        int32_t dim{0};

        if(file.read( reinterpret_cast<char *>(&dim), sizeof(dim))){            
            VectorRecord<T> record;
            record.id = id;
            record.vector.resize(dim);
            file.read(reinterpret_cast<char*>(record.vector.data()),dim*sizeof(T));
            id++;
            /*if(id<15){
                std::cout << record.id << " (size: " << record.vector.size() << "): ";

                for (size_t i = 0; i < record.vector.size();++i) {
                    std::cout << record.vector[i] << " ";
                }
                std::cout << "...\n";}*/
            store.emplace_back(record);
            

        
        }else{ break;}
    }

        return store;}

        

};

int main(){
    const std::string filepath = "siftsmall_query.fvecs";
    auto siftsmall = VectorStoreIO::read_vecs<float>(filepath);

}