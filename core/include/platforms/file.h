#pragma once

#include "core/containers/string.h"
#include "pch.h"

/**
 * @file file.h
 *
 * All utilities for working with the filesystem.
 */

namespace ntt {

/**
 * Global configuration which applies to all instances of `File`.
 *
 * @note This is used mainly for mocking the filesystem in testing.
 */
class FileSystem
{
public:
	/**
	 * Starting the configuration, allocate, setup needed resources for
	 * 		the whole system.
	 */
	static void Initialize();

	/**
	 * Shutdown the configuration, free all resources used by the
	 * 		filesystem system.
	 */
	static void Shutdown();
};

/**
 * The instance which holds the file/folder information.
 */
class Path
{
public:
	Path(const String& path);
	Path(const Path& other);
	Path(Path&& other) noexcept;
	~Path();

public:
	/**
	 * Check whether the path (can be file or directory) is on the filesystem.
	 */
	b8 isExisted() const;

	/**
	 * Check whether the path is a file (no need to be existed).
	 */
	b8 isFile() const;

	/**
	 * Check whether the path is a directory (no need to be existed).
	 */
	b8 isDirectory() const;

private:
	class PathInternal;
	PathInternal* m_pInternal; ///< The internal implementation of the Path

	String m_path; ///< The path to thePath
};

} // namespace ntt
