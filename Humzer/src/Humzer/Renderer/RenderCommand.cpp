#include "humpch.h"
#include "RenderCommand.h"
#include "../Platform/OpenGL/OpenGLRendererAPI.h"

namespace Humzer {
	
	Humzer::RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}
