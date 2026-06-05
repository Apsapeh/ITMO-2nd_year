; def hello_user_pstr(input):
;     """Greet the user with Pascal string: ask the name and greet by `Hello, <name>!` message.

;     - Result string with greet message should be represented as a correct Pascal string.
;     - Buffer size for the message -- `0x20`, starts from `0x00`.
;     - End of input -- new line.
;     - Initial buffer values -- `_`.

;     Python example args:
;         input (str): The input string containing the user's name.

;     Returns:
;         tuple: A tuple containing the greeting message and the remaining input.
;     """
;     line, rest = read_line(input, 0x20 - len("Hello, " + "!") - 1)

;     q = "What is your name?\n"
;     if not line:
;         return [q, overflow_error_value], rest

;     greet = "Hello, " + line + "!"
;     return q + greet, rest


; assert hello_user_pstr('Alice\n') == ('What is your name?\nHello, Alice!', '')
; # and mem[0..31]: 0d 48 65 6c 6c 6f 2c 20 41 6c 69 63 65 21 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f
; assert hello_user_pstr('Bob\n') == ('What is your name?\nHello, Bob!', '')
; # and mem[0..31]: 0b 48 65 6c 6c 6f 2c 20 42 6f 62 21 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f 5f


    .data

out_buff:            .word 0, 0, 0, 0, 0, 0, 0, 0
out_buff_size:       .byte 0x20
out_buff_init_value: .byte '_'

hello:               .byte 7, 'Hello, '
mark:                .byte 1, '!'
quest:               .byte 19, 'What is your name?\n'

input_addr:          .word 0x80
output_addr:         .word  0x84               

input_terminator:    .byte '\n'

overflow_death:      .word 0xCCCCCCCC
stack_top:           .word 0x1000

    .text
.org 0x100

_start:
    ; Set stack top
    lui  sp, %hi(stack_top)
    addi sp, sp, %lo(stack_top)
    lw   sp, 0(sp)

    addi sp, sp, -32

    jal  ra, init_out_buff

    addi a0, sp, 0                 ; Stack: 0..31 - byte uname[0x20]
    jal  ra, read_line_or_halt

    addi a0, sp, 0
    jal  ra, print_to_out_buff

    lui  a0, %hi(quest)
    addi a0, a0, %lo(quest)
    jal  ra, print

    lui  a0, %hi(out_buff)
    addi a0, a0, %lo(out_buff)
    jal  ra, print

    addi sp, sp, 32
    halt

; void init_out_buff()
init_out_buff:
    ; t0 - out_buff_pointer
    lui  t0, %hi(out_buff)
    addi t0, t0, %lo(out_buff)

    ; t1 - out_buff_size
    lui  t1, %hi(out_buff_size)
    addi t1, t1, %lo(out_buff_size)
    lb   t1, 0(t1)
    addi t1, t1, -1                         ; First byte is len 

    ; t2 - out_buff_init_value ('_')
    lui  t2, %hi(out_buff_init_value)
    addi t2, t2, %lo(out_buff_init_value)
    lb   t2, 0(t2)
    
    ; t4 - counter
    mv t4, zero
    iob_while:
        beq t4, t1, iob_while_end
    
        ; out_buff[t4] = '_'
        add t5, t0, t4 
        sb  t2, 1(t5)
    
        addi t4, t4, 1
        j    iob_while
    iob_while_end:
    
    jr ra

; void read_line_or_halt(byte* buff_ptr: a0)
read_line_or_halt:
    ; t0 - input_addr
    lui  t0, %hi(input_addr)
    addi t0, t0, %lo(input_addr)
    lw   t0, 0(t0)

    ; t1 - input_terminator
    lui  t1, %hi(input_terminator)
    addi t1, t1, %lo(input_terminator)
    lb   t1, 0(t1)

    ; t2 - input_limit (out_buff_size - hello.len - mark.len - 1) aka i
    ; temp used: t3
    lui  t2, %hi(hello)
    addi t2, t2, %lo(hello)
    lb   t2, 0(t2)

    lui  t3, %hi(mark)
    addi t3, t3, %lo(mark)
    lb   t3, 0(t3)

    add  t3, t2, t3
    addi t3, t3, 1

    lui  t2, %hi(out_buff_size)
    addi t2, t2, %lo(out_buff_size)
    lb   t2, 0(t2)
    sub  t2, t2, t3

    ; t3 - i
    mv t3, zero

    rloh_while:
        ; if (t2 <= t3) exit(0xCCCCCCCC);
        ble t2, t3, overflow_exit

        ; ch = read()
        lw t4, 0(t0)

        ; if (ch == input_terminator) break;
        beq t4, t1, rloh_while_end

        ; buff_ptr[i] = ch
        add t5, a0, t3
        sb  t4, 1(t5)
    
        addi t3, t3, 1
        j    rloh_while
    rloh_while_end:
    
    sb   t3, 0(a0)

    jr ra


;  fn overflow_exit() -> !
overflow_exit:
    ; print question
    lui  a0, %hi(quest)
    addi a0, a0, %lo(quest)
    jal  ra, print

    
    ; t0 - output_addr
    lui  t0 , %hi(output_addr)
    addi t0, t0, %lo(output_addr)
    lw   t0, 0(t0)

    ; t1 - overflow_death
    lui  t1, %hi(overflow_death)
    addi t1, t1, %lo(overflow_death)
    lw   t1, 0(t1)

    sw t1, 0(t0)
    halt

    
; void print_to_out_buff(byte const* uname: a0)
print_to_out_buff:
    addi sp, sp, -12
    sw   ra, 0(sp)
    sw   a0, 4(sp) ; uname_ptr

    lui  a0, %hi(out_buff)
    addi a0, a0, %lo(out_buff)
    sw   a0, 8(sp) ; out_buff_ptr

    ; strcat(out_buff, hello)    
    lui  a1, %hi(hello)
    addi a1, a1, %lo(hello)
    jal  ra, strcat

    ; strcat(out_buff, uname)
    lw   a0, 8(sp)
    lw   a1, 4(sp)
    jal  ra, strcat

    ; strcat(out_buff, mark)
    lw   a0, 8(sp)
    lui  a1, %hi(mark)
    addi a1, a1, %lo(mark)
    jal  ra, strcat

    lw   ra, 0(sp)
    addi sp, sp, 12
    jr   ra


; void print(byte const* data: a0)
print:
    ; t0 - output_addr
    lui  t0 , %hi(output_addr)
    addi t0, t0, %lo(output_addr)
    lw   t0, 0(t0)

    ; t1 - data.end
    lb t1, 0(a0)
    add t1, t1, a0

    print_while:
        ; if (data == data.end) break;
        beq a0, t1, print_while_end
        
        lb t2, 1(a0)
        sb t2, 0(t0)

        addi a0, a0, 1
        j print_while
    print_while_end:

    jr ra


; void strcat(byte* dest: a0, byte* src: a1)
strcat:
    ; t0 - old dest.len
    ; t1 - src.len
    ; t2 - new dest.len
    ; update size
    lb  t0, a0
    lb  t1, a1
    add t2, t0, t1
    sb  t2, 0(a0)

    ; sync indices -> dest = dest + old dest.len
    ; drop t0
    add  a0, a0, t0

    ; t0 - i
    mv  t0, zero
    strcat_while:
        ; if (i == src.ln) break;
        beq t0, t1, strcat_while_end

        ; dest[i] = src[i]
        add t3, a1, t0
        lb  t3, 1(t3)
        add t4, a0, t0
        sb  t3, 1(t4)
        
        addi t0, t0, 1
        j    strcat_while
    strcat_while_end:
    
    jr ra

