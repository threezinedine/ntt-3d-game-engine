#pragma once

namespace ntt {

/**
 * Owned string type which are used throughout the engine.
 */
class String
{
public:
	String();
	String(const char* cstr);
	String(const String& other);
	String(String&& other) noexcept;
	~String();

private:
	char* m_data;
};

} // namespace ntt
