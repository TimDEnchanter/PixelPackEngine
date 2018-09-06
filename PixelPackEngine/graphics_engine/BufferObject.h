#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "ObjectResource.h"

namespace pxpk
{
	class BufferObject : public ObjectResource
	{
	private:

	public:
		template <typename T> virtual void createResource(std::vector<T>);
		virtual void bindResource();
		virtual void deleteResource();

		BufferObject(std::string);
		virtual ~BufferObject();
	};
}

#endif //BUFFER_OBJECT_H
