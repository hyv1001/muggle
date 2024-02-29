#include "hello_triangle.h"
#include "muggle.h"

#include <shaderc/shaderc.hpp>

static const char* shaderCodeVertex = R"(
#version 460 core
layout (location=0) out vec3 color;
const vec2 pos[3] = vec2[3](
    vec2(-0.6, -0.4),
    vec2(0.6, -0.4),
    vec2(0.0, 0.6)
);
const vec3 col[3] = vec3[3](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);
void main()
{
    gl_Position = vec4(pos[gl_VertexIndex], 0.0, 1.0);
    color = col[gl_VertexIndex];
}
)";

static const char* shaderCodeFragment = R"(
#version 460 core
layout (location=0) in vec3 color;
layout (location=0) out vec4 out_FragColor;
void main()
{
    out_FragColor = vec4(color, 1.0);
}
)";

void compileShader()
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
        shaderCodeVertex, 
        shaderc_shader_kind::shaderc_glsl_vertex_shader, 
        "vertex", 
        options);
    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOG_ERROR("compile vertex shader failed: {}", module.GetErrorMessage().c_str());
        return;
    }

    module = compiler.CompileGlslToSpv(
        shaderCodeFragment, 
        shaderc_shader_kind::shaderc_glsl_fragment_shader, 
        "fragment", 
        options);
    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOG_ERROR("compile fragment shader failed: {}", module.GetErrorMessage().c_str());
        return;
    }

    LOG_INFO("compile shader success");
}

int main(void)
{
    muggle::init();

    compileShader();

    HelloTriangleApplication app;
    app.run(1024, 720);

    muggle::terminate();

    return 0;
}