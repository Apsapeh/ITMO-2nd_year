
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

input_addr:      .word  0x80               ; Input address where the number 'n' is stored
output_addr:     .word  0x84               ; Output address where the result should be stored

input_limit: .word 0x40
input_terminator: .byte '\n', 0, 0, 0

stack_top: .word 0x1000

    .text
.org             0x100

; A5 - input_addr
; A6 - output_addr
; D7 - input_terminator

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
    link A6, -400 ; 300 bytes for some variables
    ; some code with variables
    ; link A5, -168 ; struct ReadedString
    move.l -168, D7
    jsr read_input

    ; if (line is empty) {exit()}
    bne m_line_is_ok
    ; TODO: impl 0xCCCCCCCC
    m_line_is_ok:

    move.l -168(A6), D0
    move.l 1, D1
    move.l 0, D2

    ; D7 - Line iter
    ; D3 - Current char
    move.l -164, D7
    m_loop:
    move.l 0(A6, D7), D3
    beq m_loop_exit

    ; ch = line[i]
    ; if ch != p_ch || cnt == 9 || next_ch == '\0':
    ;     if print_cnt == 40:
    ;         exit(0xCCCCCC)
    ;     else:
    ;         print(cht .. ch)
    ; else:
    ;     cnt += 1

    cmp.l D3, D0
    bne m_new_char
    cmp.l 9, D1
    beq m_new_char
    add.l 1, D1
    cmp.l 0, 4(A6, D7)
    beq m_new_char
    jmp m_old_char

    m_new_char:
    move.l D1, -328(A6, D2)
    move.l D0, -324(A6, D2)
    move.l 1, D1
    move.l D3, D0
    add.l 8, D2
    cmp.l 328, D2
    bne m_r_not_over
    ; TODO: impl
    m_r_not_over:
    
    m_old_char:
    
    add.l 4, D7
    jmp m_loop
    m_loop_exit:


    movea.l output_addr, A5
    movea.l (A5), A5

    move.l 0, D3
    m_print_loop:
    cmp.l D2, D3
    beq m_print_loop_exit

    move.l -328(A6, D3), D0
    add.l '0', D0
    move.l D0, (A5)
    move.l -324(A6, D3), (A5)
    
    add.l 8, D3
    jmp m_print_loop
    m_print_loop_exit:

    
    ; ... some code with ReadedString and variables
    ; unlk A5 ; drop ReadedString
    ; some code with variables
    unlk A6
    rts



; struct ReadedString (42 words = 168 bytes)
;     - <0>   word str[41] - 40-words null-terminated string
;     - <164> word is_eof

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
        move.l D3, 164(A6, D7)

    rts
