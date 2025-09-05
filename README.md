# Jui
Jui is a general purpose Turing Complete minimal programming language in Bangla. This is written fully in C with the help of a parser generator (https://github.com/Irtiaz/IrParseTableGenerator and https://github.com/Irtiaz/IrParser) and a lexer generator (https://github.com/Irtiaz/IrLex)

## Official Website and Online Playground
Visit the [Official Website](https://jui-7cof.onrender.com/) and the [online playground](https://jui-7cof.onrender.com/playground)

## Installation
The source code (`main.c`) and the parse table file (`parse-table.txt`) along with the lexical analysis rules file (`rules.txt`) has to be put together and made with a C compiler (a sample `Makefile` is provided). The grammar for the language is provided (`language-grammar.txt`). If you want to bring some changes to the grammar you are welcome to do so. Make sure to compile the grammar with the parse table generator (https://github.com/Irtiaz/IrParseTableGenerator)

## Examples
The `examples/` directory contains different examples that you can compile and try yourself. Similar examples exist in the [Official Website](https://jui-7cof.onrender.com/)

## Syntax
### variables
Variable usage is just like C. Currently all the variables are integers.
Remember, every variable is an array. `a = 0` would just mean `a[0] = 0.`The sizes of the arrays can grow indefinitely. This is a perfectly valid code - 
```
a = 10
a[100] = 4
```
This would mean the array `a` has 10 in it's beginning and 4 at index 100. Indexing starts at 0. Note that the code mentioned above is analogous to
```
a[0] = 10
a[100] = 4
```
Memory allocation and deallocation is completely **automatic**!!

### operators
Most operators are like C. Except for the relational operators and the modulo operator
Modulo operator is done by the keyword `vagshesh`
`a < b` is replaced by `a, b er choto`
`a == b` is replaced by `a, b er soman`
`a > b` is replaced by `a, b er boro`

That's all! Wait then how do we write `!=` or `<=`?
`a <= b` can be written as `a, b er boro na`
`a >= b` can be written as `a, b er choto na`
`a != b` can be written as `a, b er soman na`
Basically the `!` operator is replaced by the keyword `na`
So a C expression like `!(!(a < b))` would look like `a, b er choto na na` in Jui

Increment or decrement operators and bitwise operators are not supported

### output and input
write an expression to the terminal with the `likho` command. For example
```
likho (a + 2) vagshesh 10
```
read a variable from the terminal with the `poro` command. For example
```
poro a[2]
```

### conditional
The syntax for a conditional statement is explained below with an example 
```
jodi a, 10 er soman hoy taile
	a = a + 5
koro
noile jodi 2, 3 er choto na hoy taile
	a = 0
koro
ar noile
	a = 1
koro
```
conditions can be joined together with `othoba` for logical OR and `ebong` for logical AND

### loops
The syntax for a loop is - 
```
jotokkhon <condition> thake totokkhon
	<statements>
koro
```
for example,
```
jotokkhon i, n er choto thake totokkhon/
	i = i + 1
koro
```


