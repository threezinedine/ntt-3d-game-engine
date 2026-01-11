#include "utils/id/id.h"

#define ID_UNIQUE_ID_SHIFT	   (0)
#define ID_REVERSED_BITS_SHIFT (ID_UNIQUE_ID_SHIFT + ID_UNIQUE_BITS)
#define ID_TYPE_SHIFT		   (ID_REVERSED_BITS_SHIFT + ID_REVERSED_BITS_BITS)
#define ID_VERSION_SHIFT	   (ID_TYPE_SHIFT + ID_TYPE_BITS)

#define ID_COMPONENT_MASK(type, shift) (static_cast<IDRawType>(~(type(0))) << (shift))

#define ID_UNIQUE_ID_MASK	  ID_COMPONENT_MASK(IDUniqueType, ID_UNIQUE_ID_SHIFT)
#define ID_REVERSED_BITS_MASK ID_COMPONENT_MASK(IDReversedBitsType, ID_REVERSED_BITS_SHIFT)
#define ID_TYPE_MASK		  ID_COMPONENT_MASK(IDTypeType, ID_TYPE_SHIFT)
#define ID_VERSION_MASK		  ID_COMPONENT_MASK(IDVersionType, ID_VERSION_SHIFT)

#define GET_ID_VERSION(rawId)		(static_cast<u16>(((rawId) & ID_VERSION_MASK) >> ID_VERSION_SHIFT))
#define GET_ID_TYPE(rawId)			(static_cast<IDTypes>(((rawId) & ID_TYPE_MASK) >> ID_TYPE_SHIFT))
#define GET_ID_REVERSED_BITS(rawId) (static_cast<u8>(((rawId) & ID_REVERSED_BITS_MASK) >> ID_REVERSED_BITS_SHIFT))
#define GET_ID_UNIQUE_ID(rawId)		(static_cast<u32>(((rawId) & ID_UNIQUE_ID_MASK) >> ID_UNIQUE_ID_SHIFT))

namespace ntt {
ID::ID()
	: m_version(0)
	, m_type(0)
	, m_reversedBits(0)
	, m_uniqueId(~0U)
{
}

ID::ID(u64 rawId)
	: m_version(GET_ID_VERSION(rawId))
	, m_type(GET_ID_TYPE(rawId))
	, m_reversedBits(GET_ID_REVERSED_BITS(rawId))
	, m_uniqueId(GET_ID_UNIQUE_ID(rawId))
{
}

ID::ID(u16 version, IDTypes type, u32 uniqueId)
	: m_version(version)
	, m_type(type)
	, m_reversedBits(0)
	, m_uniqueId(uniqueId)
{
}

ID::ID(const ID& other)
	: m_version(other.m_version)
	, m_type(other.m_type)
	, m_reversedBits(other.m_reversedBits)
	, m_uniqueId(other.m_uniqueId)
{
}

ID::ID(ID&& other) noexcept
	: m_version(other.m_version)
	, m_type(other.m_type)
	, m_reversedBits(other.m_reversedBits)
	, m_uniqueId(other.m_uniqueId)
{
	other.m_version		 = 0;
	other.m_type		 = 0;
	other.m_reversedBits = 0;
	other.m_uniqueId	 = ~0U;
}

ID::~ID()
{
}

void ID::operator=(const ID& other)
{
	m_version	   = other.m_version;
	m_type		   = other.m_type;
	m_reversedBits = other.m_reversedBits;
	m_uniqueId	   = other.m_uniqueId;
}

void ID::operator=(ID&& other) noexcept
{
	m_version	   = other.m_version;
	m_type		   = other.m_type;
	m_reversedBits = other.m_reversedBits;
	m_uniqueId	   = other.m_uniqueId;

	other.m_version		 = 0;
	other.m_type		 = 0;
	other.m_reversedBits = 0;
	other.m_uniqueId	 = INVALID_ID_UNIQUE;
}

u64 ID::ToRaw() const
{
	return (static_cast<u64>(m_version) << ID_VERSION_SHIFT) | (static_cast<u64>(m_type) << ID_TYPE_SHIFT) |
		   (static_cast<u64>(m_reversedBits) << ID_REVERSED_BITS_SHIFT) |
		   (static_cast<u64>(m_uniqueId) << ID_UNIQUE_ID_SHIFT);
}

b8 ID::IsEqual(const ID& other) const
{
	return (m_type == other.m_type) && (m_uniqueId == other.m_uniqueId);
}

b8 ID::IsValid() const
{
	return m_uniqueId != INVALID_ID_UNIQUE;
}

b8 ID::IsLatest() const
{
	// Note: The actual implementation would require access to the ID Manager to verify the latest version.
	// Here we provide a placeholder implementation that always returns true for demonstration purposes.
	return NTT_TRUE;
}

b8 ID::IsType(IDType type) const
{
	return (m_type & type) != 0;
}

} // namespace ntt
