#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vulkan/vulkan.h>

#include "gpu_enum.h"

namespace muggle
{
static constexpr uint32_t kInvalidIndex = 0xffffffff;

using ResourceHandle = uint32_t;

struct BufferHandle
{
    ResourceHandle index{kInvalidIndex};
};

struct TextureHandle
{
    ResourceHandle index{kInvalidIndex};
};

struct ShaderStateHandle
{
    ResourceHandle index{kInvalidIndex};
};

struct SamplerHandle
{
    ResourceHandle index{kInvalidIndex};
};

struct DescriptorSetLayoutHandle
{
    ResourceHandle index{kInvalidIndex};
};

struct DescriptorSetHandle
{
    ResourceHandle index{kInvalidIndex};
};

struct PipelineHandle
{
    ResourceHandle index{kInvalidIndex};
};

struct RenderPassHandle
{
    ResourceHandle index{kInvalidIndex};
};

static constexpr BufferHandle        kInvalidBuffer{kInvalidIndex};
static constexpr TextureHandle       kInvalidTexture{kInvalidIndex};
static constexpr ShaderStateHandle   kInvalidShaderState{kInvalidIndex};
static constexpr SamplerHandle       kInvalidSampler{kInvalidIndex};
static constexpr DescriptorSetLayoutHandle kInvalidDescriptorSetLayout{kInvalidIndex};
static constexpr DescriptorSetHandle kInvalidDescriptorSet{kInvalidIndex};
static constexpr PipelineHandle      kInvalidPipeline{kInvalidIndex};
static constexpr RenderPassHandle    kInvalidRenderPass{kInvalidIndex};

// Maximum number of images/render targets/fbo attachments usable
static constexpr uint8_t kMaxImageOutputs = 8u;
// Maximum number of layouts in the pipeline
static constexpr uint8_t kMaxDescriptorSetLayouts = 8u;
// Maximum number of simultaneous shader stages. Applicable to all different type of pipelins
static constexpr uint8_t kMaxShaderStages = 5u;
// Maximum list elements for both descriptor set layout and descriptor sets
static constexpr uint8_t kMaxDescriptorsPerSet = 16u;

static constexpr uint8_t kMaxVertexStreams = 16u;
static constexpr uint8_t kMaxVertexAttributes = 16u;

static constexpr uint8_t kMaxSwapChainImages = 3u;

struct Rect2D
{
    float x{0.f};
    float y{0.f};
    float width{0.f};
    float height{0.f};
};

struct Rect2DInt
{
    int16_t x{0};
    int16_t y{0};
    int16_t width{0};
    int16_t height{0};
};

struct ViewPort
{
    Rect2DInt rect;
    float  minDepth{0.f};
    float  maxDepth{1.f};
};

struct ViewportState
{
    uint32_t viewportCount{0};
    uint32_t scissorsCount{0};

    ViewPort* viewports{nullptr};
    Rect2DInt* scissors{nullptr};
};

struct StencilOperationState
{
    VkStencilOp failure{VK_STENCIL_OP_KEEP};
    VkStencilOp pass {VK_STENCIL_OP_KEEP};
    VkStencilOp depthFail {VK_STENCIL_OP_KEEP};
    VkCompareOp compare {VK_COMPARE_OP_ALWAYS};
    uint32_t compareMask {0xff};
    uint32_t writeMask {0xff};
    uint32_t reference {0xff};
};

struct DepthStencilCreation
{
    StencilOperationState front;
    StencilOperationState back;
    VkCompareOp depthCompare {VK_COMPARE_OP_ALWAYS};

    uint8_t depthEnable : 1;
    uint8_t depthWriteEnable : 1;
    uint8_t stencilEnable : 1;
    uint8_t pad : 5;
};

struct BlendState
{
    VkBlendFactor srcColor { VK_BLEND_FACTOR_ONE};
    VkBlendFactor dstColor { VK_BLEND_FACTOR_ONE};
    VkBlendOp colorOperation { VK_BLEND_OP_ADD};

