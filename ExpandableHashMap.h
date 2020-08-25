//
//  ExpandableHashMap.hpp
//  proj4
//
//  Created by Janice Tsai on 3/4/20.
//  Copyright © 2020 Janice Tsai. All rights reserved.
//

#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <utility>

// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    unsigned int getBucketNumber(const KeyType& key) const {
      /*  std::hash<std::string> str_hash;
        unsigned int hashValue = str_hash(key);
        unsigned int bucketNum = hashValue % numOfBuckets;
        return bucketNum;*/
        
        unsigned int hasher(const KeyType& k); // prototype
        unsigned int h = hasher(key);
        return h % numOfBuckets;
    }
    std::vector<std::list<std::pair<KeyType, ValueType>>> myMap;
    size_t mapSize;
    int numOfBuckets;
    double maxLoadFactor;
};

template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor) : myMap(8), mapSize(0), maxLoadFactor(maximumLoadFactor) {
    numOfBuckets = 8;
}

template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap() {
    // do i need anything here??
}

template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset() {
    myMap.clear();
    mapSize = 0;
    numOfBuckets = 8;
    myMap.resize(numOfBuckets);
}

template <typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const { // returns number of associations in hashmap
    return mapSize;
}

template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
    unsigned int b = getBucketNumber(key);
    if (find(key) != nullptr) { // means that key already in hashmap
        ValueType* v = find(key); // local so that's probably why it is not working?
        *v = value; // update value for that key value
    } else { // not already in hashmap so add pair and increase mapSize
        myMap[b].emplace_back(key, value);
        mapSize++;
    }
    
    // if necessary, rehash
    if (static_cast<double>(mapSize) / numOfBuckets > maxLoadFactor) {
        std::vector<std::list<std::pair<KeyType, ValueType>>> tempMap(numOfBuckets * 2); // double size
        numOfBuckets = numOfBuckets * 2;
        std::swap(tempMap, myMap);
        
        for (std::list<std::pair<KeyType, ValueType>>& bucket : tempMap) {
            for (auto& [k, v] : bucket) {
                myMap[getBucketNumber(k)].emplace_back(k, v);
            }
        }
    }
}

template <typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const {
    int bucketNum = getBucketNumber(key);
    for (const auto &itPair : myMap[bucketNum]) { // only check at that particular bucketNum
        if (itPair.first == key) {
            return &itPair.second;
        }
    }
    // not found so return nullptr
    return nullptr;
}
//#endif /* ExpandableHashMap_h
