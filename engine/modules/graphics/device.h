#pragma once

namespace muggle
{
    struct DeviceCreation
    {
        void* window {nullptr};
        int width {0};
        int height {0};
    };

    class GPUDevice
    {
    public:
        void init(const DeviceCreation& creation);
        void destroy();
        
        
    };


} // namespace muggle