    VkBlendFactor srcAlpha { VK_BLEND_FACTOR_ONE};
    VkBlendFactor dstAlpha { VK_BLEND_FACTOR_ONE};
    VkBlendOp alphaOperation { VK_BLEND_OP_ADD};

    ColorWriteEnabled::Mask colorWriteMask { ColorWriteEnabled::All_Mask};

    uint8_t isBlendEnabled : 1;
    uint8_t isSeparateBlend : 1;
    uint8_t pad : 6;

    BlendState()
        : isBlendEnabled{0}
    , isSeparateBlend{0}
    , pad{0}
    {}

    BlendState& setColor(VkBlendFactor sourceColor, VkBlendFactor destinationColor, VkBlendOp colorOp);
    BlendState& setAlpha(VkBlendFactor sourceAlpha, VkBlendFactor destinationAlpha, VkBlendOp alphaOp);
    BlendState& setColorWriteMask(ColorWriteEnabled::Mask mask);
};

struct BlendStateCreation
{
    BlendState blendStates[kMaxImageOutputs];
    uint32_t active_states {0};

    BlendStateCreation& reset();
    BlendState& addBlendState();
};

struct RasterizationCreation
{
    VkCullModeFlagBits cullMode { VK_CULL_MODE_NONE};
    VkFrontFace frontFace { VK_FRONT_FACE_COUNTER_CLOCKWISE};
    FillMode::Enum fillMode { FillMode::Solid};
};

struct BufferCreation
{
    VkBufferUsageFlags usageFlags { 0};
    ResourceUsageType::Enum usageType { ResourceUsageType::Immutable};
    uint32_t size { 0};
    void* initialData { nullptr};

    const char* name {nullptr};
};

struct TextureCreation
{
    void* initialData { nullptr};
    uint16_t width = 1;
    uint16_t height = 1;
    uint16_t depth = 1;
    uint8_t mipmaps = 1;
    uint8_t flags = 0; // TextureFlags bitmasks

    VkFormat format { VK_FORMAT_UNDEFINED};
    TextureType::Enum type { TextureType::Texture2D};

    const char* name {nullptr};
};

struct SamplerCreation
{
    VkFilter minFilter { VK_FILTER_NEAREST};
    VkFilter magFilter { VK_FILTER_NEAREST};
    VkSamplerMipmapMode mipmapMode { VK_SAMPLER_MIPMAP_MODE_NEAREST};
    
    VkSamplerAddressMode addressModeU { VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE};
    VkSamplerAddressMode addressModeV { VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE};
    VkSamplerAddressMode addressModeW { VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE};

    const char* name {nullptr};
};

struct ShaderStage
{
    const char* code {nullptr};
    uint32_t codeSize {0};
    VkShaderStageFlagBits type {VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM};
};

struct ShaderStateCreation
{
    ShaderStage stages[kMaxShaderStages];
    uint32_t stagesCount {0};
    uint32_t spvInput {0};

    const char* name {nullptr};

    ShaderStateCreation& reset();
    ShaderStateCreation& setName(const char* name);
    ShaderStateCreation& addStage(const char* code, uint32_t codeSize, VkShaderStageFlagBits type);
    ShaderStateCreation& setSpvInput(uint32_t spvInput);

};

struct DescriptorSetLayoutCreation
{
    struct Binding
    {
        VkDescriptorType type {VK_DESCRIPTOR_TYPE_MAX_ENUM};
        uint16_t start {0};
        uint16_t count {0};
        const char* name { nullptr};
    };
    
    Binding bindings[kMaxDescriptorsPerSet];
    uint32_t bindingsCount {0};
    uint32_t setIndex {0};

    const char* name {nullptr};

