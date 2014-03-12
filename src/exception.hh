#ifndef IZNA_EXCEPTION_HPP
#define IZNA_EXCEPTION_HPP

#include <stdexcept>

namespace izna {

class type_error: std::exception
{};

class unknown_identifier_error: std::exception
{};

} //izna

#endif
