
     ; def rle_compress(input):
     ;     """Run-length compression: compress consecutive characters.

     ;     Examples:
     ;     - "AAABBBBCCCC" -> "3A4B4C"
     ;     - "aaaaaaaaaa" -> "9a1a" (splits runs > 9)

     ;     - Buffer size for the compressed message -- `0x40`, starts from `0x00`.
     ;     - End of input -- new line.

     ;     Python example args:
     ;         input (str): The input string containing data to compress.

     ;     Returns:
     ;         tuple: A tuple containing the compressed string and the remaining input.
     ;     """
     ;     line, rest = read_line(input, 0x40)
     ;     if line is None:
     ;         return [overflow_error_value], rest

     ;     if not line:
     ;         return "", rest

     ;     try:
     ;         compressed = []
     ;         i = 0
     ;         while i < len(line):
     ;             current_char = line[i]
     ;             count = 1
     ;             while (
     ;                 i + count < len(line) and line[i + count] == current_char and count < 9
     ;             ):
     ;                 count += 1
     ;             compressed.append(str(count) + current_char)
     ;             i += count
     ;         result = "".join(compressed)
     ;         if len(result) + 1 > 0x40:  # +1 for null terminator
     ;             return [overflow_error_value], rest
     ;         return cstr(result, 0x40)[0], rest

     ;     except Exception:
     ;         return [-1], rest


     ; assert rle_compress('AAABBBBCCCC\n') == ('3A4B4C', '')
     ; assert rle_compress('aaaaaaaaaa\n') == ('9a1a', '')
     ; assert rle_compress('ABC\n') == ('1A1B1C', '')



    .data

input_addr:       .word  0x80               ; Input address where the number 'n' is stored
output_addr:      .word  0x84               ; Output address where the result should be stored

input_limit:      .byte  0x40
input_terminator: .byte  '\n'

overflow_err:     .word  0xCCCCCCCC

stack_top:        .word  0x1000

    .text
.org             0x100


_start:
    movea.l stack_top, A7
    movea.l (A7), A7
    jsr     main
    
    halt

; A6 - Stack
main:
    ; 0..63   - byte in_buff[64]
    ; 64..127 - byte out_buff[64]
    link A6, -128

    move.l -65, D7                        
    jsr    read_input

    

    ; D0 - prev char
    ; D1 - prev char_counter
    ; D2 - result_size
    move.b 1, D1
    move.b 0, D2
    move.b -65(A6), D0
    beq    exit

     
    ; Main compression algorithm
    ; D7 - Line iter, started from i=1
    ; D3 - Current char
    move.l -64, D7
    m_loop:
    ; ch = read()
    move.b 0(A6, D7), D3
    ; if ch == 0 {break;}
    beq    m_loop_exit

    ; ch = line[i]
    ; if ch != p_ch || cnt == 9:
    ;     if print_cnt == 62:
    ;         exit(0xCCCCCC)
    ;     else:
    ;         print(cht .. ch)
    ; else:
    ;     cnt += 1
    
    cmp.b D3, D0
    bne m_new_char
    cmp.b 9, D1
    beq m_new_char
    add.b 1, D1
    jmp m_new_char_over

    m_new_char:
    ; Store pair
    move.b D1, -128(A6, D2)
    move.b D0, -127(A6, D2)
    ; Update prev_char and counter
    move.b 1, D1
    move.b D3, D0
    ; Shift and check output buffer
    add.b 2, D2
    cmp.b 62, D2 ; reserve 2 bytes to end
    beq overflow_exit
    m_new_char_over:
    
    add.l 1, D7
    jmp m_loop
    m_loop_exit:

    ; Add last pair
    move.b D1, -128(A6, D2)
    move.b D0, -127(A6, D2)
    add.b 2, D2

    ; Print pairs
    ; A5 - output addr
    movea.l output_addr, A5
    movea.l (A5), A5

    ; movea.l A6, A5

    ; Output buffer iter
    move.b 0, D3  
    m_print_loop:
    ; while iter != buf_len
    cmp.b D2, D3
    beq m_print_loop_exit

    move.b -128(A6, D3), D0
    add.b '0', D0
    move.b D0, (A5)
    move.b -127(A6, D3), (A5)

    add.b 2, D3 ; Shift iter
    jmp m_print_loop
    m_print_loop_exit:

    m_exit:

    unlk A6
    rts


; read_value(byte* out: <A6 + D7>)
; A6 - foreign stack
; D7 - Forign stack offset
read_input:
    ; A2 - Input
    movea.l input_addr, A2
    movea.l (A2), A2

    ; D6 - Input terminator
    movea.l input_terminator, A0
    move.b  (A0), D6

    ; D5 - Input end idx. Foreign stack offset + input_limit
    movea.l input_limit, A0
    move.b  (A0), D5
    add.l   D7, D5

    ; D4 - Input begin idx. for(D4 = D7; D4 < D5; D4 += 1)
    move.l  D7, D4
    ri_loop:
        ; if (i == input_limit) {overflow_exit}
        cmp.l D5, D4
        beq   overflow_exit

        ; ch = read()
        move.l (A2), D0
    
        ; if (ch == '\n') {break;};
        cmp.b  D6, D0
        beq    ri_loop_exit
    
        move.b D0, 0(A6, D4)
        
        add.l 1, D4
        jmp ri_loop

    ri_loop_exit:
        move.b 0, 0(A6, D4) ; null-terminator

    rts

exit:
    halt

overflow_exit:
    movea.l output_addr, A5
    movea.l (A5), A5
    movea.l overflow_err, A0
    move.l (A0), (A5)
    halt
