#include <memory>
#include <mutex>

class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& getInstance() {
        static Singleton instance; 
        return instance;
    }

private:
    Singleton(){}
};