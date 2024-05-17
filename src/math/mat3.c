/**
 * \file
 * \author Christien Alden
 * \brief 3x3 matrix definition and functions implementation.
 */
#include <ace/math/mat3.h>
#include <ace/math/math.h>
#include <assert.h>
#include <math.h>
#include <string.h>

static const float ac_mat3_place_sign[3][3] = {
    {  1.0f, -1.0f,  1.0f },
    { -1.0f,  1.0f, -1.0f },
    {  1.0f, -1.0f,  1.0f }
};

ac_mat3 ac_mat3_identity(void)
{
    return (ac_mat3){ .a = 1.0f,
                      .b = 0.0f,
                      .c = 0.0f,
                      .d = 0.0f,
                      .e = 1.0f,
                      .f = 0.0f,
                      .g = 0.0f,
                      .h = 0.0f,
                      .i = 1.0f };
}

ac_mat3 ac_mat3_nan(void)
{
    return (ac_mat3){ NAN };
}

ac_mat3 ac_mat3_zero(void)
{
    return (ac_mat3){ 0.0f };
}

ac_mat3 ac_mat3_create(const float values[AC_MAT3_NUM_ELEMENTS])
{
    ac_mat3 matrix;
    ac_mat3_set_values(&matrix, values);
    return matrix;
}

void ac_mat3_set_values(ac_mat3* m, const float values[AC_MAT3_NUM_ELEMENTS])
{
    // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    memcpy(m->data, values, sizeof(float) * AC_MAT3_NUM_ELEMENTS);
}

bool ac_mat3_is_identity(const ac_mat3* m)
{
    ac_mat3 identity = ac_mat3_identity();
    return ac_mat3_is_equal(m, &identity);
}

bool ac_mat3_is_nan(const ac_mat3* m)
{
    // if any element is NaN, the matrix is said to be NaN
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        if ( isnan(m->data[i]) )
        {
            return true;
        }
    }

    return false;
}

bool ac_mat3_is_zero(const ac_mat3* m)
{
    ac_mat3 zero = ac_mat3_zero();
    return ac_mat3_is_equal(m, &zero);
}

bool ac_mat3_is_equal(const ac_mat3* a, const ac_mat3* b)
{
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        if ( !ac_float_compare(a->data[i], b->data[i], AC_EPSILON) )
        {
            return false;
        }
    }

    return true;
}

ac_mat3 ac_mat3_add_matrix(const ac_mat3* a, const ac_mat3* b)
{
    ac_mat3 result;
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] + b->data[i];
    }

    return result;
}

ac_mat3 ac_mat3_sub_matrix(const ac_mat3* a, const ac_mat3* b)
{
    ac_mat3 result;
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] - b->data[i];
    }

    return result;
}

ac_mat3 ac_mat3_mult_matrix(const ac_mat3* a, const ac_mat3* b)
{
    ac_mat3 result;
    result.a = a->a * b->a + a->b * b->d + a->c * b->g;
    result.b = a->a * b->b + a->b * b->e + a->c * b->h;
    result.c = a->a * b->c + a->b * b->f + a->c * b->i;
    result.d = a->d * b->a + a->e * b->d + a->f * b->g;
    result.e = a->d * b->b + a->e * b->e + a->f * b->h;
    result.f = a->d * b->c + a->e * b->f + a->f * b->i;
    result.g = a->g * b->a + a->h * b->d + a->i * b->g;
    result.h = a->g * b->b + a->h * b->e + a->i * b->h;
    result.i = a->g * b->c + a->h * b->f + a->i * b->i;
    return result;
}

ac_mat3 ac_mat3_add_scalar(const ac_mat3* m, float scalar)
{
    ac_mat3 result;
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        result.data[i] = m->data[i] + scalar;
    }

    return result;
}

ac_mat3 ac_mat3_sub_scalar(const ac_mat3* m, float scalar)
{
    ac_mat3 result;
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        result.data[i] = m->data[i] - scalar;
    }

    return result;
}

