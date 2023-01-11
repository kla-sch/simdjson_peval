//
// Copyright 2023 kla-sch@gmx.de
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef SIMDJSON_PEVAL_H
#define SIMDJSON_PEVAL_H 1

#include <simdjson.h>

#include <utility>
#include <variant>
#include <optional>
#include <functional>
#include <string_view>
#include <string>


/**
 * Evaluate JSON data by prototype.
 */
namespace simdjson_peval {

#if _SIMDJSON_PEVAL_DO_TRACE == 1

// Switch on debugging.
#ifdef _SIMDJSON_PEVAL_DEBUG
#   undef _SIMDJSON_PEVAL_DEBUG
#endif
#define _SIMDJSON_PEVAL_DEBUG 1

#define _SIMDJSON_PEVAL_TRACE_SET_NAME(name)    \
    static const auto _sjp_trace_method_name = name
#define _SIMDJSON_PEVAL_TRACE(operation)                    \
    _trace_buffer.emplace_back(                                  \
        _sjp_trace_method_name, operation, __FILE__, __LINE__)
#define _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN()  \
    _SIMDJSON_PEVAL_TRACE("eval_begin")
#define _SIMDJSON_PEVAL_TRACE_EVAL_END()    \
    _SIMDJSON_PEVAL_TRACE("eval_end")
#define _SIMDJSON_PEVAL_TRACE_ERROR(code)                   \
    _SIMDJSON_PEVAL_TRACE("error:" +  std::to_string(code))

// Trace element.
struct _trace_element {
    const std::string_view name;
    const std::string operation;
    const std::string_view filename;
    size_t line;

    /// constructor
    _trace_element(
        const std::string_view name,
        const std::string_view operation,
        const std::string_view filename = "",
        size_t line = 0)
        : name(name)
        , operation(operation)
        , filename(filename)
        , line(line)
    { /* empty */ }

    /// write a trace element into a stream.
    friend std::ostream&
    operator<<(std::ostream& os, const _trace_element& logElem) {
        os << "["
           << logElem.name
           << " -> " << logElem.operation;
        if (!logElem.filename.empty()) {
            os << " (line: " << logElem.line
               << " " << logElem.filename << ")";
        }

        return os << "]";
    }

    /// compare two trace elements.
    bool
    operator==(const _trace_element &other) const noexcept {
        return (name == other.name && operation == other.operation);
    }
};

/// Type of trace element buffer.
using _trace_buffer_type = std::vector<_trace_element>;

/// Buffer to store trace elements.
extern _trace_buffer_type _trace_buffer;

/// Declare gloval trace buffer.
#define _SIMDJSON_PEVAL_DEF_GLOBAL_TRACE_BUFFER \
     simdjson_peval::_trace_buffer_type simdjson_peval::_trace_buffer

/// write vector of log elements.
inline std::ostream&
operator<<(std::ostream& os, const _trace_buffer_type &trace) {
    static const auto indent = 2;

    os << "[ DebugTrace:\n";
    auto lvl = 0;
    for (const auto &elem : trace) {
        if (elem.operation == "eval_end") {
            lvl -= indent;
        }

        os << "  ";
        if (lvl >= 0) {
            os << std::string(lvl, ' ');
        }
        else {
            os << "[lvl=" << lvl << "!]";
        }

        os << elem << '\n';

        if (elem.operation == "eval_begin") {
            lvl += indent;
        }
    }
    os << ']';

    return os;
}


#else // _SIMDJSON_PEVAL_DO_TRACE != 1

/// \cond DO_NOT_DOCUMENT
#define _SIMDJSON_PEVAL_TRACE_SET_NAME(name)
#define _SIMDJSON_PEVAL_TRACE(operation)
#define _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN()
#define _SIMDJSON_PEVAL_TRACE_EVAL_END()
#define _SIMDJSON_PEVAL_TRACE_ERROR(code)
#define _SIMDJSON_PEVAL_DEF_GLOBAL_TRACE_BUFFER
/// \endcond

#endif // _SIMDJSON_PEVAL_DO_TRACE


#if _SIMDJSON_PEVAL_DEBUG == 1
#    define _SIMDJSON_PEVAL_ASSERT(condition) assert(condition)
#else // _VECTOR_SET_DEBUG == 1

/// \cond DO_NOT_DOCUMENT
#    define _SIMDJSON_PEVAL_ASSERT(condition)
/// \endcond
#endif // _VECTOR_SET_DEBUG


/**
 * Single error message.
 */
class error_message {
    friend class error;
public:

    /**
     * Constructor.
     *
     * @param code Code of error to create.
     * @param path Path to the JSON element where the error occurred.
     */
    error_message(simdjson::error_code code, const std::string &path)
        : code(code), path(path)
    {
        _SIMDJSON_PEVAL_ASSERT(code);
    }

    /**
     * get error code.
     */
    simdjson::error_code
    get_code() const {
        return code;
    }

    /**
     * get error message.
     */
    const char *
    get_text() const {
        return simdjson::error_message(code);
    }

    /**
     * get path to element where the error occurred.
     */
    const std::string &
    get_path() const {
        return path;
    }

    /**
     * Converts all information into a one-line error message.
     */
    std::string
    to_string() const;

private:

    /// error code
    simdjson::error_code code;

    /// path to object.
    std::string path;

}; // class error_message


/**
 * Error container for collecting all errors that occurred during the
 * evaluation.
 */
class error {
public:
    using message_container = std::vector<error_message>;

    /**
     * Scope class to create path information.
     *
     * Create an instance of this class to create another path level
     * inside an error container.
     */
    class path_scope {
    public:

        /**
         * Constructor for array elements.
         *
         * @param err Pointer to error container to create a new
         *            path level for new errors added.
         * @param idx Index of array which is processing.
         */
        path_scope(error *err, size_t idx);

        /**
         * Constructor for object member elements.
         *
         * @param err Pointer to error container to create a new
         *            path level for new errors added.
         * @param member Name of the member.
         */
        path_scope(error *err, std::string_view member);

        /**
         * Destructor.
         */
        ~path_scope();

