#include "pch.h"
#include "ScreenCapture.h"
#include <wil/resource.h>

namespace winrt
{
    using namespace Windows;

    namespace Windows
    {
        using namespace Foundation;

        namespace Foundation
        {
            using namespace Numerics;
        }

        using namespace Graphics;

        namespace Graphics
        {
            using namespace Capture;
            using namespace DirectX;

            namespace DirectX
            {
                using namespace Direct3D11;
            }
        }

        using namespace System;
        using namespace UI;

        namespace UI
        {
            using namespace Composition;
        }
    }
}



std::future<winrt::com_ptr<ID3D11Texture2D>> ScreenCapture::TakeAsync(
    winrt::IDirect3DDevice const& device,
    winrt::GraphicsCaptureItem const& item,
    winrt::DirectXPixelFormat const& pixelFormat)
{
    winrt::apartment_context context;
    auto d3dDevice = GetDXGIInterfaceFromObject<ID3D11Device>(device);
    // com_ptr<::IInspectable> d3d_device;
    // impl::abi_t<IInspectable>** a = d3d_device.put();
    // IInspectable** b = a;
    // impl::abi<::IInspectable>::type** b = a;
    // impl::abi_t<Windows::Foundation::IUnknown>** c = b;
    winrt::com_ptr<ID3D11DeviceContext> d3dContext;
    d3dDevice->GetImmediateContext(d3dContext.put());

    // Creating our frame pool with CreateFreeThreaded means that we 
    // will be called back from the frame pool's internal worker thread
    // instead of the thread we are currently on. It also disables the
    // DispatcherQueue requirement.
    auto framePool = winrt::Direct3D11CaptureFramePool::CreateFreeThreaded(
        device,
        pixelFormat,
        1,
        item.Size());
    auto session = framePool.CreateCaptureSession(item);

    wil::shared_event captureEvent(wil::EventOptions::ManualReset);
    winrt::Direct3D11CaptureFrame frame{nullptr};
    framePool.FrameArrived([&frame, captureEvent](auto& framePool, auto&)
    {
        frame = framePool.TryGetNextFrame();

        // Complete the operation
        captureEvent.SetEvent();
    });

    session.StartCapture();
    co_await winrt::resume_on_signal(captureEvent.get());
    co_await context;

    // End the capture
    session.Close();
    framePool.Close();

    auto texture = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
    auto result = util::CopyD3DTexture(d3dDevice, texture, true);

    co_return result;
}
