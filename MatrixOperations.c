#include <stdio.h>
#include <stdlib.h>

// Safe integer input
int getInt(const char *msg, int positiveOnly) {
    int value, valid;
    do {
        printf("%s", msg);
        valid = scanf("%d", &value);

        // Clear buffer safely
        int c;
        while((c = getchar()) != '\n' && c != EOF);

        if(valid != 1 || (positiveOnly && value <= 0)) {
            printf("Invalid input! Please enter %s integer.\n",
                   positiveOnly ? "a positive" : "an");
            valid = 0;
        }
    } while(valid != 1 || (positiveOnly && value <= 0));
    return value;
}

// Allocate matrix dynamically with full error check
int **allocateMatrix(int rows, int cols) {
    int **M = malloc(rows * sizeof(int *));
    if(M == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < rows; i++) {
        M[i] = malloc(cols * sizeof(int));
        if(M[i] == NULL) {
            // Free previously allocated rows
            for(int j = 0; j < i; j++) free(M[j]);
            free(M);
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    return M;
}

// Free matrix
void freeMatrix(int **M, int rows) {
    for(int i = 0; i < rows; i++) free(M[i]);
    free(M);
}

// Input matrix with clear prompts
void inputMatrix(int rows, int cols, int **M, const char *name) {
    printf("Enter elements of Matrix %s:\n", name);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            M[i][j] = getInt(prompt, 0);
        }
    }
}

// Display matrix
void displayMatrix(int rows, int cols, int * const *M) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) printf("%d ", M[i][j]);
        printf("\n");
    }
}

// Addition
void addMatrix(int rows, int cols,
               int * const *A, int * const *B,
               int **result) {
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            result[i][j] = A[i][j] + B[i][j];
}

// Multiplication
void multiplyMatrix(int r1, int c1,
                    int * const *A,
                    int r2, int c2,
                    int * const *B,
                    int **result) {
    if(c1 != r2) {
        printf("Multiplication not possible (columns of A != rows of B)\n");
        return;
    }
    for(int i = 0; i < r1; i++) {
        for(int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for(int k = 0; k < c1; k++)
                result[i][j] += A[i][k] * B[k][j];
        }
    }
}

// Transpose
void transposeMatrix(int rows, int cols,
                     int * const *A,
                     int **result) {
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            result[j][i] = A[i][j];
}

int main() {
    int r1 = getInt("Rows for Matrix A: ", 1);
    int c1 = getInt("Cols for Matrix A: ", 1);
    int **A = allocateMatrix(r1, c1);
    inputMatrix(r1, c1, A, "A");

    int r2 = getInt("Rows for Matrix B: ", 1);
    int c2 = getInt("Cols for Matrix B: ", 1);
    int **B = allocateMatrix(r2, c2);
    inputMatrix(r2, c2, B, "B");

    // Addition
    if(r1 == r2 && c1 == c2) {
        int **sum = allocateMatrix(r1, c1);
        addMatrix(r1, c1, A, B, sum);
        printf("\nMatrix Addition:\n");
        displayMatrix(r1, c1, sum);
        freeMatrix(sum, r1);
    } else {
        printf("\nAddition not possible (different dimensions)\n");
    }

    // Multiplication (allocate only if possible)
    if(c1 == r2) {
        int **product = allocateMatrix(r1, c2);
        multiplyMatrix(r1, c1, A, r2, c2, B, product);
        printf("\nMatrix Multiplication:\n");
        displayMatrix(r1, c2, product);
        freeMatrix(product, r1);
    } else {
        printf("\nMultiplication not possible (A cols != B rows)\n");
    }

    // Transpose of A
    int **transposeA = allocateMatrix(c1, r1);
    transposeMatrix(r1, c1, A, transposeA);
    printf("\nTranspose of Matrix A:\n");
    displayMatrix(c1, r1, transposeA);
    freeMatrix(transposeA, c1);

    // Transpose of B
    int **transposeB = allocateMatrix(c2, r2);
    transposeMatrix(r2, c2, B, transposeB);
    printf("\nTranspose of Matrix B:\n");
    displayMatrix(c2, r2, transposeB);
    freeMatrix(transposeB, c2);

    // Free original matrices
    freeMatrix(A, r1);
    freeMatrix(B, r2);

    return 0;
}


