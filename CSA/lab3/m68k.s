
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

input_size: .word 0x40
input_terminator: .byte '\n', 0, 0, 0

.org             0x100
    .text

; A5 - input_addr
; A6 - output_addr
; D7 - input_terminator

_start:
    ; load inuput and outpu registers
    movea.l  input_addr, A5
    movea.l  (A5), A5
    movea.l  output_addr, A6
    movea.l  (A6), A6

    movea.l input_terminator, A0
    move.l (A0), D7

    ; movea.l input_size, A0
    ; move.l (A0), D1
    loop:
    move.l   (A5), D0

    ; if (ch == '\n') break;
    cmp.b D7, D0
    beq loop_exit

    ; print
    move.l   D0, (A6)

    jmp loop

    loop_exit:

    
    halt
