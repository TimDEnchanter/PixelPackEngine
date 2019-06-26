#ifndef QUEUE_EVENT_H
#define QUEUE_EVENT_H

#pragma warning( disable : 4996 )

#include <cstdint>
#include <vector>
#include <algorithm>
#include <memory>

#include "../../dependencies/freeGLUT/include/GL/freeglut.h"
#include "../../dependencies/glm/vec3.hpp"
#include "../../dependencies/glm/gtc/quaternion.hpp"

namespace pxpk
{
	class QueueEvent
	{
		private:
			std::uint8_t type;
			std::uint16_t ID;
			size_t payloadSize;
			std::uint8_t *payload;

		public:
			QueueEvent(std::uint8_t inType, std::uint16_t inID) :
				type(inType),
				ID(inID),
				payloadSize(0),
				payload(nullptr)
			{};

			QueueEvent(const pxpk::QueueEvent&);
			pxpk::QueueEvent& operator=(const pxpk::QueueEvent&);

			QueueEvent(pxpk::QueueEvent&&);
			pxpk::QueueEvent& operator=(pxpk::QueueEvent&&);

			~QueueEvent();

			std::uint8_t getType();
			std::uint16_t getID();
			size_t getPayloadSize();

			void writePayload(std::vector<GLfloat>&);
			void writePayload(std::vector<GLuint>&);
			void writePayload(glm::vec2&);
			void writePayload(glm::vec3&);
			void writePayload(glm::quat&);
			void writePayload(GLfloat&);
			void writePayload(std::vector<char>&);
			void writePayload(bool&);

			void readPayload(std::vector<GLfloat>&);
			void readPayload(std::vector<GLuint>&);
			void readPayload(glm::vec2&);
			void readPayload(glm::vec3&);
			void readPayload(glm::quat&);
			void readPayload(GLfloat&);
			void readPayload(std::vector<char>&);
			void readPayload(bool&);
	};
}

#endif  //  !QUEUE_EVENT_H