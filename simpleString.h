#ifndef MATRIX_H
#define MATRIX_H
#include <cstddef> // std::size_t
#include <cstring>
#include <utility>
#define npos static_cast<std::size_t>(-1)

class SimpleString
{
protected:
    static std::size_t instances_;
    char* data_;
    std::size_t size_;
    std::size_t capacity_;
public:
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    char* data() const { return data_; }
    const char* c_str() const { return data_; }

    static std::size_t instances() {return instances_;}

    SimpleString() : data_(new char[1]{'\0'}), size_(0), capacity_(0)
    {
        data_ = new char[1];
        data_[0] = '\0';
        ++instances_;
    }
    SimpleString(const char* text) : size_(std::strlen(text)), capacity_(size_ + 1)
    {
        data_ = new char[capacity_];
        std::memcpy(data_, text, size_ + 1);
        ++instances_;
    }
    SimpleString(const SimpleString& other) : size_(other.size_), capacity_(other.capacity_)
    {
        data_ = new char[capacity_];
        std::memcpy(data_, other.data_, size_ + 1);
        ++instances_;
    }
    ~SimpleString()
    {
        if (data_) {
            delete[] data_;
            --instances_;
        }
    }

    void assign(const char* new_text)
    {
        std::size_t new_size = std::strlen(new_text);
        if (new_size + 1 > capacity_)
        {
            char* new_data = new char[new_size + 1];
            delete[] data_;
            data_ = new_data;
            capacity_ = new_size + 1;
        }
        std::memcpy(data_, new_text, new_size + 1);
        size_ = new_size;
    }
    int equal_to(const SimpleString& other, bool case_sensitive=true) const
    {
        if (case_sensitive) {return strcmp(data(), other.data()) == 0;}
        if(size_ != other.size_) {return false;}
        for (std::size_t i = 0; i < size_; ++i)
        {
            if (tolower(data_[i]) != tolower(other.data_[i])){return false;}
        }
        return true;
    }
    void append(const SimpleString& other)
    {
        std::size_t new_size = size_ + other.size_;
        if (new_size + 1 > capacity_)
        {
            capacity_ = new_size + 1;
            char* new_data = new char[capacity_];
            std::memcpy(new_data, data_, size_);
            delete[] data_;
            data_ = new_data;
        }
        std::memcpy(data_ + size_, other.data_, other.size_ + 1);
        size_ = new_size;
    }

    SimpleString substr(size_t pos = 0, size_t count = npos) const
    {
        if (pos >= size_) {return SimpleString();}
        count = (count == npos || pos + count > size_) ? size_ - pos : count;
        SimpleString result;
        result.assign(std::string(data_ + pos, count).c_str());
        return result;
    }

    int compare(const SimpleString& other, bool case_sensitive = true) const
    {
        if (case_sensitive) {return std::strcmp(data_, other.data_);}
        for (std::size_t i = 0; i < size_ && i < other.size_; ++i) {
            char c1 = std::tolower(data_[i]);
            char c2 = std::tolower(other.data_[i]);
            if (c1 != c2) return c1 - c2;
        }
        return size_ - other.size_;
    }

    SimpleString(SimpleString&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = new char[1]{'\0'};
        other.size_ = 0;
        other.capacity_ = 1;
        ++instances_;
    }
};

#endif // MATRIX_H
