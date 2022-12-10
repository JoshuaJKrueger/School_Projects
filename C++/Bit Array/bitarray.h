#include <algorithm>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cctype>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

template <class IType = std::size_t>
class BitArray
{
    enum
    {
        BITS_PER_BLOCK = CHAR_BIT * sizeof(IType),
    };

    class bitproxy
    {
        BitArray &b;
        std::size_t pos;

    public:
        bitproxy(BitArray &bs, std::size_t p) : b(bs), pos(p){};

        bitproxy &operator=(bool bit)
        {
            b.assign_bit(pos, bit);

            return *this;
        }

        bitproxy &operator=(const bitproxy &bp)
        {
            b = bp.b;

            return *this;
        }

        operator bool() const { return b.read_bit(pos); }
    };

    std::vector<IType> bit_blocks;
    std::size_t biu{0};

    double get_blocks(std::size_t sz) const
    {
        auto dv{std::div(sz, BITS_PER_BLOCK)};

        return dv.quot + dv.rem / static_cast<double>(BITS_PER_BLOCK);
    }

    bool read_bit(std::size_t bit_pos) const
    {
        if (!in_bounds(bit_pos))
            throw std::logic_error("Out of bounds.");

        std::size_t block_pos{static_cast<std::size_t>(floor(get_blocks(bit_pos)))};
        std::size_t rel_bit_pos{bit_pos - block_pos * BITS_PER_BLOCK};

        return bit_blocks.at(block_pos) & 1ULL << rel_bit_pos;
    }

    void assign_bit(std::size_t bit_pos, bool val, bool check_range = true)
    {
        if (!in_bounds(bit_pos) && check_range)
            throw std::logic_error("Out of bounds.");

        std::size_t block_pos{static_cast<std::size_t>(floor(get_blocks(bit_pos)))};
        std::size_t rel_bit_pos{bit_pos - block_pos * BITS_PER_BLOCK};

        if (val)
            bit_blocks.at(block_pos) |= 1ULL << rel_bit_pos;
        else
            bit_blocks.at(block_pos) &= ~(1ULL << rel_bit_pos);
    }

    bool in_bounds(std::size_t pos) const { return pos < biu; }

    void trim()
    {
        for (int i{static_cast<int>(capacity()) - 1}; i >= biu; --i)
            assign_bit(i, 0, false);
    }

public:
    // Object Management
    explicit BitArray(std::size_t alloc_sz = 0)
    {
        bit_blocks.resize(ceil(get_blocks(alloc_sz)));
        biu = alloc_sz;
    }

    explicit BitArray(const std::string &s)
    {
        // Throw runtime error if string has anything except 1s and 0s
        // Empty string is accepted
        std::size_t res{s.find_first_not_of("01")};
        if (res != std::string::npos)
            throw std::runtime_error(s.at(res) + " is not allowed. Only 0s and 1s, or an empty string.");

        bit_blocks.resize(ceil(get_blocks(s.length())));
        biu = s.length();

        std::string rev{s};
        std::reverse(rev.begin(), rev.end());

        for (std::size_t i{0}; i < rev.length(); ++i)
            assign_bit(i, rev.at(i) == '1');
    }

    BitArray(const BitArray &b) = default;            // Copy constructor
    BitArray &operator=(const BitArray &b) = default; // Copy assignment

    BitArray(BitArray &&b) noexcept : bit_blocks(b.bit_blocks), biu(b.biu) // Move constructor
    {
        b.bit_blocks.clear();
        b.biu = 0;
    }

    BitArray &operator=(BitArray &&b) noexcept // Move assignment
    {
        if (&b == this)
            return *this;

        bit_blocks.clear();
        biu = 0;

        bit_blocks = b.bit_blocks;
        biu = b.biu;

        b.bit_blocks.clear();
        b.biu = 0;

        return *this;
    }

    std::size_t capacity() const { return bit_blocks.size() * BITS_PER_BLOCK; } // # of bits the current allocation can hold

    // Mutators
    BitArray &operator+=(bool b) // Append a bit
    {
        if (biu++ == capacity())
            bit_blocks.resize(bit_blocks.size() + 1);

        *this <<= 1ULL;
        assign_bit(0, b);

        return *this;
    }

    BitArray &operator+=(const BitArray &b) // Append a BitArray
    {
        BitArray a{b};

        for (int i{static_cast<int>(a.biu) - 1}; i >= 0; --i)
            *this += a.read_bit(i);

        return *this;
    }

    void erase(std::size_t pos, std::size_t nbits = 1) // Remove “nbits” bits at a position
    {
        if (!in_bounds(pos) || !in_bounds(pos + (nbits - 1)))
            throw std::logic_error("Out of bounds.");

        std::string s{to_string()};
        s.erase(pos, nbits);
        *this = BitArray(s);
    }

    void insert(std::size_t bit_pos, bool val) // Insert a bit at a position (slide "right")
    {
        if (biu == capacity())
            bit_blocks.resize(bit_blocks.size() + 1);

        if (!in_bounds(bit_pos))
            throw std::logic_error("Out of bounds.");

        std::string s{to_string()};
        s.insert(bit_pos, 1, val ? '1' : '0');
        *this = BitArray(s);
    }

