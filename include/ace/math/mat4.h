/**
 * \file
 * \author Christien Alden
 * \brief 4x4 matrix definition and functions.
 */
#pragma once
#include "mat3.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup mat4 mat4
 * \brief 4x4 matrix definitions and functions.
 */

/**
 * \ingroup mat4
 * \def AC_MAT4_NUM_ELEMENTS
 * \brief The number of elements in a 4x4 matrix.
 */
#define AC_MAT4_NUM_ELEMENTS 16

/**
 * \union ac_mat4
 * \brief A 4x4 matrix in column-major order.
 * \details
 * Accessing the elements of the matrix using the data member will give
 * the elements in column-major order.
 *
 * Accessing the elements using any of the other members will give the elements
 * in a logical row-major order.
 *
 * As shown here:
 * \code{.unparsed}
 * index using m{row}{col} notation
 * | m00, m01, m02, m03 |
 * | m10, m11, m12, m13 |
 * | m20, m21, m22, m23 |
 * | m30, m31, m32, m33 |
 *
 * index using a,b,... notation
 * | a, b, c, d |
 * | e, f, g, h |
 * | i, j, k, l |
 * | m, n, o, p |
 * \endcode
 */
typedef union ac_mat4
{
    struct
    {
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
    };
    struct
    {
        float a, e, i, m;
        float b, f, j, n;
        float c, g, k, o;
        float d, h, l, p;
    };
    float data[AC_MAT4_NUM_ELEMENTS];
} ac_mat4;

/**
 * \ingroup mat4
 * \brief Generates and returns the identity matrix.
 * \return The identity matrix.
 */
ac_mat4 ac_mat4_identity(void);
/**
 * \ingroup mat4
 * \brief Generates and returns a matrix with all elements set to NaN.
 * \return The nan matrix.
 */
ac_mat4 ac_mat4_nan(void);
/**
 * \ingroup mat4
 * \brief Generates and returns a matrix with all elements set to zero.
 * \return The zero matrix.
 */
ac_mat4 ac_mat4_zero(void);
/**
 * \ingroup mat4
 * \brief Creates a 4x4 matrix from an array of values in column-major order.
 * \param[in] values The array of values in column-major order.
 * \return A 4x4 matrix with the data set to the values.
 * \warning The \p values array must contain at least 16 elements.
 */
ac_mat4 ac_mat4_create(const float values[AC_MAT4_NUM_ELEMENTS]);
/**
 * \ingroup mat4
 * \brief Sets the values of the matrix.
 * \param[out] m The matrix.
 * \param[in] values The array of values in column-major order.
 * \warning The \p values array must contain at least 16 elements.
 */
void    ac_mat4_set_values(ac_mat4* m, const float values[AC_MAT4_NUM_ELEMENTS]);
/**
 * \ingroup mat4
 * \brief Checks whether the matrix is the identity matrix.
 * \param[in] m The matrix to check.
 * \retval true if the matrix is the identity matrix.
 * \retval false if the matrix is not the identity matrix.
 * \see ac_mat4_identity
 */
bool    ac_mat4_is_identity(const ac_mat4* m);
/**
 * \ingroup mat4
 * \brief Checks whether the matrix contains any NaN values.
 * \param[in] m The matrix to check.
 * \retval true if the matrix contains any NaN values.
 * \retval false if the matrix does not contain any NaN values.
 */
bool    ac_mat4_is_nan(const ac_mat4* m);
/**
 * \ingroup mat4
 * \brief Checks whether the matrix is the zero matrix.
 * \param[in] m The matrix to check.
 * \retval true if the matrix is the zero matrix.
 * \retval false if the matrix is not the zero matrix.
 */
bool    ac_mat4_is_zero(const ac_mat4* m);
/**
 * \ingroup mat4
 * \brief Checks whether two matrices are equal.
 * \param[in] a The first matrix to compare.
 * \param[in] b The second matrix to compare.
 * \retval true if the matrices are equal.
 * \retval false if the matrices are not equal.
 * \details
 * The function uses \ref ac_float_compare with an epsilon value of
 * \ref AC_EPSILON to compare the elements of the matrices.
 */
bool    ac_mat4_is_equal(const ac_mat4* a, const ac_mat4* b);
/**
 * \ingroup mat4
 * \brief Adds two matrices together, a + b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The sum of the two matrices.
 */
ac_mat4 ac_mat4_add_matrix(const ac_mat4* a, const ac_mat4* b);
/**
 * \ingroup mat4
 * \brief Subtracts two matrices, a - b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The difference of the two matrices.
 */
ac_mat4 ac_mat4_sub_matrix(const ac_mat4* a, const ac_mat4* b);
/**
 * \ingroup mat4
 * \brief Multiplies two matrices together, a * b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The product of the two matrices.
 */
