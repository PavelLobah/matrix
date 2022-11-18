#ifndef MATRIX_H_
#define MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MY_NAN 0.0 / 0.0
#define MY_INFINITY 1.0 / 0.0

#define SUCCESS 1
#define FAILURE 0

#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2
#define FAIL 1
#define OK 0

typedef struct matrix_struct {
    double **matrix;
    int rows;
    int columns;
} matrix_t;

int create_matrix(int rows, int columns, matrix_t *result);
void remove_matrix(matrix_t *A);
int eq_matrix(matrix_t *A, matrix_t *B);
int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int mult_number(matrix_t *A, double number, matrix_t *result);
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int transpose(matrix_t *A, matrix_t *result);
int calc_complements(matrix_t *A, matrix_t *result);
int determinant(matrix_t *A, double *result);
int inverse_matrix(matrix_t *A, matrix_t *result);

int check_matrix(matrix_t matrix);
int minor_matrix(int i_a, int j_a, matrix_t *A, matrix_t *result);
void write_matrix(matrix_t matrix, double index);

#endif  //  MATRIX_H_
