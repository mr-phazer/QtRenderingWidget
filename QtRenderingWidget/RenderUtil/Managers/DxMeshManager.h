#pragma once





namespace Rldx
{
    class DxMeshManager {
    private:
        static DxMeshManager* instance;

        Singleton() {
            // private constructor to prevent instantiation
        }

    public:
        static DxMeshManager* getInstance() {
            if (instance == nullptr) {
                instance = new Singleton();
            }
            return instance;
        }

    };   
   
}