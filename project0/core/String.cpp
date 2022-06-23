// String.cpp
//
// ICS 46 Spring 2022
// Project #0: Getting to Know the ICS 46 VM
//
// Implement all of your String member functions in this file.
//
// Note that the entire standard library -- both the C Standard
// Library and the C++ Stazdard Library -- is off-limits for this
// task, as the goal is to exercise your low-level implementation
// skills (pointers, memory management, and so on).

#include "String.hpp"
#include "OutOfBoundsException.hpp"

namespace {
    template<typename T>
    void copy_over_array(char* dest, T transfer, unsigned int size){
        for (unsigned int i = 0; i < size; i++){
            dest[i] = transfer[i];
    }
}

}

String::String() {
    m_size = 0;
    m_char_array = new char[1];
    *m_char_array = 0;
}

String::String(const char* chars) {
    const char* index_ptr = chars;
    m_size = 0;
    while(*index_ptr != 0) {
        m_size++;
        index_ptr++;
    }
    m_char_array = new char[m_size+1]; 
    copy_over_array(m_char_array, chars, m_size+1); 
}

String::String(const String& s) 
    : m_size(s.m_size), m_char_array(new char[s.length()+1])
{
    copy_over_array(m_char_array, s.m_char_array, m_size+1);
}

String::~String() noexcept{
    delete[] m_char_array;
}

String& String::operator=(const String& s) {
    if (this != &s) {
        m_size = s.length();
        char* temp = new char[m_size+1];
        copy_over_array(temp, s.m_char_array, m_size+1);
        delete m_char_array;
        m_char_array = temp;
    }
    return *this;
}

void String::append(const String& s) {
    char* copy_array = new char[m_size];
    copy_over_array(copy_array, m_char_array, m_size);
    m_size+=s.length();
    delete[] m_char_array;
    m_char_array = new char[m_size+1];
    copy_over_array(m_char_array, copy_array, m_size-s.length());
    int counter = 0;
    for (unsigned int i = m_size-s.length(); i < m_size+1; i++){
        m_char_array[i] = s.m_char_array[counter];
        counter++;
    }
    delete[] copy_array;
}

char String::at(unsigned int index) const {
    if (index >= m_size){
        throw OutOfBoundsException();
    }
    return m_char_array[index];
}

char& String::at(unsigned int index){
    if (index >= m_size){
        throw OutOfBoundsException();
    }
    char& refchar = m_char_array[index];
    return refchar;
}

void String::clear(){
    //*m_char_array = 0; //m_char_array[0] = 0;
    for (unsigned int i = 0; i < m_size; i++){
        m_char_array[i] = 0;
    }
    m_size = 0;
}

int String::compareTo(const String& s) const noexcept{
    int test_length = m_size;
    if (m_size > s.length())
        test_length = s.m_size;

    for (unsigned int i = 0; i < test_length; i++) {
        if (m_char_array[i] < s.m_char_array[i])
            return -1;
        else if (m_char_array[i] > s.m_char_array[i])
            return 1;
    }

    if (m_size > s.length())
        return 1;
    else if (m_size < s.length())
        return -1;
    return 0;
}

//append modifies
//concatenate returns new string
String String::concatenate(const String& s) const {
    String new_String_obj = *this;
    new_String_obj.append(s);
    return new_String_obj;
}

bool String::contains(const String& substring) const noexcept{
    // if (this->find(substring) == -1) {
    //     return false;
    // }
    // return true;
    if (*m_char_array != 0) {
        if (this->find(substring) != -1)
            return true;
    }
    return false;
}

bool String::equals(const String& s) const noexcept{
    if (this->compareTo(s) == 0){
        return true;
    }
    return false;
}

int String::find(const String& substring) const noexcept{
    if (*m_char_array != 0) {
        for (unsigned int i = 0; i < m_size-substring.length()+1; i++) {
            if ((this->substring(i, i+substring.length())).equals(substring)) {
                return i;
            }
        }
    }
    return -1;

}

bool String::isEmpty() const noexcept {
    if (m_size == 0 && *m_char_array == 0)
        return true;
    return false;
}

unsigned int String::length() const noexcept {
    return m_size;
}

String String::substring(unsigned int startIndex, unsigned int endIndex) const {
    if ((startIndex < m_size+1) && (endIndex < m_size+1)){
        if (startIndex <= endIndex) {
            String new_string = String();
            new_string.m_size = endIndex-startIndex;
            delete[] new_string.m_char_array;
            new_string.m_char_array = new char[m_size+1];
            int slice_index = startIndex;
            for (unsigned int i = 0; slice_index<endIndex; i++)
                new_string.m_char_array[i] = m_char_array[slice_index];
                slice_index++;
            }
            new_string.m_char_array[new_string.length()] = 0;
            return new_string;
        }
        return "";
    }
    throw OutOfBoundsException(); 
}

const char* String::toChars() const noexcept { 
    // char* cstring = nullptr;
    // char* index = cstring;
    // for (int i = 0; i<m_size+1; i++){
    //     *index = m_char_array[i];
    //     index++;
    // }
    // return cstring;
    return m_char_array;
}


