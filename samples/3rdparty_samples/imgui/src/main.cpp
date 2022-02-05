#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

#include <cstdio>
#include <cstdlib>

struct ImRenderData
{
    GLuint shaderVertex;
    GLuint shaderFragment;
    GLuint shaderProgram;
    GLuint vao;
    GLuint handleVBO;
    GLuint handleElements;
    GLuint perFrameDataBuffer;
};

void imgui_init()
{
    ImGui::CreateContext();
    ImGuiIO& imguiIO = ImGui::GetIO();
    imguiIO.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    ImFontConfig imguiFontCfg         = ImFontConfig();
    imguiFontCfg.FontDataOwnedByAtlas = false;
    imguiFontCfg.RasterizerMultiply   = 1.5f;
    imguiFontCfg.SizePixels           = 768.f / 32.f;
    imguiFontCfg.PixelSnapH           = true;
    imguiFontCfg.OversampleH          = 4;
    imguiFontCfg.OversampleV          = 4;

    ImFont* imguiFont =
        imguiIO.Fonts->AddFontFromFileTTF("content/fonts/OpenSans-Light.ttf", imguiFontCfg.SizePixels, &imguiFontCfg);
    if (imguiFont == nullptr)
    {
        printf("Error: can't load font\n");
        return;
    }

    // build imgui font texture atlas bitmap
    unsigned char* imguiFontAtlasTexturePixels = nullptr;
    int            imguiFontAtlasTextureWidth, imguiFontAtlasTextureHeight;
    imguiIO.Fonts->GetTexDataAsRGBA32(
        &imguiFontAtlasTexturePixels, &imguiFontAtlasTextureWidth, &imguiFontAtlasTextureHeight);
    GLuint imguiFontAtlasTexture;
    glCreateTextures(GL_TEXTURE_2D, 1, &imguiFontAtlasTexture);
    glTextureParameteri(imguiFontAtlasTexture, GL_TEXTURE_MAX_LEVEL, 0);
    glTextureParameteri(imguiFontAtlasTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(imguiFontAtlasTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(imguiFontAtlasTexture, 1, GL_RGBA8, imguiFontAtlasTextureWidth, imguiFontAtlasTextureHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(imguiFontAtlasTexture,
                        0,
                        0,
                        0,
                        imguiFontAtlasTextureWidth,
                        imguiFontAtlasTextureHeight,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        imguiFontAtlasTexturePixels);
    glBindTextures(0, 1, &imguiFontAtlasTexture);

    imguiIO.Fonts->TexID            = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(imguiFontAtlasTexture));
    imguiIO.FontDefault             = imguiFont;
    imguiIO.DisplayFramebufferScale = ImVec2(1, 1);
}

void imgui_set_render_data(ImRenderData& renderData)
{
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
    GLuint vao;
    glCreateVertexArrays(1, &vao);
    GLuint handleVBO;
    glCreateBuffers(1, &handleVBO);
    glNamedBufferStorage(handleVBO, 128 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);
    GLuint handleElements;
    glCreateBuffers(1, &handleElements);
    glNamedBufferStorage(handleElements, 256 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(vao, handleElements);
    glVertexArrayVertexBuffer(vao, 0, handleVBO, 0, sizeof(ImDrawVert));

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, pos));
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, uv));
    glVertexArrayAttribFormat(vao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, IM_OFFSETOF(ImDrawVert, col));

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribBinding(vao, 2, 0);

    glBindVertexArray(vao);

    GLuint perFrameDataBuf;
    glCreateBuffers(1, &perFrameDataBuf);
    glNamedBufferStorage(perFrameDataBuf, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuf, 0, sizeof(glm::mat4));

    renderData.shaderVertex   = shaderVertex;
    renderData.shaderFragment = shaderFragment;
    renderData.shaderProgram  = program;
    renderData.vao            = vao;
    renderData.handleVBO      = handleVBO;
    renderData.handleElements = handleElements;
}

void imgui_draw(int width, int height, const ImRenderData& renderData)
{
    ImGuiIO& imguiIO    = ImGui::GetIO();
    imguiIO.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();

    const ImDrawData* drawData = ImGui::GetDrawData();

    const float left   = drawData->DisplayPos.x;
    const float right  = drawData->DisplayPos.x + drawData->DisplaySize.x;
    const float top    = drawData->DisplayPos.y;
    const float bottom = drawData->DisplayPos.y + drawData->DisplaySize.y;

    const glm::mat4 orthoProj = glm::ortho(left, right, bottom, top);
    glNamedBufferSubData(renderData.perFrameDataBuffer, 0, sizeof(glm::mat4), glm::value_ptr(orthoProj));

    for (int index = 0; index < drawData->CmdListsCount; index++)
    {
        const ImDrawList* cmdList = drawData->CmdLists[index];
        glNamedBufferSubData(renderData.handleVBO,
                             0,
                             static_cast<GLsizeiptr>(cmdList->VtxBuffer.Size * sizeof(ImDrawVert)),
                             cmdList->VtxBuffer.Data);
        glNamedBufferSubData(renderData.handleElements,
                             0,
                             static_cast<GLsizeiptr>(cmdList->IdxBuffer.Size * sizeof(ImDrawIdx)),
                             cmdList->IdxBuffer.Data);

        for (int cmdIndex = 0; cmdIndex < cmdList->CmdBuffer.Size; cmdIndex++)
        {
            const ImDrawCmd* cmdBuf   = &cmdList->CmdBuffer[cmdIndex];
            const ImVec4     clipRect = cmdBuf->ClipRect;
            glScissor(static_cast<int>(clipRect.x),
                      static_cast<int>(height - clipRect.w),
                      static_cast<int>(clipRect.z - clipRect.x),
                      static_cast<int>(clipRect.w - clipRect.y));
            glBindTextureUnit(0, reinterpret_cast<GLuint>(cmdBuf->TextureId));
            glDrawElementsBaseVertex(GL_TRIANGLES,
                                     static_cast<GLsizei>(cmdBuf->ElemCount),
                                     GL_UNSIGNED_SHORT,
                                     reinterpret_cast<void*>(cmdBuf->IdxOffset * sizeof(ImDrawIdx)),
                                     static_cast<GLint>(cmdBuf->VtxOffset));
        }
    }

    glScissor(0, 0, width, height);
}

void imgui_destroy(ImRenderData& renderData)
{
    glDeleteProgram(renderData.shaderProgram);
    glDeleteShader(renderData.shaderFragment);
    glDeleteShader(renderData.shaderVertex);
    glDeleteVertexArrays(1, &renderData.vao);
    glDeleteBuffers(1, &renderData.perFrameDataBuffer);

    ImGui::DestroyContext();
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

    GLFWwindow* window = glfwCreateWindow(1024, 720, "Imgui Example", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        (void)(scancode);
        (void)(action);
        (void)(mods);
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        (void)(window);
        ImGui::GetIO().MousePos = ImVec2(static_cast<float>(x), static_cast<float>(y));
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        (void)(window);
        (void)(mods);
        auto&     io      = ImGui::GetIO();
        const int idx     = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : 1;
        io.MouseDown[idx] = action == GLFW_PRESS;
    });

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // init imgui
    imgui_init();

    ImRenderData imguiRenderData;
    imgui_set_render_data(imguiRenderData);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        imgui_draw(width, height, imguiRenderData);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imgui_destroy(imguiRenderData);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}