ac_mat4 ac_mat4_mult_matrix(const ac_mat4* a, const ac_mat4* b);
/**
 * \ingroup mat4
 * \brief Adds a scalar to each element of a matrix, m + scalar.
 * \param[in] m The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with the scalar added to each element.
 */
ac_mat4 ac_mat4_add_scalar(const ac_mat4* m, float scalar);
/**
 * \ingroup mat4
 * \brief Subtracts a scalar from each element of a matrix, m - scalar.
 * \param[in] m The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with the scalar subtracted from each element.
 */
ac_mat4 ac_mat4_sub_scalar(const ac_mat4* m, float scalar);
/**
 * \ingroup mat4
 * \brief Multiplies each element of a matrix by a scalar, m * scalar.
 * \param[in] m The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with each element multiplied by the scalar.
 */
ac_mat4 ac_mat4_mult_scalar(const ac_mat4* m, float scalar);
/**
 * \ingroup mat4
 * \brief Transposes the matrix.
 * \param[in] m The matrix to transpose
 * \return The transposed matrix.
 * \details
 * The transpose of a matrix is the matrix with the rows and columns swapped.
 * \code{.unparsed}
 * | a, b, c, d |      | a, e, i, m |
 * | e, f, g, h |  ->  | b, f, j, n |
 * | i, j, k, l |      | c, g, k, o |
 * | m, n, o, p |      | d, h, l, p |
 * \endcode
 */
ac_mat4 ac_mat4_transpose(const ac_mat4* m);
/**
 * \ingroup mat4
 * \brief Calculates the determinant of the matrix.
 * \param[in] m The matrix.
 * \return The determinant of the matrix.
 */
float   ac_mat4_determinant(const ac_mat4* m);
/**
 * \ingroup mat4
 * \brief Generates a submatrix (3x3) for the given matrix (4x4).
 * \param[in] m The matrix.
 * \param[in] row_to_exclude The row to exclude from the submatrix.
 * \param[in] col_to_exclude The column to exclude from the submatrix.
 * \return The submatrix that excludes the given row and column.
 * \details
 * The function generates a 3x3 submatrix from the 4x4 matrix by excluding
 * the given row and column. The submatrix is generated by copying the
 * elements of the 4x4 matrix into the 3x3 matrix, excluding the row and
 * column specified. This is used internally to generate the cofactor matrix
 * and adjugate matrix for calculating the inverse of a 4x4 matrix.
 */
ac_mat3 ac_mat4_to_submatrix(const ac_mat4* m, int row_to_exclude, int col_to_exclude);
/**
 * \ingroup mat3
 * \brief Calculates the minor of a matrix element.
 * \param[in] m The matrix.
 * \param[in] row The row index of the element.
 * \param[in] col The column index of the element.
 * \return The minor of the element.
 * \details
 * The minor of a matrix element is the determinant of the 3x3 submatrix
 * that excludes the row and column of the element. The minor is calculated
 * by generating the submatrix and then calculating the determinant of the
 * submatrix.
 */
float   ac_mat4_minor(const ac_mat4* m, int row, int col);
/**
 * \ingroup mat3
 * \brief Calculates the cofactor of a matrix element.
 * \param[in] m The matrix.
 * \param[in] row The row index of the element.
 * \param[in] col The column index of the element.
 * \return The cofactor of the element.
 * \details
 * The cofactor of a matrix element is the minor of the element multiplied
 * by -1 raised to the power of the row and column index (the place sign).
 */
float   ac_mat4_cofactor_element(const ac_mat4* m, int row, int col);
/**
 * \ingroup mat4
 * \brief Calculates the cofactor matrix of the matrix.
 * \param[in] m The matrix.
 * \return The cofactor matrix.
 * \details
 * The cofactor matrix is generated by calculating the determinant of each
 * 3x3 submatrix of the 4x4 matrix. The cofactor is then calculated by
 * multiplying the determinant by -1 raised to the power of the row and
 * column index. The cofactor matrix is then generated by setting the
 * elements of the cofactor matrix to the calculated cofactor values.
 * \see ac_mat4_to_submatrix
 */
ac_mat4 ac_mat4_cofactor_matrix(const ac_mat4* m);
/**
 * \ingroup mat4
 * \brief Calculates the adjugate matrix of the matrix.
 * \param[in] m The matrix.
 * \return The adjugate matrix.
 * \details
 * The adjugate matrix is the transpose of the cofactor matrix.
 */
ac_mat4 ac_mat4_adjugate(const ac_mat4* m);
/**
 * \ingroup mat4
 * \brief Calculates the inverse of the matrix.
 * \param[in] m The matrix.
 * \return The inverse of the matrix.
 * \details
 * The inverse of a 4x4 matrix is calculated by dividing the adjugate matrix
 * by the determinant of the matrix.
 */
ac_mat4 ac_mat4_inverse(const ac_mat4* m);

#ifdef __cplusplus
}
#endif
