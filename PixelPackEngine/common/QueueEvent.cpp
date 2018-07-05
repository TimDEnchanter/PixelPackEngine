#include "QueueEvent.h"

// COPY CONSTRUCTOR
pxpk::QueueEvent::QueueEvent(const pxpk::QueueEvent &input)
{
	type = input.type;
	ID = input.ID;
	if (input.payload == nullptr)
	{
		payloadSize = 0;
		payload = nullptr;
	}
	else
	{
		payloadSize = input.payloadSize;
		payload = new std::uint8_t[input.payloadSize];
		std::copy(input.payload, input.payload + input.payloadSize, payload);
	}
}

//COPY ASSIGNMENT
pxpk::QueueEvent & pxpk::QueueEvent::operator=(const pxpk::QueueEvent &input)
{
	type = input.type;
	ID = input.ID;
	if (input.payload == nullptr)
	{
		payloadSize = 0;
		payload = nullptr;
	}
	else
	{
		payloadSize = input.payloadSize;
		payload = new std::uint8_t[input.payloadSize];
		std::copy(input.payload, input.payload + input.payloadSize, payload);
	}

	return *this;
}

//MOVE CONSTRUCTOR
pxpk::QueueEvent::QueueEvent(pxpk::QueueEvent && input)
{
	type = std::move(input.type);
	ID = std::move(input.ID);
	if (input.payload == nullptr)
	{
		payloadSize = 0;
		payload = nullptr;
	}
	else
	{
		payloadSize = std::move(input.payloadSize);
		payload = std::move(input.payload);
	}
}

//MOVE ASSIGNMENT
pxpk::QueueEvent & pxpk::QueueEvent::operator=(pxpk::QueueEvent && input)
{
	type = std::move(input.type);
	ID = std::move(input.ID);
	if (input.payload == nullptr)
	{
		payloadSize = 0;
		payload = nullptr;
	}
	else
	{
		payloadSize = std::move(input.payloadSize);
		payload = std::move(input.payload);
	}

	return *this;
}

//DESTRUCTOR
pxpk::QueueEvent::~QueueEvent()
{
	if (payload != nullptr) delete [] payload;
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
	payload = new std::uint8_t[payloadSize];
	std::copy(reinterpret_cast<uint8_t*>(data.data()), 
		reinterpret_cast<uint8_t*>(data.data()) + payloadSize, 
		payload);

	//payload = reinterpret_cast<uint8_t *>(&data[0]);
}

void pxpk::QueueEvent::writePayload(std::vector<GLuint>& data)
{
	payloadSize = sizeof(GLuint)*data.size();
	payload = new std::uint8_t[payloadSize];
	std::copy(reinterpret_cast<uint8_t*>(data.data()),
		reinterpret_cast<uint8_t*>(data.data()) + payloadSize,
		payload);
}

void pxpk::QueueEvent::writePayload(glm::vec3& data)
{
	payloadSize = sizeof(glm::vec3);
	payload = new std::uint8_t[payloadSize];
	std::copy(reinterpret_cast<uint8_t*>(&data),
		reinterpret_cast<uint8_t*>(&data) + payloadSize,
		payload);
}

void pxpk::QueueEvent::writePayload(glm::quat& data)
{
	payloadSize = sizeof(glm::quat);
	payload = new std::uint8_t[payloadSize];
	std::copy(reinterpret_cast<uint8_t*>(&data),
		reinterpret_cast<uint8_t*>(&data) + payloadSize,
		payload);
}

void pxpk::QueueEvent::writePayload(GLfloat& data)
{
	payloadSize = sizeof(GLfloat);
	payload = new std::uint8_t[payloadSize];
	std::copy(reinterpret_cast<uint8_t*>(&data),
		reinterpret_cast<uint8_t*>(&data) + payloadSize,
		payload);
}

void pxpk::QueueEvent::readPayload(std::vector<GLfloat>& dest)
{
	dest.resize(payloadSize / sizeof(GLfloat));
	std::copy(payload, payload + payloadSize, reinterpret_cast<uint8_t*>(dest.data()));
}

void pxpk::QueueEvent::readPayload(std::vector<GLuint>& dest)
{
	dest.resize(payloadSize / sizeof(GLfloat));
	std::copy(payload, payload + payloadSize, reinterpret_cast<uint8_t*>(dest.data()));
}

void pxpk::QueueEvent::readPayload(glm::vec3& dest)
{
	std::copy(payload, payload + payloadSize, reinterpret_cast<uint8_t*>(&dest));
}

void pxpk::QueueEvent::readPayload(glm::quat& dest)
{
	std::copy(payload, payload + payloadSize, reinterpret_cast<uint8_t*>(&dest));
}

void pxpk::QueueEvent::readPayload(GLfloat& dest)
{
	std::copy(payload, payload + payloadSize, reinterpret_cast<uint8_t*>(&dest));
}