#include "platforms/file.h"
#include <cstdlib>
#include <cstring>

#if NTT_PLATFORM_LINUX
#include <filesystem>

namespace fs = std::filesystem;
#elif NTT_PLATFORM_WINDOWS
#error "File system implementation is not done for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "File system implementation is not done for this platform yet."
#endif // NTT_PLATFORM_LINUX

namespace ntt {

void FileSystem::Initialize()
{
	// Currently no global initialization needed for the filesystem
}

void FileSystem::Shutdown()
{
	// Currently no global shutdown needed for the filesystem
}

class Path::PathInternal
{
public:
#if NTT_PLATFORM_LINUX
	fs::path fsPath;
#elif NTT_PLATFORM_WINDOWS
#error "PathInternal is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "PathInternal is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX
};

Path::Path(const String& path)
	: m_path(path)
{
	m_pInternal = new PathInternal();

#if NTT_PLATFORM_LINUX
	m_pInternal->fsPath = fs::path(m_path.c_str());
#elif NTT_PLATFORM_WINDOWS
#error "Path constructor is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "Path constructor is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX
}

Path::Path(const Path& other)
	: m_path(other.m_path)
{
	m_pInternal = new PathInternal();
	memcpy((void*)m_pInternal, (void*)other.m_pInternal, sizeof(PathInternal));
}

Path::Path(Path&& other) noexcept
	: m_path((String&&)(other.m_path))
{
	m_pInternal		  = other.m_pInternal;
	other.m_pInternal = nullptr;
}

Path::~Path()
{
	if (m_pInternal != nullptr)
	{
		delete m_pInternal;
		m_pInternal = nullptr;
	}
}

b8 Path::isExisted() const
{
#if NTT_PLATFORM_LINUX
	return fs::exists(m_pInternal->fsPath);
#elif NTT_PLATFORM_WINDOWS
#error "Path::isExisted is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "Path::isExisted is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX
}

b8 Path::isFile() const
{
#if NTT_PLATFORM_LINUX
	return fs::is_regular_file(m_pInternal->fsPath);
#elif NTT_PLATFORM_WINDOWS
#error "Path::isExisted is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "Path::isExisted is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX
}

b8 Path::isDirectory() const
{
#if NTT_PLATFORM_LINUX
	return fs::is_directory(m_pInternal->fsPath);
#elif NTT_PLATFORM_WINDOWS
#error "Path::isDirectory is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "Path::isDirectory is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX
}

} // namespace ntt
