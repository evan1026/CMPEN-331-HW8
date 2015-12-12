#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <bitset>

#define WAYS 4
#define SETS 64

unsigned int Cache_Set(unsigned int);
unsigned int Cache_Tag(unsigned int);
bool Cache_Access(unsigned int);
unsigned int Victim(unsigned int);
unsigned int readInt(std::ifstream&);

struct cache_bits {
    unsigned char tag;
    unsigned int counter;
    bool valid;
};

cache_bits cache[WAYS][SETS];
int counter = 0;

unsigned int Cache_Set(unsigned int address) {
    return (address & 0b0000000011111100) >> 2;
}

unsigned int Cache_Tag(unsigned int address) {
    return (address & 0b1111111100000000) >> 8;
}

bool Cache_Access(unsigned int address) {
    ++counter;

    unsigned int cache_set = Cache_Set(address);
    unsigned int cache_tag = Cache_Tag(address);

    //First, let's look for a hit
    for (int i = 0; i < WAYS; ++i) {
        if (cache[i][cache_set].valid && cache[i][cache_set].tag == cache_tag) {
            cache[i][cache_set].counter = counter; //if it's a hit, just update the counter
            return true; //Hit -> return true
        }
    }

    //No hit, gonna have to put it in then

    //First, let's find where to put it
    int way = Victim(cache_set);

    //Now put it there
    cache[way][cache_set].tag = cache_tag;
    cache[way][cache_set].counter = counter;
    cache[way][cache_set].valid = true;

    return false; //Not a hit -> return false
}

//I've modified this function from the specification so that it will also
//return the first invalid block, if there is one
//This way, finding the block can be done all in one function
//It still works as specified, but with an extra feature
unsigned int Victim(unsigned int cache_set) {
    int lru = 0;
    for (int i = 0; i < WAYS; ++i) {
        if (!cache[i][cache_set].valid) return i;

        if (cache[i][cache_set].counter < cache[lru][cache_set].counter)
            lru = i;
    }
    return lru;
}

unsigned int readInt(std::ifstream& file) {
    unsigned int a;
    file >> a;
    return a;
}

int main() {

    //Initialize cache
    for (int i = 0; i < WAYS; ++i) {
        for (int j = 0; j < SETS; ++j) {
            cache[i][j].valid = false; //since we test for validity before reading, no need to initialize the rest
        }
    }

    //Initialize variables
    std::ifstream inputFile("input.txt");
    int hitCount = 0;
    int count = 0;

    //Error handling
    if (!inputFile.is_open()) {
        std::cout << "Could not open \"input.txt\". Please make sure the file exists and that you have read permissions for it." << std::endl;
        return 1;
    }

    //Time to start reading
    unsigned int address = readInt(inputFile);
    while (inputFile.good()) { //Basically, while the last read operation succeeded, do the next stuff
        address = address & 0xffff; //ensure only 16 bits
        int tag = Cache_Tag(address); //Get tag
        int set = Cache_Set(address); //Get set
        int bof = address & 0x0003; //byte offset

        //Prints out debug info splitting the bits into parts - you can ignore it
        std::cout << std::bitset<16>(address) << "(" << address << "): " 
            << std::bitset<8>(tag) << "(" << std::setw(3) << (tag) << ") "
            << std::bitset<6>(set) << "(" << std::setw(2) << (set) << ") " 
            << std::bitset<2>(bof) << "(" << bof << ")";

        ++count;

        //Determine if it's a hit or miss
        if (Cache_Access(address)) {
            ++hitCount;
            std::cout << " hit";
        } else {
            std::cout << " miss";
        }
        std::cout << std::endl;

        //Get the next one
        address = readInt(inputFile);
    }

    //Print out final numbers
    int percentage = (int)round((((float)hitCount) / count) * 100);
    std::cout << std::endl;
    std::cout << "Number of cache accesses: " << count << std::endl;
    std::cout << "Number of cache hits:     " << hitCount << std::endl;
    std::cout << "Hit percentage:           " << percentage << "%" << std::endl;
}