    DescriptorSetLayoutCreation& reset();
    DescriptorSetLayoutCreation& setName(const char* name);
    DescriptorSetLayoutCreation& addBinding(const Binding& binding);
    DescriptorSetLayoutCreation& setSetIndex(uint32_t setIndex);
};

struct DescriptorSetCreation
{
    ResourceHandle resources[kMaxDescriptorsPerSet];
    SamplerHandle samplers[kMaxDescriptorsPerSet];
    uint16_t bindings[kMaxDescriptorsPerSet];

    DescriptorSetLayoutHandle layout;
    uint32_t resourcesCount {0};

    const char* name {nullptr};

    DescriptorSetCreation& reset();
    DescriptorSetCreation& texture(TextureHandle texture, uint16_t binding);
    DescriptorSetCreation& buffer(BufferHandle buffer, uint16_t binding);
    DescriptorSetCreation& textureSampler(TextureHandle texture, SamplerHandle sampler, uint16_t binding);
};

struct DescriptorSetUpdate
{
    DescriptorSetHandle descriptorSet;
    uint32_t frameIssued {0};
};

struct VertexAttribute
{
    uint16_t location {0};
    uint16_t binding {0};
    uint32_t offset {0};
    VertexComponentFormat::Enum format {VertexComponentFormat::Float};
};

struct VertexStream
{
    uint16_t binding {0};    
    uint16_t stride {0};
    VertexInputRate::Enum rate {VertexInputRate::Count};
};

struct VertexInputCreation
{
    VertexStream vertexStreams[kMaxVertexStreams];
    VertexAttribute vertexAttributes[kMaxVertexAttributes];
    uint32_t vertexStreamCount {0};
    uint32_t vertexAttributeCount {0};

    VertexInputCreation& reset();
    VertexInputCreation& addStream(const VertexStream& stream);
    VertexInputCreation& addAttribute(const VertexAttribute& attribute);
};

struct RenderPassOutput
{
    VkFormat colorFormats[kMaxImageOutputs];
    VkFormat depthStencilFormat {VK_FORMAT_UNDEFINED};
    uint32_t colorFormatsCount {0};

    RenderPassOperation::Enum colorOp { RenderPassOperation::DontCare};
    RenderPassOperation::Enum depthOp { RenderPassOperation::DontCare};
    RenderPassOperation::Enum stencilOp { RenderPassOperation::DontCare};

    RenderPassOutput& reset();
    RenderPassOutput& color(VkFormat format);
    RenderPassOutput& depth(VkFormat format);
    RenderPassOutput& setOperations(RenderPassOperation::Enum color, RenderPassOperation::Enum depth, RenderPassOperation::Enum stencil);
};

struct RenderPassCreation
{
    uint16_t renderTargetsCount {0};
    RenderPassType::Enum type {RenderPassType::Geometry};

    TextureHandle outputTextures[kMaxImageOutputs];
    TextureHandle depthStencilTexture;

    float scaleX {1.f};
    float scaleY {1.f};
    uint8_t resize = 1;

    RenderPassOperation::Enum colorOp { RenderPassOperation::DontCare};
    RenderPassOperation::Enum depthOp { RenderPassOperation::DontCare};
    RenderPassOperation::Enum stencilOp { RenderPassOperation::DontCare};

    const char* name {nullptr};

    RenderPassCreation& reset();
    RenderPassCreation& setRenderTexture(TextureHandle texture);
    RenderPassCreation& setDepthStencilTexture(TextureHandle texture);
    RenderPassCreation& setScale(float x, float y, uint8_t resize);
    RenderPassCreation& setOperations(RenderPassOperation::Enum color, RenderPassOperation::Enum depth, RenderPassOperation::Enum stencil);
    RenderPassCreation& setName(const char* name);
    RenderPassCreation& setType(RenderPassType::Enum type);
};

struct PipelineCreation
{
    RasterizationCreation rasterization;
    BlendStateCreation blendState;
    DepthStencilCreation depthStencil;
    VertexInputCreation vertexInput;
    ShaderStateCreation shaders;

