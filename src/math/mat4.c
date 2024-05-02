/**
 * \file
 * \author Christien Alden
 * \brief 4x4 matrix definition and functions implementation.
 */
#include <ace/math/mat4.h>
#include <ace/math/math.h>
#include <assert.h>
#include <math.h>
#include <string.h>

static const float ac_mat4_place_sign[4][4] = {
    {  1.0f, -1.0f,  1.0f, -1.0f },
    { -1.0f,  1.0f, -1.0f,  1.0f },
    {  1.0f, -1.0f,  1.0f, -1.0f },
    { -1.0f,  1.0f, -1.0f,  1.0f }
};

ac_mat4 ac_mat4_identity(void)
{
    return (ac_mat4){ .a = 1.0f,
                      .b = 0.0f,
                      .c = 0.0f,
                      .d = 0.0f,
                      .e = 0.0f,
                      .f = 1.0f,
                      .g = 0.0f,
                      .h = 0.0f,
                      .i = 0.0f,
                      .j = 0.0f,
                      .k = 1.0f,
                      .l = 0.0f,
                      .m = 0.0f,
                      .n = 0.0f,
                      .o = 0.0f,
                      .p = 1.0f };
}

ac_mat4 ac_mat4_nan(void)
{
    return (ac_mat4){ NAN };
}

ac_mat4 ac_mat4_zero(void)
{
    return (ac_mat4){ 0.0f };
}

ac_mat4 ac_mat4_create(const float values[AC_MAT4_NUM_ELEMENTS])
{
    ac_mat4 matrix;
    ac_mat4_set_values(&matrix, values);
    return matrix;
}

void ac_mat4_set_values(ac_mat4* m, const float values[AC_MAT4_NUM_ELEMENTS])
{
    // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    memcpy(m->data, values, sizeof(float) * AC_MAT4_NUM_ELEMENTS);
}

bool ac_mat4_is_identity(const ac_mat4* m)
{
    ac_mat4 identity = ac_mat4_identity();
    return ac_mat4_is_equal(m, &identity);
}

bool ac_mat4_is_nan(const ac_mat4* m)
{
    // if any element is NaN, the matrix is said to be NaN
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        if ( isnan(m->data[i]) )
        {
            return true;
        }
    }
    return false;
}

bool ac_mat4_is_zero(const ac_mat4* m)
{
    ac_mat4 zero = ac_mat4_zero();
    return ac_mat4_is_equal(m, &zero);
}

bool ac_mat4_is_equal(const ac_mat4* a, const ac_mat4* b)
{
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        if ( !ac_float_compare(a->data[i], b->data[i], AC_EPSILON) )
        {
            return false;
        }
    }

    return true;
}

ac_mat4 ac_mat4_add_matrix(const ac_mat4* a, const ac_mat4* b)
{
    ac_mat4 result;
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] + b->data[i];
    }

    return result;
}

ac_mat4 ac_mat4_sub_matrix(const ac_mat4* a, const ac_mat4* b)
{
    ac_mat4 result;
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] - b->data[i];
    }

    return result;
}

ac_mat4 ac_mat4_mult_matrix(const ac_mat4* a, const ac_mat4* b)
{
    ac_mat4 result;
    result.a = a->a * b->a + a->b * b->e + a->c * b->i + a->d * b->m;
    result.b = a->a * b->b + a->b * b->f + a->c * b->j + a->d * b->n;
    result.c = a->a * b->c + a->b * b->g + a->c * b->k + a->d * b->o;
    result.d = a->a * b->d + a->b * b->h + a->c * b->l + a->d * b->p;
    result.e = a->e * b->a + a->f * b->e + a->g * b->i + a->h * b->m;
    result.f = a->e * b->b + a->f * b->f + a->g * b->j + a->h * b->n;
    result.g = a->e * b->c + a->f * b->g + a->g * b->k + a->h * b->o;
    result.h = a->e * b->d + a->f * b->h + a->g * b->l + a->h * b->p;
    result.i = a->i * b->a + a->j * b->e + a->k * b->i + a->l * b->m;
    result.j = a->i * b->b + a->j * b->f + a->k * b->j + a->l * b->n;
    result.k = a->i * b->c + a->j * b->g + a->k * b->k + a->l * b->o;
    result.l = a->i * b->d + a->j * b->h + a->k * b->l + a->l * b->p;
    result.m = a->m * b->a + a->n * b->e + a->o * b->i + a->p * b->m;
    result.n = a->m * b->b + a->n * b->f + a->o * b->j + a->p * b->n;
    result.o = a->m * b->c + a->n * b->g + a->o * b->k + a->p * b->o;
    result.p = a->m * b->d + a->n * b->h + a->o * b->l + a->p * b->p;
    return result;
}

