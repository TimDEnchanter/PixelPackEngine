#pragma once

#include <PixelPackEngine.h>
#include <entt\entt.hpp>

class TestLayer : public PixelPack::LayerInterface
{
public:
	TestLayer(bool enabled = true);
	virtual ~TestLayer();

	virtual void Attatch() override;
	virtual void Detach() override;
	virtual void Update() override;

	virtual void SetEnabled(bool enabled) override;
	virtual bool IsEnabled() const override;

	virtual bool OnEvent(entt::any event) override;

private:
	bool Enabled;
	std::shared_ptr<entt::dispatcher> sptr_Dispatcher;

private:
	void OnKeyEvent(const PixelPack::KeyEvent& event);
};