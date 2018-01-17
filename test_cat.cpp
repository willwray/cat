//#include <cassert>

#include "cat.hpp"

// constexpr operator== specialisation for char array
template <size_t A, size_t B>
constexpr bool operator==(std::array<char,A> const&,
                          std::array<char,B> const&)
{ return false; }

template <size_t N>
constexpr bool operator==(std::array<char,N> const& a,
                          std::array<char,N> const& b)
{
    for (size_t i=0; i<N; ++i)
        if (a[i]!=b[i])
            return false;
    return true;
}

int main()
{
    using namespace ltl;

    // Remember - cat truncates a C char array without a null check

    // Oh heck, where did the 'o' of hello go? Oh yeah, truncated.
    static_assert( cat((char[5]){'h','e','l','l','o'})
           == std::array<char,4>{'h','e','l','l'});

    { // Test cat with single char-array argument

        // Prepare char arrays hi ho
        constexpr char hi[] {"hello"};
        constexpr char ho[] {'h','e','l','l','o','!'}; // not null terminated
        static_assert(std::size(hi)==6);
        static_assert(std::size(ho)==6);
        constexpr auto hello = std::array<char,5>{'h','e','l','l','o'};

        // cat char array -> std::array, truncates always, null or no
        constexpr auto chi = cat(hi);
        constexpr auto cho = cat(ho);
        static_assert(chi.size()==5);
        static_assert(cho.size()==5);
        static_assert(chi == hello);
        static_assert(cho == hello);

        // cat std::array -> std::array idempotent; doesn't truncate
        constexpr auto chic = cat(chi);
        static_assert(chic.size()==5);
        static_assert(chic == hello);
    }

    { // Test impl:: size & data functions

        // test character literal argument
        static_assert(impl::size('a') == 1);
        static_assert(*impl::data('a') == 'a');

        // test character lval argument
        char d = 'd';
        static_assert(impl::size(d) == 1);
        //static_assert(*impl::data(d) == 'd'); // non-constant
        constexpr char e = 'e';
        static_assert(*impl::data(e) == 'e');

        // test character rval argument
        auto f = []{return 'f';};
        static_assert(impl::size(f()) == 1);
        static_assert(*impl::data(f()) == 'f');

        // test character array literal argument
        static_assert(impl::size("") == 0);
        static_assert(impl::size("1") == 1);

        // test character array lval argument
        char a[] = "";
        static_assert(impl::size(a) == 0);
        char b[] = "1";
        static_assert(impl::size(b) == 1);

        // test character array rval-ish argument
        static_assert(impl::size((char[1]){}) == 0);
    }
    { // Test single-argument cat

        // test cats producing zero length array
        constexpr auto n0 = cat();
        constexpr auto n1 = cat("");
        static_assert( n0==n1);
        static_assert( n0.size()==0 );

        // test cats producing single-element array
        constexpr auto ac = cat('a');
        constexpr auto as = cat("a");
        static_assert( ac==as);
    }
    {
        constexpr auto abcc = cat('a','b');
        constexpr auto abca = cat('a',"b");
        constexpr auto abac = cat("a",'b');
        constexpr auto abaa = cat("a","b");
        constexpr std::array<char,2> ab{'a','b'};
        static_assert(abcc == ab);
        static_assert(abca == ab);
        static_assert(abac == ab);
        static_assert(abaa == ab);
    }
    {
        // Antisocial compile timing tests

        //constexpr std::array<char,65536> K64{};
        //constexpr auto K256 = cat(K64, K64, K64, K64); // 1.1s
        //constexpr auto K256 = K64 + K64 + K64 + K64; // 3.3s
        //static_assert(K256.size() == 262144);

        // 256K is the default constexpr loop limit
        //constexpr std::array<char,262142> G{};
        // This takes 8s to compile:
        //constexpr auto G8 = cat('g',G,"gg",G,"gg",G,"gg",G,"gg",G,"gg",G,"gg",G,"gg",G,'g');
        // This equivalent line fails after 22s - it blows the loop limit
        //constexpr auto G2 = 'g'+G+"gg"+G+"gg"+G+"gg"+G+"gg"+G+"gg"+G+"gg"+G+"gg"+G+'g';
        //static_assert(G8.size() == 262144*8);
    }
}