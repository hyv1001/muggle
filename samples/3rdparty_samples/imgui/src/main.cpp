#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <cstdio>
#include <cstdlib>

static const char* shaderCodeVertex = R"(
#version 460 core
layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec4 Color;
layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 MVP;
};
out vec2 Frag_UV;
out vec4 Frag_Color;
void main()
{
    Frag_UV = UV;
    Frag_Color = Color;
    gl_Position = MVP * vec4(Position.xy, 0, 1);
}

)";

static const char* shaderCodeFragment = R"(
#version 460 core
in vec2 Frag_UV;
in vec4 Frag_Color;
layout (binding = 0) uniform sampler2D Texture;
layout (location = 0) out vec4 out_Color;

void main()
{
    out_Color = Frag_Color * texture(Texture, Frag_UV.st);
}

)";

struct ImDrawVert
{
    ImVec2 pos;
    ImVec2 uv;
    ImU32  col;
};

void ImGuiInit()
{
    ImGui::CreateContext();
    ImGuiIO& imguiIO = ImGui::GetIO();
    imguiIO.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    ImFontConfig fontCfg = ImFontConfig();
    fontCfg.FontDataOwnedByAtlas = false;
    fontCfg.RasterizerMultiply   = 1.5f;
    fontCfg.SizePixels           = 768.f / 32.f;
    fontCfg.PixelSnapH           = true;
    fontCfg.OversampleH          = 4;
    fontCfg.OversampleV          = 4;
}

int main(void)
{
    glfwSetErrorCallback([](int error, const char* description) {
        (void)error;
        fprintf(stderr, "Error: %s\n", description);
    });

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 720, "GLFW Example", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        (void)(scancode);
        (void)(action);
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);





    const GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &shaderCodeVertex, nullptr);
    glCompileShader(shaderVertex);

    const GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &shaderCodeFragment, nullptr);
    glCompileShader(shaderFragment);

    const GLuint program = glCreateProgram();
    glAttachShader(program, shaderVertex);
    glAttachShader(program, shaderFragment);

    glLinkProgram(program);
    glUseProgram(program);

    // 256kb for vertices and indices data
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    GLuint handleVBO;
    glCreateBuffers(1, &handleVBO);
    glNamedBufferStorage(handleVBO, 256 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);
    GLuint handleElements;
    glCreateBuffers(1, &handleElements);
    glNamedBufferStorage(handleElements, 256 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(VAO, handleElements);
    glVertexArrayVertexBuffer(VAO, 0, handleVBO, 0, sizeof(ImDrawVert));
    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    glEnableVertexArrayAttrib(VAO, 2);
    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, pos));
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, uv));
    glVertexArrayAttribFormat(VAO, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, IM_OFFSETOF(ImDrawVert, col));
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribBinding(VAO, 2, 0);
    glBindVertexArray(VAO);

    glClearColor(1.f, 1.f, 1.f, 1.f);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteShader(shaderFragment);
    glDeleteShader(shaderVertex);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}