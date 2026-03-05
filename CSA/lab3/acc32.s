    .data

input_addr:      .word  0x80
output_addr:     .word  0x84

    ; Some consts
v_8:             .word  8
v_24:            .word  24
v_ff:            .word  0xff
v_ff00:          .word  0xff00
v_ff0000:        .word  0xff0000
v_ff000000:      .word  0xff000000

    ; Some variables
const_input_value: .word  0x00               ; Readed value from the input stream
var_tmp:         .word  0x00               ; Temporary buffer between a byte swap operations


    .text

_start:
    ; Read input number to variable 'const_input_value'
    load         input_addr
    load_acc
    store        const_input_value           ;                   12.34.56.78 as example

    ; Move the first byte to the place of the fourth
    ; AF.00.00.00 -> 00.00.00.AF
    shiftr       v_24                        ; acc >> 24;        12.34.56.78 -> **.**.**.12
    and          v_ff                        ; acc & 0xff        **.**.**.12 & 00.00.00.FF -> 00.00.00.12
    store        var_tmp                     ; tmp = acc

    ; 2 -> 3
    ; Move the second byte to the place of the third
    ; 00.AF.00.00 -> 00.00.AF.00
    load         const_input_value
    shiftr       v_8                         ; acc >> 8;         12.34.56.78 >> 8          = **.12.34.56
    and          v_ff00                      ; acc & 0xff00;     **.12.34.56 & 00.00.FF.00 = 00.00.34.00
    or           var_tmp                     ; acc | tmp;        00.00.34.00 | 00.00.00.12 = 00.00.34.12
    store        var_tmp                     ; tmp = acc

    ; 3 -> 2
    ; Move the third byte to the place of the second
    ; 00.00.AF.00 -> 00.AF.00.00
    load         const_input_value
    shiftl       v_8                         ; acc << 8;         12.34.56.78 << 8           = 34.56.78.**
    and          v_ff0000                    ; acc & 0xff0000;   34.56.78.** & 00.FF.00.00  = 00.56.00.00
    or           var_tmp                     ; acc | tmp;        00.56.00.00 | 00.00.34.12  = 00.56.34.12
    store        var_tmp                     ; tmp = acc;

    ; 4 -> 1
    ; Move the fourth byte to the place of the first
    ; 00.00.00.AF -> AF.00.00.00
    load         const_input_value
    shiftl       v_24                        ; acc << 24;        12.34.56.78 << 34          = 78.**.**.**
    and          v_ff000000                  ; acc & 0xff000000; 78.**.**.** & FF.00.00.00  = 78.00.00.00
    or           var_tmp                     ; acc | tmp;        78.00.00.00 | 00.56.34.12  = 78.56.34.12

    ; Write result to the output
    store_ind    output_addr

    halt
