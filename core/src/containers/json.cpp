#include "containers/json.h"

namespace ntt {
String jsonToString(const Json& json)
{
	return json.dump(4);
}
} // namespace ntt
