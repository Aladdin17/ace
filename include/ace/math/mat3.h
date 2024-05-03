/**
 * \file
 * \author Christien Alden
 * \brief 3x3 matrix definition and functions.
 */
#pragma once
#include "mat2.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup mat3 mat3
 * \brief 3x3 matrix definitions and functions.
 */

/**
 * \ingroup mat3
 * \def AC_MAT3_NUM_ELEMENTS
 * \brief The number of elements in a 3x3 matrix.
 */
#define AC_MAT3_NUM_ELEMENTS 9

/**
 * \union ac_mat3
 * \brief A 3x3 matrix in column-major order.
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
 * | m00, m01, m02 |
 * | m10, m11, m12 |
 * | m20, m21, m22 |
 *
 * index using a,b,... notation
 * | a, b, c |
 * | d, e, f |
 * | g, h, i |
 * \endcode
 */
typedef union ac_mat3
{
    struct
    {
        float m00, m10, m20;
        float m01, m11, m21;
        float m02, m12, m22;
    };
    struct
    {
        float a, d, g;
        float b, e, h;
        float c, f, i;
    };
    float data[AC_MAT3_NUM_ELEMENTS];
} ac_mat3;

/**
 * \ingroup mat3
 * \brief Generates and returns the identity matrix.
 * \return The identity matrix.
 */
ac_mat3 ac_mat3_identity(void);
/**
 * \ingroup mat3
 * \brief Generates and returns a matrix with all elements set to NaN.
 * \return The nan matrix.
 */
ac_mat3 ac_mat3_nan(void);
/**
 * \ingroup mat3
 * \brief Generates and returns a matrix with all elements set to zero.
 * \return The zero matrix.
 */
ac_mat3 ac_mat3_zero(void);
/**
 * \ingroup mat3
 * \brief Creates a 3x3 matrix from an array of values in column-major order.
 * \param[in] values The array of values in column-major order.
 * \return A 3x3 matrix with the data set to the values.
 * \warning The \p values array must contain at least 9 elements.
 */
ac_mat3 ac_mat3_create(const float values[AC_MAT3_NUM_ELEMENTS]);
/**
 * \ingroup mat3
 * \brief Sets the values of the matrix.
 * \param[out] m The matrix.
 * \param[in] values The array of values in column-major order.
 * \warning The \p values array must contain at least 9 elements.
 */
void    ac_mat3_set_values(ac_mat3* m, const float values[AC_MAT3_NUM_ELEMENTS]);
/**
 * \ingroup mat3
 * \brief Checks whether the matrix is the identity matrix.
 * \param[in] m The matrix to check.
 * \retval true if the matrix is the identity matrix.
 * \retval false if the matrix is not the identity matrix.
 * \see ac_mat3_identity
 */
bool    ac_mat3_is_identity(const ac_mat3* m);
/**
 * \ingroup mat3
 * \brief Checks whether the matrix contains any NaN values.
 * \param[in] m The matrix to check.
 * \retval true if the matrix contains any NaN values.
 * \retval false if the matrix does not contain any NaN values.
 */
bool    ac_mat3_is_nan(const ac_mat3* m);
/**
 * \ingroup mat3
 * \brief Checks whether the matrix is the zero matrix.
 * \param[in] m The matrix to check.
 * \retval true if the matrix is the zero matrix.
 * \retval false if the matrix is not the zero matrix.
 */
bool    ac_mat3_is_zero(const ac_mat3* m);
/**
 * \ingroup mat3
 * \brief Checks whether two matrices are equal.
 * \param[in] a The first matrix to compare.
 * \param[in] b The second matrix to compare.
 * \retval true if the matrices are equal.
 * \retval false if the matrices are not equal.
 * \details
 * The function uses \ref ac_float_compare with an epsilon value of
 * \ref AC_EPSILON to compare the elements of the matrices.
 */
bool    ac_mat3_is_equal(const ac_mat3* a, const ac_mat3* b);
/**
 * \ingroup mat3
 * \brief Adds two matrices together, a + b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The sum of the two matrices.
 */
ac_mat3 ac_mat3_add_matrix(const ac_mat3* a, const ac_mat3* b);
/**
 * \ingroup mat3
 * \brief Subtracts two matrices, a - b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The difference of the two matrices.
 */
ac_mat3 ac_mat3_sub_matrix(const ac_mat3* a, const ac_mat3* b);
/**
 * \ingroup mat3
 * \brief Multiplies two matrices together, a * b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The product of the two matrices.
 */
