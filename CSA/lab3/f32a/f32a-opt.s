     \ n = 90000
     \ if n <= 0:
     \     print(-1)
     \ elif n > 92681:
     \     printf(0xCCCCCCCC)
     \ else:
     \     print((n // 2) * ((n // 2) + 1))

    .data

input_addr:      .word  0x80
output_addr:     .word  0x84

max_ok_value:    .word  -92682             \ Real maximum value is 92681, but 92682 just for simlicity
error_value:     .word  0xCCCCCCCC
bad_exit_code:   .word  -1

    .text


_start:
    @p input_addr a! @

    \ if n <= 0:
    \     return -1
    dup if leq_check         \ n == 0, exit
    dup -if over_leq_check   \ n > 0, ok
leq_check:
    @p bad_exit_code
    exit ;
over_leq_check:

    \ check if n <= 92681, cuz larger values cause overflow
    dup @p max_ok_value +
    -if overflow_exit

    2/                       \ m = n // 2; Stack: [m]
    dup lit 1 +              \ Stack: [m, m + 1]
    mul_T_S                  \ m * (m + 1); Stack: [result]

    exit ;



mul_T_S:
    a!                       \ A <- T
    lit 0

    \ Mul each bit
    lit 31 >r
mul_loop:
    +*
    next mul_loop

    drop                     \ lit 0
    drop                     \ S

    a
    ;


fn_check_overflow:
    1 eam
    0 0 + \ Carry -> 
    -if overflow_exit
    0 eam
    ;


overflow_exit:
    @p error_value
    exit ;


exit:
    @p output_addr a! !
    halt
