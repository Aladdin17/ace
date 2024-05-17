/**
 * \file
 * \author Christien Alden
 * \brief 2x2 matrix types and functions implementation.
 */
#include <ace/math/mat2.h>
#include <ace/math/math.h>
#include <math.h>
#include <string.h>

static const float ac_mat2_place_sign[2][2] = {
    {  1.0f, -1.0f },
    { -1.0f,  1.0f }
};

ac_mat2 ac_mat2_identity(void)
{
    return (ac_mat2){ .a = 1.0f, .b = 0.0f, .c = 0.0f, .d = 1.0f };
}

ac_mat2 ac_mat2_nan(void)
{
    return (ac_mat2){ NAN };
}

ac_mat2 ac_mat2_zero(void)
{
    return (ac_mat2){ 0.0f };
}

ac_mat2 ac_mat2_create(const float values[AC_MAT2_NUM_ELEMENTS])
{
    ac_mat2 result;
    ac_mat2_set_values(&result, values);
    return result;
}

void ac_mat2_set_values(ac_mat2* m, const float values[AC_MAT2_NUM_ELEMENTS])
{
    // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    memcpy(m->data, values, sizeof(float) * AC_MAT2_NUM_ELEMENTS);
}

bool ac_mat2_is_identity(const ac_mat2* m)
{
    ac_mat2 identity = ac_mat2_identity();
    return ac_mat2_is_equal(m, &identity);
}

bool ac_mat2_is_nan(const ac_mat2* m)
{
    // if any element is NaN, the matrix is said to be NaN
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        if ( isnan(m->data[i]) )
        {
            return true;
        }
    }

    return false;
}

bool ac_mat2_is_zero(const ac_mat2* m)
{
    ac_mat2 zero = ac_mat2_zero();
    return ac_mat2_is_equal(m, &zero);
}

bool ac_mat2_is_equal(const ac_mat2* a, const ac_mat2* b)
{
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        if ( !ac_float_compare(a->data[i], b->data[i], AC_EPSILON) )
        {
            return false;
        }
    }

    return true;
}

ac_mat2 ac_mat2_add_matrix(const ac_mat2* a, const ac_mat2* b)
{
    ac_mat2 result;
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] + b->data[i];
    }

    return result;
}

ac_mat2 ac_mat2_sub_matrix(const ac_mat2* a, const ac_mat2* b)
{
    ac_mat2 result;
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] - b->data[i];
    }

    return result;
}

ac_mat2 ac_mat2_mult_matrix(const ac_mat2* a, const ac_mat2* b)
{
    // this works due to the fact that the aliases are used to abstract the
    // ordering of the elements in the matrix array.
    ac_mat2 result;
    result.a = a->a * b->a + a->b * b->c;
    result.b = a->a * b->b + a->b * b->d;
    result.c = a->c * b->a + a->d * b->c;
    result.d = a->c * b->b + a->d * b->d;
    return result;
}

ac_mat2 ac_mat2_add_scalar(const ac_mat2* a, float scalar)
{
    ac_mat2 result;
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] + scalar;
    }

    return result;
}

ac_mat2 ac_mat2_sub_scalar(const ac_mat2* a, float scalar)
{
    ac_mat2 result;
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] - scalar;
    }

    return result;
}

ac_mat2 ac_mat2_mult_scalar(const ac_mat2* a, float scalar)
{
    ac_mat2 result;
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        result.data[i] = a->data[i] * scalar;
    }

    return result;
}

ac_mat2 ac_mat2_transpose(const ac_mat2* m)
{
    return (ac_mat2){ .a = m->a, .b = m->c, .c = m->b, .d = m->d };
}

float ac_mat2_determinant(const ac_mat2* m)
{
    // the determinant of a 2x2 matrix is ad - bc
    return m->a * m->d - m->b * m->c;
}

float ac_mat2_minor(const ac_mat2* m, int row, int col)
{
    // the minor of a 2x2 matrix is the determinant of the 1x1 submatrix
    // which for a 2x2 matrix is the element that is not in the row or column
    return m->data[!col * 2 + !row];
}

float ac_mat2_cofactor_element(const ac_mat2* m, int row, int col)
{
    if ( row < 0 || row >= 2 || col < 0 || col >= 2 )
    {
        return NAN;
    }

    // calculate the minor of the element
    float minor = ac_mat2_minor(m, row, col);
    // calculate the cofactor based on the minor and the place sign
    return ac_mat2_place_sign[row][col] * minor;
}

ac_mat2 ac_mat2_cofactor_matrix(const ac_mat2* m)
{
    ac_mat2 cofactor_matrix = ac_mat2_zero();

    // calculate the cofactor of each element
    for ( int i = 0; i < AC_MAT2_NUM_ELEMENTS; ++i )
    {
        int row                 = i % 2;
        int col                 = i / 2;
        cofactor_matrix.data[i] = ac_mat2_cofactor_element(m, row, col);
    }

    return cofactor_matrix;
}

ac_mat2 ac_mat2_adjugate(const ac_mat2* m)
{
    // the adjugate of a 2x2 matrix is the transpose of the cofactor matrix
    // which can be calculated by swapping the diagonal elements and
    // negating the off-diagonal elements.
    // | a b |  ->  |d -b |
    // | c d |      |-c  a|

    return (ac_mat2){ .a = m->d, .b = -m->b, .c = -m->c, .d = m->a };
}

ac_mat2 ac_mat2_inverse(const ac_mat2* m)
{
    // the inverse is defined as the inv_det * adjugate
    float det = ac_mat2_determinant(m);
    if ( ac_float_compare(det, 0.0f, AC_EPSILON) )
    {
        return ac_mat2_nan();
    }

    float   inv_det  = 1.0f / det;
    ac_mat2 adjugate = ac_mat2_adjugate(m);
    return ac_mat2_mult_scalar(&adjugate, inv_det);
}