    RenderPassOutput renderPassOutput;
    DescriptorSetLayoutHandle descriptorSetLayouts[kMaxDescriptorSetLayouts];
    const ViewportState* viewportState {nullptr};

    uint32_t activeLayoutsCount {0};

    const char* name {nullptr};

    RenderPassOutput& renderPass();
};

// Helper functions for texture format
namespace TextureFormat
{
inline bool isDepthStencil(VkFormat format)
{
    return format == VK_FORMAT_D16_UNORM_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT || format == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

inline bool isDepthOnly(VkFormat format)
{
    return format >= VK_FORMAT_D16_UNORM && format < VK_FORMAT_D32_SFLOAT;
}

inline bool isStencilOnly(VkFormat format)
{
    return format == VK_FORMAT_S8_UINT;
}

inline bool hasDepth(VkFormat format)
{
    return (format >= VK_FORMAT_D16_UNORM && format < VK_FORMAT_S8_UINT) || (format >= VK_FORMAT_D16_UNORM_S8_UINT && format <= VK_FORMAT_D32_SFLOAT_S8_UINT);
}

inline bool hasStencil(VkFormat format)
{
    return format >= VK_FORMAT_S8_UINT && format <= VK_FORMAT_D32_SFLOAT_S8_UINT;
}

inline bool hasDepthOrStencil(VkFormat format)
{
    return format >= VK_FORMAT_D16_UNORM && format <= VK_FORMAT_D32_SFLOAT_S8_UINT; 
}

}

struct ResourceData
{
    void* data {nullptr};
    uint32_t size {0};
};

struct ResourceBinding
{
    uint16_t type {0};
    uint16_t start { 0};
    uint16_t count { 0};
    uint16_t set{0};

    const char* name {nullptr};
};

struct ShaderStateDescription
{
    void* nativeHandle {nullptr};
    const char* name {nullptr};
};

struct BufferDescription
{
    void* nativeHandle {nullptr};
    const char* name {nullptr};

    VkBufferUsageFlags typeFlags { 0};
    ResourceUsageType::Enum usage { ResourceUsageType::Immutable};
    uint32_t size { 0};
    BufferHandle parentHandle;
};

struct TextureDescription
{
    void* nativeHandle {nullptr};
    const char* name {nullptr};

    uint16_t width { 1};
    uint16_t height { 1};
    uint16_t depth { 1};
    uint8_t mipmaps { 1};
    uint8_t renderTarget { 0};
    uint8_t computeAccess { 0};
    
    VkFormat format { VK_FORMAT_UNDEFINED};
    TextureType::Enum type { TextureType::Texture2D};
    
};

struct SamplerDescription
{
    const char* name {nullptr};

    VkFilter minFilter { VK_FILTER_NEAREST};
    VkFilter magFilter { VK_FILTER_NEAREST};
    VkSamplerMipmapMode mipmapMode { VK_SAMPLER_MIPMAP_MODE_NEAREST};
    
    VkSamplerAddressMode addressModeU { VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE};
    VkSamplerAddressMode addressModeV { VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE};
    VkSamplerAddressMode addressModeW { VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE};

};

struct DescriptorSetLayoutDescription
{
    ResourceBinding bindings[kMaxDescriptorsPerSet];
    uint32_t activeBindingsCount {0};
};

struct DescriptorSetDescription
{
    ResourceData resources[kMaxDescriptorsPerSet];
    uint32_t activeResourcesCount {0};
};

struct PipelineDescription
{
    ShaderStateHandle shader;
};

struct MapBufferParams
{
    BufferHandle buffer;
    uint32_t offset{0};
    uint32_t size{0};
};

struct ImageBarrier
{
    TextureHandle texture;
};

struct MemoryBarrier
{
    BufferHandle buffer;
};

struct ExecutionBarrier
{
    PipelineStage::Enum sourcePipelineStage;
    PipelineStage::Enum destinationPipelineStage;

