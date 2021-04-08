#include <PixelPackEngine.h>

class StudioApp : public PixelPack::EngineApp
{
public:
	StudioApp()
	{

	}

	~StudioApp()
	{

	}
};

PixelPack::EngineApp* PixelPack::CreateEngine()
{
	return new StudioApp();
}