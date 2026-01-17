#include "runtime_application.h"
#include "imgui_layer.h"

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

static GLuint vao;

void RuntimeApplication::startBeginImpl()
{
	Shader defaultVertexShader(STRINGIFY(NTT_ENGINE_DIRECTORY) "core/assets/shaders/opengl/default.vert");
	defaultVertexShader.Compile();
	Shader defaultFragmentShader(STRINGIFY(NTT_ENGINE_DIRECTORY) "core/assets/shaders/opengl/default.frag");
	defaultFragmentShader.Compile();

	m_pProgram = CreateScope<Program>(GetWindow()->GetSurface().get());
	m_pProgram->AttachShader(std::move(defaultFragmentShader));
	m_pProgram->AttachShader(std::move(defaultVertexShader));
	m_pProgram->Link();
}

void RuntimeApplication::startEndImpl()
{
	GL_ASSERT(glGenVertexArrays(1, &vao));
	GL_ASSERT(glBindVertexArray(vao));
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
	m_pProgram->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);
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
