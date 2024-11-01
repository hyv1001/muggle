#pragma once
#include <cstdint>

namespace muggle
{
namespace CullMode {
enum Enum : uint8_t
{
    None = 0, Front, Back, Count    
};

enum Mask : uint8_t
{
    None_Mask = 1 << 0,
    Front_Mask = 1 << 1,
    Back_Mask = 1 << 2,
    Count_Mask = 1 << 3
};

static const char* valueNames[] = { "None", "Front", "Back", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
    
}

namespace FillMode {
enum Enum : uint8_t
{
    Solid = 0, Wireframe, Point, Count
};

enum Mask : uint8_t
{
    Solid_Mask = 1 << 0,
    Wireframe_Mask = 1 << 1,
    Point_Mask = 1 << 2,
    Count_Mask = 1 << 3
};

static const char* valueNames[] = { "Solid", "Wireframe", "Point", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace ColorWriteEnabled {
enum Enum : uint8_t
{
    Red = 0, Green, Blue, Alpha, All, Count
};

enum Mask : uint8_t
{
    Red_Mask = 1 << 0,
    Green_Mask = 1 << 1,
    Blue_Mask = 1 << 2,
    Alpha_Mask = 1 << 3,
    All_Mask = Red_Mask | Green_Mask | Blue | Alpha_Mask,
};

static const char* valueNames[] = { "Red", "Green", "Blue", "Alpha", "All", "Count"};
static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "UnKnown";
}
}

namespace DepthWriteMask {
enum Enum : uint8_t
{
    Zero, All, Count
};

enum Mask : uint8_t
{
    Zero_Mask = 1 << 0,
    All_Mask = 1 << 1,
    Count_Mask = 1 << 2
};

static const char* valueNames[] = { "Zero", "All", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace FrontClockwise {

enum Enum : uint8_t
{
    False, True, Count
};

enum Mask : uint8_t
{
    False_Mask = 1 << 0,
    True_Mask = 1 << 1,
    Count_Mask = 1 << 2
};

static const char* valueNames[] = { "False", "True", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace StencilOperation{

enum Enum : uint8_t
{
    Keep, Zero, Replace, IncrementClamp, DecrementClamp, Invert, IncrementWrap, DecrementWrap, Count
};

enum Mask : uint16_t
{
    Keep_Mask = 1 << 0,
    Zero_Mask = 1 << 1,
    Replace_Mask = 1 << 2,
    IncrementClamp_Mask = 1 << 3,
    DecrementClamp_Mask = 1 << 4,
    Invert_Mask = 1 << 5,
    IncrementWrap_Mask = 1 << 6,
    DecrementWrap_Mask = 1 << 7,
    Count_Mask = 1 << 8
};

static const char* valueNames[] = { "Keep", "Zero", "Replace", "IncrementClamp", "DecrementClamp", "Invert", "IncrementWrap", "DecrementWrap", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}

}

namespace TopologyType {

enum Enum : uint8_t
{
    PointList, LineList, LineStrip, TriangleList, TriangleStrip, TriangleFan, Count
};

enum Mask : uint8_t
{
    PointList_Mask = 1 << 0,
    LineList_Mask = 1 << 1,
    LineStrip_Mask = 1 << 2,
    TriangleList_Mask = 1 << 3,
    TriangleStrip_Mask = 1 << 4,
    TriangleFan_Mask = 1 << 5,
    Count_Mask = 1 << 6
};

static const char* valueNames[] = { "PointList", "LineList", "LineStrip", "TriangleList", "TriangleStrip", "TriangleFan", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}

}

namespace ResourceUsageType {

enum Enum : uint8_t
{
    Immutable, Dynamic, Stream, Count
};

enum Mask : uint8_t
{
    Immutable_Mask = 1 << 0,
    Dynamic_Mask = 1 << 1,
    Stream_Mask = 1 << 2,
    Count_Mask = 1 << 3
};

static const char* valueNames[] = { "Immutable", "Dynamic", "Stream", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace IndexType {

enum Enum : uint8_t
{
    UInt16, UInt32, Count
};

enum Mask : uint8_t
{
    UInt16_Mask = 1 << 0,
    UInt32_Mask = 1 << 1,
    Count_Mask = 1 << 2
};

static const char* valueNames[] = { "UInt16", "UInt32", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace TextureType {
enum Enum : uint8_t
{
    Texture1D, Texture2D, Texture3D, TextureCube, Texture_1D_Array, Texture_2D_Array, Texture_Cube_Array, Count
};

enum Mask : uint8_t
{
    Texture1D_Mask = 1 << 0,
    Texture2D_Mask = 1 << 1,
    Texture3D_Mask = 1 << 2,
    TextureCube_Mask = 1 << 3,
    Texture_1D_Array_Mask = 1 << 4,
    Texture_2D_Array_Mask = 1 << 5,
    Texture_Cube_Array_Mask = 1 << 6,
    Count_Mask = 1 << 7
};

static const char* valueNames[] = { "Texture1D", "Texture2D", "Texture3D", "TextureCube",
    "Texture_1D_Array", "Texture_2D_Array", "Texture_Cube_Array", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace VertexComponentFormat
{
enum Enum : uint8_t
{
    Float, Float2, Float3, Float4, Mat4, Byte, Byte4N, UByte, UByte4N, Short2, Short2N, Short4, Short4N, UInt, UInt2, UInt4, Count
};

static const char* valueNames[] = { "Float", "Float2", "Float3", "Float4", "Mat4", "Byte", "Byte4N", "UByte",
    "UByte4N", "Short2", "Short2N", "Short4", "Short4N", "UInt", "UInt2", "UInt4", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace VertexInputRate
{

enum Enum : uint8_t
{
    Vertex, Instance, Count
};

enum Mask : uint8_t
{
    Vertex_Mask = 1 << 0,
    Instance_Mask = 1 << 1,
    Count_Mask = 1 << 2
};

static const char* valueNames[] = { "Vertex", "Instance", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace LogicOperation
{
enum Enum : uint8_t
{
    Clear, Set, Copy, CopyInverted, Noop, Invert, And, Nand, Or, Nor, Xor, Equiv, AndReverse, AndInverted, OrReverse, OrInverted, Count
};

enum Mask : uint32_t
{
    Clear_Mask = 1 << 0,
    Set_Mask = 1 << 1,
    Copy_Mask = 1 << 2,
    CopyInverted_Mask = 1 << 3,
    Noop_Mask = 1 << 4,
    Invert_Mask = 1 << 5,
    And_Mask = 1 << 6,
    Nand_Mask = 1 << 7,
    Or_Mask = 1 << 8,
    Nor_Mask = 1 << 9,
    Xor_Mask = 1 << 10,
    Equiv_Mask = 1 << 11,
    AndReverse_Mask = 1 << 12,
    AndInverted_Mask = 1 << 13,
    OrReverse_Mask = 1 << 14,
    OrInverted_Mask = 1 << 15,
    Count_Mask = 1 << 16
};

static const char* valueNames[] = { "Clear", "Set", "Copy", "CopyInverted", "Noop", "Invert", "And", "Nand", "Or",
    "Nor", "Xor", "Equiv", "AndReverse", "AndInverted", "OrReverse", "OrInverted", "Count"};


static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace QueueType
{
enum Enum : uint8_t
{
    Graphics, Compute, Transfer, Count
};

enum Mask : uint8_t
{
    Graphics_Mask = 1 << 0,
    Compute_Mask = 1 << 1,
    Transfer_Mask = 1 << 2,
    Count_Mask = 1 << 3
};

static const char* valueNames[] = { "Graphics", "Compute", "Transfer", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
}

namespace CommandType
{
enum Enum : uint8_t
{
    BindPipeline,
    BindResourceTable,
    BindVertexBuffer,
    BindIndexBuffer,
    BindResourceSet,
    Draw,
    DrawIndexed,
    DrawInstanced,
    DrawIndexedInstanced,
    Dispatch,
    CopyResource,
    SetScissor,
    SetViewport,
    Clear,
    ClearDepth,
    ClearStencil,
    BeginPass,
    EndPass,
    Count
};

static const char* valueNames[] = { "BindPipeline", "BindResourceTable", "BindVertexBuffer", "BindIndexBuffer",
    "BindResourceSet","Draw", "DrawIndexed", "DrawInstanced", "DrawIndexedInstanced", "Dispatch", "CopyResource",
    "SetScissor", "SetViewport", "Clear", "ClearDepth", "ClearStencil", "BeginPass", "EndPass", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";
}
} // namespace CommandType

enum class  DeviceExtensions : uint8_t
{
    DebugCallback = 1 << 0
};

namespace TextureFlags
{
enum Enum : uint8_t
{
    None = 0,
    RenderTarget,
    Compute,
    Count
};

enum Mask : uint8_t
{
    None_Mask = 1 << 0,
    RenderTarget_Mask = 1 << 1,
    Compute_Mask = 1 << 2,
    Count_Mask = 1 << 3
};

static const char* valueNames[] = { "None", "RenderTarget", "Compute", "Count"};

static const char* toString(Enum value)
{
    uint32_t index = static_cast<uint32_t>(value);
    return (index < static_cast<uint32_t>(Count)) ? valueNames[index] : "Unknown";

}
} // namespace TextureFlags

namespace PipelineStage
{
    enum Enum : uint8_t
    {
        DrawIndirect = 0,
        VertexInput = 1,
        VertexShader = 2,
        FragmentShader = 3,
        RenderTarget = 4,
        ComputeShader = 5,
        Transfer = 6
    };

    enum Mask : uint8_t
    {
        DrawIndirect_Mask = 1 << 0,
        VertexInput_Mask = 1 << 1,
        VertexShader_Mask = 1 << 2,
        FragmentShader_Mask = 1 << 3,
        RenderTarget_Mask = 1 << 4,
        ComputeShader_Mask = 1 << 5,
        Transfer_Mask = 1 << 6
    };
}

namespace RenderPassType
{
    enum Enum : uint8_t
    {
        Geometry, Swapchain, Compute
    };
}

namespace ResourceDeletionType
{
    enum Enum : uint8_t
    {
        Buffer, Texture, Pipeline, Sampler, DescriptorSetLayout, DescriptorSet, RenderPass, ShaderState, Count
    };
}

namespace PresentMode
{
    enum Enum : uint8_t
    {
        Immediate, VSync, VSyncFast, VSyncRelaxed, Count
    };
}

namespace RenderPassOperation
{
    enum Enum : uint8_t
    {
        DontCare, Load, Clear, Count
    };
}

enum class ResourceState : uint16_t
{
    UNDEFINED = 0,
    VERTEX_AND_CONSTANT_BUFFER = 0x1,
    INDEX_BUFFER = 0x2,
    RENDER_TARGET=0x4,
    UNORDERED_ACCESS = 0x8,
    DEPTH_WRITE = 0x10,
    DEPTH_READ = 0x20,
    NOW_PIXEL_SHADER_RESOURCE = 0x40,
    PIXEL_SHADER_RESOURCE = 0x80,
    SHADER_RESOURCE = 0x40 | 0x80,
    STREAM_OUT = 0x100,
    INDIRECT_ARGUMENT = 0x200,
    COPY_DEST = 0x400,
    COPY_SOURCE = 0x800,
    GENERIC_READ = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
    PRESENT = 0x1000,
    COMMON = 0x2000,
    RAYTRACING_ACCELERATION_STRUCTURE = 0x4000,
    SHADING_RATE_SOURCE = 0x8000
};

}