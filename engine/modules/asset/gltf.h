#pragma once

#include <cstdint>
#include <cassert>
#include <limits>
#include <string>

namespace muggle
{
namespace glTF
{
    static const int32_t kInvalidIntValue = 0x7fffffff;
    static_assert(kInvalidIntValue == INT32_MAX, "kInvalidIntValue must be INT32_MAX");
    static const float kInvalidFloatValue = std::numeric_limits<float>::max();

    struct Asset
    {
        std::string copyright;
        std::string generator;
        std::string minVersion;
        std::string version;
    };

    struct CameraOrthographic
    {
        float xmag;
        float ymag;
        float zfar;
        float znear;
    };

    struct AccessorSparse
    {
        int32_t count;
        int32_t indices;
        int32_t values;
    };

    struct Camera
    {
        int32_t orthographic;
        int32_t perspective;
        std::string type; // orthographic or perspective
    };

    struct AnimationChannel
    {
        enum class TargetType
        {
            Translation,
            Rotation,
            Scale,
            Weights,
            Count
        };

        int32_t sampler;
        int32_t targetNode;
        TargetType targetType;
    };

    struct AnimationSampler
    {
        int32_t inputKeyFrameBufferIndex; // The index of the accessor containing keyframe input values, e.g., time.
        int32_t outputKeyFrameBufferIndex; // The index of the accessor containing keyframe output values.

        enum class Interpolation
        {
            // LINEAR: The animated values are linearly interpolated between keyframes. When targeting a rotation,
            // spherical linear interpolation (slerp) should be used to interpolate quaternions. The number of output
            // elements must equal the number of input elements.
            Linear,

            // STEP: The animated values remain constant to the output of the first keyframe, until the next keyframe.
            // The float of output elements **MUST** equal the float of input elements.
            Step,

            // CUBICSPLINE: The animation's interpolation is computed using a cubic spline with specified tangents. The
            // number of output elements must equal three times the number of input elements. For each input element, the
            // output stores three elements, which are the x, y, and z of the tangent vector at the point. The first and
            // last tangents are used as control points to calculate the start and end tangents of the curve.
            CubicSpline,
            Count
        };

        Interpolation interpolation;
    };

    struct Skin
    {
        int32_t inverseBindMatricesBufferIndex;
        int32_t skeletonNodeIndex;
        uint32_t jonitCount;
        int32_t* joints;
    };

    struct BufferView
    {
        enum class Target
        {
            ARRAY_BUFFER = 34962, // Vertex Data
            ELEMENT_ARRAY_BUFFER = 34963, // Index Data
        };

        int32_t buffer;
        int32_t byteLength;
        int32_t byteOffset;
        int32_t byteStride;
        int32_t target;
        std::string name;
    };

    struct Image
    {
        int32_t bufferView;
        std::string mimeType; // image/jpeg or image/png;
        std::string uri;
    };

    struct Node
    {
        int32_t camera;
        uint32_t childrenCount;
        int32_t* children;
        uint32_t matrixCount;
        float* matrix;
        int32_t mesh;
        uint32_t rotationCount;
        float* rotation;
        uint32_t scaleCount;
        float* scale;
        int32_t skin;
        uint32_t translationCount;
        float* translation;
        uint32_t weightsCount;
        float* weights;
        std::string name;
    };

    struct TextureInfo
    {
        int32_t index;
        int32_t texCoord;
    };

    struct MaterialPBRMetallicRoughness
    {
        uint32_t baseColorFactorCount;
        float* baseColorFactor;
        TextureInfo* baseColorTexture;
        float metallicFactor;
        TextureInfo* metallicRoughnessTexture;
        float roughnessFactor;
    };

    struct MeshPrimitive
    {
        struct Attribute
        {
            enum class Semantic
            {
                POSITION,
                NORMAL,
                TANGENT,
                TEXCOORD_0,
                TEXCOORD_1,
                COLOR_0,
                JOINTS_0,
                WEIGHTS_0,
                Count
            };

            int32_t accessor;
            Semantic semantic;
        };

        uint32_t attributesCount;
        Attribute* attributes;
        int32_t indices;
        int32_t material;
        // 0: POINTS
        // 1: LINES
        // 2: LINE_LOOP
        // 3: LINE_STRIP
        // 4: TRIANGLES
        // 5: TRIANGLE_STRIP
        // 6: TRIANGLE_FAN
        uint32_t mode;
        uint32_t targetsCount;
        Attribute* targets;
    };

    struct AcessorSparseIndices
    {
        int32_t bufferView;
        int32_t byteOffset;

        // 5121 (UNSIGNED_BYTE)
        // 5123 (UNSIGNED_SHORT)
        // 5125 (UNSIGNED_INT)
        int32_t componentType;
    };

