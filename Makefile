CSTANDARD := -ansi

_FLAGS   :=-Wall -Wextra -pedantic -Wparentheses -Wformat=2 \
					 -Wshadow -Wwrite-strings -Wredundant-decls -Wmissing-include-dirs \
					 -pedantic-errors -Wuninitialized -Wmissing-declarations \
					 -Wdouble-promotion -Wfloat-equal -Wmain -Wundef
_CFLAGS  :=$(CSTANDARD) -Wstrict-prototypes -Wnested-externs -Wold-style-definition \
					 -Wbad-function-cast -Wno-unknown-pragmas -Wno-unused-function
# GCC warnings that Clang doesn't provide:
ifeq ($(CC),gcc)
	_CFLAGS+=-Wjump-misses-init -Wlogical-op
endif

all:
	cp parse-table.txt rulepack.txt
	cat rules.txt >> rulepack.txt
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386 --add-section .note.GNU-stack=/dev/null rulepack.txt rulepack.o
	gcc *.c rulepack.o -o jui $(_FLAGS) $(_CFLAGS)

debug:
	cp parse-table.txt > rulepack.txt
	cat rules.txt >> rulepack.txt
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386 --add-section .note.GNU-stack=/dev/null rulepack.txt rulepack.o
	gcc *.c rulepack.o -g3 -o jui $(_FLAGS) $(_CFLAGS) -fsanitize=address,null,undefined,alignment,leak