    uint32_t newBarrierExperimental = std::numeric_limits<uint32_t>::max();
    uint32_t loadOperation { 0};

    uint32_t imageBarrierCount {0};
    uint32_t memoryBarrierCount {0};

    ExecutionBarrier& reset();
    ExecutionBarrier& set(PipelineStage::Enum source, PipelineStage::Enum destination);
    ExecutionBarrier& addImageBarrier(const ImageBarrier& barrier);
    ExecutionBarrier& addMemoryBarrier(const MemoryBarrier& barrier);
};

struct ResourceUpdate
{
    ResourceDeletionType::Enum type {ResourceDeletionType::Count};
    ResourceHandle handle;
    uint32_t currentFrame {0};
};

struct Buffer
{
    VkBuffer vkBuffer {VK_NULL_HANDLE};
    //VmaAllocation allocation {VK_NULL_HANDLE};
    VkDeviceMemory deviceMemory {VK_NULL_HANDLE};
    VkDeviceSize deviceSize{ 0};

    VkBufferUsageFlags typeFlags { 0};
    ResourceUsageType::Enum usage { ResourceUsageType::Immutable};
    uint32_t size { 0};

    BufferHandle handle;
    BufferHandle parentHandle;

    const char* name {nullptr};
};

struct Sampler
{
    VkSampler vkSampler { VK_NULL_HANDLE};

    VkFilter minFilter {VK_FILTER_NEAREST};
    VkFilter magFilter {VK_FILTER_NEAREST};
    VkSamplerMipmapMode mipFilter { VK_SAMPLER_MIPMAP_MODE_NEAREST};

    VkSamplerAddressMode addressModeU { VK_SAMPLER_ADDRESS_MODE_REPEAT};
    VkSamplerAddressMode addressModeV { VK_SAMPLER_ADDRESS_MODE_REPEAT};
    VkSamplerAddressMode addressModeW { VK_SAMPLER_ADDRESS_MODE_REPEAT};

    const char* name { nullptr};
};

struct Texture
{
    VkImage vkmage { VK_NULL_HANDLE};
    VkImageView vkImageView { VK_NULL_HANDLE};
    VkFormat vkFormat { VK_FORMAT_UNDEFINED};
    VkImageLayout vkLayout { VK_IMAGE_LAYOUT_UNDEFINED};
    //VMaAllocation allocation { VK_NULL_HANDLE};

    uint16_t width { 1};
    uint16_t height { 1};
    uint16_t depth { 1};
    uint8_t mipmaps { 1};
    uint8_t flags { 0};

    TextureHandle handle;
    TextureType::Enum type { TextureType::Texture2D};

    Sampler* sampler {nullptr};
    const char* name {nullptr};
};

struct ShaderState
{
    VkPipelineShaderStageCreateInfo vkStageInfo[kMaxShaderStages];
    const char* name {nullptr};

    uint32_t activeShadersCount {0};
    bool isGraphicsPipeline {false};
};

struct DescriptorBinding
{
    VkDescriptorType type {VK_DESCRIPTOR_TYPE_MAX_ENUM};
    uint16_t start {0};
    uint16_t count {0};
    uint16_t set {0};

    const char* name {nullptr};
};

struct DescriptorSetLayout
{
    VkDescriptorSetLayout vkDescriptorSetLayout { VK_NULL_HANDLE};
    VkDescriptorSetLayoutBinding* vkBinding { nullptr};
    DescriptorBinding* bindings { nullptr};
    uint16_t bindingsCount {0};
    uint16_t setIndex {0};

    DescriptorSetLayoutHandle handle;
};

struct DescriptorSet
{
    VkDescriptorSet vkDescriptorSet { VK_NULL_HANDLE};

    ResourceHandle* resources { nullptr};
    SamplerHandle* samplers { nullptr};
    uint16_t* bindings { nullptr};
    
