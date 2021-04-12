#pragma once

#include <entt\entt.hpp>

#include "core\ApiCore.h"

namespace PixelPack
{
	// Interface representing a layer
	// Implemented by client classes
	class PXPK_API LayerInterface
	{
	public:
		virtual ~LayerInterface() = default;

		virtual void Attatch() = 0;
		virtual void Detach() = 0;
		virtual void Update() = 0;

		virtual void SetEnabled(bool enabled) = 0;
		virtual bool IsEnabled() const = 0;

		virtual bool OnEvent(entt::any event) = 0;
	};
}