    void insert(std::size_t pos, const BitArray &b) // Insert an entire BitArray object
    {
        for (int i{static_cast<int>(b.biu) - 1}; i >= 0; --i)
            insert(pos, b.read_bit(i));
    }

    // Bitwise ops
    bitproxy operator[](std::size_t pos)
    {
        if (!in_bounds(pos))
            throw std::logic_error("Out of bounds.");

        return bitproxy(*this, pos);
    }

    bool operator[](std::size_t pos) const { return read_bit(pos); }

    void toggle(std::size_t bit_pos)
    {
        if (!in_bounds(bit_pos))
            throw std::logic_error("Out of bounds.");

        std::size_t block_pos{static_cast<std::size_t>(floor(get_blocks(bit_pos)))};
        std::size_t rel_bit_pos{bit_pos - block_pos * BITS_PER_BLOCK};

        bit_blocks.at(block_pos) ^= 1ULL << rel_bit_pos;
    }

    void toggle() // Toggles all bits
    {
        for (std::size_t i{0}; i < biu; ++i)
            toggle(i);
    }

    BitArray operator~() const
    {
        BitArray ret{*this};
        ret.toggle();

        return ret;
    }

    // Shift operators do not resize, data loss can occur
    BitArray operator<<(unsigned int shifts) const // Shift operators...
    {
        BitArray ret{*this};
        // ret.bit_blocks[0] <<= 1ULL;
        for (auto it{ret.bit_blocks.rbegin()}; it != ret.bit_blocks.rend(); ++it)
            *it <<= shifts;

        ret.trim();

        return ret;
    }

    BitArray operator>>(unsigned int shifts) const
    {
        BitArray ret{*this};

        for (auto it{ret.bit_blocks.rbegin()}; it != ret.bit_blocks.rend(); ++it)
            *it >>= shifts;

        ret.trim();

        return ret;
    }

    BitArray &operator<<=(unsigned int shifts)
    {
        for (auto it{bit_blocks.rbegin()}; it != bit_blocks.rend(); ++it)
            *it <<= shifts;

        trim();

        return *this;
    }

    BitArray &operator>>=(unsigned int shifts)
    {
        for (auto it{bit_blocks.rbegin()}; it != bit_blocks.rend(); ++it)
            *it >>= shifts;

        trim();

        return *this;
    }

    // Extraction ops
    BitArray slice(std::size_t pos, std::size_t count) const // Extracts a new sub-array
    {
        BitArray ret{};
        for (int i{static_cast<int>(count - 1)}; i >= 0; --i)
            ret += read_bit(pos + i);

        return ret;
    }

    // Comparison ops
    // Compare lexicographically
    bool operator==(const BitArray &b) const { return std::string{to_string()} == std::string{b.to_string()}; }
    bool operator!=(const BitArray &b) const { return !(*this == b); }
    bool operator<(const BitArray &b) const { return std::string{to_string()} < std::string{b.to_string()}; }
    bool operator<=(const BitArray &b) const { return std::string{to_string()} <= std::string{b.to_string()}; }
    bool operator>(const BitArray &b) const { return std::string{to_string()} > std::string{b.to_string()}; }
    bool operator>=(const BitArray &b) const { return std::string{to_string()} >= std::string{b.to_string()}; }

    // Counting ops
    std::size_t size() const { return biu; } // Number of bits in use in the vector

    std::size_t count() const // The number of 1-bits present
    {
        std::size_t count{0};

        for (std::size_t i{0}; i < biu; ++i)
            if (read_bit(i))
                count++;

        return count;
    }

    bool any() const // Optimized version of count() > 0
    {
        for (const auto &block : bit_blocks)
            if (block > 0)
                return true;

        return false;
    }

    // Stream I/O
    friend std::ostream &operator<<(std::ostream &os, const BitArray &b)
    {
        os << b.to_string();

        return os;
    }

    friend std::istream &operator>>(std::istream &is, BitArray &b)
    {
        // Take only 1s and 0s leave other chars in stream
        // set the stream to a fail state if there are
        //   no valid bit characters to be read in the input stream
        //   (after skipping white space, of course)
        std::string s{""};
        char c;
        while (is.get(c))
        {
            if (c != '1' && c != '0')
            {
                is.unget();

                if (std::isspace(c))
                    is.seekg(is.tellg() += 1);
                else
                    break;
            }
            else
                s += c;
        }

        bool has_valid = false;
        std::streampos pos{is.tellg()};
        while (c != -1)
        {
            c = is.peek();

            if (c == '1' || c == '0')
            {
                has_valid = true;
                break;
            }

            is.seekg(is.tellg() += 1);
        }

        is.seekg(pos);

        if (!has_valid)
            is.setstate(std::ios::failbit);

        if (!s.empty())
            b = BitArray{s};

        return is;
    }

    // String conversion
    std::string to_string() const
    {
        std::string ret{""};
        for (std::size_t i{0}; i < biu; ++i)
            ret += read_bit(i) ? '1' : '0';

        std::reverse(ret.begin(), ret.end());

        return ret;
    }
};
