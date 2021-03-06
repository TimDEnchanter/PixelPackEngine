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
		//base settings
		RENDER_SHOW_STATS,
		//object
		RENDER_OBJ_ADD,
		RENDER_OBJ_REMOVE,
		RENDER_OBJ_CLEAR,
		RENDER_OBJ_SET_POS,
		RENDER_OBJ_SET_ORIENT,
		RENDER_OBJ_SET_ORIENT_EULER,
		RENDER_OBJ_SET_SCALE,
		RENDER_OBJ_TRANSLATE,
		RENDER_OBJ_ROTATE,
		RENDER_OBJ_ROTATE_EULER,
		RENDER_OBJ_LOOKAT,
		RENDER_OBJ_SET_AMBI,
		RENDER_OBJ_SET_DIFF,
		RENDER_OBJ_SET_SPEC,
		RENDER_OBJ_SET_SHINE,
		RENDER_OBJ_SET_MESH,
		RENDER_OBJ_SET_TEX,
		RENDER_OBJ_SET_SHADER,
		//camera
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
		RENDER_CAM_SET_ACTIVE,
		//light
		RENDER_LIGHT_REMOVE,
		RENDER_LIGHT_CLEAR,
		RENDER_LIGHT_SET_POS,
		RENDER_LIGHT_SET_ORIENT,
		RENDER_LIGHT_SET_ORIENT_EULER,
		RENDER_LIGHT_TRANSLATE,
		RENDER_LIGHT_ROTATE,
		RENDER_LIGHT_ROTATE_EULER,
		RENDER_LIGHT_LOOKAT,
		RENDER_LIGHT_SET_SHADER,
		RENDER_LIGHT_SET_AMBI,
		RENDER_LIGHT_SET_DIFF,
		RENDER_LIGHT_SET_SPEC,
		//light: point light
		RENDER_LIGHT_POINT_ADD,
		RENDER_LIGHT_POINT_SET_CONST,
		RENDER_LIGHT_POINT_SET_LIN,
		RENDER_LIGHT_POINT_SET_QUAD,
		//light: direction light
		RENDER_LIGHT_DIR_ADD,
		RENDER_LIGHT_DIR_SET_DIR,
		//light: spot light
		RENDER_LIGHT_SPOT_ADD,
		RENDER_LIGHT_SPOT_SET_DIR,
		RENDER_LIGHT_SPOT_SET_IN_CUTOFF,
		RENDER_LIGHT_SPOT_SET_OUT_CUTOFF,
		RENDER_LIGHT_SPOT_SET_CONST,
		RENDER_LIGHT_SPOT_SET_LIN,
		RENDER_LIGHT_SPOT_SET_QUAD,
		//screenspace text
		RENDER_SCREEN_SET_POS,
		RENDER_SCREEN_SET_SIZE,
		RENDER_SCREEN_SET_SHADER,
		//text
		RENDER_SCREEN_TEXT_ADD,
		RENDER_SCREEN_TEXT_SET_FONT,
		RENDER_SCREEN_TEXT_SET_DATA,
		RENDER_SCREEN_TEXT_SET_COLOR
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

			void showStats(bool);

			void objAdd(unsigned short);
			void objRemove(unsigned short);
			void objClear();
			void objSetAmbient(unsigned short, glm::vec3);
			void objSetDiffuse(unsigned short, glm::vec3);
			void objSetSpecular(unsigned short, glm::vec3);
			void objSetShininess(unsigned short, GLfloat);
			void objSetMesh(unsigned short, std::string);
			void objSetTex(unsigned short, std::string);
			void objSetShader(unsigned short, std::string, std::string);
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

			void lightRemove(unsigned short);
			void lightClear();
			void lightSetPos(unsigned short, glm::vec3);
			void lightSetOrient(unsigned short, glm::quat);
			void lightSetOrientEuler(unsigned short, glm::vec3);
			void lightTrans(unsigned short, glm::vec3);
			void lightRot(unsigned short, glm::quat);
			void lightRotEuler(unsigned short, glm::vec3);
			void lightLookat(unsigned short, glm::vec3);
			void lightSetShader(unsigned short, std::string, std::string);
			void lightSetAmbient(unsigned short, glm::vec3);
			void lightSetDiffuse(unsigned short, glm::vec3);
			void lightSetSpecular(unsigned short, glm::vec3);

			void lightPointAdd(unsigned short);
			void lightPointSetConstant(unsigned short, GLfloat);
			void lightPointSetLinear(unsigned short, GLfloat);
			void lightPointSetQuadratic(unsigned short, GLfloat);

			void lightDirAdd(unsigned short);
			void lightDirSetDir(unsigned short, glm::vec3);

			void lightSpotAdd(unsigned short);
			void lightSpotSetDir(unsigned short, glm::vec3);
			void lightSpotSetInCutoff(unsigned short, GLfloat);
			void lightSpotSetOutCutoff(unsigned short, GLfloat);
			void lightSpotSetConstant(unsigned short, GLfloat);
			void lightSpotSetLinear(unsigned short, GLfloat);
			void lightSpotSetQuadratic(unsigned short, GLfloat);

			void screenSetPosition(unsigned short, glm::vec2);
			void screenSetSize(unsigned short, glm::vec2);
			void screenSetShader(unsigned short, std::string, std::string);

			void screenTextAdd(unsigned short);
			void screenTextSetData(unsigned short, std::string);
			void screenTextSetColor(unsigned short, glm::vec3);
			void screenTextSetFont(unsigned short, std::string);
	};
}

#endif  //  !RENDER_QUEUE_H