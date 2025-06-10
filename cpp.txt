#include <iostream>
#include <vector>
#include <string>

class HashTable {
private:
    struct Brawl_Fighter {
        std::string name;
        int trophies;
        bool isActive;

        Brawl_Fighter() : name(""), trophies(0), isActive(false) {}
    };

    std::vector<Brawl_Fighter> table;
    int capacity;

    size_t hash1(const std::string& key) const {
        size_t hash = 0;
        for (char c : key) {
            hash = (hash * 31) + c;
        }
        return hash;
    }

    size_t hash2(const std::string& key) const {
        size_t hash = 0;
        for (char c : key) {
            hash = (hash * 17) + c;
        }
        return (hash << 1) | 1;
    }

public:
    HashTable(int size = 10) : capacity(size) {
        table.resize(capacity);
    }

    void insert(const std::string& name, int trophies) {
        size_t idx = hash1(name) % capacity;
        size_t step = hash2(name) % capacity;

        for (int i = 0; i < capacity; ++i) {
            size_t current = (idx + i * step) % capacity;
            if (!table[current].isActive || table[current].name == name) {
                table[current].name = name;
                table[current].trophies = trophies;
                table[current].isActive = true;
                return;
            }
        }
    }

    bool find(const std::string& name, int& trophies) {
        size_t idx = hash1(name) % capacity;
        size_t step = hash2(name) % capacity;

        for (int i = 0; i < capacity; ++i) {
            size_t current = (idx + i * step) % capacity;
            if (!table[current].isActive) break;
            if (table[current].name == name) {
                trophies = table[current].trophies;
                return true;
            }
        }
        return false;
    }

    bool remove(const std::string& name) {
        size_t idx = hash1(name) % capacity;
        size_t step = hash2(name) % capacity;

        for (int i = 0; i < capacity; ++i) {
            size_t current = (idx + i * step) % capacity;
            if (!table[current].isActive) break;
            if (table[current].name == name) {
                table[current].isActive = false;
                return true;
            }
        }
        return false;
    }

    void print() const {
        for (int i = 0; i < capacity; ++i) {
            std::cout << "[" << i << "]: ";
            if (table[i].isActive) {
                std::cout << table[i].name << " - " << table[i].trophies << " trophies";
            }
            else {
                std::cout << "Empty";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    HashTable warriors;

    warriors.insert("Piper", 1000);
    warriors.insert("Surge", 897);
    warriors.insert("Rico", 848);
    warriors.insert("Spike", 1008);
    warriors.insert("Clancy", 1312);
    std::cout << "Warriors Hash Table:\n";
    warriors.print();

    return 0;
}
