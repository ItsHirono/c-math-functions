#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
    NO_SOLUTION,
    INFINITE_SOLUTIONS,
    LINEAR,
    ONE_REAL_ROOT,
    TWO_REAL_ROOTS,
    TWO_COMPLEX_ROOTS
} QuadResult;

typedef struct {
    QuadResult type;
    double real1, imag1;
    double real2, imag2;
} QuadSolution;

/* Returns 1 if the string is a valid decimal number, 0 otherwise */
int is_valid_double(const char *str) {
    if (str == NULL || *str == '\0') return 0;

    int has_digits = 0;
    int i = 0;

    if (str[i] == '+' || str[i] == '-') i++;   /* optional leading sign */

    while (str[i] >= '0' && str[i] <= '9') { has_digits = 1; i++; }

    if (str[i] == '.') {                        /* optional decimal point */
        i++;
        while (str[i] >= '0' && str[i] <= '9') { has_digits = 1; i++; }
    }

    if (str[i] == 'e' || str[i] == 'E') {      /* optional exponent */
        i++;
        if (str[i] == '+' || str[i] == '-') i++;
        if (str[i] < '0' || str[i] > '9') return 0;
        while (str[i] >= '0' && str[i] <= '9') i++;
    }

    return has_digits && str[i] == '\0';        /* nothing left over */
}

/* Prompts the user for a coefficient, retrying until valid input is given */
double read_coefficient(const char *name, int reject_zero) {
    char buf[64];

    while (1) {
        printf("  Enter %s: ", name);

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            fprintf(stderr, "Error: failed to read input.\n");
            exit(1);
        }

        /* Strip the trailing newline */
        int len = 0;
        while (buf[len] && buf[len] != '\n') len++;
        buf[len] = '\0';

        if (!is_valid_double(buf)) {
            printf("  Invalid input '%s' — please enter a number (e.g. 3, -1.5, 2e4).\n", buf);
            continue;
        }

        double value = atof(buf);

        if (reject_zero && value == 0.0) {
            printf("  'a' cannot be zero — this would not be a quadratic equation. Try again.\n");
            continue;
        }

        return value;
    }
}

QuadSolution solve_quadratic(double a, double b, double c) {
    QuadSolution sol = {0};

    if (a == 0.0) {
        if (b == 0.0) {
            sol.type = (c == 0.0) ? INFINITE_SOLUTIONS : NO_SOLUTION;
        } else {
            sol.type  = LINEAR;
            sol.real1 = -c / b;
        }
        return sol;
    }

    double disc = b * b - 4.0 * a * c;

    if (disc > 0.0) {
        sol.type  = TWO_REAL_ROOTS;
        sol.real1 = (-b + sqrt(disc)) / (2.0 * a);
        sol.real2 = (-b - sqrt(disc)) / (2.0 * a);
    } else if (disc == 0.0) {
        sol.type  = ONE_REAL_ROOT;
        sol.real1 = -b / (2.0 * a);
    } else {
        sol.type  = TWO_COMPLEX_ROOTS;
        sol.real1 = sol.real2 = -b / (2.0 * a);
        sol.imag1 =  sqrt(-disc) / (2.0 * a);
        sol.imag2 = -sol.imag1;
    }

    return sol;
}

void print_solution(QuadSolution s) {
    switch (s.type) {
        case NO_SOLUTION:
            printf("No solution.\n");
            break;
        case INFINITE_SOLUTIONS:
            printf("Infinite solutions (0 = 0).\n");
            break;
        case LINEAR:
            printf("Linear equation — one root: x = %.6g\n", s.real1);
            break;
        case ONE_REAL_ROOT:
            printf("One repeated root: x = %.6g\n", s.real1);
            break;
        case TWO_REAL_ROOTS:
            printf("Two real roots:\n");
            printf("  x1 = %.6g\n", s.real1);
            printf("  x2 = %.6g\n", s.real2);
            break;
        case TWO_COMPLEX_ROOTS:
            printf("Two complex roots:\n");
            printf("  x1 = %.6g + %.6gi\n", s.real1, s.imag1);
            printf("  x2 = %.6g - %.6gi\n", s.real2, fabs(s.imag2));
            break;
    }
}

int main(void) {
    printf("Quadratic equation solver: ax^2 + bx + c = 0\n");
    printf("------------------------------------------------\n");

    double a = read_coefficient("a (must be non-zero)", 1);
    double b = read_coefficient("b", 0);
    double c = read_coefficient("c", 0);

    printf("\nSolving: %.6gx^2 + %.6gx + %.6g = 0\n", a, b, c);

    QuadSolution s = solve_quadratic(a, b, c);
    print_solution(s);

    return 0;
}