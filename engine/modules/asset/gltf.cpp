#include "gltf.h"

#include "muggle.h"
#include "nlohmann/json.hpp"

#include <cstdlib>

namespace muggle
{
static void tryLoadString(nlohmann::json& jsonData, const char* key, std::string& outStr)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
        return;

    std::string value = jsonData.value(key, "");
    outStr.append(value);
}

static void tryLoadInt(nlohmann::json& jsonData, const char* key, int32_t& outValue)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outValue = glTF::kInvalidIntValue;
        return;
    }

    outValue = jsonData.value(key, 0);
}

static void tryLoadFloat(nlohmann::json& jsonData, const char* key, float& outValue)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outValue = glTF::kInvalidFloatValue;
        return;
    }

    outValue = jsonData.value(key, 0.0f);
}

static void tryLoadBool(nlohmann::json& jsonData, const char* key, bool& outValue)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outValue = false;
        return;
    }

    outValue = jsonData.value(key, false);
}

static void tryLoadType(nlohmann::json& jsonData, const char* key, glTF::Accessor::Type& outType)
{
    std::string value = jsonData.value(key, "");
    if (value == "SCALAR")
    {
        outType = glTF::Accessor::Type::Scalar;
    }
    else if (value == "VEC2")
    {
        outType = glTF::Accessor::Type::Vec2;
    }
    else if (value == "VEC3")
    {
        outType = glTF::Accessor::Type::Vec3;
    }
    else if (value == "VEC4")
    {
        outType = glTF::Accessor::Type::Vec4;
    }
    else if (value == "MAT2")
    {
        outType = glTF::Accessor::Type::Mat2;
    }
    else if (value == "MAT3")
    {
        outType = glTF::Accessor::Type::Mat3;
    }
    else if (value == "MAT4")
    {
        outType = glTF::Accessor::Type::Mat4;
    }
    else
    {
        assert(false);
    }
}

static void tryLoadIntArray(nlohmann::json& jsonData, const char* key, uint32_t& outCount, int32_t** outArray)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outCount  = 0;
        *outArray = nullptr;
    }

    nlohmann::json jsonArray = jsonData.at(key);
    outCount                 = jsonArray.size();

    int32_t* values = (int32_t*)malloc(sizeof(int32_t) * outCount);
    for (uint32_t i = 0; i < outCount; ++i)
    {
        values[i] = jsonArray.at(i);
    }

    *outArray = values;
}

static void tryLoadFloatArray(nlohmann::json& jsonData, const char* key, uint32_t& outCount, float** outArray)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outCount  = 0;
        *outArray = nullptr;
    }

    nlohmann::json jsonArray = jsonData.at(key);
    outCount                 = jsonArray.size();

    float* values = (float*)malloc(sizeof(float) * outCount);
    for (uint32_t i = 0; i < outCount; ++i)
    {
        values[i] = jsonArray.at(i);
    }

    *outArray = values;
}

static void loadAsset(nlohmann::json& jsonData, glTF::Asset& outAsset)
{
    nlohmann::json asset = jsonData.at("asset");

    tryLoadString(jsonData, "copyright", outAsset.copyright);
    tryLoadString(jsonData, "generator", outAsset.generator);
    tryLoadString(jsonData, "minVersion", outAsset.minVersion);
    tryLoadString(jsonData, "version", outAsset.version);
}

static void loadScene(nlohmann::json& jsonData, glTF::Scene& outScene)
{
    tryLoadIntArray(jsonData, "nodes", outScene.nodesCount, &outScene.nodes);
}

static void loadScenes(nlohmann::json& jsonData, glTF::glTF& gltfData)
{
    nlohmann::json scenes = jsonData.at("scenes");

    size_t sceneCount    = scenes.size();
    gltfData.scenes      = (glTF::Scene*)malloc(sizeof(glTF::Scene) * sceneCount);
    gltfData.scenesCount = sceneCount;

    for (uint32_t i = 0; i < gltfData.scenesCount; ++i)
    {
        loadScene(scenes.at(i), gltfData.scenes[i]);
    }
}

