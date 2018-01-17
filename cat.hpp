#pragma once

#include <array>
//#include <iterator>

namespace ltl
{

// Constexpr concatentation of char, string literal and char sequence types
// such as std::array<char> with constexpr size() and data() free functions

namespace impl
{
// Implement free size() and data() for char
// Specialise size() for string literal - override to ignore null terminator
// constexpr copy_n for char sequences

// size(char) -> 1 overload to complement std::size
constexpr size_t size(char const&) { return 1U; }

// data(char) -> char* overload to complement std::data
constexpr const char* data(char const& c) { return &c; }

// size(char[N]) overrides std::size(char[N]) as more specialised
// ***Assumes char[N] is string literal*** ***STRIPS ZERO TERMINATOR***
template <size_t N>
constexpr size_t size(char const (&)[N]) { return N-1; }

constexpr char* copy_n(const char* cs, size_t n, char* p)
{
    for (size_t i=0; i<n; ++i) *(p+i) = *(cs+i);
    return p + n;
}
} // namespace impl


// Returns std::array<char> concatenation of the input character sequences
template <typename... Cs>
constexpr
auto cat(Cs const&... cs)
{
    using impl::size;
    using impl::data;
    using std::data;
    std::array<char, (0U + ... + size(cs))> ret{{}};
    if constexpr (ret.size())
    {
        char* p = ret.data();
        ((p = impl::copy_n(data(cs), size(cs), p)), ...);
    }
    return ret;
}

// Operator+ for car, array<char>, string literal & char.
// At least one arg must be class type - not both string literal or char.
// Prefer cat(...) over + ... + for multi args if efficiency > readability.
// ToDo: requires constraints
template <typename A, typename B>
constexpr
auto operator+(A const& a, B const& b)
{
    return cat(a,b);
}

} // namespace ltl
