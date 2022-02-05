#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/version.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "muggle.h"

using namespace muggle;

static const char* shaderCodeVertex = R"(
#version 460 core
layout (std140, binding = 0) uniform PerFrameData {
    uniform mat4 MVP;
    uniform int isWireframe;
};
layout (location=0) in vec3 pos;
layout (location=0) out vec3 color;

void main()
{
    gl_Position = MVP * vec4(pos, 1.0);
    color = isWireframe > 0 ? vec3(0.0) : pos.xyz;
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

struct PerFrameData
{
    glm::mat4 mvp;
    int       isWireFrame;
};

bool aiLoadMesh(const char* scene_file, std::vector<glm::vec3>& positions)
{
    const aiScene* scene = aiImportFile(scene_file, aiProcess_Triangulate);
    if (!scene || !scene->HasMeshes())
    {
        LOG_ERROR("assimp import file {} failed!", scene_file);
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0];
    for (uint32_t index = 0; index < mesh->mNumFaces; index++)
    {
        const aiFace&  face   = mesh->mFaces[index];
        const uint32_t idx[3] = {face.mIndices[0], face.mIndices[1], face.mIndices[2]};
        for (int j = 0; j < 3; j++)
        {
            const aiVector3D v = mesh->mVertices[idx[j]];
            positions.push_back(glm::vec3(v.x, v.z, v.y));
        }
    }
    aiReleaseImport(scene);
    return true;
}

int main(void)
{
    muggle::init();

    glfwSetErrorCallback([](int error, const char* description) {
        (void)error;
        LOG_ERROR("Error: {}", description);
    });

    if (!glfwInit())
    {
        LOG_FATAL("Error: glfw init failed!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 720, "Assimp Example", nullptr, nullptr);
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

    std::vector<glm::vec3> positions;
    const char*            mesh_file = "content/rubber_duck/scene.gltf";
    if (!aiLoadMesh(mesh_file, positions))
    {
        LOG_FATAL("Error: load mesh failed!");
    }

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    constexpr GLsizeiptr kBufferSize = sizeof(PerFrameData);
    GLuint               perFrameDataBuf;
    glCreateBuffers(1, &perFrameDataBuf);
    glNamedBufferStorage(perFrameDataBuf, kBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuf, 0, kBufferSize);

    GLuint meshData;
    glCreateBuffers(1, &meshData);
    glNamedBufferStorage(meshData, sizeof(glm::vec3) * positions.size(), positions.data(), 0);
    glVertexArrayVertexBuffer(vao, 0, meshData, 0, sizeof(glm::vec3));
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.f, -1.f);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float     ratio = static_cast<float>(width) / static_cast<float>(height);
        const glm::mat4 m     = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -0.5f, -1.5f)),
                                        static_cast<float>(glfwGetTime()),
                                        glm::vec3(0.f, 1.f, 0.f));
        const glm::mat4 p     = glm::perspective(45.f, ratio, 0.1f, 1000.f);

        PerFrameData perFrameData;
        perFrameData.mvp         = p * m;
        perFrameData.isWireFrame = false;

        // draw the fill cube
        glNamedBufferSubData(perFrameDataBuf, 0, kBufferSize, &perFrameData);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(positions.size()));

        // draw the wire cube
        perFrameData.isWireFrame = true;
        glNamedBufferSubData(perFrameDataBuf, 0, kBufferSize, &perFrameData);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(positions.size()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &perFrameDataBuf);
    glDeleteProgram(program);
    glDeleteShader(shaderFragment);
    glDeleteShader(shaderVertex);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    muggle::terminate();

    return 0;
}