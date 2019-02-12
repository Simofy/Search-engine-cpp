#include "operands.h"

std::wstring stows(const std::string & t_str) {
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.from_bytes(t_str);
}