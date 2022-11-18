#include "matrix.h"

int create_matrix(int rows, int columns, matrix_t *result) {
    int res = OK;
    if (rows < 1 || columns < 1) {
        res = FAIL;
    } else {
        result->columns = columns;
        result->rows = rows;
        result->matrix = calloc(rows, sizeof(double *));
        if (result->matrix == NULL) {
            res = FAIL;
        } else {
            for (int i = 0; i < rows; i++) {
                result->matrix[i] = calloc(columns, sizeof(double));
                if (result->matrix[i] == NULL) {
                    remove_matrix(result);
                    res = FAIL;
                    break;
                }
            }
        }
    }

    return res;
}
void remove_matrix(matrix_t *A) {
    for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
    }
    free(A->matrix);
    A->rows = 0;
    A->columns = 0;
}
int eq_matrix(matrix_t *A, matrix_t *B) {
    int res = SUCCESS;
    if ((A->columns == B->columns) && (A->rows == B->rows)) {
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                if (fabs(fabs(A->matrix[i][j]) - fabs(B->matrix[i][j])) >= 1e-06) {
                    res = FAILURE;
                }
            }
        }
    } else {
        res = FAILURE;
    }
    return res;
}
int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = OK;
    if (check_matrix(*A) || check_matrix(*B)) {
        res = INCORRECT_MATRIX;
    } else {
        if ((A->columns == B->columns) && (A->rows == B->rows)) {
            create_matrix(A->rows, A->columns, result);
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < A->columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
                }
            }
        } else {
            res = CALCULATION_ERROR;
        }
    }

    return res;
}
int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = OK;
    if (check_matrix(*A) || check_matrix(*B)) {
        res = INCORRECT_MATRIX;
    } else {
        if ((A->columns == B->columns) && (A->rows == B->rows)) {
            create_matrix(A->rows, A->columns, result);
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < A->columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
                }
            }
        } else {
            res = CALCULATION_ERROR;
        }
    }
    return res;
}
int mult_number(matrix_t *A, double number, matrix_t *result) {
    int res = OK;
    if (check_matrix(*A)) {
        res = INCORRECT_MATRIX;
    } else {
        if (!isnan(number)) {
            create_matrix(A->rows, A->columns, result);
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < A->columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] * number;
                }
            }
        } else {
            res = CALCULATION_ERROR;
        }
    }

    return res;
}
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = OK;
    if (check_matrix(*A) || check_matrix(*B)) {
        res = INCORRECT_MATRIX;
    } else {
        if ((A->columns == B->rows)) {
            create_matrix(A->rows, B->columns, result);
            for (int i = 0; i < result->rows; i++)
                for (int j = 0; j < result->columns; j++) {
                    for (int k = 0; k < A->columns; k++)
                        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
                }

        } else {
            res = CALCULATION_ERROR;
        }
    }

    return res;
}
int transpose(matrix_t *A, matrix_t *result) {
    int res = OK;
    if (!check_matrix(*A)) {
        create_matrix(A->columns, A->rows, result);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                result->matrix[j][i] = A->matrix[i][j];
            }
        }
    } else {
        res = INCORRECT_MATRIX;
    }
    return res;
}
int calc_complements(matrix_t *A, matrix_t *result) {
    int res = 0;
    if (check_matrix(*A)) {
        res = INCORRECT_MATRIX;
    } else if (A->columns != A->rows) {
        res = CALCULATION_ERROR;
    } else if (A->rows == 1) {
        create_matrix(1, 1, result);
        result->matrix[0][0] = 1;
    } else {
        create_matrix(A->rows, A->columns, result);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                double det = 0;
                matrix_t M;
                create_matrix(A->rows - 1, A->columns - 1, &M);
                minor_matrix(i, j, A, &M);
                determinant(&M, &det);
                result->matrix[i][j] = det * pow(-1, i + j);
                remove_matrix(&M);
            }
        }
    }
    return res;
}
int determinant(matrix_t *A, double *result) {
    int res = 0;
    *result = 0;
    if (check_matrix(*A)) {
        res = INCORRECT_MATRIX;
    } else if (A->columns != A->rows) {
        res = CALCULATION_ERROR;
    } else if (A->columns == 1 && A->rows == 1) {
        *result = A->matrix[0][0];
    } else if (A->columns == 2) {
        *result = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    } else {
        for (int i = 0; i < A->rows; i++) {
            double det = 0;
            matrix_t M;
            create_matrix(A->rows - 1, A->columns - 1, &M);
            minor_matrix(i, 0, A, &M);
            determinant(&M, &det);
            *result += A->matrix[i][0] * det * pow(-1, i);
            remove_matrix(&M);
        }
    }
    return res;
}
int inverse_matrix(matrix_t *A, matrix_t *result) {
    if (check_matrix(*A)) {
        return INCORRECT_MATRIX;
    }
    if (A->rows != A->columns) {
        return CALCULATION_ERROR;
    }
    double check = 0;
    if (determinant(A, &check) == 0 && check == 0) {
        return CALCULATION_ERROR;
    }
    int res = 0;
    matrix_t temp;
    matrix_t trans;
    calc_complements(A, &temp);
    transpose(&temp, &trans);
    mult_number(&trans, 1.0 / fabs(check), result);
    remove_matrix(&temp);
    remove_matrix(&trans);
    return res;
}
int check_matrix(matrix_t matrix) {
    int check = OK;
    if (matrix.rows < 1 || matrix.columns < 1 || matrix.matrix == NULL) {
        check = INCORRECT_MATRIX;
    }
    return check;
}
int minor_matrix(int i_a, int j_a, matrix_t *A, matrix_t *result) {
    int r = 0, c = 0;
    for (int i = 0; i < A->rows; i++) {
        if (i == i_a) continue;
        c = 0;
        for (int j = 0; j < A->columns; j++) {
            if (j == j_a) continue;
            result->matrix[r][c] = A->matrix[i][j];
            c++;
        }
        r++;
    }
    return 0;
}
void write_matrix(matrix_t matrix, double index) {
    int count = 0;
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.columns; j++) {
            matrix.matrix[i][j] = ++count * index;
        }
    }
}
