#include <stdio.h>
int main() {
    int f, i, j;
    int h[9] = {0}, x[6] = {0}, y[6] = {0};
    FILE *input = fopen("../input/3.txt", "r");
    for (i = 0; i < 9; i++) fscanf(input, "%d", &h[i]);
    for (i = 0; i < 6; i++) fscanf(input, "%d", &x[i]);
    for (i = 0; i < 6; i++) fscanf(input, "%d", &y[i]);
    fclose(input);
    int *p_x = &x[0];
    int *p_h = &h[0];
    int *p_y = &y[0];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            for (f = 0; f < 3; f++)
                asm volatile(
                    "li     t0, 3\n\t"         // 3
                    "slli   t1, %[i], 2\n\t"   // i
                    "slli   t2, %[j], 2\n\t"   // j
                    "slli   t3, %[f], 2\n\t"   // f
                    "mul    t4, t1, t0\n\t"    // 3 * i
                    "slli   t5, t1, 1\n\t"     // 2 * i
                    "slli   t6, t3, 1\n\t"     // 2 * f
                    "add    s1, t4, t3\n\t"    // 3i + f
                    "add    s2, t6, t2\n\t"    // 2f + j
                    "add    s3, t5, t2\n\t"    // 2i + j
                    "add    t1, s1, %[h]\n\t"  // address of h[3i + f]
                    "add    t2, s2, %[x]\n\t"  // address of x[2f + j]
                    "add    t3, s3, %[y]\n\t"  // address of y[2i + j]
                    "lw     t4, 0(t1)\n\t"     // value of h[3i + f]
                    "lw     t5, 0(t2)\n\t"     // value of x[2f + j]
                    "lw     t6, 0(t3)\n\t"     // value of y[2i + j]
                    "mul    t0, t4, t5\n\t"    // h[3i + f] * x[2f + j]
                    "add    t0, t0, t6\n\t"    // y[2i + j] + h[3i + f] * x[2f + j]
                    "sw     t0, 0(t3)\n\t"     // y[2i + j] += h[3i + f] * x[2f + j]
                    :
                    : [i] "r"(i), [j] "r"(j), [f] "r"(f), [h] "r"(h), [x] "r"(x), [y] "r"(y));
        }
    }
    p_y = &y[0];
    for (i = 0; i < 6; i++)
        printf("%d ", *p_y++);
    printf("\n");
    return 0;
}

/*
    "lw     x5, 0(x21)\n\t"      // value of h[3i + f]
    "lw     x6, 0(x22)\n\t"      // value of x[2f + j]
    "mul    x7, x5, x6"          // h[3i + f] * x[2f + j]
    "sw     x7, 0(x23)"          // y[2i + j] = h[3i + f] * x[2f + j]
    */