static void loadBuffer(nlohmann::json& jsonData, glTF::Buffer& outBuffer)
{
    tryLoadString(jsonData, "uri", outBuffer.uri);
    tryLoadInt(jsonData, "byteLength", outBuffer.byteLength);
    tryLoadString(jsonData, "name", outBuffer.name);
}

static void loadBuffers(nlohmann::json& jsonData, glTF::glTF& gltfData)
{
    nlohmann::json buffers = jsonData.at("buffers");

    size_t bufferCount    = buffers.size();
    gltfData.buffers      = (glTF::Buffer*)malloc(sizeof(glTF::Buffer) * bufferCount);
    gltfData.buffersCount = bufferCount;

    for (uint32_t i = 0; i < gltfData.buffersCount; ++i)
    {
        loadBuffer(buffers.at(i), gltfData.buffers[i]);
    }
}

static void loadBufferView(nlohmann::json& jsonData, glTF::BufferView& outBufferView)
{
    tryLoadInt(jsonData, "buffer", outBufferView.buffer);
    tryLoadInt(jsonData, "byteOffset", outBufferView.byteOffset);
    tryLoadInt(jsonData, "byteLength", outBufferView.byteLength);
    tryLoadInt(jsonData, "byteStride", outBufferView.byteStride);
    tryLoadInt(jsonData, "target", outBufferView.target);
    tryLoadString(jsonData, "name", outBufferView.name);
}

static void loadBufferViews(nlohmann::json& jsonData, glTF::glTF& gltfData)
{
    nlohmann::json bufferViews = jsonData.at("bufferViews");

    size_t bufferViewCount    = bufferViews.size();
    gltfData.bufferViews      = (glTF::BufferView*)malloc(sizeof(glTF::BufferView) * bufferViewCount);
    gltfData.bufferViewsCount = bufferViewCount;

    for (uint32_t i = 0; i < gltfData.bufferViewsCount; ++i)
    {
        loadBufferView(bufferViews.at(i), gltfData.bufferViews[i]);
    }
}

static void loadNode(nlohmann::json& jsonData, glTF::Node& outNode)
{
    tryLoadInt(jsonData, "camera", outNode.camera);
    tryLoadInt(jsonData, "mesh", outNode.mesh);
    tryLoadInt(jsonData, "skin", outNode.skin);
    tryLoadIntArray(jsonData, "children", outNode.childrenCount, &outNode.children);
    tryLoadFloatArray(jsonData, "matrix", outNode.matrixCount, &outNode.matrix);
    tryLoadFloatArray(jsonData, "rotation", outNode.rotationCount, &outNode.rotation);
    tryLoadFloatArray(jsonData, "scale", outNode.scaleCount, &outNode.scale);
    tryLoadFloatArray(jsonData, "translation", outNode.translationCount, &outNode.translation);
    tryLoadFloatArray(jsonData, "weights", outNode.weightsCount, &outNode.weights);
    tryLoadString(jsonData, "name", outNode.name);
}

static void loadNodes(nlohmann::json& jsonData, glTF::glTF& gltfData)
{
    nlohmann::json nodes = jsonData.at("nodes");

    size_t nodeCount    = nodes.size();
    gltfData.nodes      = (glTF::Node*)malloc(sizeof(glTF::Node) * nodeCount);
    gltfData.nodesCount = nodeCount;

    for (uint32_t i = 0; i < gltfData.nodesCount; ++i)
    {
        loadNode(nodes.at(i), gltfData.nodes[i]);
    }
}