ac_mat3 ac_mat3_mult_scalar(const ac_mat3* m, float scalar)
{
    ac_mat3 result;
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        result.data[i] = m->data[i] * scalar;
    }

    return result;
}

ac_mat3 ac_mat3_transpose(const ac_mat3* m)
{
    return (ac_mat3){ .a = m->a,
                      .b = m->d,
                      .c = m->g,
                      .d = m->b,
                      .e = m->e,
                      .f = m->h,
                      .g = m->c,
                      .h = m->f,
                      .i = m->i };
}

float ac_mat3_determinant(const ac_mat3* m)
{
    float product_a = m->a * ac_mat3_cofactor_element(m, 0, 0);
    float product_b = m->b * ac_mat3_cofactor_element(m, 0, 1);
    float product_c = m->c * ac_mat3_cofactor_element(m, 0, 2);
    return product_a + product_b + product_c;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ac_mat2 ac_mat3_to_submatrix(const ac_mat3* m, int row_to_exclude, int col_to_exclude)
{
    // bounds check
    if ( row_to_exclude < 0 || row_to_exclude >= 3 || col_to_exclude < 0 || col_to_exclude >= 3 )
    {
        return ac_mat2_nan();
    }

    ac_mat2 result = ac_mat2_zero();
    int     index  = 0;

    for ( int ci = 0; ci < 3; ++ci )
    {
        if ( ci == col_to_exclude )
        {
            continue;
        }

        for ( int ri = 0; ri < 3; ++ri )
        {
            if ( ri == row_to_exclude )
            {
                continue;
            }

            // multiply the column index by 3 and add the row index
            result.data[index] = m->data[ci * 3 + ri];
            index++;
        }
    }

    assert(index == 4);
    return result;
}

float ac_mat3_minor(const ac_mat3* m, int row, int col)
{
    // calculate the matrix of minors
    ac_mat2 minor_matrix = ac_mat3_to_submatrix(m, row, col);
    if ( ac_mat2_is_nan(&minor_matrix) )
    {
        return NAN;
    }

    // calculate the determinant of the minor matrix
    return ac_mat2_determinant(&minor_matrix);
}

float ac_mat3_cofactor_element(const ac_mat3* m, int row, int col)
{
    // bounds check
    if ( row < 0 || row >= 3 || col < 0 || col >= 3 )
    {
        return NAN;
    }

    // calculate the minor of the element
    float minor = ac_mat3_minor(m, row, col);

    // calculate the cofactor based on the minor and the place sign
    return ac_mat3_place_sign[row][col] * minor;
}

ac_mat3 ac_mat3_cofactor_matrix(const ac_mat3* m)
{
    // initialize the cofactor matrix to zero
    ac_mat3 cofactor_matrix = ac_mat3_zero();

    // calculate the elements of the cofactor matrix
    for ( int i = 0; i < AC_MAT3_NUM_ELEMENTS; ++i )
    {
        int row                 = i % 3;
        int col                 = i / 3;
        cofactor_matrix.data[i] = ac_mat3_cofactor_element(m, row, col);
    }

    return cofactor_matrix;
}

ac_mat3 ac_mat3_adjugate(const ac_mat3* m)
{
    // the adjugate matrix is defined as the transpose of the cofactor matrix
    ac_mat3 cofactor_matrix = ac_mat3_cofactor_matrix(m);
    return ac_mat3_transpose(&cofactor_matrix);
}

ac_mat3 ac_mat3_inverse(const ac_mat3* m)
{
    // calculate the determinant of the matrix
    float determinant = ac_mat3_determinant(m);

    // if the determinant is zero, the matrix is not invertible
    if ( ac_float_compare(determinant, 0.0f, AC_EPSILON) )
    {
        return ac_mat3_nan();
    }

    // calculate the adjugate matrix and inverse determinant
    ac_mat3 adjugate_matrix = ac_mat3_adjugate(m);
    float   inv_determinant = 1.0f / determinant;

    // calculate the inverse matrix
    return ac_mat3_mult_scalar(&adjugate_matrix, inv_determinant);
}
