#pragma once
#include "project_impl.h"
#include "types.h"
#include "utils/logger/logger.h"
#include <platforms/common.h>

#define NTT_APPLICATION_LOG_TRACE(message, ...)                                                                        \
	NTT_LOG_TRACE(LogTagMaskBit::LOG_TAG_MASK_APPLICATION, message, ##__VA_ARGS__)

#define NTT_APPLICATION_LOG_DEBUG(message, ...)                                                                        \
	NTT_LOG_DEBUG(LogTagMaskBit::LOG_TAG_MASK_APPLICATION, message, ##__VA_ARGS__)

#define NTT_APPLICATION_LOG_INFO(message, ...)                                                                         \
	NTT_LOG_INFO(LogTagMaskBit::LOG_TAG_MASK_APPLICATION, message, ##__VA_ARGS__)

#define NTT_APPLICATION_LOG_WARN(message, ...)                                                                         \
	NTT_LOG_WARN(LogTagMaskBit::LOG_TAG_MASK_APPLICATION, message, ##__VA_ARGS__)

#define NTT_APPLICATION_LOG_ERROR(message, ...)                                                                        \
	NTT_LOG_ERROR(LogTagMaskBit::LOG_TAG_MASK_APPLICATION, message, ##__VA_ARGS__)

namespace ntt {

/**
 * The global access point for the whole runtime application.
 * This is the interface for the user-defined application class.
 */
class NTT_BINDING Application
{
public:
	/**
	 * Constructor and Destructor
	 */
	Application();
	virtual ~Application();

public:
	inline b8 IsInitialized() const NTT_BINDING;

	/**
	 * Be called at the starting of the runtime.
	 */
	void Start() NTT_BINDING;

	/**
	 * Be called every frame to update the application.
	 * @param deltaTime The time elapsed since the last frame.
	 */
	void Update(f32 deltaTime) NTT_BINDING;

	/**
	 * Be called at the shutting down of the runtime.
	 */
	void Shutdown() NTT_BINDING;

	/**
	 * Get whether the application window is open.
	 * @return True if the window is open, false otherwise.
	 */
	virtual b8 IsOpen() const = 0;

protected:
	/**
	 * Default empty user-defined start implementation (begin).
	 */
	virtual void startBeginImpl() {};

	/**
	 * Default empty user-defined start implementation (end).
	 */
	virtual void startEndImpl() {};

	/**
	 * Default empty user-defined update implementation.
	 * @param deltaTime The time elapsed since the last frame.
	 */
	virtual void updateBeginImpl(f32 deltaTime) {};

	/**
	 * Default empty user-defined update implementation.
	 * @param deltaTime The time elapsed since the last frame.
	 */
	virtual void updateEndImpl(f32 deltaTime) {};

	/**
	 * Default empty user-defined shutdown implementation.
	 */
	virtual void shutdownBeginImpl() {};

	virtual void shutdownEndImpl() {};

private:
	Scope<Project> m_pProject;
	b8			   m_isInitialized;
};

inline b8 Application::IsInitialized() const
{
	return m_isInitialized;
}

} // namespace ntt