static void loadMeshPrimitive(nlohmann::json& jsonData, glTF::MeshPrimitive& outMeshPrimitive)
{
    tryLoadInt(jsonData, "indices", outMeshPrimitive.indices);
    tryLoadInt(jsonData, "material", outMeshPrimitive.material);
    tryLoadInt(jsonData, "mode", outMeshPrimitive.mode);

    nlohmann::json attributes = jsonData.at("attributes");
    outMeshPrimitive.attributes =
        (glTF::MeshPrimitive::Attribute*)malloc(sizeof(glTF::MeshPrimitive::Attribute) * attributes.size());
    outMeshPrimitive.attributesCount = attributes.size();

    uint32_t index = 0;
    for (auto jsonAttribute : attributes.items())
    {
        std::string                     key           = jsonAttribute.key();
        glTF::MeshPrimitive::Attribute& outAttributes = outMeshPrimitive.attributes[index];

        outAttributes.key           = key;
        outAttributes.accessorIndex = jsonAttribute.value();

        ++index;
    }
}

static void loadMeshPrimitives(nlohmann::json& jsonData, glTF::Mesh& outMesh)
{
    nlohmann::json primitives = jsonData.at("primitives");

    size_t primitiveCount   = primitives.size();
    outMesh.primitives      = (glTF::MeshPrimitive*)malloc(sizeof(glTF::MeshPrimitive) * primitiveCount);
    outMesh.primitivesCount = primitiveCount;

    for (uint32_t i = 0; i < outMesh.primitivesCount; ++i)
    {
        loadMeshPrimitive(primitives.at(i), outMesh.primitives[i]);
    }
}

static void loadMesh(nlohmann::json& jsonData, glTF::Mesh& outMesh)
{
    loadMeshPrimitives(jsonData, outMesh);
    tryLoadFloatArray(jsonData, "weights", outMesh.weightsCount, &outMesh.weights);
    tryLoadString(jsonData, "name", outMesh.name);
}

static void loadMeshes(nlohmann::json& jsonData, glTF::glTF& gltfData)
{
    nlohmann::json meshes = jsonData.at("meshes");

    size_t meshCount     = meshes.size();
    gltfData.meshes      = (glTF::Mesh*)malloc(sizeof(glTF::Mesh) * meshCount);
    gltfData.meshesCount = meshCount;

    for (uint32_t i = 0; i < gltfData.meshesCount; ++i)
    {
        loadMesh(meshes.at(i), gltfData.meshes[i]);
    }
}

static void loadAccessor(nlohmann::json& jsonData, glTF::Accessor& outAccessor)
{
    tryLoadInt(jsonData, "bufferView", outAccessor.bufferView);
    tryLoadInt(jsonData, "byteOffset", outAccessor.byteOffset);

    int componentType = 0;
    tryLoadInt(jsonData, "componentType", componentType);
    outAccessor.componentType = static_cast<glTF::Accessor::ComponentType>(componentType);

    tryLoadInt(jsonData, "count", outAccessor.count);
    tryLoadInt(jsonData, "sparse", outAccessor.sparse);

    tryLoadFloatArray(jsonData, "max", outAccessor.maxCount, &outAccessor.max);
    tryLoadFloatArray(jsonData, "min", outAccessor.minCount, &outAccessor.min);

    tryLoadBool(jsonData, "normalized", outAccessor.normalized);
    tryLoadType(jsonData, "type", outAccessor.type);
}

static void loadAccessors(nlohmann::json& jsonData, glTF::glTF& gltfData)
{
    nlohmann::json accessors = jsonData.at("accessors");

    size_t accessorCount    = accessors.size();
    gltfData.accessors      = (glTF::Accessor*)malloc(sizeof(glTF::Accessor) * accessorCount);
    gltfData.accessorsCount = accessorCount;

    for (uint32_t i = 0; i < gltfData.accessorsCount; ++i)
    {
        loadAccessor(accessors.at(i), gltfData.accessors[i]);
    }
}

static void tryLoadTextureInfo(nlohmann::json& jsonData, const char* key, glTF::TextureInfo** outTextureInfo)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outTextureInfo = nullptr;
        return;
    }

    glTF::TextureInfo* textureInfo = (glTF::TextureInfo*)_aligned_malloc(64, sizeof(glTF::TextureInfo));

    tryLoadInt(*it, "index", textureInfo->index);
    tryLoadInt(*it, "texCoord", textureInfo->texCoord);

    *outTextureInfo = textureInfo;
}

