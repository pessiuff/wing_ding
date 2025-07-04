#pragma once

#include <string>

namespace wing_ding::globals {
	const std::string g_window_name = "Wing Ding";
	constexpr int g_window_width = 720;
	constexpr int g_window_height = 1280;

	const std::string g_vertex_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
}
)";
	const std::string g_fragment_source = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0);
}
)";
}