    const DescriptorSetLayout* layout { nullptr};
    uint32_t resourcesCount {0};
};


struct Pipeline
{
    VkPipeline vkPipeline { VK_NULL_HANDLE};
    VkPipelineLayout vkPipelineLayout { VK_NULL_HANDLE};

    VkPipelineBindPoint vkBindPoint;

    ShaderStateHandle shaderState;

    const DescriptorSetLayout* descriptorSetLayouts[kMaxDescriptorSetLayouts];
    DescriptorSetLayoutHandle descriptorSetLayoutHandles[kMaxDescriptorSetLayouts];
    uint32_t activeLayoutsCount {0};

    DepthStencilCreation depthStencil;
    BlendStateCreation blendState;
    RasterizationCreation rasterization;

    PipelineHandle handle;
    bool isGraphicsPipeline {true};
};

struct RenderPass
{
    VkRenderPass vkRenderPass { VK_NULL_HANDLE};
    VkFramebuffer vkFrameBuffer { VK_NULL_HANDLE};

    RenderPassOutput output;
    
    TextureHandle outputTextures[kMaxImageOutputs];
    TextureHandle outputDepthStencilTexture;

    RenderPassType::Enum type;

    float scaleX {1.f};
    float scaleY {1.f};
    uint16_t width { 0};
    uint16_t height { 0};
    uint16_t dispatchX { 0};
    uint16_t dispatchY { 0};
    uint16_t dispatchZ { 0};

    uint8_t resize { 0};
    uint8_t renderTargetCount { 0};

    const char* name { nullptr};
};

// Enum translations, Use tables or switches depending on the case
static std::string toCompilerExtension(VkShaderStageFlagBits value)
{
    switch (value)
    {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return "vert";
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return "frag";
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return "comp";
        default:
            break;
    }
    return "";
}

static std::string toStageDefines(VkShaderStageFlagBits value)
{
    switch (value)
    {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return "VERTEX";
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return "FRAGMENT";
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return "COMPUTE";
        default:
            break;
    }

    return "";
}

static VkImageType toVKImageType(TextureType::Enum type)
{
    static VkImageType sVKTarget[TextureType::Count] = {
        VK_IMAGE_TYPE_1D,
        VK_IMAGE_TYPE_2D,
        VK_IMAGE_TYPE_3D,
        VK_IMAGE_TYPE_1D,
        VK_IMAGE_TYPE_2D,
        VK_IMAGE_TYPE_3D,};
    return sVKTarget[type];
}

static VkImageViewType toVKImageViewType(TextureType::Enum type)
{
    static VkImageViewType sVKData[] = {
        VK_IMAGE_VIEW_TYPE_1D,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_VIEW_TYPE_3D,
        VK_IMAGE_VIEW_TYPE_1D_ARRAY,
        VK_IMAGE_VIEW_TYPE_2D_ARRAY,
        VK_IMAGE_VIEW_TYPE_CUBE_ARRAY};
    return sVKData[type];
}

static VkFormat toVKVertexFormat(VertexComponentFormat::Enum value)
{
    static VkFormat sVKVertexFormats[VertexComponentFormat::Count] = {
        VK_FORMAT_R32_SFLOAT,
        VK_FORMAT_R32G32_SFLOAT,
        VK_FORMAT_R32G32B32_SFLOAT,
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_FORMAT_R8_SINT,
        VK_FORMAT_R8G8B8A8_SNORM,
        VK_FORMAT_R8_UINT,
        VK_FORMAT_R8G8B8A8_UINT,
        VK_FORMAT_R16G16_SINT,
        VK_FORMAT_R16G16_SNORM,
        VK_FORMAT_R16G16B16A16_SINT,
        VK_FORMAT_R16G16B16A16_SNORM,
        VK_FORMAT_R32_UINT,
        VK_FORMAT_R32G32B32A32_UINT
    };
    return sVKVertexFormats[value];
}

static VkPipelineStageFlags toVKPipelineStage(PipelineStage::Enum value)
{
    static VkPipelineStageFlags sVKPipelineStages[] = {
        VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
        VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT
    };
    return sVKPipelineStages[value];
}

static VkAccessFlags toVKAccessFlags(ResourceState state)
{
    VkAccessFlags ret = 0;
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::COPY_SOURCE))
    {
        ret |= VK_ACCESS_TRANSFER_READ_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::COPY_DEST))
    {
        ret |= VK_ACCESS_TRANSFER_WRITE_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::VERTEX_AND_CONSTANT_BUFFER))
    {
        ret |= VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::INDEX_BUFFER))
    {
        ret |= VK_ACCESS_INDEX_READ_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::UNORDERED_ACCESS))
    {
        ret |= VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::INDIRECT_ARGUMENT))
    {
        ret |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::RENDER_TARGET))
    {
        ret |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::DEPTH_WRITE))
    {
        ret |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::SHADER_RESOURCE))
    {
        ret |= VK_ACCESS_SHADER_READ_BIT;
    }
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::PRESENT))
    {
        ret |= VK_ACCESS_MEMORY_READ_BIT;
    }