static void tryLoadMaterialNormalTextureInfo(nlohmann::json&                   jsonData,
                                             std::string                       key,
                                             glTF::MaterialNormalTextureInfo** outTextureInfo)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outTextureInfo = nullptr;
        return;
    }

    glTF::MaterialNormalTextureInfo* textureInfo =
        (glTF::MaterialNormalTextureInfo*)_aligned_malloc(64, sizeof(glTF::MaterialNormalTextureInfo));

    tryLoadInt(*it, "index", textureInfo->index);
    tryLoadInt(*it, "texCoord", textureInfo->texCoord);
    tryLoadFloat(*it, "scale", textureInfo->scale);

    *outTextureInfo = textureInfo;
}

static void tryLoadMaterialOcclusionTextureInfo(nlohmann::json&                      jsonData,
                                                std::string                          key,
                                                glTF::MaterialOcclusionTextureInfo** outTextureInfo)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outTextureInfo = nullptr;
        return;
    }

    glTF::MaterialOcclusionTextureInfo* textureInfo =
        (glTF::MaterialOcclusionTextureInfo*)_aligned_malloc(64, sizeof(glTF::MaterialOcclusionTextureInfo));

    tryLoadInt(*it, "index", textureInfo->index);
    tryLoadInt(*it, "texCoord", textureInfo->texCoord);
    tryLoadFloat(*it, "strength", textureInfo->strength);

    *outTextureInfo = textureInfo;
}

static void tryLoadMaterialPbrMetallicRoughness(nlohmann::json&                      jsonData,
                                                std::string                          key,
                                                glTF::MaterialPBRMetallicRoughness** outPbrMetallicRoughness)
{
    auto it = jsonData.find(key);
    if (it == jsonData.end())
    {
        outPbrMetallicRoughness = nullptr;
        return;
    }

    glTF::MaterialPBRMetallicRoughness* pbrMetallicRoughness =
        (glTF::MaterialPBRMetallicRoughness*)_aligned_malloc(64, sizeof(glTF::MaterialPBRMetallicRoughness));

    tryLoadFloatArray(
        *it, "baseColorFactor", pbrMetallicRoughness->baseColorFactorCount, &pbrMetallicRoughness->baseColorFactor);
    tryLoadTextureInfo(*it, "baseColorTexture", &pbrMetallicRoughness->baseColorTexture);
    tryLoadFloat(*it, "metallicFactor", pbrMetallicRoughness->metallicFactor);
    tryLoadFloat(*it, "roughnessFactor", pbrMetallicRoughness->roughnessFactor);
    tryLoadTextureInfo(*it, "metallicRoughnessTexture", &pbrMetallicRoughness->metallicRoughnessTexture);

    *outPbrMetallicRoughness = pbrMetallicRoughness;
}

static void loadMaterial(nlohmann::json& jsonData, glTF::Material& outMaterial)
{
    tryLoadFloatArray(jsonData, "emissiveFactor", outMaterial.emissiveFactorCount, &outMaterial.emissiveFactor);
    tryLoadFloat(jsonData, "alphaCutoff", outMaterial.alphaCutoff);
    tryLoadString(jsonData, "alphaMode", outMaterial.alphaMode);
    tryLoadBool(jsonData, "doubleSided", outMaterial.isDoubleSided);

    tryLoadTextureInfo(jsonData, "emissiveTexture", &outMaterial.emissiveTexture);
    tryLoadMaterialNormalTextureInfo(jsonData, "normalTexture", &outMaterial.normalTexture);
    tryLoadMaterialOcclusionTextureInfo(jsonData, "occlusionTexture", &outMaterial.occlusionTexture);
    tryLoadMaterialPbrMetallicRoughness(jsonData, "pbrMetallicRoughness", &outMaterial.pbrMetallicRoughness);

    tryLoadString(jsonData, "name", outMaterial.name);
}

