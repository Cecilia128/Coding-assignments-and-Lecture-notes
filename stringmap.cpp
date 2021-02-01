/*
 * File: stringmap.cpp
 * -------------------
 * This file implements the stringmap.h interface using a hash table
 * with open addressing as the underlying representation.
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include "error.h"
#include "stringmap.h"
#include "testing/SimpleTest.h"
#include "testing/MemoryDiagnostics.h"
using namespace std;


StrMap::StrMap() {
    this->_bucket = new Bucket [ DEFAULT_MAP_CAP ];
    _size = 0;
    _capacity = DEFAULT_MAP_CAP;
}

StrMap::~StrMap() {
    delete [] this->_bucket;
}

StrMap::return_type StrMap::operator[](const string & key) const {
    if (contains(key)) {
        int bucket = hash_value(key) % _capacity;
        int bucket1;
        for (size_t i=0; i<-_capacity; i++) {
            bucket1 = (bucket+i)%_capacity;
            if (this->_bucket[bucket1].key == key) return std::ref(this->_bucket[bucket1].value);
        }
    }
    return std::nullopt;
}

void StrMap::insert(const string &key, const string &value) {

    // If key exists, simply replace the value
    if (contains(key)) {
       int bucket = hash_value(key) % _capacity;
       while (this->_bucket[bucket].key != key) {
           bucket = (bucket+1)%_capacity;
       }
       this->_bucket[bucket].value = value;
   }
    else {
        // if load_factor() >= MAX_LOAD_FACTOR, do rehash() first
        if (load_factor() >= MAX_LOAD_FACTOR) rehash();
        int bucket = hash_value(key) % _capacity;
        // use linear probing and open addressing to locate a new bucket for the key-value pair
        while (this->_bucket[bucket].used) {
            bucket = (bucket+1)%_capacity;
        }
        this->_bucket[bucket].key = key;
        this->_bucket[bucket].value = value;
        this->_bucket[bucket].dummy = false;
        this->_bucket[bucket].used = true;
        _size++;
    }
}

void StrMap::erase(const string &key) {
    // if the key exists, remove the key and mark the slot as dummy
    if (contains(key)) {
        int bucket = hash_value(key) % _capacity;
        int bucket1;
        for (size_t i=0; i<-_capacity; i++) {
            bucket1 = (bucket+i)%_capacity;
            if (this->_bucket[bucket1].key == key) {
                this->_bucket[bucket1].key = "";
                this->_bucket[bucket1].value = "";
                this->_bucket[bucket1].used = false;
                this->_bucket[bucket1].dummy = true;
                _size--;
            }
        }
   }
}

bool StrMap::contains(const string &key) const {
    if (empty()) return false;
    int bucket = hash_value(key) % _capacity;
    int bucket1;
    for (int i=0; i<-_capacity; i++) {
        bucket1 = (bucket+i)%_capacity;
        // if current bucket is NULL, that means key is impossible to appear latter.
        if (!this->_bucket[bucket1].used && !this->_bucket[bucket1].dummy) break;

        // if current bucket is unused and dummy, continue to search
        else if (this->_bucket[bucket1].dummy) continue;

        // if the key of current bucket is key, return true
        else if (this->_bucket[bucket1].key == key) return true;

        // if current bucket is used, but not what we want
        else continue;
    }
    return false;
}

bool StrMap::empty() const {
    if (_size != 0) return false;
    return true;
}

size_t StrMap::size() const {
    return _size;
}

size_t StrMap::capacity() const {
    return _capacity;
}

void StrMap::clear() {
    // clear all hashmap entries and reset capacity to default
   delete [] this->_bucket;
   this->_bucket = new Bucket[DEFAULT_MAP_CAP];
   _capacity = DEFAULT_MAP_CAP;
   _size = 0;
}

void StrMap::rehash() {
   // Grow the hashmap capacity to 3 times
   Bucket* old_bucket = new Bucket[_capacity];
   for (int i=0; i<_capacity; i++) {
       old_bucket[i] = this->_bucket[i];
   }
   _capacity = 3 * _capacity;
   this->_bucket = new Bucket[_capacity];
   _size = 0;
   // rehash all existing entries
   for (int i=0; i<_capacity/3; i++) {
       if (old_bucket[i].used) {
           insert(old_bucket[i].key,old_bucket[i].value);
       }
   }

   // delete original bucket array
   delete [] old_bucket;
}

float StrMap::load_factor() const {
    return static_cast<float>( size() ) / static_cast<float>( capacity() );
}

// Generate random strings for testing
static inline std::string random_string() {
    std::string a {};
    int size = rand() % 255 + 13;
    while ( size -- ) {
        a.push_back( 'A' + rand() % 26 );
    }
    return a;
}

PROVIDED_TEST("strmap insert simple", StrMap) {
    StrMap map;
    map.insert( "A", "A" );
    map.insert( "B", "B" );
    map.insert( "C", "C" );
    EXPECT( !map.contains( "D" ) );
    EXPECT( !map["D"] );
    EXPECT( map.contains( "A" ) );
    EXPECT_EQUAL( map.size(), 3 );
    for ( auto a : { "A", "B", "C" }) {
        EXPECT_EQUAL( map[a]->get(), a );
    }
    // test replacement
    map.insert( "A", "AA" );
    EXPECT_EQUAL( map[ "A" ]->get(), "AA" );

}

PROVIDED_TEST("strmap insert multiple", StrMap) {
    StrMap map;
    std::unordered_map<std::string, std::string> compare;
    int size = rand() % 20000 + 500;
    for ( int i = 0; i < size; ++i) {
        auto a = random_string();
        auto b = random_string();
        compare.insert( { a, b } );
        map.insert(a, b);
    }

    for ( auto & i : compare ) {
        EXPECT( map[ i.first ].has_value() );
        EXPECT_EQUAL( map[ i.first ]->get(), i.second );
        EXPECT_EQUAL( map.contains( i.first ), compare.count( i.first ) > 0 );
    }
    EXPECT_EQUAL( map.size(), compare.size() );
}


PROVIDED_TEST("strmap clear", StrMap) {
    StrMap map;
    std::unordered_map<std::string, std::string> compare;
    for ( int i = 0; i < 111; ++i) {
        auto a = random_string();
        auto b = random_string();
        map.insert(a, b);
    }
    map.clear();
    for ( int i = 0; i < 222; ++i) {
        auto a = random_string();
        auto b = random_string();
        compare.insert( { a, b } );
        map.insert(a, b);
    }
    for ( auto & i : compare ) {
        EXPECT( map[ i.first ].has_value() );
        EXPECT_EQUAL( map[ i.first ]->get(), i.second );
        EXPECT_EQUAL( map.contains( i.first ), compare.count( i.first ) > 0 );
        auto key = random_string();
        EXPECT_EQUAL( map.contains( key ), compare.count( key ) > 0 );
    }
    EXPECT_EQUAL( map.size(), compare.size() );
}