    private:

        /// Pointer to error container.
        error *error_ptr;

    }; // class path_scope


    /**
     * Constructor.
     *
     * @param pflag Flag indicating whether the container should collect
     *              element paths for errors.
     */
    error(bool pflag = false) : path_flag(pflag) { /* empty */ }


    /**
     * Convert error container into boolean.
     *
     * @return `true` if errors where collected.
     */
    operator bool() const {
        return (!messages.empty());
    }

    /**
     * Get all collected messages.
     *
     * @return Const reference to vector with collected error elements.
     */
    const message_container &
    get_messages() const {
        return messages;
    }

    /**
     * Add a new error if not success.
     *
     * Collect any error code which is not `simdjson::SUCCESS`.
     *
     * @param code Errorcode.
     */
    void
    add(simdjson::error_code code);

    /**
     * Checks if path information collection is enabled.
     *
     * @return `true` if collection of path information is enabled.
     */
    bool
    is_path_enabled() const {
        return path_flag;
    }

    /**
     * Convert all collected errors into one string.
     *
     * @return All collected errors in one string. Errors are separated
     *         by line breaks.
     */
    std::string
    to_string() const;

private:
    /// path element value.
    using path_value = std::variant<size_t, std::string_view>;

    /// All collected messages.
    message_container messages;

    /// Current path to elements.
    std::vector<path_value> path_vector;

    /// Flag: track path for error messages.
    bool path_flag;

