#pragma once
#include "platforms/common.h"

namespace ntt {

class IDManager;

// ===================== Modify ID Configuration below this line =====================
#define ID_UNIQUE_BITS		  32
#define ID_REVERSED_BITS_BITS 8
#define ID_TYPE_BITS		  8
#define ID_VERSION_BITS		  16

typedef u64 IDRawType;
typedef u32 IDUniqueType;
typedef u8	IDReversedBitsType;
typedef u8	IDTypeType;
typedef u16 IDVersionType;

constexpr IDUniqueType INVALID_ID_UNIQUE = static_cast<IDUniqueType>(~IDUniqueType(0));
constexpr IDRawType	   INVALID_ID_RAW	 = static_cast<IDRawType>(INVALID_ID_UNIQUE);

enum NTT_BINDING IDType : IDTypeType
{
	ID_TYPE_RESOURCE = NTT_BIT(0),
	ID_TYPE_ENTITY	 = NTT_BIT(1),
	ID_TYPE_COUNT NTT_HIDDEN,
};

typedef IDTypeType IDTypes;
// ===================== Modify ID Configuration above this line =====================

/**
 * Each entity, resource in side the engine, its assigned a unique ID. The ID contains
 * the information:
 * - Runtime version (start from 0, incremented when the state of the entity/resource changes)
 * - Reversed bits: (8 bits)
 * - Type of the entity/resource
 * - Unique identifier
 *
 * This mechanism allows other systems to quickly identify the entities/resources which is modified
 * since last frame, by comparing the runtime version stored in the ID.
 *
 * Format:
 * |  16 bits  |    8 bits   |    8 bits     |          32 bits          |
 * |-----------|-------------|---------------|---------------------------|
 * |  Version  |    Type     | Reversed Bits |       Unique ID           |
 *
 * The Id with all 1 in the unique ID part is considered as invalid ID.
 *
 * @note The runtime version will not be saved when serializing the ID to disk.
 * @note All the ids are unique and be managed by the IDManager system.
 * @note The runtime only affects the runtime version part of the ID, other parts remains unchanged.
 */
class NTT_BINDING ID
{
public:
	/**
	 * Default constructor creates an invalid ID.
	 */
	ID();

	/**
	 * Constructs an ID from a raw 64-bit integer.
	 * @param rawId The raw 64-bit integer representing the ID.
	 */
	ID(IDRawType rawId);

	/**
	 * Constructs an ID with the given parameters.
	 * @param version The version of the ID.
	 * @param type The type of the ID.
	 * @param uniqueId The unique identifier.
	 */
	ID(IDVersionType version, IDTypes type, IDUniqueType uniqueId);
	/**
	 * Copy constructor. The runtime version, reversed bits, type and unique ID are copied.
	 * @param other The ID to copy from.
	 */
	ID(const ID& other);

	/**
	 * Move constructor. The moved-from id will be set to invalid.
	 * @param other The ID to move from.
	 */
	ID(ID&& other) noexcept;

	/**
	 * Destructor for the ID class.
	 */
	~ID();

public:
	/**
	 * Converts the ID to its raw 64-bit integer representation.
	 * @return The raw 64-bit integer representing the ID.
	 *
	 * @note The raw type does not contains the version information.
	 */
	IDRawType ToRaw() const NTT_BINDING;

	inline operator IDRawType() const
	{
		return ToRaw();
	}

	/**
	 * Compares this ID with another ID for equality.
	 * @param other The other ID to compare with.
	 * @return True if both IDs are equal, false otherwise.
	 *
	 * @note The runtime version, reversed bits are not considered in the comparison.
	 */
	b8 IsEqual(const ID& other) const NTT_BINDING;

	inline b8 operator==(const ID& other) const
	{
		return IsEqual(other);
	}

	inline b8 operator!=(const ID& other) const
	{
		return !IsEqual(other);
	}

	inline b8 operator==(IDRawType rawId) const
	{
		return IsEqual(ID(rawId));
	}

	inline b8 operator!=(IDRawType rawId) const
	{
		return !IsEqual(ID(rawId));
	}

	void operator=(const ID& other);
	void operator=(ID&& other) noexcept;

	inline void operator=(IDRawType rawId)
	{
		*this = ID(rawId);
	}

	inline IDTypeType GetType() const NTT_BINDING
	{
		return m_type;
	}

	inline IDUniqueType GetUniqueID() const NTT_BINDING
	{
		return m_uniqueId;
	}

public:
	/**
	 * Checking whether current ID is valid or not.
	 * @return True if the ID is valid, false otherwise.
	 */
	b8 IsValid() const NTT_BINDING;

	/**
	 * Checking whether the ID is the latest version.
	 * @return True if the ID's current runtime version is the highest in
	 *      the system, false otherwise.
	 *
	 * @note The ID Manager is responsible for managing the versions of IDs.
	 */
	b8 IsLatest() const NTT_BINDING;

	/**
	 * Validating whether the ID is of the given type.
	 * @param type The type to check against.
	 * @return True if the ID is of the given type, false otherwise.
	 */
	b8 IsType(IDType type) const NTT_BINDING;

	/**
	 * Increment the global version of the ID, indicating a state change.
	 * If the ID is not valid, not latest or not managed by the IDManager, this
	 * function will trigger an assertion failure.
	 */
	void Update() NTT_BINDING;

private:
	/**
	 * Used only by the IDManager reset the version of the ID when the state changes.
	 */
	void ResetVersion();

private:
	IDVersionType	   m_version;
	IDTypes			   m_type;
	IDReversedBitsType m_reversedBits;
	IDUniqueType	   m_uniqueId;

public:
	friend class IDManager;
};

} // namespace ntt

// --- Hashing for ID to be used in unordered containers ---
namespace std {
template <>
struct hash<ntt::ID>
{
	size_t operator()(const ntt::ID& id) const noexcept
	{
		return std::hash<ntt::IDRawType>()(static_cast<ntt::IDRawType>(id));
	}
};

} // namespace std