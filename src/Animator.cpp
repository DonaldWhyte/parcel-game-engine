/*
 * File:   Animator.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on July 9, 2009, 8:30 PM
 */

#include "Animator.h"
#include "Matrix.h"
#include "Exceptions.h"

namespace parcel
{

namespace graphics
{

    const std::string Animator::noAnimation = "NONE!";

    Animator::Animator() : repeat(false), speed(0.1f)
    {
    }

    bool Animator::IsAnimation(const std::string& id)
    {
        AnimationTable::const_iterator it = animations.find(id);
        return (it != animations.end());
    }

    void Animator::Load(const std::vector<Bone>& newBones, const AnimationTable& newAnimations)
    {
        // Stores the given bones and animations
        bones = newBones;
        animations = newAnimations;

        /* Resizes the vector that holds the quaternion's current orientations
         * to match the number of bones the animator now has. */
        currentOrientations.resize(bones.size());
        // Fills the vector with the bone's default positions
        for (unsigned int i = 0; (i < currentOrientations.size()); ++i)
        {
            currentOrientations[i] = bones[i].orientation;
        }
        // Also resizes the vector which holds every bone's FINAL orientation in matrix form
        finalOrientations.resize(bones.size());

        // Makes current animation and bind pose the first animation in the list
        AnimationTable::iterator it = animations.begin();
        if (it != animations.end()) // Checks if map contains any animations at all
        {
            bindPose = it->first;
            currentAnimation = &it->second;
        }
        /* If there are no animations in the list, just make the bind pose nothing
         * the current animation a NULL pointer. */
        else
        {
            bindPose = noAnimation;
            currentAnimation = NULL;
        }

        // Alters the appropriate flags
        animationPaused = false;
    }

    void Animator::Play(const std::string& id)
    {
        // If given ID belongs to a valid animation
        if (IsAnimation(id))
        {
            // Make that animation the currently playing one
            currentAnimation = &animations[id];
            /* Get the first keyframe and the next keyframe after that. If there
             * is no keyframe after the first, make nextKeyFrame a NULL pointer. */
            currentKeyframe = &currentAnimation->keyframes[0];
            nextKeyframe = (currentAnimation->keyframes.size() > 1) ? &currentAnimation->keyframes[1] : NULL;
            // Resets other variables to 0
            progressOnKeyframe = 0.0f;
            keyframeIndex = 0;
        }
        else
        {
            throw debug::Exception("Animator::Play - Animation with ID '" + id
                + "' does not exist, so it cannot be played.");
        }
    }

    void Animator::Play(Animation* animation)
    {
        // If given animation is valid...
        if (animation)
        {
            // Make it the currently playing animation and set appropriate data
            currentAnimation = animation;
            currentKeyframe = &currentAnimation->keyframes[0];
            nextKeyframe = (currentAnimation->keyframes.size() > 1) ? &currentAnimation->keyframes[1] : NULL;
            progressOnKeyframe = 0.0f;
            keyframeIndex = 0;
        }
        // Throw an exception if it isn't
        else
        {
            throw debug::Exception("Animator::Play - Given animation is a NULL pointer!");
        }
    }

    void Animator::Stop()
    {
        // Stops current animation and makes it the binding pose
        currentAnimation = &animations[bindPose];
    }

    void Animator::Pause()
    {
        animationPaused = true;
    }

    void Animator::Resume()
    {
        animationPaused = false;
    }

    void Animator::TogglePause()
    {
        animationPaused = !animationPaused;
    }

