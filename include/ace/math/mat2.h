/**
 * \file
 * \author Christien Alden
 * \brief 2x2 matrix definition and functions.
 */
#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup mat2 mat2
 * \brief 2x2 matrix functions and definitions.
 */

/**
 * \ingroup mat2
 * \def AC_MAT2_NUM_ELEMENTS
 * \brief The number of elements in a 2x2 matrix.
 */
#define AC_MAT2_NUM_ELEMENTS 4

/**
 * \ingroup mat2
 * \union ac_mat2
 * \brief A 2x2 matrix in column-major order.
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
 * | m00, m01 |
 * | m10, m11 |
 *
 * index using a,b,... notation
 * | a, b |
 * | c, d |
 * \endcode
 */
typedef union ac_mat2
{
    struct
    {
        float m00, m10;
        float m01, m11;
    };
    struct
    {
        float a, c;
        float b, d;
    };
    float data[AC_MAT2_NUM_ELEMENTS];
} ac_mat2;

/**
 * \ingroup mat2
 * \brief Generates and returns the identity matrix.
 * \return The identity matrix.
 */
ac_mat2 ac_mat2_identity(void);
/**
 * \ingroup mat2
 * \brief Generates and returns a matrix with all elements set to NaN.
 * \return A nan matrix.
 */
ac_mat2 ac_mat2_nan(void);
/**
 * \ingroup mat2
 * \brief Generates and returns a matrix with all elements set to zero.
 * \return The zero matrix.
 */
ac_mat2 ac_mat2_zero(void);
/**
 * \ingroup mat2
 * \brief Creates a 2x2 matrix from an array of values in column-major order.
 * \param[in] values The array of values in column-major order.
 * \return A 2x2 matrix with the data set to the values.
 * \warning The \p values array must contain at least 4 elements.
 */
ac_mat2 ac_mat2_create(const float values[AC_MAT2_NUM_ELEMENTS]);
/**
 * \ingroup mat2
 * \brief Sets the values of the matrix.
 * \param[out] m The matrix.
 * \param[in] values The array of values in column-major order.
 * \warning The \p values array must contain at least 4 elements.
 */
void    ac_mat2_set_values(ac_mat2* m, const float values[AC_MAT2_NUM_ELEMENTS]);
/**
 * \ingroup mat2
 * \brief Checks whether the matrix is the identity matrix.
 * \param[in] m The matrix to check.
 * \retval true if the matrix is the identity matrix.
 * \retval false if the matrix is not the identity matrix.
 */
bool    ac_mat2_is_identity(const ac_mat2* m);
/**
 * \ingroup mat2
 * \brief Checks whether the matrix contains any NaN values.
 * \param[in] m The matrix to check.
 * \retval true if the matrix contains any NaN values.
 * \retval false if the matrix does not contain any NaN values.
 */
bool    ac_mat2_is_nan(const ac_mat2* m);
/**
 * \ingroup mat2
 * \brief Checks whether the matrix is the zero matrix.
 * \param[in] m The matrix to check.
 * \retval true if the matrix is the zero matrix.
 * \retval false if the matrix is not the zero matrix.
 */
bool    ac_mat2_is_zero(const ac_mat2* m);
/**
 * \ingroup mat2
 * \brief Checks whether two matrices are equal.
 * \param[in] a The first matrix to compare.
 * \param[in] b The second matrix to compare.
 * \retval true if the matrices are equal.
 * \retval false if the matrices are not equal.
 * \details
 * The function uses \ref ac_float_compare with an epsilon value of
 * \ref AC_EPSILON to compare the elements of the matrices.
 */
bool    ac_mat2_is_equal(const ac_mat2* a, const ac_mat2* b);
/**
 * \ingroup mat2
 * \brief Adds two matrices together, a + b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The sum of the two matrices.
 */
ac_mat2 ac_mat2_add_matrix(const ac_mat2* a, const ac_mat2* b);
/**
 * \ingroup mat2
 * \brief Subtracts two matrices, a - b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The difference of the two matrices.
 */
ac_mat2 ac_mat2_sub_matrix(const ac_mat2* a, const ac_mat2* b);
/**
 * \ingroup mat2
 * \brief Multiplies two matrices together, a * b.
 * \param[in] a The first matrix.
 * \param[in] b The second matrix.
 * \return The product of the two matrices.
 */