static void loadMaterials(nlohmann::json& jsonData, glTF::glTF& outGltfData)
{
    nlohmann::json materials = jsonData.at("materials");

    size_t materialCount       = materials.size();
    outGltfData.materials      = (glTF::Material*)malloc(sizeof(glTF::Material) * materialCount);
    outGltfData.materialsCount = materialCount;

    for (uint32_t i = 0; i < outGltfData.materialsCount; ++i)
    {
        loadMaterial(materials.at(i), outGltfData.materials[i]);
    }
}

static void loadTexture(nlohmann::json& jsonData, glTF::Texture& outTexture)
{
    tryLoadInt(jsonData, "sampler", outTexture.sampler);
    tryLoadInt(jsonData, "source", outTexture.source);
    tryLoadString(jsonData, "name", outTexture.name);
}

static void loadTextures(nlohmann::json& jsonData, glTF::glTF& outGltfData)
{
    nlohmann::json textures = jsonData.at("textures");

    size_t textureCount       = textures.size();
    outGltfData.textures      = (glTF::Texture*)malloc(sizeof(glTF::Texture) * textureCount);
    outGltfData.texturesCount = textureCount;

    for (uint32_t i = 0; i < outGltfData.texturesCount; ++i)
    {
        loadTexture(textures.at(i), outGltfData.textures[i]);
    }
}

static void loadImage(nlohmann::json& jsonData, glTF::Image& outImage)
{
    tryLoadInt(jsonData, "bufferView", outImage.bufferView);
    tryLoadString(jsonData, "mimeType", outImage.mimeType);
    tryLoadString(jsonData, "uri", outImage.uri);
}

static void loadImages(nlohmann::json& jsonData, glTF::glTF& outGltfData)
{
    nlohmann::json images = jsonData.at("images");

    size_t imageCount       = images.size();
    outGltfData.images      = (glTF::Image*)malloc(sizeof(glTF::Image) * imageCount);
    outGltfData.imagesCount = imageCount;

    for (uint32_t i = 0; i < outGltfData.imagesCount; ++i)
    {
        loadImage(images.at(i), outGltfData.images[i]);
    }
}

static void loadSampler(nlohmann::json& jsonData, glTF::Sampler& outSampler)
{
    int magFilter, minFilter, wrapS, wrapT;
    tryLoadInt(jsonData, "magFilter", magFilter);
    tryLoadInt(jsonData, "minFilter", minFilter);
    tryLoadInt(jsonData, "wrapS", wrapS);
    tryLoadInt(jsonData, "wrapT", wrapT);

    outSampler.magFilter = static_cast<glTF::Sampler::MagFilter>(magFilter);
    outSampler.minFilter = static_cast<glTF::Sampler::MinFilter>(minFilter);
    outSampler.wrapS     = static_cast<glTF::Sampler::WrapMode>(wrapS);
    outSampler.wrapT     = static_cast<glTF::Sampler::WrapMode>(wrapT);
}

static void loadSamplers(nlohmann::json& jsonData, glTF::glTF& outGltfData)
{
    nlohmann::json samplers = jsonData.at("samplers");

    size_t samplerCount       = samplers.size();
    outGltfData.samplers      = (glTF::Sampler*)malloc(sizeof(glTF::Sampler) * samplerCount);
    outGltfData.samplersCount = samplerCount;

    for (uint32_t i = 0; i < outGltfData.samplersCount; ++i)
    {
        loadSampler(samplers.at(i), outGltfData.samplers[i]);
    }
}

static void loadSkin(nlohmann::json& jsonData, glTF::Skin& outSkin)
{
    tryLoadInt(jsonData, "skeleton", outSkin.skeletonRootNodeIndex);
    tryLoadInt(jsonData, "inverseBindMatrices", outSkin.inverseBindMatricesBufferIndex);
    tryLoadIntArray(jsonData, "joints", outSkin.jonitsCount, &outSkin.joints);
}

