# SONOGRAM
> Programming language compiler written in C++17 that translates its source code to C++17.

## Introduction
> this design is my first ever language design attempt,
> so current wording can be ambiguous, misleading, and confusing.

> `(idea)` indicates features that are still under consideration and may not be implemented.

> `(temp)` indicates features that are temporarily included for testing purposes and may be removed later.

## Roadmap
> Not rushed; eight-year (starting from 2025) plan for release.

- **2025-2029**: _Alpha versions_;
- **2029-2032**: _Beta versions_;
- **2032-2033**: _Pre-release_;
- **2033-08-08**: _Official release_.

## Comments
> single line comments start with `# ` or with copyright symbol `Ⓒ ` (U+24B8) followed by space

> multi-line comments start with ~~(`#:` and end with `;#`)~~

> if multi-line supported the entire last line containing the closing `;#`
> is always ignored—no code or tokens after `;#` are processed.
> This prevents comment injection, unlike C++ multi-line comments (`/* ... */`)

## Keywords
> starts with `-` symbol, `false` and `true` not supported as they does not fit well with keyword style,
> use integer 0b0 and 0b1, or 0 and 1 instead

| keyword | Description  |
|---------|--------------|
|`program`| Main entry point (executable application) |
|`library`| Interface and implementation of components |
|`package`| (idea) Collection of libraries (with visibility control?) |
|`func`   | Function declaration and definition with return value |
|`proc`   | (idea) Procedure (void function) |
|`void`   | (idea) Uninitialized marker to indicate uninitialized value |
|`move`   | (idea) Right hand variable specifier for move semantics (like C++ std::move) |
|`args`   | (idea) Built-in array of command-line arguments |
|`echo`   | (temp) Built-in function to print to standard output |
|`exit`   | Built-in variable to return value from `-func` and `-program` |
|`case`   | Conditional branching (replaces traditional `if` and `else-if`) |
|`else`   | Traditional else branching |

### Types (built-in, keywords continued)
> initialization uses `{}` appended to the type (e.g., `var_name-real{8.64}`),
> assignment appends to the variable name (e.g., `var_name{9.81}`),
> that allows differentiating initialization from assignment.

> (idea) For single-argument initialization and assignment,
> the `{}` brackets may be omitted (e.g., `var_name-type value` instead of `var_name-type{value}`).
> This is only allowed when there is exactly one argument.
> Omitting `{}` for multi-argument cases is not permitted.
> Note: Allowing this may lead to mistakes, so support for this feature is unclear.

> The `flag` type is designed for future research;
> currently, only the least significant bit is used for logic,
> but other bits may be used for context-specific status or flags.

| type | Description |
|------|-------------|
|`size`| Platform-specific unsigned integer (4 bytes on 32-bit, 8 bytes on 64-bit) |
|`long`| (idea) Platform-specific signed integer (twice the size of `-size`) |
|`int0`| Signed 1 byte integer |
|`int1`| Signed 2 byte integer |
|`int2`| Signed 4 byte integer |
|`int3`| Signed 8 byte integer |
|`unt0`| Unsigned 1 byte integer |
|`unt1`| Unsigned 2 byte integer |
|`unt2`| Unsigned 4 byte integer |
|`unt3`| Unsigned 8 byte integer |
|`real`| 8 byte floating-point |
|`flag`| (idea) Specific 1 byte boolean (by default "logic" bit is lsb) other bits context dependent |
|`char`| Character (utf-8)? |
|`void`| (idea) Special uninitialized value declaration marker |
|`cast`| (idea) Special marker that allows implicit type conversion in parameter declaration or argument passing |
|`auto`| (idea) Type is deduced from initialization expression |

Array type is declared by appending `[]` to any built-in type (e.g., `int2[]`).

### Specifiers (for parameters and variables, keywords continued)
> no specifier means value copy, (only one specifier is allowed per parameter)

| specifier | Description |
|-----------|-------------|
| `out`     | Reference mutable type |
| `ptr`     | Pointer mutable type (cannot be const and point to const values, opposite to `-ref`) |
| `ref`     | Reference constant type (always const, opposite to `-ptr`) |
| `fwd`     | (idea) Forwarding mutable type (move semantics from C++, not sure to support it) |

## Literals
> simply for most part equivalent to C/C++ literals

| literal | Description |
|---------|-------------|
| ' '     | Character literal, single quotes, e.g., `'A'`, `'\n'`, `'\x41'` |
| " "     | String literal, double quotes, e.g., `"Hello, World!\n"` |
| 42      | Integer literals, decimal (e.g., `42`), hexadecimal (e.g., `0x2A`), binary (e.g., `0b101010`) |
| 3.14    | Real number literals with decimal point (e.g., `3.14`, `0.001`, `2.0`) |
| 0b0/0b1 | (idea) Boolean true and false for `-flag` type |
| 0-ptr   | (idea) Null pointer literal for pointer types |

## Conversions
> without -cast, all conversions must be explicit and compatible types enforced at compile time.

> that's why `-cast` specifier are allowed in almost all possible variants like

