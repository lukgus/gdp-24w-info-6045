#include "animationsystem.h"

#include <assimp/scene.h>

#include <glm/gtx/easing.hpp>

void AnimationSystem::Update(const std::vector<GameObject*>& gameObjects) const
{
	for (GameObject* gameObject : gameObjects)
	{
		Character* character = dynamic_cast<Character*>(gameObject);
		if (character != nullptr && character->m_Animation != nullptr)
		{
			Animation* animation = character->m_Animation;

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


void AnimationSystem::UpdateCharacter(const std::vector<GameObject*>& gameObjects) const
{
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		Character* character = dynamic_cast<Character*>(gameObjects[i]);
		if (character != nullptr)
		{
			Model* model = character->m_Model;

			glm::mat4 origin = glm::mat4(1.0f);
			glm::mat4 rootTransformation = glm::mat4(1.f);

			CalculateMatrices(model, model->CharacterAnimations[0], model->RootNode, rootTransformation, 0.f);
		}
	}
}

void AnimationSystem::CalculateMatrices(Model* model, CharacterAnimation* animation, Node* node, const glm::mat4& parentTransformationMatrix, double keyFrameTime) const
{
	std::string nodeName(node->Name);		// use this for lookups, bones, animation nodes

	glm::mat4 transformationMatrix = node->Transformation;

	// Project #2
	// Animation calculation
	// AnimationData* data = FindAnimationDat(nodeName);
	//if (data != nullptr)
	{
		// glm::vec3 position = GetAnimationPosition(data, keyFrameTime);	/// POSITION update in previous function
		// glm::vec3 scale = GetAnimationPosition(data, keyFrameTime);		/// SCALE from your project
		// glm::vec3 rotation = GetAnimationPosition(data, keyFrameTime);	/// ROTATION update from previous function

		// calculate the matrices
		// glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
		// glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		// glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

		// transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix
	}

	// Calculate the global transformation
	glm::mat4 globalTransformation = parentTransformationMatrix * transformationMatrix;

	// If there is a bone associated with this name, assign the global transformation
	auto boneMapIt = model->BoneNameToIdMap.find(nodeName);
	if (boneMapIt != model->BoneNameToIdMap.end())
	{
		BoneInfo& boneInfo = model->BoneInfoVec[boneMapIt->second];
		boneInfo.FinalTransformation = model->GlobalInverseTransformation * globalTransformation * boneInfo.BoneOffset;
		boneInfo.GlobalTransformation = globalTransformation;
	}

	// Calculate all children
	for (int i = 0; i < node->Children.size(); ++i)
	{
		CalculateMatrices(model, animation, node->Children[i], globalTransformation, keyFrameTime);
	}
}