static void loadSkins(nlohmann::json& jsonData, glTF::glTF& outGltfData)
{
    nlohmann::json skins = jsonData.at("skins");

    size_t skinCount       = skins.size();
    outGltfData.skins      = (glTF::Skin*)malloc(sizeof(glTF::Skin) * skinCount);
    outGltfData.skinsCount = skinCount;

    for (uint32_t i = 0; i < outGltfData.skinsCount; ++i)
    {
        loadSkin(skins.at(i), outGltfData.skins[i]);
    }
}

static void loadAnimation(nlohmann::json& jsonData, glTF::Animation& outAnimation)
{
    nlohmann::json json_samplers = jsonData.at("samplers");
    if (json_samplers.is_array())
    {
        size_t samplerCount = json_samplers.size();

        glTF::AnimationSampler* values = (glTF::AnimationSampler*)malloc(sizeof(glTF::AnimationSampler) * samplerCount);

        for (size_t i = 0; i < samplerCount; ++i)
        {
            nlohmann::json          element = json_samplers.at(i);
            glTF::AnimationSampler& sampler = values[i];

            tryLoadInt(element, "input", sampler.inputKeyFrameBufferIndex);
            tryLoadInt(element, "output", sampler.outputKeyFrameBufferIndex);

            std::string interpolation = element.value("interpolation", "LINEAR");
            if (interpolation == "LINEAR")
            {
                sampler.interpolation = glTF::AnimationSampler::Interpolation::Linear;
            }
            else if (interpolation == "STEP")
            {
                sampler.interpolation = glTF::AnimationSampler::Interpolation::Step;
            }
            else if (interpolation == "CUBICSPLINE")
            {
                sampler.interpolation = glTF::AnimationSampler::Interpolation::CubicSpline;
            }
            else
            {
                assert(false);
            }
        }

        outAnimation.samplers      = values;
        outAnimation.samplersCount = samplerCount;
    }

    nlohmann::json json_channels = jsonData.at("channels");
    if (json_channels.is_array())
    {
        size_t channelCount = json_channels.size();

        glTF::AnimationChannel* values = (glTF::AnimationChannel*)malloc(sizeof(glTF::AnimationChannel) * channelCount);

        for (size_t i = 0; i < channelCount; ++i)
        {
            nlohmann::json          element = json_channels.at(i);
            glTF::AnimationChannel& channel = values[i];

            tryLoadInt(element, "sampler", channel.sampler);

            nlohmann::json targetElement = element.at("target");
            tryLoadInt(targetElement, "node", channel.targetNode);

            std::string targetPath = targetElement.value("path", "translation");
            if (targetPath == "translation")
            {
                channel.targetType = glTF::AnimationChannel::TargetType::Translation;
            }
            else if (targetPath == "rotation")
            {
                channel.targetType = glTF::AnimationChannel::TargetType::Rotation;
            }
            else if (targetPath == "scale")
            {
                channel.targetType = glTF::AnimationChannel::TargetType::Scale;
            }
            else if (targetPath == "weights")
            {
                channel.targetType = glTF::AnimationChannel::TargetType::Weights;
            }
            else
            {
                assert(false);
                channel.targetType = glTF::AnimationChannel::TargetType::Count;
            }
        }

        outAnimation.channels      = values;
        outAnimation.channelsCount = channelCount;
    }
}

static void loadAnimations(nlohmann::json& jsonData, glTF::glTF& outGltfData)
{
    nlohmann::json animations = jsonData.at("animations");

    size_t animationCount       = animations.size();
    outGltfData.animations      = (glTF::Animation*)malloc(sizeof(glTF::Animation) * animationCount);
    outGltfData.animationsCount = animationCount;

    for (uint32_t i = 0; i < outGltfData.animationsCount; ++i)
    {
        loadAnimation(animations.at(i), outGltfData.animations[i]);
    }
}

