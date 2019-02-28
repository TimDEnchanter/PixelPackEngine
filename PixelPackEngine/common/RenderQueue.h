#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include "DoubleBufferQueue.h"

namespace pxpk
{
	extern std::mutex RenderQ_Mutex;
	extern std::condition_variable RenderQ_Write_CV;
	extern std::condition_variable RenderQ_Read_CV;
	extern bool isRenderWriterReady;
	//extern bool isRenderReaderReady;

	enum RenderType : std::uint8_t
	{
		RENDER_OBJ_ADD,
		RENDER_OBJ_REMOVE,
		RENDER_OBJ_CLEAR,
		RENDER_OBJ_SET_COLOR,
		RENDER_OBJ_SET_MESH,
		RENDER_OBJ_SET_TEX,
		RENDER_OBJ_SET_POS,
		RENDER_OBJ_SET_ORIENT,
		RENDER_OBJ_SET_ORIENT_EULER,
		RENDER_OBJ_SET_SCALE,
		RENDER_OBJ_TRANSLATE,
		RENDER_OBJ_ROTATE,
		RENDER_OBJ_ROTATE_EULER,
		RENDER_OBJ_LOOKAT,
		RENDER_CAM_ADD,
		RENDER_CAM_REMOVE,
		RENDER_CAM_CLEAR,
		RENDER_CAM_SET_POS,
		RENDER_CAM_SET_ORIENT,
		RENDER_CAM_SET_ORIENT_EULER,
		RENDER_CAM_TRANSLATE,
		RENDER_CAM_ROTATE,
		RENDER_CAM_ROTATE_EULER,
		RENDER_CAM_LOOKAT,
		RENDER_CAM_SET_FOV,
		RENDER_CAM_SET_NEAR,
		RENDER_CAM_SET_FAR,
		RENDER_CAM_SET_ACTIVE
	};

	class RenderQueue : public pxpk::DoubleBuffferQueue
	{
		private:
			RenderQueue();

		public:
			static RenderQueue& getInstance()
			{
				static RenderQueue instance;
				return instance;
			};

			RenderQueue(RenderQueue const&) = delete;
			void operator=(RenderQueue const&) = delete;

			~RenderQueue();

			void objAdd(unsigned short);
			void objRemove(unsigned short);
			void objClear();
			void objSetColor(unsigned short, glm::vec3);
			void objSetMesh(unsigned short, std::string);
			void objSetTex(unsigned short, std::string);
			void objSetPos(unsigned short, glm::vec3);
			void objSetOrient(unsigned short, glm::quat);
			void objSetOrientEuler(unsigned short, glm::vec3);
			void objSetScale(unsigned short, glm::vec3);
			void objTrans(unsigned short, glm::vec3);
			void objRot(unsigned short, glm::quat);
			void objRotEuler(unsigned short, glm::vec3);
			void objLookat(unsigned short, glm::vec3);

			void camAdd(unsigned short);
			void camRemove(unsigned short);
			void camClear();
			void camSetPos(unsigned short, glm::vec3);
			void camSetOrient(unsigned short, glm::quat);
			void camSetOrientEuler(unsigned short, glm::vec3);
			void camTrans(unsigned short, glm::vec3);
			void camRot(unsigned short, glm::quat);
			void camRotEuler(unsigned short, glm::vec3);
			void camLookat(unsigned short, glm::vec3);
			void camSetFov(unsigned short, GLfloat);
			void camSetNear(unsigned short, GLfloat);
			void camSetFar(unsigned short, GLfloat);
			void camSetActive(unsigned short);
	};
}

#endif  //  !RENDER_QUEUE_H