ac_mat2 ac_mat2_mult_matrix(const ac_mat2* a, const ac_mat2* b);
/**
 * \ingroup mat2
 * \brief Adds a scalar to each element of a matrix, m + scalar.
 * \param[in] a The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with the scalar added to each element.
 */
ac_mat2 ac_mat2_add_scalar(const ac_mat2* a, float scalar);
/**
 * \ingroup mat2
 * \brief Subtracts a scalar from each element of a matrix, m - scalar.
 * \param[in] a The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with the scalar subtracted from each element.
 */
ac_mat2 ac_mat2_sub_scalar(const ac_mat2* a, float scalar);
/**
 * \ingroup mat2
 * \brief Multiplies each element of a matrix by a scalar, m * scalar.
 * \param[in] a The matrix.
 * \param[in] scalar The scalar value.
 * \return The matrix with each element multiplied by the scalar.
 */
ac_mat2 ac_mat2_mult_scalar(const ac_mat2* a, float scalar);
/**
 * \ingroup mat2
 * \brief Transposes the matrix.
 * \param[in] m The matrix to transpose
 * \return The transposed matrix.
 * \details
 * The transpose of a matrix is the matrix with the rows and columns swapped.
 * \code{.unparsed}
 * | a, b |  ->  | a, c |
 * | c, d |      | b, d |
 * \endcode
 */
ac_mat2 ac_mat2_transpose(const ac_mat2* m);
/**
 * \ingroup mat2
 * \brief Calculates the determinant of the matrix.
 * \param[in] m The matrix.
 * \return The determinant of the matrix.
 * \details
 * The determinant of a 2x2 matrix is calculated as follows:
 * \code{.unparsed}
 * | a, b |  -> a * d - b * c
 * | c, d |
 * \endcode
 */
float   ac_mat2_determinant(const ac_mat2* m);
/**
 * \ingroup mat2
 * \brief Calculates the minor of a matrix element.
 * \param[in] m The matrix.
 * \param[in] row The row index of the element.
 * \param[in] col The column index of the element.
 * \return The minor of the element.
 * \details
 * The minor of a matrix element is the determinant of the 1x1 submatrix
 * that excludes the row and column of the element. The minor is calculated
 * by generating the submatrix and then calculating the determinant of the
 * submatrix.
 */
float   ac_mat2_minor(const ac_mat2* m, int row, int col);
/**
 * \ingroup mat2
 * \brief Calculates the cofactor of a matrix element.
 * \param[in] m The matrix.
 * \param[in] row The row index of the element.
 * \param[in] col The column index of the element.
 * \return The cofactor of the element.
 * \details
 * The cofactor of a matrix element is the minor of the element multiplied
 * by -1 raised to the power of the row and column index (the place sign).
 */
float   ac_mat2_cofactor_element(const ac_mat2* m, int row, int col);
/**
 * \ingroup mat2
 * \brief Calculates the cofactor matrix of the matrix.
 * \param[in] m The matrix.
 * \return The cofactor matrix.
 * \details
 * The cofactor matrix is generated by calculating the determinant of each
 * 1x1 submatrix of the 2x2 matrix. The cofactor is then calculated by
 * multiplying the determinant by -1 raised to the power of the row and
 * column index. The cofactor matrix is then generated by setting the
 * elements of the cofactor matrix to the calculated cofactor values.
 */
ac_mat2 ac_mat2_cofactor_matrix(const ac_mat2* m);
/**
 * \ingroup mat2
 * \brief Calculates the adjugate matrix of the matrix.
 * \param[in] m The matrix.
 * \return The adjugate matrix.
 * \details
 * The adjugate matrix of a 2x2 matrix is as follows:
 * \code{.unparsed}
 * | a, b |  ->  | d, -b |
 * | c, d |      | -c, a |
 * \endcode
 */
ac_mat2 ac_mat2_adjugate(const ac_mat2* m);
/**
 * \ingroup mat2
 * \brief Calculates the inverse of the matrix.
 * \param[in] m The matrix.
 * \return The inverse of the matrix.
 * \details
 * The inverse of a 2x2 matrix is calculated by dividing the adjugate matrix
 * by the determinant of the matrix.
 */
ac_mat2 ac_mat2_inverse(const ac_mat2* m);

#ifdef __cplusplus
}
#endif
