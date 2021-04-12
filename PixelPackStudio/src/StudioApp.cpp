#include <PixelPackEngine.h>
#include <core\EntryPoint.h>

#include "TestLayer.h"

class StudioApp : public PixelPack::EngineApp
{
public:
	StudioApp()
	{
		PushLayer(std::shared_ptr<TestLayer>(new TestLayer()));
	}

	~StudioApp()
	{

	}
};

PixelPack::EngineApp* PixelPack::CreateEngine()
{
	return new StudioApp();
}