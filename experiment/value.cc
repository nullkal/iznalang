#include "value.hh"

namespace izna {
namespace {

const std::int64_t NAN = 0x7ff8000000000000;
const std::int64_t PTR_MASK = 0x0007ffffffffffff;

std::int64_t MakeConst(std::intptr_t v)
{

} //izna::(anonymous)

value::value():
	m_val(MakeConst(NIL))
{}

value::value(int32_t v):
	m_val(NAN | (static_cast<std::uint64_t>(v) << 2) | 0x3)
{}

value::value(double v):
	m_val(isnan(v) ? NAN : v)
{}

value::value(bool v):
	m_val(v ? TRUE : FALSE)
{}

value::value(const std::string &v):
	m_val(NAN | (PTR_MASK & MakeHeapObj(v)))
{}

} //izna

