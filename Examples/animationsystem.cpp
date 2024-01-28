#include "animationsystem.h"

#include <glm/gtx/easing.hpp>

void AnimationSystem::Update(const std::vector<GameObject*>& gameObjects) const
{
	for (GameObject* gameObject : gameObjects)
	{
		if (gameObject->m_Animation != nullptr)
		{
			Animation* animation = gameObject->m_Animation;

			double time = animation->m_Time;


			/// POSITION
			{


				// If there is only 1 key frame, use that one.
				// If we are equal to or greater than the time of the last key frame, use the last keyframe
				if (animation->m_PositionKeyFrames.size() == 1)
				{
					gameObject->m_Position = animation->m_PositionKeyFrames[0].m_Position;
				}
				else
				{
					// Find our position keyframes (Start and End)
					int KeyFrameEndIndex = 0;
					for (; KeyFrameEndIndex < animation->m_PositionKeyFrames.size(); KeyFrameEndIndex++)
					{
						if (animation->m_PositionKeyFrames[KeyFrameEndIndex].m_Time > time)
						{
							break;
						}
					}

					if (KeyFrameEndIndex >= animation->m_PositionKeyFrames.size())
					{
						// we are at or past the last key frame use the last keyframe only
						gameObject->m_Position = animation->m_PositionKeyFrames[KeyFrameEndIndex - 1].m_Position;
						return;
					}
					int KeyFrameStartIndex = KeyFrameEndIndex - 1;


					PositionKeyFrame startKeyFrame = animation->m_PositionKeyFrames[KeyFrameStartIndex];
					PositionKeyFrame endKeyFrame = animation->m_PositionKeyFrames[KeyFrameEndIndex];

					float percent = (time - startKeyFrame.m_Time) / (endKeyFrame.m_Time - startKeyFrame.m_Time);
					float result = 0.f;
					switch (endKeyFrame.n_EaseType)
					{
					case EasingType::Linear:
						result = percent;
						break;

					case EasingType::sineEaseIn:
						result = glm::sineEaseIn(percent);
						break;

					case EasingType::sineEaseOut:
						result = glm::sineEaseOut(percent);
						break;

					case EasingType::sineEaseInOut:
						result = glm::sineEaseInOut(percent);
						break;
					}

					glm::vec3 delta = endKeyFrame.m_Position - startKeyFrame.m_Position;
					gameObject->m_Position = startKeyFrame.m_Position + delta * result;
				}
			}







			/// ROTATION
			{

				// If there is only 1 key frame, use that one.
				// If we are equal to or greater than the time of the last key frame, use the last keyframe
				if (animation->m_RotationKeyFrames.size() == 1)
				{
					gameObject->m_Rotation = animation->m_RotationKeyFrames[0].m_Rotation;
				}
				else
				{
					// Find our rotation keyframes (Start and End)
					int KeyFrameEndIndex = 0;
					for (; KeyFrameEndIndex < animation->m_RotationKeyFrames.size(); KeyFrameEndIndex++)
					{
						if (animation->m_RotationKeyFrames[KeyFrameEndIndex].m_Time > time)
						{
							break;
						}
					}

					if (KeyFrameEndIndex >= animation->m_RotationKeyFrames.size())
					{
						// we are at or past the last key frame use the last keyframe only
						gameObject->m_Rotation = animation->m_RotationKeyFrames[KeyFrameEndIndex - 1].m_Rotation;
						return;
					}
					int KeyFrameStartIndex = KeyFrameEndIndex - 1;


					RotationKeyFrame startKeyFrame = animation->m_RotationKeyFrames[KeyFrameStartIndex];
					RotationKeyFrame endKeyFrame = animation->m_RotationKeyFrames[KeyFrameEndIndex];

					float percent = (time - startKeyFrame.m_Time) / (endKeyFrame.m_Time - startKeyFrame.m_Time);
					float result = 0.f;
					switch (endKeyFrame.n_EaseType)
					{
					case EasingType::Linear:
						result = percent;
						break;

					case EasingType::sineEaseIn:
						result = glm::sineEaseIn(percent);
						break;

					case EasingType::sineEaseOut:
						result = glm::sineEaseOut(percent);
						break;

					case EasingType::sineEaseInOut:
						result = glm::sineEaseInOut(percent);
						break;
					}

					gameObject->m_Rotation = glm::slerp(startKeyFrame.m_Rotation, endKeyFrame.m_Rotation, result);
				}
			}

		}

		Update(gameObject->m_Children);
	}

}