int32_t glTF::getDataOffset(int32_t accessorOffset, int32_t bufferViewOffset)
{
    int32_t byteOffset = bufferViewOffset == kInvalidIntValue ? 0 : bufferViewOffset;
    byteOffset += accessorOffset == kInvalidIntValue ? 0 : accessorOffset;

    return byteOffset;
}

glTF::glTF gltfLoadFile(const char* filename)
{
    glTF::glTF gltfData {};

    if (!gFileSystem->isFileExists(filename))
    {
        LOG_ERROR("Error: file {} not found", filename);
        return gltfData;
    }

    auto           fileBlob = gFileSystem->readFile(filename);
    nlohmann::json jsonData = nlohmann::json::parse((char*)fileBlob->data());

    for (auto propertis : jsonData.items())
    {
        if (propertis.key() == "asset")
        {
            loadAsset(jsonData, gltfData.asset);
        }
        else if (propertis.key() == "scene")
        {
            tryLoadInt(jsonData, "scene", gltfData.scene);
        }
        else if (propertis.key() == "scenes")
        {
            loadScenes(jsonData, gltfData);
        }
        else if (propertis.key() == "buffers")
        {
            loadBuffers(jsonData, gltfData);
        }
        else if (propertis.key() == "bufferViews")
        {
            loadBufferViews(jsonData, gltfData);
        }
        else if (propertis.key() == "nodes")
        {
            loadNodes(jsonData, gltfData);
        }
        else if (propertis.key() == "meshes")
        {
            loadMeshes(jsonData, gltfData);
        }
        else if (propertis.key() == "accessors")
        {
            loadAccessors(jsonData, gltfData);
        }
        else if (propertis.key() == "materials")
        {
            loadMaterials(jsonData, gltfData);
        }
        else if (propertis.key() == "textures")
        {
            loadTextures(jsonData, gltfData);
        }
        else if (propertis.key() == "images")
        {
            loadImages(jsonData, gltfData);
        }
        else if (propertis.key() == "samplers")
        {
            loadSamplers(jsonData, gltfData);
        }
        else if (propertis.key() == "skins")
        {
            loadSkins(jsonData, gltfData);
        }
        else if (propertis.key() == "animations")
        {
            loadAnimations(jsonData, gltfData);
        }
    }

    return gltfData;
}

void gltfFree(glTF::glTF& scene)
{
    if (scene.scenes)
    {
        free(scene.scenes);
        scene.scenes = nullptr;
    }

    if (scene.buffers)
    {
        free(scene.buffers);
        scene.buffers = nullptr;
    }

    if (scene.bufferViews)
    {
        free(scene.bufferViews);
        scene.bufferViews = nullptr;
    }

    if (scene.nodes)
    {
        free(scene.nodes);
        scene.nodes = nullptr;
    }

    if (scene.meshes)
    {
        free(scene.meshes);
        scene.meshes = nullptr;
    }

    if (scene.accessors)
    {
        free(scene.accessors);
        scene.accessors = nullptr;
    }

    if (scene.materials)
    {
        free(scene.materials);
        scene.materials = nullptr;
    }

    if (scene.textures)
    {
        free(scene.textures);
        scene.textures = nullptr;
    }

    if (scene.images)
    {
        free(scene.images);
        scene.images = nullptr;
    }

    if (scene.samplers)
    {
        free(scene.samplers);
        scene.samplers = nullptr;
    }

    if (scene.skins)
    {
        free(scene.skins);
        scene.skins = nullptr;
    }

    if (scene.animations)
    {
        free(scene.animations);
        scene.animations = nullptr;
    }
}

int32_t gltfGetAttributeAccessorIndex(const glTF::MeshPrimitive::Attribute* attributes,
                                      uint32_t                              attributeCount,
                                      std::string                           attributeName)
{
    for (uint32_t i = 0; i < attributeCount; ++i)
    {
        if (attributes[i].key == attributeName)
        {
            return attributes[i].accessorIndex;
        }
    }

    return glTF::kInvalidIntValue;
}

} // namespace muggle
