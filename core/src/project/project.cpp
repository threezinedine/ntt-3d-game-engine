#include "project/project_impl.h"

namespace ntt {

Project::Project(ProjectDescription const& description)
	: m_name(description.name)
	, m_version(description.version)
{
}

Project::~Project()
{
}

} // namespace ntt