    struct Accessor
    {
        enum class ComponentType
        {
            BYTE = 5120,
            UNSIGNED_BYTE = 5121,
            SHORT = 5122,
            UNSIGNED_SHORT = 5123,
            UNSIGNED_INT = 5125,
            FLOAT = 5126,
        };

        enum class Type
        {
            Scalar,
            Vec2,
            Vec3,
            Vec4,
            Mat2,
            Mat3,
            Mat4
        };

        int32_t bufferView;
        int32_t byteOffset;

        ComponentType componentType;
        int32_t count;
        uint32_t maxCount;
        float* max;
        uint32_t minCount;
        float* min;
        bool normalized;
        int32_t sparse;
        Type type;
    };

    struct Mesh
    {
        uint32_t primitivesCount;
        MeshPrimitive* primitives;
        uint32_t weightsCount;
        float* weights;
        std::string name;
    };

    struct Texture
    {
        int32_t sampler;
        int32_t source;
        std::string name;
    };

    struct MaterialNormalTextureInfo
    {
        int32_t index;
        int32_t texCoord;
        float scale;
    };

    struct MaterialOcclusionTextureInfo
    {
        int32_t index;
        int32_t texCoord;
        float strength;
    };

    struct Material 
    {
        float alphaCutoff;
        // OPAQUE: The alpha value is ignored and the rendered output is fully opaque.
        // MASK: The rendered output is either fully opaque or fully transparent depending on the alpha value and the 
        // specified alpha cutoff value. Th exact apperance of the edges **MAY** be subjec to implementation-dependent
        // techniques, such as "Alpha-to-Coverage".
        // BLEND: The alpha value is used to composite the source and destination areas. The rendered output is combined
        // with the background using the normal painting operation (i.e. the Porter and Duff over operator).
        std::string alphaMode;
        bool isDoubleSided;
        uint32_t emissiveFactorCount;
        float* emissiveFactor;
        TextureInfo* emissiveTexture;
        MaterialNormalTextureInfo* normalTexture;
        MaterialOcclusionTextureInfo* occlusionTexture;
        MaterialPBRMetallicRoughness* pbrMetallicRoughness;
        std::string name;
    };

    struct Buffer
    {
        int32_t byteLength;
        std::string uri;
        std::string name;
    };

    struct CameraPerspective
    {
        float aspectRatio;
        float yfov;
        float zfar;
        float znear;
    };

    struct Animation
	{
		uint32_t channelsCount;
		AnimationChannel* channels;
		uint32_t samplersCount;
		AnimationSampler* samplers;
	};

    struct AccessorSparseValues
	{
		int32_t bufferView;
		int32_t byteOffset;
	};

    struct Scene
    {
        uint32_t nodesCount;
        int32_t* nodes;
    };

    struct Sampler
	{
		enum class MagFilter
		{
			NEAREST = 9728,
			LINEAR = 9729,
		};

		enum class MinFilter
		{
			NEAREST = 9728,
			LINEAR = 9729,
			NEAREST_MIPMAP_NEAREST = 9984,
			LINEAR_MIPMAP_NEAREST = 9985,
			NEAREST_MIPMAP_LINEAR = 9986,
			LINEAR_MIPMAP_LINEAR = 9987,
		};

		enum class WrapMode
		{
			CLAMP_TO_EDGE = 33071,
			MIRRORED_REPEAT = 33648,
			REPEAT = 10497,
		};

		MagFilter magFilter;
		MinFilter minFilter;
		WrapMode wrapS;
		WrapMode wrapT;
	};

    struct glTF
	{
		Asset asset;
		uint32_t buffersCount;
		Buffer* buffers;
		uint32_t bufferViewsCount;
		BufferView* bufferViews;
		uint32_t accessorsCount;
		Accessor* accessors;
		uint32_t imagesCount;
		Image* images;
		uint32_t texturesCount;
		Texture* textures;
		uint32_t materialsCount;
		Material* materials;
		uint32_t meshesCount;
		Mesh* meshes;
		uint32_t nodesCount;
		Node* nodes;
		uint32_t samplersCount;
		Sampler* samplers;
        int32_t scene;
		uint32_t scenesCount;
		Scene* scenes;
		uint32_t skinsCount;
		Skin* skins;
		uint32_t camerasCount;
		Camera* cameras;
		uint32_t animationsCount;
		Animation* animations;
        uint32_t extensionsRequiredCount;
        std::string* extensionsRequired;
        uint32_t extensionsUsedCount;
        std::string* extensionsUsed;
	};

    int32_t getDataOffset(int32_t accessorOffset, int32_t bufferViewOffset);
} // namespace glTF

    glTF::glTF gltfLoadFile(const char* filename);
    void gltfFree(glTF::glTF* gltf);

    int32_t gltfGetAttributeAccessorIndex(
        const glTF::MeshPrimitive::Attribute* attributes,
        uint32_t attributeCount,
        glTF::MeshPrimitive::Attribute::Semantic semantic);

} // namespace muggle