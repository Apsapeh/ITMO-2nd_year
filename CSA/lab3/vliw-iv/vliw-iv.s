     ; def determinant_3x3(*xs):
     ;     """Input: 3x3 matrix in format a_10, a_20, a_30, a_11, ...

     ;     Need to calculate determinant of this matrix
     ;     """
     ;     result = (
     ;           xs[0] * xs[4] * xs[8]
     ;         + xs[1] * xs[5] * xs[6]
     ;         + xs[2] * xs[3] * xs[7]
     ;         - xs[0] * xs[5] * xs[7]
     ;         - xs[1] * xs[3] * xs[8]
     ;         - xs[2] * xs[4] * xs[6]
     ;     )

     ;     if result > 0xFFFFFFFF:
     ;         return [0xCCCCCCCC]

     ;     return [result]


     ; assert determinant_3x3(0, 0, 0, 0, 0, 0, 0, 0, 0) == [0]
     ; assert determinant_3x3(1, 2, 3, 4, 5, 6, 7, 8, 9) == [0]
     ; assert determinant_3x3(0, 0, 1, 0, 1, 0, 1, 0, 0) == [-1]
     ; assert determinant_3x3(7, -5, 4, 32, 8, 3, 5, 2, 8) == [1707]



    .data
input_addr:      .word  0x80               ; Input address where the number 'n' is stored
output_addr:     .word  0x84               ; Output address where the result should be stored

    .text
    .org 0x100
_start:
    ; Load input_addr constant and get the address
    lui s11, %hi(input_addr)       / lui s10, %hi(output_addr)       / nop            / nop
    addi s11, s11, %lo(input_addr) / addi s10, s10, %lo(output_addr) / nop            / nop
    nop                            / nop                             / lw s11, 0(s11) / nop

    ; Load first matrix elements to save registers
    nop                            / nop                             / lw s1, 0(s11)  / nop  ; xs[0] -> s1
    nop                            / nop                             / lw s2, 0(s11)  / nop  ; xs[1] -> s2
    nop                            / nop                             / lw s3, 0(s11)  / nop  ; xs[2] -> s3
    nop                            / nop                             / lw s4, 0(s11)  / nop  ; xs[3] -> s4

    mul t0, s3, s4                 / mul t1, s2, s4                  / lw s4, 0(s11)  / nop  ; t0 = xs[2] * xs[3];    t1 = xs[1] * xs[3];    xs[4] -> s4
    mul t2, s1, s4                 / mul t3, s3, s4                  / lw s4, 0(s11)  / nop  ; t2 = xs[0] * xs[4];    t3 = xs[2] * xs[4];    xs[5] -> s4
    mul t4, s2, s4                 / mul t5, s1, s4                  / lw s4, 0(s11)  / nop  ; t4 = xs[1] * xs[5];    t5 = xs[0] * xs[5];    xs[6] -> s4
    mul t4, t4, s4                 / mul t3, t3, s4                  / lw s4, 0(s11)  / nop  ; t4 =    t4 * xs[6];    t3 =    t3 * xs[6];    xs[7] -> s4
    mul t0, t0, s4                 / mul t5, t5, s4                  / lw s4, 0(s11)  / nop  ; t0 =    t0 * xs[7];    t5 =    t5 * xs[7];    xs[8] -> s4
    mul t2, t2, s4                 / mul t1, t1, s4                  / nop            / nop  ; t2 =    t2 * xs[8];    t1 =    t1 * xs[8];

    ; t2 + t4 + t0 - t5 - t1 - t3 = (t2 - t5) + (t4 - t1) + (t0 - t3)
    sub t2, t2, t5                 / sub t0, t0, t3                  / nop            / nop
    ; t2 + (t4 - t1) + t0 = (t2 + t0) + (t4 - t1)
    add t2, t2, t0                 / sub t4, t4, t1                  / nop            / nop
    ; t2 + t4
    add t2, t2, t4                 / nop                             / lw s10, 0(s10) / nop  ; Also load out_addr from memory to s10

    nop                            / nop                             / sw t2, 0(s10)  / nop
    nop                            / nop                             / nop            / halt