ac_mat4 ac_mat4_add_scalar(const ac_mat4* m, float scalar)
{
    ac_mat4 result;
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        result.data[i] = m->data[i] + scalar;
    }

    return result;
}

ac_mat4 ac_mat4_sub_scalar(const ac_mat4* m, float scalar)
{
    ac_mat4 result;
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        result.data[i] = m->data[i] - scalar;
    }

    return result;
}

ac_mat4 ac_mat4_mult_scalar(const ac_mat4* m, float scalar)
{
    ac_mat4 result;
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        result.data[i] = m->data[i] * scalar;
    }

    return result;
}

ac_mat4 ac_mat4_transpose(const ac_mat4* m)
{
    return (ac_mat4){ .a = m->a,
                      .b = m->e,
                      .c = m->i,
                      .d = m->m,
                      .e = m->b,
                      .f = m->f,
                      .g = m->j,
                      .h = m->n,
                      .i = m->c,
                      .j = m->g,
                      .k = m->k,
                      .l = m->o,
                      .m = m->d,
                      .n = m->h,
                      .o = m->l,
                      .p = m->p };
}

float ac_mat4_determinant(const ac_mat4* mat)
{
    float product_a = mat->a * ac_mat4_cofactor_element(mat, 0, 0);
    float product_b = mat->b * ac_mat4_cofactor_element(mat, 0, 1);
    float product_c = mat->c * ac_mat4_cofactor_element(mat, 0, 2);
    float product_d = mat->d * ac_mat4_cofactor_element(mat, 0, 3);
    return product_a + product_b + product_c + product_d;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ac_mat3 ac_mat4_to_submatrix(const ac_mat4* m, int row_to_exclude, int col_to_exclude)
{
    // bounds check
    if ( row_to_exclude < 0 || row_to_exclude >= 4 || col_to_exclude < 0 || col_to_exclude >= 4 )
    {
        return ac_mat3_nan();
    }

    ac_mat3 result = ac_mat3_zero();
    int     index  = 0;

    for ( int ci = 0; ci < 4; ++ci )
    {
        if ( ci == col_to_exclude )
        {
            continue;
        }

        for ( int ri = 0; ri < 4; ++ri )
        {
            if ( ri == row_to_exclude )
            {
                continue;
            }

            // multiply the column index by 3 and add the row index
            result.data[index] = m->data[ci * 4 + ri];
            index++;
        }
    }

    assert(index == 9);
    return result;
}

float ac_mat4_minor(const ac_mat4* m, int row, int col)
{
    ac_mat3 submatrix = ac_mat4_to_submatrix(m, row, col);
    if ( ac_mat3_is_nan(&submatrix) )
    {
        return NAN;
    }

    return ac_mat3_determinant(&submatrix);
}

float ac_mat4_cofactor_element(const ac_mat4* m, int row, int col)
{
    if ( row < 0 || row >= 4 || col < 0 || col >= 4 )
    {
        return NAN;
    }

    // calculate the minor of the element
    float minor = ac_mat4_minor(m, row, col);
    // calculate the cofactor based on the minor and the place sign
    return ac_mat4_place_sign[row][col] * minor;
}

ac_mat4 ac_mat4_cofactor_matrix(const ac_mat4* m)
{
    ac_mat4 cofactor_matrix = ac_mat4_zero();

    // calculate the cofactor of each element
    for ( int i = 0; i < AC_MAT4_NUM_ELEMENTS; ++i )
    {
        int row                 = i % 4;
        int col                 = i / 4;
        cofactor_matrix.data[i] = ac_mat4_cofactor_element(m, row, col);
    }

    return cofactor_matrix;
}

ac_mat4 ac_mat4_adjugate(const ac_mat4* m)
{
    // the adjugate matrix is defined as the transpose of the cofactor matrix
    ac_mat4 cofactor_matrix = ac_mat4_cofactor_matrix(m);
    return ac_mat4_transpose(&cofactor_matrix);
}

ac_mat4 ac_mat4_inverse(const ac_mat4* m)
{
    // calculate the determinant of the matrix
    float determinant = ac_mat4_determinant(m);

    // if the determinant is zero, the matrix is not invertible
    if ( ac_float_compare(determinant, 0.0f, AC_EPSILON) )
    {
        return ac_mat4_nan();
    }

    // calculate the adjugate matrix and inverse determinant
    ac_mat4 adjugate        = ac_mat4_adjugate(m);
    float   inv_determinant = 1.0f / determinant;

    // divide the adjugate matrix by the determinant
    return ac_mat4_mult_scalar(&adjugate, inv_determinant);
}