```son
a-cast-int{ b },                    # a variable are allowed to be implicitly converted in all contexts
test-cast-proc( a-int0, b-int0 ):;  # parameters allowed to be implicitly converted in all contexts
test-proc( a-cast-int0, b-int0 ):;  # only a parameter allowed to be implicitly converted in all contexts
case-cast ( expression ) : ;        # all variables in expression allowed to be implicitly converted
b-int3{ c-cast },                   # c allowed to be implicitly converted when assigning to b
```

The -cast specifier relaxes restrictions on type conversions based on context:,
- on type declaration it make it fully relaxed for any read or write operation,
- on variable directly it relaxes conversion for that one read operation only
- on function declaration it relaxes all function parameters for any operation (and on return -exit?),
- on conditional (like -case-cast) it relaxes all variables in that expression for any operation.

## Operators
> almost all operators are two characters for consistency,
> except basic mathematical ones `+`, `-`, `*`, and `/`

### Arithmetic
> no unary ~~plus~~ and ~~minus~~ (hate those guys)

Added: unary operators are allowed! Just unary operators like:
`-`, `+`, `!` should appear as as suffix (instead of prefix like in C/C++)
so  son `a!` equal to `!a` in C++.

Reason: almost all operators like `()` (function call) `[]` (array subscript)
`{}` initialization, `.` class member access that solely operate on single
argument are suffix ones, so making unary operators be also suffix brings more
CONSISTENCY.

| arithmetic | Description |
|------------|-------------|
| `+`        | Addition binary, `a + b` |
| `-`        | Subtraction binary, `a - b` |
| `*` \| `**`| Multiplication binary; or compound assignment `a ** b` same as `a{ a * b }` |
| `/` \| `//`| Division binary or compound assignment `a // b` same as `a{ a / b }` |
| `++`pre    | (idea) Prefix `++pre a` same as `pre{ pre + a }`; if `a` omitted, defaults to `1` |
| `--`pre    | (idea) Prefix `--pre a` same as `pre{ pre - a }`; if `a` omitted, defaults to `1` |
| post`++`   | (idea) Postfix `post++ a` returns original value, then increments by `a` (defaults to `1` if omitted) |
| post`--`   | (idea) Postfix `post-- a` returns original value, then decrements by `a` (defaults to `1` if omitted) |

### Equality
> **Important**: not confuse `<<` and `>>` with C++ bitwise shift operators (see bitwise operators)

| equality | Description |
|----------|-------------|
| `==`      | Equal, true if both equal |
| `!=`      | Not equal, true if both different |
| `!<`      | Not less-than, same as `>=` |
| `!>`      | Not greater-than, same as `<=` |
| `<=`      | Less-than or equal |
| `>=`      | Greater-than or equal |
| `<<`      | Less-than, **Important** see bitwise operators |
| `>>`      | Greater-than, **Important** see bitwise operators |

### Logical and Bitwise
> All bitwise operators start with `~` symbol to differentiate from logical operators.

| logical | Description |
|---------|-------------|
| `&&`    | Logical AND: true if both true |
| `\|\|`  | Logical OR: true if either true |
| `^^`    | Logical XOR: true if only one true |
| `~&`    | Bitwise AND |
| `~\|`   | Bitwise OR |
| `~^`    | Bitwise XOR |
| `~<`    | Bitwise shift left |
| `~>`    | Bitwise shift right |

## Control flow
> The `-case` keyword is used for all conditional branching, replacing traditional `if` and `else-if`. There is no `-if` keyword.

> reading uninitialized built-in `-exit` variable causes compile-time error,
> so return from function without value is disallowed in `-func` and allowed in `-proc`

- `case`: Combined if/else-if
- block of code: : `code` ; same as { `code` } in C/C++
- exit: return value, basically a built-in (function local) `uninitialized` variable, formally like -exit-void for procedure only

## Example (TODO improve)
> symbol `-` cannot be used in identifiers (only for keywords),

```son
# sonogram here is program name and also special function (main entry point),
# so basically -program is a -func that returns integer
sonogram-program: # -exit built-in for -program func should be initialized with zero by default?
    # function (procedure) that swaps two real numbers by mutable reference
    swap-proc(param_a-out-real, param_b-out-real): # can use spaces if preferred swap -proc, param_a -out -real, and so on
        -case param_a == param_b: -exit;
        temp-auto{param_a}, # can be written as temp-auto param_a
        param_a param_b, # same as param_a{param_b}
        param_b{temp}, # this comma can be omitted, just allowed trailing comma example
    ;
    # function that returns minimum of two signed 8 byte integers
    min-func-int3(lhs-cast-int3, rhs-cast-int3): # allow implicit type conversion in parameters via -cast specifier
        -case lhs << rhs: -exit{lhs}; # operator << is less-than
        -case-flag{0b1}:; # do nothing, boolean usage example
        -exit{rhs};
    ;

    args-args, # copy of built-in command line arguments, better usage args-out-args for mutable reference
    arg1-real{args[0]}, # same as arg1-real{-args[0]} using built-in array directly
    arg2-cast-unt3{args[1]}, # variable arg2 allowed to make implicit type conversion via -cast specifier

    -case min(arg1, arg2) == arg1: swap(arg1, arg2); # min parameters allow casts, as arg2 variable when passing to any function
    -exit{-int3{arg1}} # need explicit type cast for return integer value into environment, as arg1 is real without -cast
;
Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
```

###### each folder MAY contain README with additional information about project
