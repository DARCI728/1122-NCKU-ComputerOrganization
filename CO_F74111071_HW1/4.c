#include <stdio.h>
int main() {
    int i = 0;
    int h[9] = {0}, x[6] = {0}, y[6] = {0};
    FILE *input = fopen("../input/4.txt", "r");
    for (i = 0; i < 9; i++) fscanf(input, "%d", &h[i]);
    for (i = 0; i < 6; i++) fscanf(input, "%d", &x[i]);
    for (i = 0; i < 6; i++) fscanf(input, "%d", &y[i]);
    fclose(input);

    int *p_x = &x[0];
    int *p_h = &h[0];
    int *p_y = &y[0];

    asm volatile(
        "li     s7, 2\n\t"  // 2
        "li     s8, 3\n\t"  // 3
        "li     s4, 0\n\t"  // i = 0
        "loop_i:\n\t"
        "li     s5, 0\n\t"  // j = 0

        "loop_j:\n\t"
        "li     s6, 0\n\t"  // f = 0

        "loop_f:\n\t"
        "slli   t1, s4, 2\n\t"       // 4 * i
        "slli   t2, s5, 2\n\t"       // 4 * j
        "slli   t3, s6, 2\n\t"       // 4 * f
        "mul    t4, t1, s8\n\t"      // 3 * i
        "slli   t5, t1, 1\n\t"       // 2 * i
        "slli   t6, t3, 1\n\t"       // 2 * f
        "add    s1, t4, t3\n\t"      // 3i + f
        "add    s2, t6, t2\n\t"      // 2f + j
        "add    s3, t5, t2\n\t"      // 2i + j
        "add    t1, s1, %[h]\n\t"    // address of h[3i + f]
        "add    t2, s2, %[x]\n\t"    // address of x[2f + j]
        "add    t3, s3, %[y]\n\t"    // address of y[2i + j]
        "lw     t4, 0(t1)\n\t"       // value of h[3i + f]
        "lw     t5, 0(t2)\n\t"       // value of x[2f + j]
        "lw     t6, 0(t3)\n\t"       // value of y[2i + j]
        "mul    t0, t4, t5\n\t"      // h[3i + f] * x[2f + j]
        "add    t0, t0, t6\n\t"      // y[2i + j] + h[3i + f] * x[2f + j]
        "sw     t0, 0(t3)\n\t"       // y[2i + j] += h[3i + f] * x[2f + j]
        "addi   s6, s6, 1\n\t"       // f++
        "blt    s6, s8, loop_f\n\t"  // if f < 3, then loop_f

        "addi   s5, s5, 1\n\t"       // j++
        "blt    s5, s7, loop_j\n\t"  // if j < 2, then loop_j

        "addi   s4, s4, 1\n\t"       // i++
        "blt    s4, s8, loop_i\n\t"  // if i < 3, then loop_i
        :
        : [h] "r"(h), [x] "r"(x), [y] "r"(y));
    p_y = &y[0];
    for (i = 0; i < 6; i++)
        printf("%d ", *p_y++);
    printf("\n");
    return 0;
}
