#pragma once
#include "types.h"
#include "utils/logger/logger.h"
#include <platforms/common.h>

#if NTT_USE_IMGUI
#include "imgui.h"
#endif // NTT_USE_IMGUI

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

class Window;
class Layer;
class Project;
class Program;

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
	NTT_DELETE_COPY(Application)
	NTT_DELETE_MOVE(Application)
	virtual ~Application();

public:
	/**
	 * Get whether the application is initialized.
	 * @return True if initialized, false otherwise.
	 */
	inline b8 IsInitialized() const NTT_BINDING;

	/**
	 * Load a project from the specified file path.
	 * @param projectFilePath The file path of the project to load. The path is absolute or relative to the current
	 * working directory.
	 *
	 * @note If there is currently has a project loadded, it will be unloaded first before loading the new project.
	 */
	void LoadProject(const String& projectFilePath) NTT_BINDING;

	/**
	 * Close the application window.
	 */
	void Close();

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
	virtual b8 IsOpen() const;

public:
	inline Scope<Window>& GetWindow()
	{
		return m_pWindow;
	}

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
	virtual void updateImpl(f32 deltaTime) {};

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

protected:
	/**
	 * Enable a layer by its index in the layer stack.
	 * @param layerIndex The index of the layer to enable. If
	 * 		the index is out of bounds or the layer is already enabled,
	 * 		a warning will be logged and no action will be taken.
	 *
	 */
	void enableLayer(u32 layerIndex);

	/**
	 * Disable a layer by its index in the layer stack.
	 * @param layerIndex The index of the layer to disable. If
	 * 		the index is out of bounds or the layer is already disabled,
	 * 		a warning will be logged and no action will be taken.
	 */
	void disableLayer(u32 layerIndex);

protected:
	Array<Scope<Layer>> m_pLayers;

private:
	Set<u32> m_enabledLayerIndices; /// These layers will be disabled when the application is shutting down.

private:
	Scope<Project> m_pProject;
	Scope<Window>  m_pWindow;
	b8			   m_isInitialized;
};

inline b8 Application::IsInitialized() const
{
	return m_isInitialized;
}

} // namespace ntt