ac_mat3 ac_mat3_mult_matrix(const ac_mat3* a, const ac_mat3* b);
/**
 * \ingroup mat3
 * \brief Adds a scalar to each element of a matrix, m + scalar.
 * \param[in] m The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with the scalar added to each element.
 */
ac_mat3 ac_mat3_add_scalar(const ac_mat3* m, float scalar);
/**
 * \ingroup mat3
 * \brief Subtracts a scalar from each element of a matrix, m - scalar.
 * \param[in] m The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with the scalar subtracted from each element.
 */
ac_mat3 ac_mat3_sub_scalar(const ac_mat3* m, float scalar);
/**
 * \ingroup mat3
 * \brief Multiplies each element of a matrix by a scalar, m * scalar.
 * \param[in] m The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with each element multiplied by the scalar.
 */
ac_mat3 ac_mat3_mult_scalar(const ac_mat3* m, float scalar);
/**
 * \ingroup mat3
 * \brief Transposes the matrix.
 * \param[in] m The matrix to transpose
 * \return The transposed matrix.
 * \details
 * The transpose of a matrix is the matrix with the rows and columns swapped.
 * \code{.unparsed}
 * | a, b, c |      | a, d, g |
 * | d, e, f |  ->  | b, e, h |
 * | g, h, i |      | c, f, i |
 * \endcode
 */
ac_mat3 ac_mat3_transpose(const ac_mat3* m);
/**
 * \ingroup mat3
 * \brief Calculates the determinant of the matrix.
 * \param[in] m The matrix.
 * \return The determinant of the matrix.
 */
float   ac_mat3_determinant(const ac_mat3* m);
/**
 * \ingroup mat3
 * \brief Generates a submatrix (2x2) for the given matrix (3x3).
 * \param[in] m The matrix.
 * \param[in] row_to_exclude The row to exclude from the submatrix.
 * \param[in] col_to_exclude The column to exclude from the submatrix.
 * \return The submatrix that excludes the given row and column.
 * \details
 * The function generates a 2x2 submatrix from the 3x3 matrix by excluding
 * the given row and column. The submatrix is generated by copying the
 * elements of the 3x3 matrix into the 2x2 matrix, excluding the row and
 * column specified. This is used internally to generate the cofactor matrix
 * and adjugate matrix for calculating the inverse of a 3x3 matrix.
 */
ac_mat2 ac_mat3_to_submatrix(const ac_mat3* m, int row_to_exclude, int col_to_exclude);
/**
 * \ingroup mat3
 * \brief Calculates the minor of a matrix element.
 * \param[in] m The matrix.
 * \param[in] row The row index of the element.
 * \param[in] col The column index of the element.
 * \return The minor of the element.
 * \details
 * The minor of a matrix element is the determinant of the 2x2 submatrix
 * that excludes the row and column of the element. The minor is calculated
 * by generating the submatrix and then calculating the determinant of the
 * submatrix.
 */
float   ac_mat3_minor(const ac_mat3* m, int row, int col);
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
float   ac_mat3_cofactor_element(const ac_mat3* m, int row, int col);
/**
 * \ingroup mat3
 * \brief Calculates the cofactor matrix of the matrix.
 * \param[in] m The matrix.
 * \return The cofactor matrix.
 * \details
 * The cofactor matrix is generated by calculating the determinant of each
 * 2x2 submatrix of the 3x3 matrix. The cofactor is then calculated by
 * multiplying the determinant by -1 raised to the power of the row and
 * column index. The cofactor matrix is then generated by setting the
 * elements of the cofactor matrix to the calculated cofactor values.
 * \see ac_mat3_to_submatrix
 */
ac_mat3 ac_mat3_cofactor_matrix(const ac_mat3* m);
/**
 * \ingroup mat3
 * \brief Calculates the adjugate matrix of the matrix.
 * \param[in] m The matrix.
 * \return The adjugate matrix.
 * \details
 * The adjugate matrix is the transpose of the cofactor matrix.
 */
ac_mat3 ac_mat3_adjugate(const ac_mat3* m);
/**
 * \ingroup mat3
 * \brief Calculates the inverse of the matrix.
 * \param[in] m The matrix.
 * \return The inverse of the matrix.
 * \details
 * The inverse of a 3x3 matrix is calculated by dividing the adjugate matrix
 * by the determinant of the matrix.
 */
ac_mat3 ac_mat3_inverse(const ac_mat3* m);

#ifdef __cplusplus
}
#endif
