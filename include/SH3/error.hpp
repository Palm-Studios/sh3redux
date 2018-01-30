/** @file
 *  Definition of @ref error.
 */
#ifndef SH3_ERROR_HPP_INCLUDED
#define SH3_ERROR_HPP_INCLUDED

/**
 *  Wrapper around a result-enum.
 *
 *  The intended usage is:
 *
 *      enum class my_result
 *      {
 *          SUCCESS,
 *          ERROR_A,
 *          ERROR_B,
 *      };
 *      struct my_error final : public error<my_result>
 *      {
 *      public:
 *          std::string message() const; // convert the result to a printable string
 *      };
 *
 *  The sub-class may embed further errors and the success-constant may differ from @c result_enum::SUCCESS:
 *
 *      enum class my_other_result
 *      {
 *          OK,
 *          MY_ERROR,
 *      };
 *      struct my_error final : public error<my_result, my_result::OK>
 *      {
 *      public:
 *          void set_error(my_other_result other_res, my_result my_res) { result = other_res; my_err = my_res; }
 *      private:
 *          my_result my_err = my_result::SUCCESS;
 *      };
 *
 *  Sub-classes are encouraged to provide a @c message() method as shown in the first example.
 *
 *  @tparam result_enum The result enum.
 *  @tparam SUCCESS     Enum value indicating success.
 */
template<typename result_enum, result_enum SUCCESS = result_enum::SUCCESS>
struct error
{
protected:
    error() = default;
    error(const error&) = default;
    ~error() = default;

public:
    /**
     *  Check if error is present.
     *
     *  @returns @c true if an error is present, @c false otherwise.
     */
    operator bool() const { return result != SUCCESS; }

    /**
     *  Get the wrapped @c result_enum.
     *
     *  @returns The @c result_enum.
     */
    result_enum get_result() const { return result; }

    /**
     *  Set the wrapped @c result_enum.
     *
     *  @param res The result to assign.
     */
    void set_error(result_enum res) { result = res; }

protected:
    result_enum result = SUCCESS;  /**< The wrapped @c result_enum. */
};

#endif //SH3_ERROR_HPP_INCLUDED
