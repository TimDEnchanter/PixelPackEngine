#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H


/*
RenderObject

the phyical 3d object rendered in OpeGL. 
Usually part of another object
*/

class RenderObject
{
	private:
		bool isHidden;

	public:
		RenderObject();
		~RenderObject();
};

#endif // !RENDER_OBJECT_H