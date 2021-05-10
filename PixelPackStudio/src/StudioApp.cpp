#include "Precompile.h"

#include <PixelPackEngine.h>
#include <core\EntryPoint.h>

#include "TestLayer.h"

class StudioApp : public PixelPack::EngineApp
{
public:
	StudioApp()
	{
		PushLayer(std::shared_ptr<TestLayer>(new TestLayer()));
		PushOverlay(std::shared_ptr<PixelPack::ImGuiLayer>(new PixelPack::ImGuiLayer()));
	}

	~StudioApp()
	{

	}
};

PixelPack::EngineApp* PixelPack::CreateEngine()
{
	return new StudioApp();
}