"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"li t1, 0\n\t"

"loop:"
    "addi %[others_cnt], %[others_cnt], 1\n\t"
    "andi t2, t1, 1\n\t"
    
    "addi %[others_cnt], %[others_cnt], 1\n\t"
    "beqz t2, if\n\t"

    "addi %[others_cnt], %[others_cnt], 1\n\t"
    "j else\n\t"

    "if:"
        "addi %[lw_cnt], %[lw_cnt], 1\n\t"
        "li t2, 1\n\t"  

        "addi %[others_cnt], %[others_cnt], 1\n\t"
        "j end_if\n\t"

    "else:"
        "addi %[lw_cnt], %[lw_cnt], 1\n\t"
        "li t2, -1\n\t"
        
        "addi %[others_cnt], %[others_cnt], 1\n\t"
        "j end_if\n\t"

    "end_if:"
        "addi %[others_cnt], %[others_cnt], 1\n\t"
        "slli t3, t1, 1\n\t"
        
        "addi %[add_cnt], %[add_cnt], 1\n\t"
        "addi t3, t3, 1\n\t"
        
        "addi %[others_cnt], %[others_cnt], 1\n\t"
        "fcvt.d.w f1, t2\n\t"
        
        "addi %[others_cnt], %[others_cnt], 1\n\t"
        "fcvt.d.w f2, t3\n\t"
        
        "addi %[div_cnt], %[div_cnt], 1\n\t"
        "fdiv.d f1, f1, f2\n\t"
        
        "addi %[add_cnt], %[add_cnt], 1\n\t"
        "fadd.d %[pi], %[pi], f1\n\t"
        
        "addi %[add_cnt], %[add_cnt], 1\n\t"
        "addi t1, t1, 1\n\t"
        
        "addi %[others_cnt], %[others_cnt], 1\n\t"
        "blt t1, %[N], loop\n\t"