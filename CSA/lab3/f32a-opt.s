     \ n = 90000
     \ if n <= 0:
     \     print(-1)
     \ print((n // 2) * ((n // 2) + 1))

    .data

input_addr:      .word  0x80
output_addr:     .word  0x84

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

    2/                       \ m = n // 2; [m]
    dup lit 1 +              \ [m, m + 1]

    \ m * (m + 1), В СТОЛБИК
    a!
    lit 0

    \ Mul each bit
    lit 31
    >r
mul_loop:
    +*
    next mul_loop

    a

    exit ;


exit:
    @p output_addr a! !
    halt