#ifdef ENABLE_RAYTRACING
    if(static_cast<uint16_t>(state) & static_cast<uint16_t>(ResourceState::RAYTRACING_ACCELERATION_STRUCTURE))
    {
        ret |= VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV | VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV;
    }
#endif
    return ret;
}

static VkImageLayout toVkImageLayout(ResourceState usage)
{
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::COPY_SOURCE))
    {
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::COPY_DEST))
    {
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::RENDER_TARGET))
    {
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::DEPTH_WRITE))
    {
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::DEPTH_READ))
    {
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::UNORDERED_ACCESS))
    {
        return VK_IMAGE_LAYOUT_GENERAL;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::SHADER_RESOURCE))
    {
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::PRESENT))
    {
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    if(static_cast<uint16_t>(usage) & static_cast<uint16_t>(ResourceState::COMMON))
    {
        return VK_IMAGE_LAYOUT_GENERAL;
    }

    return VK_IMAGE_LAYOUT_UNDEFINED;
}

static VkPipelineStageFlags determinePipelineStageFlags(VkAccessFlags accessFlags, QueueType::Enum queueType)
{
    VkPipelineStageFlags flags {0};
    
    if(queueType == QueueType::Graphics)
    {
        if( (accessFlags & (VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT)) != 0)
        {
            flags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
        }

        if( (accessFlags & ( VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT)) != 0)
        {
            flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        }

        if( (accessFlags & VK_ACCESS_INPUT_ATTACHMENT_READ_BIT) != 0)
        {
            flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        if( (accessFlags & (VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)) != 0)
        {
            flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }

        if( (accessFlags & (VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)) != 0)
        {
            flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            flags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        }
    }
    else if(queueType == QueueType::Compute)
    {
        if( (accessFlags & ( VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT)) != 0 ||
            (accessFlags & ( VK_ACCESS_INPUT_ATTACHMENT_READ_BIT)) != 0 ||
            (accessFlags & ( VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)) != 0 ||
            (accessFlags & ( VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)) != 0)
        {
            return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        }
        if( (accessFlags & (VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT)) != 0)
        {
            flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        }
    }
    else if(queueType == QueueType::Transfer)
    {
        return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    }

    // Compatible with both graphics and compute queues
    if( (accessFlags & VK_ACCESS_INDIRECT_COMMAND_READ_BIT) != 0)
    {
        flags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
    }

    if( (accessFlags & (VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT)) != 0)
    {
        flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
    }

    if( (accessFlags & (VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT)) != 0)
    {
        flags |= VK_PIPELINE_STAGE_HOST_BIT;
    }

    if(flags == 0)
    {
        flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    }
    
    return flags;
}

}