    /**
     * Create string representation of element path.
     */
    std::string
    create_path();

}; // class error


// helper methods
// //////////////////////////////////////////////////////////////////////

// @name Internal implementations
// @private
namespace internal {

/**
 * Check for JS `null` and set `is_null` reference.
 *
 * @private
 *
 * @param sj_result The `simdjson::simdjson_result<>` to be checked.
 * @param is_null Pointer to location to store `null` status.
 *
 * @return `true` if `(is_null != nullptr)` and `sj_result` contains a
 *         `null` value.
 */
template<typename SJValue>
inline bool
check_null(SJValue &sj_result, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("check_null(SJValue,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");

    if (!is_null) {
        _SIMDJSON_PEVAL_TRACE("is_null:undef");
        return false;
    }

    *is_null = sj_result.value().is_null();
    _SIMDJSON_PEVAL_TRACE(*is_null ? "is_null:true" : "is_null:false");

    return (*is_null);
}

/**
 * Check for JS `null` and set optional value.
 *
 * @private
 *
 * @param sj_result The `simdjson::simdjson_result<>` to be checked.
 * @param opt_value Pointer to optional value. The value is set to
 *                  `std::nullopt`, if `sj_result` is `null`.
 *
 * @return `true` if `sj_result` contains a `null` value.
 */
template<typename SJValue, typename Optional>
inline bool
check_null(SJValue &sj_result, Optional *opt_value) {
    _SIMDJSON_PEVAL_ASSERT(opt_value != nullptr);

    _SIMDJSON_PEVAL_TRACE_SET_NAME("check_null(SJValue,Optional)");
    _SIMDJSON_PEVAL_TRACE("start");

    if (!sj_result.value().is_null()) {
        _SIMDJSON_PEVAL_TRACE("is_null:false");
        return false;
    }

    _SIMDJSON_PEVAL_TRACE("is_null:true");

    *opt_value = std::nullopt;
    return true;
}

/**
 * Create function to evaluate array element by array iterator.
 *
 * @private
 *
 * @param element_fn Evaluation function for an array element.
 *
 * @return Function that evaluates a single array element.
 */
template<typename ElementFn>
inline auto
array(ElementFn element_fn) {
    using ArrayIter =
        simdjson::simdjson_result<simdjson::ondemand::array_iterator>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("internal::array(ElementFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    return
        [element_fn]
        (ArrayIter *sj_array_iter, ArrayIter *sj_array_end,
         size_t sj_array_idx,
         error *err)
        mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            error::path_scope array_idx_scope(err, sj_array_idx);
            if (*sj_array_iter == *sj_array_end) {
                const auto code = simdjson::INDEX_OUT_OF_BOUNDS;
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                auto sj_result = **sj_array_iter;
                element_fn(sj_result, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create function to evaluate array elements by array iterator.
 *
 * @private
 *
 * @param element_fn First evaluation function for an array element.
 * @param more_element_fn More evaluation function for next array elements.
 *
 * @return Function that evaluates all array elements.
 */
template<
    typename ElementFn,
    typename ...MoreElementFn>
inline auto
array(ElementFn element_fn, MoreElementFn ...more_element_fn) {
    using ArrayIter =
        simdjson::simdjson_result<simdjson::ondemand::array_iterator>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "internal::array(ElementFn,MoreElementFn...)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto eval_more_fn =
        internal::array(
            std::forward<MoreElementFn>(more_element_fn)...);


    return
        [element_fn, eval_more_fn]
        (ArrayIter *sj_array_iter, ArrayIter *sj_array_end,
         size_t sj_array_idx,
         error *err)
        mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            {
                error::path_scope array_idx_scope(err, sj_array_idx);

                if (*sj_array_iter == *sj_array_end) {
                    const auto code = simdjson::INDEX_OUT_OF_BOUNDS;
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                }
                else {
                    auto sj_result = **sj_array_iter;
                    element_fn(sj_result, err);
                    ++sj_array_iter->value();
                }
            }

            eval_more_fn(sj_array_iter, sj_array_end, sj_array_idx+1, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

} // namespace internal

// main implementation
// //////////////////////////////////////////////////////////////////////

/**
 * Evaluate function.
 *
 * This function type is ceated by protortype functions like
 * `string_value()` `object()` or `array()` to evaluate a JSON
 * value.
 *
 * @param sj_result Parser result to evaluate.
 * @param err Pointer to error container to store errors.
 */
template<typename SJValue = simdjson::ondemand::value>
using eval_fn_type =
    std::function<
        void (simdjson::simdjson_result<SJValue> &sj_result, error *err)>;

///
/// @name Evaluate JSON scalar values.
//  /////////////////////////////////////////////////////////////////////
/// @{

/**
 * Create evalutation function to store any scalar value by function.
 *
 * The get function get a `simdjson_result` and a pointer to store the
 * value as parameters and a pointer to the error container to report
 * errors.
 *
 *     void
 *     get_fn(simdjson_result<type> *result, Value *val_ptr, error *err);
 *
 * @param val_ptr Pointer to location to store the value.
 * @param get_fn Get-function to collect and store value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value,
         typename ValuePtr,
         typename GetFn>
inline auto
value(ValuePtr *val_ptr, GetFn get_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;
    _SIMDJSON_PEVAL_TRACE_SET_NAME("value(ValuePtr,GetFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    return
        [val_ptr, get_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            get_fn(&sj_result, val_ptr, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store any scalar value by function.
 *
 * The get function get a `simdjson_result` and a pointer to store the
 * value as parameters and a pointer to the error container to store
 * errors.
 *
 *     void
 *     get_fn(simdjson_result<type> *result, Value *val_ptr, error *err);
 *
 * @param val_ptr Pointer to location to store the value.
 * @param get_fn Get-function to collect and store value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value,
         typename ValuePtr,
         typename GetFn>
inline auto
value(ValuePtr val_ptr, GetFn get_fn, bool *is_null) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("value(ValuePtr,GetFn,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    return
        [val_ptr, is_null, get_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
                _SIMDJSON_PEVAL_TRACE_EVAL_END();
                return;
            }

            get_fn(&sj_result, val_ptr, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to store any scalar value as
 * `std::optional` by function.
 *
 * The get function get a `simdjson_result` and a pointer to store the
 * value as parameters and a pointer to the error container to store
 * errors.
 *
 *     void
 *     get_fn(simdjson_result<type> *result, Value *opt_ptr, error *err);
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 * @param get_fn Get-function to collect and store value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value,
         typename Value,
         typename GetFn>
inline auto
value(std::optional<Value> *opt_ptr, GetFn get_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("value(std::optional<Value>*,GetFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    return
        [opt_ptr, get_fn]
        (ResultType &sj_result, error *err) {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, opt_ptr)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
                _SIMDJSON_PEVAL_TRACE_EVAL_END();
                return;
            }

            opt_ptr->emplace();
            get_fn(&sj_result, opt_ptr, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to execute a generic function.
 *
 * The get function get a `simdjson_result` and a pointer to store the
 * value as parameters and a pointer to the error container to store
 * errors.
 *
 *     void
 *     get_fn(simdjson_result<type> *result, error *err);
 *
 * @param get_fn Get-function to collect the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value,
         typename GetFn>
inline auto
value(GetFn get_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("value(GetFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    return
        [get_fn]
        (ResultType &sj_result, error *err) {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            get_fn(&sj_result, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to store a value into a simdjson value.
 *
 * Only array and object values work with `simdjson::ondemand::document`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
value(simdjson::simdjson_result<simdjson::ondemand::value> *val_ptr)
{
    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "value(simdjson::simdjson_result<simdjson::ondemand::value>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            if constexpr(
                std::is_same_v<
                    decltype(*sj_result),
                    simdjson::simdjson_result<simdjson::ondemand::value>&>)
            {
                _SIMDJSON_PEVAL_TRACE("type:value");
                *val_ptr = *sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                *val_ptr = sj_result->value().get_value();
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/// @}

///
/// @name Evaluate JSON string values.
//  /////////////////////////////////////////////////////////////////////
/// @{


/**
 * Create evalutation function to store a string into a
 * `std::string_view`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::string_view *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::string_view*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}


/**
 * Create evalutation function to store a string into a
 * `std::string_view`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::string_view *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::string_view*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to store a string into an
 * `std::optional<std::string_view>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::optional<std::string_view> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "string_value(std::optional<std::string_view>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_string().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}

/**
 * Create evalutation function to store a string into a
 * `std::string`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::string *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::string*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            std::string_view tmp_value;
            const auto code = sj_result->value().get_string().get(tmp_value);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                *val_ptr = tmp_value;
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to store a string into a
 * `std::string`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::string *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::string*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            std::string_view tmp_value;
            const auto code = sj_result->value().get_string().get(tmp_value);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                *val_ptr = tmp_value;
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to store a string into an
 * `std::optional<std::string>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::optional<std::string> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::optional<std::string>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            std::string_view tmp_value;
            const auto code = sj_result->value().get_string().get(tmp_value);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                **opt_ptr = tmp_value;
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}


/**
 * Create evalutation function to convert and store a string into a
 * `int64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(int64_t *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(int64_t*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_int64_in_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to convert and store a string into a
 * `int64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(int64_t *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(int64_t*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_int64_in_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to convert and store a string into an
 * `std::optional<int64_t>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::optional<int64_t> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::optional<int64_t>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_int64_in_string().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}

/**
 * Create evalutation function to convert and store a string into a
 * `uint64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(uint64_t *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(uint64_t*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_uint64_in_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to convert and store a string into a
 * `uint64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(uint64_t *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(uint64_t*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_uint64_in_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to convert and store a string into an
 * `std::optional<uint64_t>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::optional<uint64_t> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::optional<uint64_t>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_uint64_in_string().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}

/**
 * Create evalutation function to convert and store a string into a
 * `double`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(double *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(double*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_double_in_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to convert and store a string into a
 * `double`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(double *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(double*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_double_in_string().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to convert and store a string into an
 * `std::optional<double>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
string_value(std::optional<double> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("string_value(std::optional<double>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code =
                sj_result->value().get_double_in_string().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}


/// @}

///
/// @name Evaluate JSON number values.
//  /////////////////////////////////////////////////////////////////////
/// @{

/**
 * Create evalutation function to store a number into an
 * `int64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(int64_t *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(int64_t*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_int64().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to store a number into an
 * `int64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(int64_t *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(int64_t*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_int64().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to store a string into an
 * `std::optional<int64_t>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(std::optional<int64_t> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(std::optional<int64_t>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_int64().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}

/**
 * Create evalutation function to store a number into a
 * `uint64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(uint64_t *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(uint64_t*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_uint64().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to store a number into a
 * `uint64_t`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(uint64_t *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(uint64_t*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_uint64().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to store a string into a
 * `std::optional<uint64_t>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(std::optional<uint64_t> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(std::optional<uint64_t>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_uint64().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}

/**
 * Create evalutation function to store a number into a
 * `double`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(double *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(double*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_double().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to store a number into a
 * `double`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(double *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(double*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_double().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to store a string into an
 * `std::optional<double>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
number_value(std::optional<double> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("number_value(std::optional<double>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_double().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}

/// @}

///
/// @name Evaluate JSON bool values.
//  /////////////////////////////////////////////////////////////////////
/// @{

/**
 * Create evalutation function to store a number into a
 * `bool`.
 *
 * @param val_ptr Pointer to location to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
bool_value(bool *val_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("bool_value(bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_bool().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value);
}

/**
 * Create evalutation function to store a number into a
 * `bool`.
 *
 * @param val_ptr Pointer to location to store the value.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
bool_value(bool *val_ptr, bool *is_null) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("bool_value(bool*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            const auto code = sj_result->value().get_bool().get(*val_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}

/**
 * Create evalutation function to store a string into an
 * `std::optional<bool>`.
 *
 * @param opt_ptr Pointer to `std::optional` to store the value.
 *
 * @return Function to evaluate JSON value.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
bool_value(std::optional<bool> *opt_ptr) {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("bool_value(std::optional<bool>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(opt_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *opt_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            auto code = sj_result->value().get_bool().get(**opt_ptr);
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
        };

    return value<SJValue>(opt_ptr, get_value);
}

/// @}

///
/// @name Evaluate JSON objects.
//  /////////////////////////////////////////////////////////////////////
/// @{


/**
 * Create evalutation function to call a function for all members.
 *
 * For each member of the object, the created function calls the
 * function "out_fn" with the name of the member, a pointer to the
 * value as "simdjson_result<>" and a pointer to the error container
 * as parameters.
 *
 *     void
 *     get_fn(
 *         std::string_view member_name,
 *         simdjson::simdjson_result<simdjson::ondemand::value> *result,
 *         error *err);
 *
 * @param out_fn Function call for each member.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn>
inline auto
object_to_function(OutFn out_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("object_to_function(OutFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    return
        [out_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            simdjson::simdjson_result<simdjson::ondemand::object> sj_object;
            if constexpr(std::is_same_v<SJValue, simdjson::ondemand::object>)
            {
                _SIMDJSON_PEVAL_TRACE("type:object");
                sj_object = sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                sj_object = sj_result.value().get_object();
                const auto code = sj_object.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                    _SIMDJSON_PEVAL_TRACE_EVAL_END();
                    return;
                }
            }

            for (auto field : sj_object) {
                std::string_view key;
                const auto code_key = field.unescaped_key().get(key);
                if (code_key) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code_key);
                    err->add(code_key);
                }
                else {
                    error::path_scope member_error_scope(err, key);

                    _SIMDJSON_PEVAL_TRACE("get:" + std::string(key));
                    const auto code_value = field.value().error();
                    if (code_value) {
                        _SIMDJSON_PEVAL_TRACE_ERROR(code_value);
                        err->add(code_value);
                    }
                    else {
                        auto value = field.value();
                        out_fn(key, &value, err);
                    }
                }
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to call a function for all members.
 *
 * For each member of the object, the created function calls the
 * function "out_fn" with the name of the member, a pointer to the
 * value as "simdjson_result<>" and a pointer to the error container
 * as parameters.
 *
 *     void
 *     get_fn(
 *         std::string_view member_name,
 *         simdjson::simdjson_result<simdjson::ondemand::value> *result,
 *         error *err);
 *
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 * @param out_fn Function call for each member.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn>
inline auto
object_to_function(bool *is_null, OutFn out_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("object_to_function(bool*,OutFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto call_fn = object_to_function<SJValue>(out_fn);

    return
        [is_null, call_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
            }
            else {
                call_fn(sj_result, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to store object data by function.
 *
 * The created function that uses `value_fn` to valuate member of the
 * object. This function has to store the result into
 * `temp_value_ptr->second`. Then `out_fn` is called with `temp_pair_ptr`
 * and a pointer to the error class to store the element.
 *
 *     void
 *     get_fn(std::pair<StringType, ValueType> *temp_pair_ptr, error *err)
 *
 * @param out_fn Function call for each member to store the temporary value.
 * @param temp_pair_ptr Pointer to a temporary space to store pair elements.
 * @param value_fn Function to evaluate a single member value and store
 *                it into `temp_pair_ptr.second`.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn,
    typename StringType,
    typename ValueType,
    typename ValueFn>
inline auto
object_to_function(
    OutFn out_fn,
    std::pair<StringType, ValueType> *temp_pair_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;
    using SJValueType = simdjson::simdjson_result<simdjson::ondemand::value>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_pair_ptr != nullptr);

    auto gen_out_fn =
        object_to_function<SJValue>(
            [out_fn, temp_pair_ptr, value_fn]
            (std::string_view name, SJValueType *sj_result, error *err)
            mutable
            {
                _SIMDJSON_PEVAL_TRACE("start_gen_out_fn");
                _SIMDJSON_PEVAL_ASSERT(err != nullptr);

                temp_pair_ptr->first = name;
                value_fn(*sj_result, err);

                out_fn(temp_pair_ptr, err);

                _SIMDJSON_PEVAL_TRACE("end_gen_out_fn");
            });

    return
        [gen_out_fn]
        (ResultType &sj_result, error *err) mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            gen_out_fn(sj_result, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store object data by function.
 *
 * The created function that uses `value_fn` to valuate member of the
 * object. This function has to store the result into
 * `temp_value_ptr->second`. Then `out_fn` is called with `*temp_pair_ptr`
 * and a pointer to the error class.
 *
 *     void
 *     get_fn(const std::pair<StringType, ValueType> &temp_pair, error *err)
 *
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 * @param out_fn Function call for each member.
 * @param temp_pair_ptr Pointer to a temporary space to store pair elements.
 * @param value_fn Function to evaluate a single member value and store
 *                it into `temp_pair_ptr.second`.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn,
    typename StringType,
    typename ValueType,
    typename ValueFn>
inline auto
object_to_function(
    bool *is_null,
    OutFn out_fn,
    std::pair<StringType, ValueType> *temp_pair_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "object_to_function("
        "bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_pair_ptr != nullptr);

    auto call_fn =
        object_to_function<SJValue>(out_fn, temp_pair_ptr, value_fn);

    return
        [is_null, call_fn]
        (ResultType &sj_result, error *err) mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
            }
            else {
                call_fn(sj_result, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store object data into an output iterator.
 *
 * The created function that calls `value_fn` for each member of the
 * object. This function has to store the result into
 * `temp_value_ptr->second`. Then the pair is moved into the output
 * iterator after evaluation of the member.
 *
 * @param out_iter Output iterator to store values.
 * @param temp_pair_ptr Pointer to a temporary space to store pair elements.
 * @param value_fn Function to evaluate a single member value and store
 *                 it into `temp_pair_ptr.second`.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutIter,
    typename StringType,
    typename ValueType,
    typename ValueFn>
inline auto
object_to_out_iter(
    OutIter out_iter,
    std::pair<StringType, ValueType> *temp_pair_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "object_to_out_iter"
        "(OutIter,std::pair<StringType,ValueType>*,MemberFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_pair_ptr != nullptr);

    auto out_fn =
        [out_iter]
        (auto *pair_ptr, error *err) mutable {
            *out_iter = std::move(*pair_ptr);
            ++out_iter;
        };

    auto save_fn =
        object_to_function<SJValue>(out_fn, temp_pair_ptr, value_fn);

    return
        [save_fn]
        (ResultType &sj_result, error *err) mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            save_fn(sj_result, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store object data into an output iterator.
 *
 * The created function that calls `value_fn` for each member of the
 * object. This function has to store the result into
 * `temp_value_ptr->second`. Then the pair is moved into the output
 * iterator after evaluation of the member.
 *
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 * @param out_iter Output iterator to store values.
 * @param temp_pair_ptr Pointer to a temporary space to store pair elements.
 * @param value_fn Function to evaluate a single member value and store
 *                it into `temp_pair_ptr.second`.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutIter,
    typename StringType,
    typename ValueType,
    typename ValueFn>
inline auto
object_to_out_iter(
    bool *is_null,
    OutIter out_iter,
    std::pair<StringType, ValueType> *temp_pair_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "object_to_out_iter"
        "(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_pair_ptr != nullptr);

    auto save_fn =
        object_to_out_iter<SJValue>(out_iter, temp_pair_ptr, value_fn);

    return
        [is_null, save_fn]
        (ResultType &sj_result, error *err) mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
            }
            else {
                save_fn(sj_result, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to store object data.
 *
 * @param member_fn An evalutation function to evaluate a member.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename MemberFn>
inline auto
object(MemberFn member_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("object(MemberFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    return
        [member_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if constexpr(std::is_same_v<SJValue, simdjson::ondemand::object>)
            {
                _SIMDJSON_PEVAL_TRACE("type:object");
                member_fn(sj_result, err);
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");

                auto sj_object = sj_result.value().get_object();
                const auto code = sj_object.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                }
                else {
                    member_fn(sj_object, err);
                }
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to store object data.
 *
 * @param member_fn An evalutation function to evaluate a member.
 * @param more_member_fn More function to evaluate members.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename MemberFn,
    typename ...MoreMemberFn>
inline auto
object(MemberFn member_fn,  MoreMemberFn ...more_member_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("object(MemberFn,MoreMemberFn...)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto eval_more_fn =
        object<simdjson::ondemand::object>(
            std::forward<MoreMemberFn>(more_member_fn)...);

    return
        [member_fn, eval_more_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if constexpr(std::is_same_v<SJValue, simdjson::ondemand::object>) {
                _SIMDJSON_PEVAL_TRACE("type:object");
                member_fn(sj_result, err);
                eval_more_fn(sj_result, err);
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                auto sj_object = sj_result.value().get_object();
                const auto code = sj_object.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                    _SIMDJSON_PEVAL_TRACE_EVAL_END();
                    return;
                }

                member_fn(sj_object, err);
                eval_more_fn(sj_object, err);
            }
            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to store object data (`null` allowed).
 *
 * @param is_null Pointer to location to store `null` status.
 * @param all_member_fn Multiple functions to evaluate members.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename ...AllMemberFn>
inline auto
object(bool *is_null, AllMemberFn ...all_member_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("object(bool*,AllMemberFn...)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto eval_all_fn =
        object<simdjson::ondemand::object>(
            std::forward<AllMemberFn>(all_member_fn)...);

    return
        [is_null, eval_all_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
                _SIMDJSON_PEVAL_TRACE_EVAL_END();
                return;
            }

            auto sj_object = sj_result.value().get_object();
            const auto code = sj_object.error();
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                eval_all_fn(sj_object, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store object value.
 *
 * @param val_ptr Pointer to simdjson obect.
 *
 * @return Function to evaluate JSON object.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
object(simdjson::simdjson_result<simdjson::ondemand::object> *val_ptr)
{
    using SJObjectType = simdjson::simdjson_result<simdjson::ondemand::object>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "object(simdjson::simdjson_result<simdjson::ondemand::object>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");

            if constexpr(std::is_same_v<decltype(*sj_result), SJObjectType&>) {
                _SIMDJSON_PEVAL_TRACE("type:object");
                *val_ptr = *sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                auto sj_object = sj_result->value().get_object();
                const auto code = sj_object.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                }
                else {
                    *val_ptr = sj_object;
                }
            }
        };

    return value<SJValue>(val_ptr, get_value);
}


/**
 * Create evalutation function to store object value.
 *
 * @param val_ptr Pointer to simdjson obect.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON object.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
object(
    simdjson::simdjson_result<simdjson::ondemand::object> *val_ptr,
    bool *is_null)
{
    using SJObjectType = simdjson::simdjson_result<simdjson::ondemand::object>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) {
            _SIMDJSON_PEVAL_TRACE("start:get_value");

            if constexpr(std::is_same_v<decltype(*sj_result), SJObjectType&>) {
                _SIMDJSON_PEVAL_TRACE("type:object");
                *val_ptr = *sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                auto sj_object = sj_result->value().get_object();
                const auto code = sj_object.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                }
                else {
                    *val_ptr = sj_object;
                }
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}


/**
 * Create evalutation function to store data of an object member.
 *
 * @param name Name of the member.
 * @param value_fn Function to evaluate member data (any of `value(...)`,
 *                `object(...)` or `array(...)`).
 *
 * @return Function to evaluate JSON object.
 */
template<typename ValueFn>
inline auto
member(std::string_view name, ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<simdjson::ondemand::object>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("member(std::string_view,ValueFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    return
        [name, value_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            error::path_scope member_error_scope(err, name);

            auto sj_value = sj_result.value().find_field_unordered(name);
            const auto code = sj_value.error();

            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                value_fn(sj_value, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store data of an object member.
 *
 * @param name Name of the member.
 * @param value_fn Function to evaluate member data (any of `value(...)`,
 *                `object(...)` or `array(...)`).
 * @param is_undefined Pointer to the location to store, whether the
 *                     member is defined or not. If `(is_undefined ==
 *                     nullptr)`, the member must be defined.
 *
 * @return Function to evaluate JSON object.
 */
template<typename ValueFn>
inline auto
member(std::string_view name, ValueFn value_fn, bool *is_undefined)
{
    using ResultType = simdjson::simdjson_result<simdjson::ondemand::object>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("member(std::string_view,ValueFn,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");

    return
        [name, value_fn, is_undefined]
        (ResultType &sj_result, error *err)
        mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            error::path_scope member_error_scope(err, name);

            auto sj_value = sj_result.value().find_field_unordered(name);
            const auto code = sj_value.error();

            if (is_undefined) {
                *is_undefined = false;
            }

            if (code == simdjson::NO_SUCH_FIELD && is_undefined) {
                _SIMDJSON_PEVAL_TRACE(
                    "undefined_member:" + std::string(name));
                *is_undefined = true;
            }
            else if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                value_fn(sj_value, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Creates an evalutation function to add object members under a condition.
 *
 * A function that returns a `bool` is passed as the first parameter.
 * Subsequent members are evaluated only if this functon returns
 * `true`:
 *
 *     bool
 *     use_members_fn(
 *         simdjson::simdjson_result<simdjson::ondemand::object> *result,
 *         simdjson_peval::error *err)
 *
 * @param use_members_fn Function to evaluate condition.
 * @param all_member_fn Multiple functions to evaluate members.
 *
 * @return Function for conditionally evaluating the members of a JSON object.
 */
template<typename UseMembersFn,
         typename ...AllMemberFn>
inline auto
discriminator(
    UseMembersFn use_members_fn,
    AllMemberFn...all_member_fn)
{
    using ResultType = simdjson::simdjson_result<simdjson::ondemand::object>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "discriminator(UseMembersFn,MemberFn,MoreMemberFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto eval_all_fn =
        object<simdjson::ondemand::object>(
            std::forward<AllMemberFn>(all_member_fn)...);

    return
        [use_members_fn, eval_all_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            if (use_members_fn(&sj_result, err)) {
                _SIMDJSON_PEVAL_TRACE("use_members");
                eval_all_fn(sj_result, err);
            }
            else {
                _SIMDJSON_PEVAL_TRACE("skip_members");
            }
            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/// @}

///
/// @name Evaluate JSON arrays.
//  /////////////////////////////////////////////////////////////////////
/// @{


/**
 * Create evalutation function to call function for all array elements.
 *
 * For each element of the array, the created function calls the
 * function "out_fn" with array index, the "simdjson_result<>" of the
 * element value and a pointer to the error container as parameters.
 *
 *     void
 *     get_fn(
 *         size_t idx,
 *         simdjson::simdjson_result<simdjson::ondemand::value> *result,
 *         error *err);
 *
 * @param out_fn Function call for each element.
 *
 * @return Function to evaluate JSON array.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn>
inline auto
array_to_function(OutFn out_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("array_to_function(OutFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    return
        [out_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            simdjson::simdjson_result<simdjson::ondemand::array> sj_array;
            if constexpr(std::is_same_v<SJValue, simdjson::ondemand::array>)
            {
                _SIMDJSON_PEVAL_TRACE("type:array");
                sj_array = sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                sj_array = sj_result.value().get_array();
                const auto code = sj_array.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                    _SIMDJSON_PEVAL_TRACE_EVAL_END();
                    return;
                }
            }

            size_t idx = 0;
            for (auto sj_value : sj_array) {
                error::path_scope array_idx_scope(err, idx);
                _SIMDJSON_PEVAL_TRACE("get:" + std::to_string(idx));

                out_fn(idx, &sj_value, err);

                ++idx;
            }
            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to call function for all array elements.
 *
 * For each element of the array, the created function calls the
 * function "out_fn" with array index, the "simdjson_result<>" of the
 * element value and a pointer to the error container as parameters.
 *
 *     void
 *     get_fn(
 *         size_t idx,
 *         simdjson::simdjson_result<simdjson::ondemand::value> *result,
 *         error *err);
 *
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 * @param out_fn Function call for each element.
 *
 * @return Function to evaluate JSON array.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn>
inline auto
array_to_function(bool *is_null, OutFn out_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("array_to_function(bool*.OutFn)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto call_fn = array_to_function<SJValue>(out_fn);

    return
        [is_null, call_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
            }
            else {
                call_fn(sj_result, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };

}

/**
 * Create evalutation function to store object data by function.
 *
 * The created function that calls `value_fn` for each element of the
 * array which has to store the result into `*temp_value_ptr`. Then
 * `out_fn` is called with the array index, `temp_pair_ptr` and a
 * pointer to the error container to store the array element.
 *
 *     void
 *     get_fn(size_t idx, ValueType *temp_pair, error *err)
 *
 * @param out_fn Function call for each member to store the temporary value.
 * @param temp_value_ptr Pointer to a temporary space to store array elements.
 * @param value_fn Function to evaluate a single array element and store
 *                it into `temp_value_ptr`.
 *
 * @return Function to evaluate JSON array.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn,
    typename TempValue,
    typename ValueFn>
inline auto
array_to_function(
    OutFn out_fn,
    TempValue *temp_value_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;
    using SJValueType = simdjson::simdjson_result<simdjson::ondemand::value>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "array_to_function(OutFn,TempValue*,ValueFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_value_ptr != nullptr);

    auto gen_out_fn =
        array_to_function<SJValue>(
            [out_fn, temp_value_ptr, value_fn]
            (size_t idx, SJValueType *sj_result, error *err) mutable {
                _SIMDJSON_PEVAL_TRACE("start_gen_out_fn");
                _SIMDJSON_PEVAL_ASSERT(err != nullptr);

                value_fn(*sj_result, err);
                out_fn(idx, temp_value_ptr, err);

                _SIMDJSON_PEVAL_TRACE("end_gen_out_fn");
            });

    return
        [gen_out_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            gen_out_fn(sj_result, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store object data by function.
 *
 * The created function that calls `value_fn` for each element of the
 * array which has to store the result into `*temp_value_ptr`. Then
 * `out_fn` is called with `*temp_pair_ptr` and a pointer to the error
 * class to store the array element.
 *
 *     void
 *     get_fn(ValueType *temp_pair, error *err)
 *
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 * @param out_fn Function call for each member to store the temporary value.
 * @param temp_value_ptr Pointer to a temporary space to store array elements.
 * @param value_fn Function to evaluate a single array element and store
 *                 it into `temp_value_ptr`.
 *
 * @return Function to evaluate JSON array.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutFn,
    typename TempValue,
    typename ValueFn>
inline auto
array_to_function(
    bool *is_null,
    OutFn out_fn,
    TempValue *temp_value_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "array_to_function(bool*,OutFn,TempValue*,ValueFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_value_ptr != nullptr);

    auto call_fn =
        array_to_function<SJValue>(out_fn, temp_value_ptr, value_fn);

    return
        [is_null, call_fn]
        (ResultType &sj_result, error *err) mutable
        {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
            }
            else {
                call_fn(sj_result, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store array data into an output
 * iterator.
 *
 * The created function that calls `value_fn` for each element of the
 * array. This function has to store the result into `*temp_value_ptr`. Then
 * the value is moved into the output iterator.
 *
 * @param out_iter Output iterator to store values.
 * @param temp_value_ptr Pointer to a temporary space to store array elements.
 * @param value_fn Function to evaluate a single array element and store
 *                 it into `*temp_value_ptr`.
 *
 * @return Function to evaluate JSON array.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutIter,
    typename TempValue,
    typename ValueFn>
inline auto
array_to_out_iter(
    OutIter out_iter,
    TempValue *temp_value_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "array_to_out_iter(OutIter,TempValue*,ValueFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_value_ptr != nullptr);

    auto out_fn =
        [out_iter]
        (auto idx, auto *value_ptr, error *err) mutable {
            *out_iter = std::move(*value_ptr);
            ++out_iter;
        };

    auto save_fn =
        array_to_function<SJValue>(out_fn, temp_value_ptr, value_fn);

    return
        [save_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            save_fn(sj_result, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store array data into an output
 * iterator.
 *
 * The created function that calls `value_fn` for each element of the
 * array. This function has to store the result into `*temp_value_ptr`. Then
 * the value is moved into the output iterator.
 *
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 * @param out_iter Output iterator to store values.
 * @param temp_value_ptr Pointer to a temporary space to store array elements.
 * @param value_fn Function to evaluate a single array element and store
 *                 it into `*temp_value_ptr`.
 *
 * @return Function to evaluate JSON array.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename OutIter,
    typename TempValue,
    typename ValueFn>
inline auto
array_to_out_iter(
    bool *is_null,
    OutIter out_iter,
    TempValue *temp_value_ptr,
    ValueFn value_fn)
{
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(temp_value_ptr != nullptr);

    auto save_fn =
        array_to_out_iter<SJValue>(out_iter, temp_value_ptr, value_fn);

    return
        [is_null, save_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
            }
            else {
                save_fn(sj_result, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}


/**
 * Create evalutation function to store single array values.
 *
 * @param all_element_fn Values that should be evaluated in order.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename ...AllElementFn>
inline auto
array(AllElementFn ...all_element_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("array(AllElementFn...)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto eval_all_fn =
        internal::array(std::forward<AllElementFn>(all_element_fn)...);

    return
        [eval_all_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            simdjson::simdjson_result<simdjson::ondemand::array> sj_array;
            if constexpr(std::is_same_v<SJValue, simdjson::ondemand::array>) {
                _SIMDJSON_PEVAL_TRACE("type:array");
                sj_array = sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                sj_array = sj_result.value().get_array();
                const auto code = sj_array.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                    _SIMDJSON_PEVAL_TRACE_EVAL_END();
                    return;
                }
            }

            auto sj_array_iter = sj_array.value().begin();
            auto sj_array_end  = sj_array.value().end();
            eval_all_fn(&sj_array_iter, &sj_array_end, 0, err);

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store single array values.
 *
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 * @param all_element_fn Values that should be evaluated in order.
 *
 * @return Function to evaluate JSON object.
 */
template<
    typename SJValue = simdjson::ondemand::value,
    typename ...AllElementFn>
inline auto
array(bool *is_null, AllElementFn ...all_element_fn) {
    using ResultType = simdjson::simdjson_result<SJValue>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME("array(bool*,AllElementFn...)");
    _SIMDJSON_PEVAL_TRACE("start");

    auto eval_all_fn =
        array<simdjson::ondemand::array>(
            std::forward<AllElementFn>(all_element_fn)...);

    return
        [is_null, eval_all_fn]
        (ResultType &sj_result, error *err) mutable {
            _SIMDJSON_PEVAL_TRACE_EVAL_BEGIN();
            _SIMDJSON_PEVAL_ASSERT(err != nullptr);

            if (internal::check_null(sj_result, is_null)) {
                _SIMDJSON_PEVAL_TRACE("is_null");
                _SIMDJSON_PEVAL_TRACE_EVAL_END();
                return;
            }

            auto sj_array = sj_result.value().get_array();
            const auto code = sj_array.error();
            if (code) {
                _SIMDJSON_PEVAL_TRACE_ERROR(code);
                err->add(code);
            }
            else {
                eval_all_fn(sj_array, err);
            }

            _SIMDJSON_PEVAL_TRACE_EVAL_END();
        };
}

/**
 * Create evalutation function to store array value.
 *
 * @param val_ptr Pointer to simdjson array.
 *
 * @return Function to evaluate JSON object.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
array(simdjson::simdjson_result<simdjson::ondemand::array> *val_ptr)
{
    using JSArrayValue = simdjson::simdjson_result<simdjson::ondemand::array>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "array(simdjson::simdjson_result<simdjson::ondemand::array>*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) mutable {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            if constexpr(std::is_same_v<decltype(*sj_result), JSArrayValue&>) {
                _SIMDJSON_PEVAL_TRACE("type:array");
                *val_ptr = *sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                auto sj_array = sj_result->value().get_array();
                const auto code = sj_array.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                }
                else {
                    *val_ptr = sj_array;
                }
            }
        };

    return value<SJValue>(val_ptr, get_value);
}


/**
 * Create evalutation function to store array value.
 *
 * @param val_ptr Pointer to simdjson array.
 * @param is_null Pointer to location to store `null` status. If
 *                `(is_null == nullptr)` a `null` is not allowed.
 *
 * @return Function to evaluate JSON object.
 */
template<typename SJValue = simdjson::ondemand::value>
inline auto
array(
    simdjson::simdjson_result<simdjson::ondemand::array> *val_ptr,
    bool *is_null)
{
    using JSArrayValue = simdjson::simdjson_result<simdjson::ondemand::array>;

    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)");
    _SIMDJSON_PEVAL_TRACE("start");
    _SIMDJSON_PEVAL_ASSERT(val_ptr != nullptr);

    auto get_value =
        [](auto *sj_result, auto *val_ptr, auto *err) mutable {
            _SIMDJSON_PEVAL_TRACE("start:get_value");
            if constexpr(std::is_same_v<decltype(*sj_result), JSArrayValue&>) {
                _SIMDJSON_PEVAL_TRACE("type:array");
                *val_ptr = *sj_result;
            }
            else {
                _SIMDJSON_PEVAL_TRACE("type:other");
                auto sj_array = sj_result->value().get_array();
                const auto code = sj_array.error();
                if (code) {
                    _SIMDJSON_PEVAL_TRACE_ERROR(code);
                    err->add(code);
                }
                else {
                    *val_ptr = sj_array;
                }
            }
        };

    return value<SJValue>(val_ptr, get_value, is_null);
}


/// @}


// inline implementations of class error_message
// //////////////////////////////////////////////////////////////////////

inline std::string
error_message::to_string() const {
    std::string result;

    if (!path.empty()) {
        result += "at " + path + ": ";
    }

    result += get_text();

    result += " (" + std::to_string(code) + ')';

    return result;
}


// inline implementations of class error
// //////////////////////////////////////////////////////////////////////

inline void
error::add(simdjson::error_code code) {
    if (code != simdjson::SUCCESS) {
        messages.emplace_back(code, create_path());
    }
}

inline std::string
error::to_string() const {
    std::string result;
    for (const auto &msg : messages) {
        result += msg.to_string() + '\n';
    }
    result.pop_back();

    return result;
}


inline std::string
error::create_path() {
    std::string result_path;
    if (!path_flag) {
        return result_path;
    }

    result_path = "<root>";

    for (const auto &path_elem : path_vector) {
        if (std::holds_alternative<size_t>(path_elem)) {
            auto idx = std::get<size_t>(path_elem);
            result_path += '[' + std::to_string(idx) + ']';
        }
        else if (std::holds_alternative<std::string_view>(path_elem)) {
            auto member_name = std::get<std::string_view>(path_elem);
            result_path.append(1, '.').append(member_name);
        }
    }

    return result_path;
}


// inline implementations of class error::path_scope
// //////////////////////////////////////////////////////////////////////

inline
error::path_scope::path_scope(error *err, size_t idx)
    : error_ptr(err)
{
    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "error::path_scope::path_scope(error*,size_t)");
    if (error_ptr->is_path_enabled()) {
        _SIMDJSON_PEVAL_ASSERT(error_ptr != nullptr);
        _SIMDJSON_PEVAL_TRACE("add_idx:" + std::to_string(idx));
        error_ptr->path_vector.push_back(idx);
    }
}

inline
error::path_scope::path_scope(error *err, std::string_view member)
    : error_ptr(err)
{
    _SIMDJSON_PEVAL_TRACE_SET_NAME(
        "error::path_scope::path_scope(error*,std::string_view)");
    if (error_ptr->is_path_enabled()) {
        _SIMDJSON_PEVAL_ASSERT(error_ptr != nullptr);
        _SIMDJSON_PEVAL_TRACE("add_member:" + std::string(member));
        error_ptr->path_vector.push_back(member);
    }
}

inline
error::path_scope::~path_scope() {
    _SIMDJSON_PEVAL_TRACE_SET_NAME("error::path_scope::~path_scope()");
    if (error_ptr->is_path_enabled()) {
        _SIMDJSON_PEVAL_ASSERT(error_ptr != nullptr);
        _SIMDJSON_PEVAL_TRACE("delete");
        error_ptr->path_vector.pop_back();
    }
}


} // namespace simdjson_peval


#endif /* SIMDJSON_PEVAL_H */
