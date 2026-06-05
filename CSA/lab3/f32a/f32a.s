
     \ def sum_even_n(n):
     \     """Calculate the sum of even numbers from 1 to n"""
     \     if n <= 0:
     \         return -1
     \     total = 0
     \     for i in range(1, n + 1):
     \         if i % 2 == 0:
     \             total += i
     \     return total
     \
     \ assert sum_even_n(5) == 6
     \ assert sum_even_n(10) == 30
     \ assert sum_even_n(90000) == 2025045000


    .data

input_addr:      .word  0x80
output_addr:     .word  0x84
word_max_value:  .word  0x7FFFFFFF

.org 0x88
    .text

_start:
    @p input_addr a! @

    \ if n <= 0:
    \     return -1
    dup if leq_check
    dup -if over_leq_check
leq_check:
    lit -1
    exit ;
over_leq_check:

    >r                       \ range(n)
    lit 0                    \ total = 0
loop:
    r> dup >r \ i
    iter
    next loop
    exit ;                   \ End of program


iter: \ (total, i) -> total_new
    dup check_odd
    if is_even
    drop
    ;

    is_even:
    +
    \ Overflow checking
    dup a!
    inv 1 + \  -(total + i)
    @p word_max_value  \ 2^31 - 1
    + 
    -if not_overflow
    overflow_exit ;
    not_overflow:
    a
    ;


check_odd:
    0x00000001 and
    ;


overflow_exit:
    0xCCCCCCCC
    exit ;


exit:
    @p output_addr a! !
    halt
