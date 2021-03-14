#include <string>
#include <unordered_map>

struct _ListNodeDescriptor {
    uint64_t posNext;
    uint64_t posPrev;
    uint64_t posRand;
    size_t stringLength;
};

struct ListNode {
    ListNode* prev;
    ListNode* next;
    ListNode* rand;
    std::string data;
};

class List {
public:
    void Serialize(FILE* file);
    void Deserialize(FILE* file);
    void Add(std::string data, ListNode* rand);
    void AddRand(int index, ListNode* rand);
    void Clear();
    void Print();

    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int count = 0;
private:
    void CheckAddressMap(std::unordered_map<uint64_t, uint64_t>& map, ListNode* node, uint64_t* nodePos, uint64_t* currentPos);
    void SaveNode(std::unordered_map<uint64_t, ListNode*>& map, uint64_t nodePos);
    void WriteBuffer(void* data, size_t size);
    void SeekBuffer(uint64_t offset);

    std::vector<char> buffer;
    uint64_t bufferPos = 0;
};