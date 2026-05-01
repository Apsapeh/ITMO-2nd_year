
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

input_limit:      .word 256
input_terminator: .word '\n',

overflow_err:     .word 0xCCCCCCCC

stack_top: .word 0x1000

; struct ReadedString (66 words = 264 bytes)
;     - <0>   word str[65] - 0x40-words null-terminated string
;     - <260> word is_eof
sizeof_ReadedString:    .word 264


    .text
.org             0x100


_start:
    movea.l stack_top, A7
    movea.l (A7), A7
    jsr main
    
    halt

; A6 - Stack
; D0 - Prev char
; D1 - Prev char counter
; D2 - Result size
main:
    ; 0..263   - ReadedString
    ; 264..519 - word out_buff[64] (256 bytes)
    link A6, -520

    ; Ну это бред такое писать... 4 инструкции вместо одной
    movea.l sizeof_ReadedString, A0
    move.l (A0), D0
    neg.l D0
    move.l D0, D7                        
    jsr read_input


    move.l 1, D1
    move.l 0, D2
    move.l -264(A6), D0

    ; if (line is empty) {exit()}
    beq m_exit

    ; if (ReadedString.is_eof) {exit_overflow();}
    cmp.b 0, -4(A6)
    beq overflow_exit
     
    ; D7 - Line iter, started from i=1
    ; D3 - Current char
    ; Main compression algorithm
    move.l -260, D7
    m_loop:
    ; while ch != 0
    move.l 0(A6, D7), D3
    beq m_loop_exit

    ; ch = line[i]
    ; if ch != p_ch || cnt == 9:
    ;     if print_cnt == 62:
    ;         exit(0xCCCCCC)
    ;     else:
    ;         print(cht .. ch)
    ; else:
    ;     cnt += 1
    
    cmp.l D3, D0
    bne m_new_char
    cmp.b 9, D1
    beq m_new_char
    add.b 1, D1
    jmp m_new_char_over

    m_new_char:
    ; Store pair
    move.l D1, -520(A6, D2)
    move.l D0, -516(A6, D2)
    ; Update prev_char and counter
    move.l 1, D1
    move.l D3, D0
    ; Shift and check output buffer
    add.l 8, D2
    cmp.l 248, D2 ; reserve 8 bytes to end
    beq overflow_exit
    m_new_char_over:
    
    add.l 4, D7
    jmp m_loop
    m_loop_exit:

    ; Add last pair
    move.l D1, -520(A6, D2)
    move.l D0, -516(A6, D2)
    add.l 8, D2

    ; Print pairs
    movea.l output_addr, A5
    movea.l (A5), A5

    ; Output buffer iter
    move.l 0, D3  
    m_print_loop:
    ; while iter != buf_len
    cmp.l D2, D3
    beq m_print_loop_exit

    move.l -520(A6, D3), D0
    add.b '0', D0
    move.l D0, (A5)
    move.l -516(A6, D3), (A5)

    add.l 8, D3 ; Shift iter
    jmp m_print_loop
    m_print_loop_exit:

    m_exit:

    unlk A6
    rts


overflow_exit:
    movea.l output_addr, A5
    movea.l (A5), A5
    movea.l overflow_err, A0
    move.l (A0), (A5)
    halt


; read_value(out<A6 + D7>: &ReadedString)
; A6 - foreign stack
; D7 - Forign stack offset
; 
; A2 - Input
; D6 - Input terminator '\n'
; D5 - Foreign stack offset + input_limit
; D4 - Counter. for(; D4 < D5; D4 += 4)
read_input:
    ; Init registers
    movea.l input_addr, A2
    movea.l (A2), A2

    movea.l input_terminator, A0
    move.l (A0), D6

    movea.l input_limit, A0
    move.l (A0), D5
    add.l D7, D5
    
    move.l D7, D4

    ; D3 - value of out->is_eof
    ri_loop:
        ; if (i == input_limit) {out->is_eof = false; break;}
        cmp.l D5, D4
        bne not_limit
        move.l 0, D3
        jmp ri_loop_exit
        not_limit:
    
        move.l (A2), D0
    
        ; if (ch == '\n') {out->is_eof = true; break;};
        cmp.b D6, D0
        bne not_terminator
        move.l 1, D3
        jmp ri_loop_exit
        not_terminator:
    
        move.l D0, 0(A6, D4)
        
        add.l 4, D4
        jmp ri_loop

    ri_loop_exit:
        move.l 0, 0(A6, D4) ; null-terminator
        move.l D3, 260(A6, D7) ; is_eof flag

    rts
