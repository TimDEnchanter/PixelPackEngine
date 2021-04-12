#pragma once

#include "LayerInterface.h"

namespace PixelPack
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(std::shared_ptr<LayerInterface> sptr_layer);
		void PushOverlay(std::shared_ptr<LayerInterface> sptr_overlay);
		void PopLayer(std::shared_ptr<LayerInterface> sptr_layer);
		void PopOverlay(std::shared_ptr<LayerInterface> sptr_overlay);

		std::vector<std::shared_ptr<LayerInterface>>::iterator begin();
		std::vector<std::shared_ptr<LayerInterface>>::iterator end();
		std::vector<std::shared_ptr<LayerInterface>>::reverse_iterator rbegin();
		std::vector<std::shared_ptr<LayerInterface>>::reverse_iterator rend();

		std::vector<std::shared_ptr<LayerInterface>>::const_iterator begin() const;
		std::vector<std::shared_ptr<LayerInterface>>::const_iterator end() const;
		std::vector<std::shared_ptr<LayerInterface>>::const_reverse_iterator rbegin() const;
		std::vector<std::shared_ptr<LayerInterface>>::const_reverse_iterator rend() const;

	private:
		std::vector<std::shared_ptr<LayerInterface>> Layers;
		unsigned int LayerInsertIndex = 0;
	};
}

