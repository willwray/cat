# `cat` : constexpr string concatenation
#### `#include "cat.hpp"`<br>`namespace ltl;`
`cat` is a standalone header-only mod depending only on `std::array`.<br>
It is grouped with `car` in the [`car_cat`](../../readme.md) collection. It requires C++17.

Mod `cat` contains functions `cat()` and `operator+()`<br>
for compile-time concatenation of constexpr char sequences:

* `cat(...)` : variadic, concatenates char sequences and returns a char array<br>
  * `cat(Cs... cs) -> std::array<char,N>`
* `operator+()` : concatenate two char sequences and return a char array<br>
  * `operator+(Cs cs1, Cs cs2) -> std::array<char,N>`

Where **char sequence** is `array<char,N>`, `char[N]` or `char` (a sequence of 1)<br>
or, generically, a type with constexpr `size()` and `get<>()->char` functions.

`char[N]` input args are assumed to be string literal so the terminating character<br>
is assumed to be null and is truncated without checking - to check it use `car{}`.


### `char*`, `string_view` etc.
* `char*` is not accepted on principle, even if it were possible:
  * string literal is accepted directly as char(&)[N]
* `std::string_view` is not accepted in the current implementation:
  * string_view args fail the constexpr size() requirement in C++17
  * Some future 'constexpr function args' feature may enable this
  * A `car_view` type may be useful here - I'm experimenting

### Examples:
```c++
constexpr auto hello = cat("hello");
```
Returns `std::array<char,5>{'h','e','l','l','o'}` constant.
```c++
constexpr auto hello_world = cat("hello", ',', " world", '!');
```
Returns `std::array<char,13>` containing `hello, world!` with no null terminator.
```c++
constexpr auto hello_world = hello + ',' + " world" + '!';
```
Same as above but using `operator+` so not all arguments can be char or string literal.
<br>Concatentation with `+` is less efficient than with `cat()` for multiple arguments.

### Final Warning:
Remember that `cat` silently truncates a C char array with no null check:
```c++
cat((char[]){'h','e','l','l','o'}).back() == 'l'
```

### Performance notes:
* `cat` is not intended for run-time so compile-time is the appropriate metric
* Originally mplemented with `index_sequence` then switched to looping
* cat size is limited by the constexpr loop iteration count limit
* Compile times increase to seconds for cats  around 256K chars
* `cat()` is faster than `+` for multiple args
* `cat()` is limited by arg size, `+` is limited by subexpression size
* Use `+` only if you prefer its readability over performance
* Run-time use cases are constrained by the need for constexpr `size()`,<br>
 for example 3-char currency-pair stitching "USD"+"CAD" = "USDCAD"
