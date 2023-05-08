#pragma once

class ScreenCapture
{
public:
    static std::future<winrt::com_ptr<ID3D11Texture2D>> TakeAsync(
        winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device,
        winrt::Windows::Graphics::Capture::GraphicsCaptureItem const& item,
        winrt::Windows::Graphics::DirectX::DirectXPixelFormat const& pixelFormat =
            winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized);
    ScreenCapture() = delete;
};
