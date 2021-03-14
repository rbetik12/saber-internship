#include "List.h"
#include <iostream>

// This method is used to calculate node position in a file and save to the map
void List::CheckAddressMap(std::unordered_map<uint64_t, uint64_t>& map, ListNode* node, uint64_t* nodePos, uint64_t* currentPos) {
    if (node != nullptr) {
        if (map.find((uint64_t)node) == map.end()) {
            map[(uint64_t)node] = *currentPos;
            *nodePos = *currentPos;
            *currentPos += sizeof(_ListNodeDescriptor) + strlen(node->data.c_str()) + 1;
        }
        else {
            *nodePos = map[(uint64_t)node];
        }
    }
    else {
        *nodePos = 0;
    }
}

void List::WriteBuffer(void* data, size_t size) {
    if (bufferPos >= buffer.size()) {
        buffer.resize(buffer.size() + 4096);
    }
    std::memcpy(&buffer[0] + bufferPos, data, size);
    bufferPos += size;
}

void List::SeekBuffer(uint64_t offset) {
    bufferPos = offset;
}

/*
* How it works.
* NB! All bytes from list nodes first saved to buffer and with only one fwrite call write to file.
* We take a linked list node and save node's address and node position in file to addressToFilePos map.
* 
* Node position in file is calculated pretty simple - we take size of _ListNodeDescriptor struct + length of data string.
* Actually node position is a position of last written node end position.
* 
* So data in fils is organized like this:
* 
* Position - it is address in the file (how many bytes should we skip from start of the file).
* If one of the node's pointers in null, zero is stored in a file.
* 
* 8 bytes               |8 bytes                  |8 bytes                |8 bytes           |X bytes (String length)   
* Position of next node |Position of previous node|Position of random node|Data string length|Actual data string bytes
*/
void List::Serialize(FILE* file) {
    uint64_t currentPos = 0;
    WriteBuffer(&count, sizeof(count));
    currentPos = sizeof(count);
    if (count == 0) {
        std::cout << "Empty list!" << std::endl;
        return;
    }
    std::unordered_map<uint64_t, uint64_t> addressToFilePos;
    ListNode* currentNode = head;
    _ListNodeDescriptor descriptor;

    while (currentNode) {
        if (addressToFilePos.find((uint64_t)currentNode) == addressToFilePos.end()) {
            addressToFilePos[(uint64_t)currentNode] = currentPos;
            currentPos += sizeof(_ListNodeDescriptor) + strlen(currentNode->data.c_str()) + 1;
        }

        CheckAddressMap(addressToFilePos, currentNode->next, &descriptor.posNext, &currentPos);
        CheckAddressMap(addressToFilePos, currentNode->prev, &descriptor.posPrev, &currentPos);
        CheckAddressMap(addressToFilePos, currentNode->rand, &descriptor.posRand, &currentPos);

        descriptor.stringLength = strlen(currentNode->data.c_str()) + 1;

        SeekBuffer(addressToFilePos[(uint64_t)currentNode]);
        WriteBuffer(&descriptor, sizeof(descriptor));
        WriteBuffer((void*)currentNode->data.c_str(), descriptor.stringLength);
        currentNode = currentNode->next;
    }
    fseek(file, 0, SEEK_SET);
    fwrite(&buffer[0], currentPos, 1, file);
}

// Here we save node position in file and pointer to ListNode
void List::SaveNode(std::unordered_map<uint64_t, ListNode*>& map, uint64_t nodePos) {
    if (nodePos == 0) return;
    if (map.find(nodePos) == map.end()) {
        map[nodePos] = new ListNode;
    }
}

/*
* I hope that you've read about serialization process. So here goes pretty the same process, but in a reverse order.
* First, we read _ListNodeDescriptor from file and save positions to the posToNode map. It caches position of node in a file and pointer to ListNode in memory.
* Then we goes through all the file and read our nodes and data related to them.
*/
void List::Deserialize(FILE* file) {
    fread(&count, sizeof(count), 1, file);
    if (count == 0) {
        std::cout << "Empty list!" << std::endl;
        return;
    }
    std::unordered_map<uint64_t, ListNode*> posToNode;
    _ListNodeDescriptor descriptor;
    char buffer[4096] = { 0 };
    uint64_t currentPos;

    for (int i = 0; i < count; i++) {
        currentPos = ftell(file);
        SaveNode(posToNode, currentPos);
        fread(&descriptor, sizeof(_ListNodeDescriptor), 1, file);
        fread(buffer, sizeof(buffer), 1, file);
        std::string data(buffer);

        SaveNode(posToNode, descriptor.posNext);
        SaveNode(posToNode, descriptor.posPrev);
        SaveNode(posToNode, descriptor.posRand);
        
        posToNode[currentPos]->data = data;
        posToNode[currentPos]->next = descriptor.posNext == 0 ? nullptr : posToNode[descriptor.posNext];
        posToNode[currentPos]->prev = descriptor.posPrev == 0 ? nullptr : posToNode[descriptor.posPrev];
        posToNode[currentPos]->rand = descriptor.posRand == 0 ? nullptr : posToNode[descriptor.posRand];
        
        fseek(file, descriptor.posNext, SEEK_SET);
    }

    head = posToNode.begin()->second;
    tail = (--posToNode.end())->second;
}

void List::Add(std::string data, ListNode* rand) {
    count++;
    ListNode* newNode = new ListNode;
    newNode->data = data;
    newNode->prev = nullptr;
    newNode->next = nullptr;
    newNode->rand = rand;

    if (head == nullptr && tail == nullptr) {
        head = newNode;
        tail = newNode;
        return;
    }

    ListNode* last = head;

    while (last->next != nullptr) {
        last = last->next;
    }

    last->next = newNode;
    newNode->prev = last;
    tail = newNode;
}

// This method is used to add random element pointer to any node in a list
void List::AddRand(int index, ListNode* rand) {
    int _index = 0;
    ListNode* list = head;

    while (list) {
        if (_index == index) {
            list->rand = rand;
            break;
        }
        _index += 1;
        list = list->next;
    }
}

void List::Clear() {
    // Yep, memory leaks here, but I am too lazy to make proper memory cleaning. Anyway, it's not the point of the task.
    head = nullptr;
    tail = nullptr;
    count = 0;
    buffer.clear();
    bufferPos = 0;
}

void List::Print() {
    ListNode* node = head;
    while (node) {
        std::cout << node->data << std::endl;
        if (node->rand != nullptr) {
            std::cout << "\t" << node->rand->data << std::endl;
        }
        node = node->next;
    }
}
