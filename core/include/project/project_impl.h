#pragma once
#include "types.h"

namespace ntt {

class Project
{
public:
	Project(const ProjectDescription& description);

	NTT_DELETE_COPY(Project)
	NTT_DELETE_MOVE(Project)

	~Project();

public:
	inline const String& GetName() const
	{
		return m_name;
	}

private:
	String	m_name;
	Version m_version;
};

} // namespace ntt
