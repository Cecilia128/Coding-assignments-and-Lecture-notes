#include "strarray.h"
#include <vector.h>

StrArray::StrArray()
{
    void * uninitialized_memory =
            ::operator new(sizeof(std::string) * DEFAULT_CAPACITY);
    usage = start = reinterpret_cast<std::string *>(uninitialized_memory);
    end = usage + DEFAULT_CAPACITY;
}

StrArray::~StrArray()
{
    // First call `std::destroy` to destruct all elements in the memory area [start, usage).
    std::destroy(start, usage);
    // Then, deallocate the memory area with `::operator delete`
    ::operator delete(start);
}

StrArray::size_type StrArray::size() const noexcept
{
    return usage - start;
}

StrArray::size_type StrArray::capacity() const noexcept
{
    return end - start;
}

void StrArray::grow() {
    std::string * original_start = start;
    std::string * original_usage = usage;
    std::string * original_end = end;
    int capa = capacity();
    // Create new uninitialize_memory with twice capacity
    void * uninitialized_memory = ::operator new(sizeof(std::string) * capa * 2);
    // Updata new start, usage, and end pointer
    start = reinterpret_cast<std::string *>(uninitialized_memory);
    usage = start + capa;
    end = start + capa*2;
    // Move the elements in the original memory to the new memory
    std::uninitialized_move(original_start, original_end, start);
    // destruct all elements in the memory area [start, usage).
    std::destroy(original_start, original_usage);
    // deallocate the memory area with `::operator delete`
    ::operator delete(original_start);
}

void StrArray::push_back(const std::string &str)
{
    // when the array is full, call grow() to expand the capacity
    if (usage == end) grow();
    // add a new element to the end of the array;
    new(usage)std::string(str);
    usage = usage + 1;
}

std::string &StrArray::get(StrArray::size_type index)
{
    //if `index >= size()`, call error(msg) to throw an error
    if (index >= size()) error("out of range");
    // get the string at given position
    return *(start+index);
}

void StrArray::set(StrArray::size_type index, const std::string& str)
{
    //if `index >= size()`, call error(msg) to throw an error
   if (index >= size()) error("out of range");
   // set the string at given position
   new(start+index)std::string(str);
}

std::string &StrArray::operator[](StrArray::size_type index)
{
    //if `index >= size()`, call error(msg) to throw an error
    if (index >= size()) error("out of range");
    // get the string at given position
    return *(start+index);
}

PROVIDED_TEST("push_back no grow", STRARRAY) {
    std::vector<std::string> compare;
    StrArray arr;
    for (int i = 0; i < 8; ++i) {
        compare.push_back(std::string {char(48 + i)});
        arr.push_back(std::string {char(48 + i)});
    }
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQUAL(arr.get(i), compare.at(i));
    }
}

PROVIDED_TEST("push_back grow", STRARRAY) {
    std::vector<std::string> compare;
    StrArray arr;
    for (int i = 0; i < 26; ++i) {
        compare.push_back(std::string {char('a' + i)});
        arr.push_back(std::string {char('a' + i)});
    }
    for (int i = 0; i < 26; ++i) {
        EXPECT_EQUAL(arr[i], compare[i]);
    }
}
