#include "QueueEvent.h"

pxpk::QueueEvent::QueueEvent(const pxpk::QueueEvent &input)
{
	type = input.type;
	ID = input.ID;
	payloadSize = input.payloadSize;
	payload = input.payload;
}

pxpk::QueueEvent::~QueueEvent()
{
}

std::uint8_t pxpk::QueueEvent::getType()
{
	return type;
}

std::uint16_t pxpk::QueueEvent::getID()
{
	return ID;
}

size_t pxpk::QueueEvent::getPayloadSize()
{
	return payloadSize;
}


void pxpk::QueueEvent::writePayload(std::vector<GLfloat>& data)
{
	payloadSize = sizeof(GLfloat)*data.size();
	std::copy(reinterpret_cast<uint8_t*>(data.data()), 
		reinterpret_cast<uint8_t*>(data.data()) + payloadSize, 
		payload.get());
}

void pxpk::QueueEvent::writePayload(std::vector<GLuint>& data)
{
	payloadSize = sizeof(GLuint)*data.size();
	std::copy(reinterpret_cast<uint8_t*>(data.data()),
		reinterpret_cast<uint8_t*>(data.data()) + payloadSize,
		payload.get());
}

void pxpk::QueueEvent::writePayload(glm::vec3& data)
{
	payloadSize = sizeof(glm::vec3);
	std::copy(reinterpret_cast<uint8_t*>(&data),
		reinterpret_cast<uint8_t*>(&data) + payloadSize,
		payload.get());
}

void pxpk::QueueEvent::writePayload(glm::quat& data)
{
	payloadSize = sizeof(glm::quat);
	std::copy(reinterpret_cast<uint8_t*>(&data),
		reinterpret_cast<uint8_t*>(&data) + payloadSize,
		payload.get());
}

void pxpk::QueueEvent::writePayload(GLfloat& data)
{
	payloadSize = sizeof(GLfloat);
	std::copy(reinterpret_cast<uint8_t*>(&data),
		reinterpret_cast<uint8_t*>(&data) + payloadSize,
		payload.get());
}

void pxpk::QueueEvent::readPayload(std::vector<GLfloat>& dest)
{
	dest.resize(payloadSize / sizeof(GLfloat));
	std::copy(payload.get(), payload.get() + payloadSize, reinterpret_cast<uint8_t*>(dest.data()));
}

void pxpk::QueueEvent::readPayload(std::vector<GLuint>& dest)
{
	dest.resize(payloadSize / sizeof(GLfloat));
	std::copy(payload.get(), payload.get() + payloadSize, reinterpret_cast<uint8_t*>(dest.data()));
}

void pxpk::QueueEvent::readPayload(glm::vec3& dest)
{
	std::copy(payload.get(), payload.get() + payloadSize, reinterpret_cast<uint8_t*>(&dest));
}

void pxpk::QueueEvent::readPayload(glm::quat& dest)
{
	std::copy(payload.get(), payload.get() + payloadSize, reinterpret_cast<uint8_t*>(&dest));
}

void pxpk::QueueEvent::readPayload(GLfloat& dest)
{
	std::copy(payload.get(), payload.get() + payloadSize, reinterpret_cast<uint8_t*>(&dest));
}