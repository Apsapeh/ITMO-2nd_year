
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

in_value: .word 0x0
total: .word 0x0

    .text

_start:
    @p input_addr a! @
    

    \ if n <= 0:
    \     return -1
    dup
    if leq_check
    dup
    -if over_leq_check
    leq_check:
        lit -1
        exit ;
    over_leq_check:

    
    >r
    lit 0
    loop_start:
    next loop_body
    exit ;

    loop_body:
        r> dup >r
        lit 1
        +
        +
    loop_start ;
    
exit:
    @p output_addr a! !
    halt
