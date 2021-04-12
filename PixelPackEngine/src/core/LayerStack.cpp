#include "Precompile.h"
#include "LayerStack.h"

namespace PixelPack
{
	LayerStack::~LayerStack()
	{
		for (std::shared_ptr<LayerInterface> sptr_layer : Layers)
		{
			sptr_layer->Detach();
		}
	}

	void LayerStack::PushLayer(std::shared_ptr<LayerInterface> sptr_layer)
	{
		Layers.emplace(Layers.begin() + LayerInsertIndex, sptr_layer);
		LayerInsertIndex++;
		sptr_layer->Attatch();
	}

	void LayerStack::PushOverlay(std::shared_ptr<LayerInterface> sptr_overlay)
	{
		Layers.emplace_back(sptr_overlay);
		sptr_overlay->Attatch();
	}

	void LayerStack::PopLayer(std::shared_ptr<LayerInterface> sptr_layer)
	{
		auto iter = std::find(Layers.begin(), Layers.begin() + LayerInsertIndex, sptr_layer);
		if (iter != Layers.begin() + LayerInsertIndex)
		{
			sptr_layer->Detach();
			Layers.erase(iter);
			LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(std::shared_ptr<LayerInterface> sptr_overlay)
	{
		auto iter = std::find(Layers.begin() + LayerInsertIndex, Layers.end(), sptr_overlay);
		if (iter != Layers.end())
		{
			sptr_overlay->Detach();
			Layers.erase(iter);
		}
	}

	std::vector<std::shared_ptr<LayerInterface>>::iterator LayerStack::begin()
	{
		return Layers.begin();
	}

	std::vector<std::shared_ptr<LayerInterface>>::iterator LayerStack::end()
	{
		return Layers.end();
	}

	std::vector<std::shared_ptr<LayerInterface>>::reverse_iterator LayerStack::rbegin()
	{
		return Layers.rbegin();
	}

	std::vector<std::shared_ptr<LayerInterface>>::reverse_iterator LayerStack::rend()
	{
		return Layers.rend();
	}

	std::vector<std::shared_ptr<LayerInterface>>::const_iterator LayerStack::begin() const
	{
		return Layers.begin();
	}

	std::vector<std::shared_ptr<LayerInterface>>::const_iterator LayerStack::end() const
	{
		return Layers.end();
	}

	std::vector<std::shared_ptr<LayerInterface>>::const_reverse_iterator LayerStack::rbegin() const
	{
		return Layers.rbegin();
	}

	std::vector<std::shared_ptr<LayerInterface>>::const_reverse_iterator LayerStack::rend() const
	{
		return Layers.rend();
	}
}