//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <string>
    #include <memory>
    #include <stdexcept>
    #include <unordered_map>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class ResourceManager {
    //-------------//
    public:
        template <typename ResType>
        void load(std::string id, const std::string& path) {
            if (id_check(id)) { return; }
            auto resource = new ResType();
            if (!resource->loadFromFile("res/" + path))
            { throw std::runtime_error("Failed to load : res/" + path); }
            resources[id] = resource;
        }
    //-------------//
        template <typename ResType>
        ResType* get(std::string id) {
            if (id_check(id))
            { return (ResType*)resources[id]; }
            return nullptr;
        }
    //-------------//
        template <typename ResType>
        void unload(std::string id) {
            if (id_check(id)) {
                auto it = resources.find(id);
                delete (ResType*)it->second;
                resources.erase(it);
            }
        }
    //-------------//
    private:
        std::unordered_map<std::string, void*> resources;
        bool id_check(std::string id) { return (resources.count(id) > 0); }
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//