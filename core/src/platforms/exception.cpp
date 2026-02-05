#include "platforms/exception.h"

#if NTT_PLATFORM_WINDOWS
#include "platforms/windows/exception_windows.h"
#elif NTT_PLATFORM_LINUX
#include <exception>
#include <stdexcept>
#else // NTT_PLATFORM_WINDOWS
#error "Unsupported platform for exception handling"
#endif // NTT_PLATFORM_WINDOWS

namespace ntt {

void raiseException(ExceptionType type, const char* message)
{
#if NTT_PLATFORM_WINDOWS
	raiseWindowsException(type, message);
#elif NTT_PLATFORM_LINUX
	switch (type)
	{
	case EXCEPTION_TYPE_OUT_OF_MEMORY:
		throw std::bad_alloc();
		break;

	case EXCEPTION_TYPE_INVALID_ARGUMENT:
		throw std::invalid_argument(message);
		break;

	case EXCEPTION_TYPE_INDEX_OUT_OF_BOUNDS:
		throw std::out_of_range(message);
		break;
	case EXCEPTION_TYPE_KEY_NOT_FOUND:
		throw std::out_of_range(std::string("Key Not Found: ") + message);
		break;
	case EXCEPTION_TYPE_FILE_NOT_FOUND:
		throw std::runtime_error(std::string("File Not Found: ") + message);
		break;
	case EXCEPTION_TYPE_IO_ERROR:
		throw std::runtime_error(std::string("IO Error: ") + message);
		break;
	case EXCEPTION_TYPE_RUNTIME:
		throw std::runtime_error(message);
		break;
	case EXCEPTION_TYPE_LOGIC:
		throw std::logic_error(message);
		break;
	case EXCEPTION_TYPE_UNKNOWN:
	case EXCEPTION_TYPE_COUNT:
		throw std::runtime_error(std::string("Unknown Exception: ") + message);
		break;
	}
#else // NTT_PLATFORM_WINDOWS
#error "Unsupported platform for exception handling"
#endif // NTT_PLATFORM_WINDOWS
}

} // namespace ntt