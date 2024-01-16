#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

enum class EasingType
{
	Linear,
	sineEaseIn,
	sineEaseOut,
	sineEaseInOut
};

struct PositionKeyFrame
{
	PositionKeyFrame(const glm::vec3& position, double time, EasingType type = EasingType::Linear)
		: m_Position(position)
		, m_Time(time)
		, n_EaseType(type)
	{ }
	glm::vec3 m_Position;
	EasingType n_EaseType;
	double m_Time;
};

struct ScaleKeyFrame
{
	ScaleKeyFrame(const glm::vec3& scale, double time, EasingType type = EasingType::Linear)
		: m_Scale(scale)
		, m_Time(time)
		, n_EaseType(type)
	{ }
	glm::vec3 m_Scale;
	EasingType n_EaseType;
	double m_Time;
};

struct RotationKeyFrame
{
	RotationKeyFrame(const glm::quat& rotation, double time, EasingType type = EasingType::Linear)
		: m_Rotation(rotation)
		, m_Time(time)
		, n_EaseType(type)
	{ }
	glm::quat m_Rotation;
	EasingType n_EaseType;
	double m_Time;
};

struct EventKeyFrame
{
	double m_Time;
};

struct Animation
{
	std::vector<PositionKeyFrame> m_PositionKeyFrames;
	std::vector<ScaleKeyFrame> m_ScaleKeyFrames;
	std::vector<RotationKeyFrame> m_RotationKeyFrames;
	double m_Time;
};