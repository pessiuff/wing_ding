#pragma once

#include <string>

namespace wing_ding::globals {
	const std::string g_window_name = "Wing Ding";
	constexpr unsigned int g_window_width = 720;
	constexpr unsigned int g_window_height = 1280;

	const std::string g_vertex_source = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

	const std::string g_fragment_source = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoord);
}
)";
	constexpr unsigned int g_max_batch_quads = 1024;
}