    void Animator::Update(const maths::matrixf& matrix)
    {
        // Return and do nothing if the animation is paused
        if (animationPaused) return;

        // Gets each bone's orientation for this frame
        const std::vector<BoneKeyFrame>& boneKeyFrames = currentKeyframe->GetBoneKeyFrames();
        // If there is a keyframe after the current one, perform interpolation
        if (nextKeyframe)
        {
            const std::vector<BoneKeyFrame>& nextBoneKeyFrames = nextKeyframe->GetBoneKeyFrames();
            // Generates a quaternion for every bone specified in this keyframe
            for (unsigned int i = 0; (i < boneKeyFrames.size()); ++i)
            {
                // Makes sure bone indices specified by the keyframe is not out of bounds
                if ((boneKeyFrames[i].boneIndex >= bones.size()) ||
                    (nextBoneKeyFrames[i].boneIndex >= bones.size()))
                {
                    throw debug::Exception("Animator::Update - Bone index specified in a key "
                        "frame is out of bounds. A quaternion cannot be generated for this bone.");
                }
                /* Makes sure both objects are referencing the same bone before creating
                 * the interpolated quaternion. */
                if (boneKeyFrames[i].boneIndex == nextBoneKeyFrames[i].boneIndex)
                {
                    currentOrientations[boneKeyFrames[i].boneIndex] =
                        maths::quaternionf::Slerp(boneKeyFrames[i].orientation,
                        nextBoneKeyFrames[i].orientation, progressOnKeyframe);
                }
                // Throw an exception if the indices are not the same
                else
                {
                    throw debug::Exception("Animator::Update - Bone IDs for current and next "
                        "keyframe are different. Cannot specify different bones at the same"
                        "index in the boneKeyFrames vector.");
                }
            }
        }
        /* Otherwise, just use the orientation from the current keyframe to get
         * each bone's current orientation. */
        else
        {
            for (unsigned int i = 0; (i < boneKeyFrames.size()); ++i)
            {
                currentOrientations[boneKeyFrames[i].boneIndex] = boneKeyFrames[i].orientation;
            }
        }

        // Using that quaternion, get the final transformation matrix for every bone
        for (unsigned int i = 0; (i < bones.size()); ++i)
        {
            maths::matrixf parent, local, inverseBone;
            // If this bone has a parent, get it's current orientation
            if (bones[i].parent > -1) parent = currentOrientations[bones[i].parent].ToRotationMatrix();
            // Otherwise, just make it an identity matrix
            else parent = maths::matrixf::Identity(4);
            // Get the bone's current local orientation
            local = currentOrientations[i].ToRotationMatrix();
            // TODO: find out what matrix you get the inverse of!!!
            inverseBone = matrix.Inverse();//maths::matrixf::Identity(4);

            // Calculates the final transformation of bone and stores it in correct place
            finalOrientations[i] = (parent * local * inverseBone);
        }

        /* Increases progress on the current keyframe by using the speed
         * and duration of the current keyframe. */
        progressOnKeyframe += speed / currentKeyframe->GetDuration();
        // If progress on keyframe is finished
        if (progressOnKeyframe >= 1.0f)
        {
            // Increases index of current keyframe
            keyframeIndex++;
            // If the animation has ended and there are no more keyframes
            if (keyframeIndex >= currentAnimation->keyframes.size())
            {
                // If animation has been set to repeat itself, play it again
                if (repeat)
                {
                    Play(currentAnimation);
                }
                // Otherwise, set current animation to bind pose and plays it
                else
                {
                    Play(bindPose);
                }
            }
            // If animation has not finished, go to next keyframe
            else
            {
                // Makes next keyframe the current keyframe
                currentKeyframe = nextKeyframe;
                // Finds the next keyframe in the animation
                nextKeyframe = &currentAnimation->keyframes[keyframeIndex];
                // Resets progress on the current keyframe to zero
                progressOnKeyframe = 0.0f;
            }
        }
    }

    void Animator::SetBindPose(const std::string& id)
    {
        // If ID belongs to a valid animation, make it the bind pose
        if (IsAnimation(id))
        {
            bindPose = id;
        }
        // Otherwise, throw an exception
        else
        {
            throw debug::Exception("Animator::SetIdleAnimation - Animation with ID '"
                + id + "' does not exist, so it cannot be set as the idle animation.");
        }
    }

    void Animator::BlendVertices(std::vector<BlendVertex>& vertices)
    {
        // Blends every vertex using the bones
        for(unsigned int i = 0; (i < vertices.size()); ++i)
        {
            // Normalises the vertex's weights to make them add up to 1
            // First, calculate sum of all weights and number of bones it's attached to
            unsigned int numBonesAttached = 0; // This is also used during the actual blending
            float totalWeight = 0.0f;
            for (unsigned int j = 0; (j < BlendVertex::maxBlendWeights); ++j)
            {
                if ((vertices[i].boneIndices[j] > -1) && (vertices[i].boneIndices[j]
                    < static_cast<int>(bones.size())))
                {
                    // Adds weight to the total
                    totalWeight += vertices[i].weights[j];
                    // Also increments the number of bones the vertex is attached to
                    ++numBonesAttached;
                }
            }
            // Now divide each of the vertex's wegiths by the total to normalise them
            for (unsigned int j = 0; (j < numBonesAttached); ++j)
            {
                vertices[i].weights[j] /= totalWeight;
            }

            /* Uses the bone's orentations and weight values associated with
             * the vertex to calculate its new position. */
            maths::vector3f vertPosition(0, 0, 0);
            for (unsigned int j = 0; (j < numBonesAttached); ++j)
            {
                // Index of the bone the vertex is attached to
                int boneIndex = vertices[i].boneIndices[j];
                // ...if it is, transform the vertex using the bone's orientation and weight
                vertPosition += ((vertices[i].position * finalOrientations[boneIndex])
                    * vertices[i].weights[j]);
                // Also adds bone's position to the vertex
                vertPosition += bones[boneIndex].position;
            }
            vertices[i].position = vertPosition;
        }
    }

}

}

