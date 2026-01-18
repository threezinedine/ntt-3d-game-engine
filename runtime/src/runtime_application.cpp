#include "runtime_application.h"
#include "imgui_layer.h"

#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vulkan_renderpass.h"
#include "graphics/vulkan/vulkan_swapchain.h"
#endif // NTT_USE_GRAPHICS_VULKAN

namespace ntt {

RuntimeApplication::RuntimeApplication()
	: Application()
{
#if NTT_USE_IMGUI
	m_pLayers.push_back(std::move(CreateScope<ImGuiLayer>())); // ImGui layer must be the last one

#endif // NTT_USE_IMGUI
}

RuntimeApplication::~RuntimeApplication()
{
}

#if NTT_USE_GRAPHICS_OPENGL
static GLuint vao;
#endif // NTT_USE_GRAPHICS_OPENGL

void RuntimeApplication::startBeginImpl()
{
#if NTT_USE_GRAPHICS_OPENGL
	Shader defaultVertexShader(STRINGIFY(NTT_ENGINE_DIRECTORY) "core/assets/shaders/opengl/default.vert");
	Shader defaultFragmentShader(STRINGIFY(NTT_ENGINE_DIRECTORY) "core/assets/shaders/opengl/default.frag");
#elif NTT_USE_GRAPHICS_VULKAN
	Shader defaultVertexShader(Renderer::GetDevice().get(),
							   STRINGIFY(NTT_ENGINE_DIRECTORY) "core/assets/shaders/vulkan/default.vert");
	Shader defaultFragmentShader(Renderer::GetDevice().get(),
								 STRINGIFY(NTT_ENGINE_DIRECTORY) "core/assets/shaders/vulkan/default.frag");
#endif // NTT_USE_GRAPHICS_OPENGL

	defaultVertexShader.Compile();
	defaultFragmentShader.Compile();

#if NTT_USE_GRAPHICS_OPENGL
	m_pProgram = CreateScope<Program>(GetWindow()->GetSurface().get());
#elif NTT_USE_GRAPHICS_VULKAN
	m_pProgram = CreateScope<Program>(
		Renderer::GetDevice().get(), GetWindow()->GetSurface().get(), Renderer::GetRenderPass().get());
#endif // NTT_USE_GRAPHICS_OPENGL

	m_pProgram->AttachShader(std::move(defaultFragmentShader));
	m_pProgram->AttachShader(std::move(defaultVertexShader));
	m_pProgram->Link();
}

void RuntimeApplication::startEndImpl()
{
#if NTT_USE_GRAPHICS_OPENGL
	GL_ASSERT(glGenVertexArrays(1, &vao));
	GL_ASSERT(glBindVertexArray(vao));
#endif // NTT_USE_GRAPHICS_OPENGL
}

void RuntimeApplication::updateBeginImpl(f32 deltaTime)
{
	if (Input::IsKeyPressed(KEY_CODE_ESCAPE))
	{
		NTT_APPLICATION_LOG_INFO("Escape key pressed. Closing the window.");
		Close();
	}

#if NTT_USE_IMGUI
	if (Input::IsKeyPressed(KEY_CODE_F12))
	{
		Scope<Layer>& imguiLayer = m_pLayers.back(); // The imgui layer is always the last one
		if (imguiLayer->IsAttached())
		{
			disableLayer(m_pLayers.size() - 1); // The imgui layer is always the last one
		}
		else
		{
			enableLayer(m_pLayers.size() - 1); // The imgui layer is always the last one
		}
	}

#endif // NTT_USE_IMGUI
}

void RuntimeApplication::updateImpl(f32 deltaTime)
{
#if NTT_USE_GRAPHICS_OPENGL
	m_pProgram->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
#elif NTT_USE_GRAPHICS_VULKAN
	m_pProgram->Bind(Renderer::GetCurrentRenderCommandBuffer()); // TODO: Pass the command buffer here
#endif // NTT_USE_GRAPHICS_OPENGL
}

void RuntimeApplication::updateEndImpl(f32 deltaTime)
{
}

void RuntimeApplication::shutdownBeginImpl()
{
}

void RuntimeApplication::shutdownEndImpl()
{
	m_pProgram.reset();
}

} // namespace ntt
