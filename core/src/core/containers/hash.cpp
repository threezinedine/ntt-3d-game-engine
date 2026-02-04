#include "core/containers/flat_map.h"

namespace ntt {

template <>
HashType hash<s32>(const s32& key)
{
	return static_cast<HashType>(key * 2654435761u);
}

} // namespace ntt
