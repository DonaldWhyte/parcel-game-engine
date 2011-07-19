/*
 * File:   Animation.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on July 8, 2009, 2:38 PM
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <map>
#include "Quaternion.h"
#include "Vertex.h"
#include "Util.h"

namespace parcel
{

namespace graphics
{

    /* TODO: implement and finish animation system. */

    /* Contains initial orientation of the bone and the index to its parent.
     * The index is -1 if the bone does not have a parent. */
    struct Bone
    {
        maths::vector3f position; // Position of the bone
        maths::quaternionf orientation; // Original orientation of bone
        int parent; // Index of parent bone
    };

    /* Stores the orientation of a single bone on a keyframe. The 'boneIndex'
     * variable stores the index of thr bone it's storing the orientatino for. */
    struct BoneKeyFrame
    {
        unsigned int boneIndex; // Index of the bone
        maths::quaternionf orientation; // The bone's orientation in this frame

        /* Useful constructors. */
        BoneKeyFrame() : boneIndex(0) { }
        BoneKeyFrame(int bIndex, const maths::quaternionf& bOrientation) :
            boneIndex(bIndex), orientation(bOrientation) { }
    };

    /* Contains enough BoneKeyFrame for every bone of the animation. Also stores
     * how long the keyframe lasts using float variable, duration. */
    class KeyFrame
    {


    private:

        std::vector<BoneKeyFrame> boneKeyFrames; // Contains BoneKeyframe object for every bone of model
        float duration; // How long this keyframe lasts


    public:

        /* Useful constructors. */
        KeyFrame() : duration(0.0f) { }
        KeyFrame(const std::vector<BoneKeyFrame>& bKeyFrames, float frameDuration) :
            boneKeyFrames(bKeyFrames), duration(frameDuration) { }
        /* Getters. */
        const std::vector<BoneKeyFrame>& GetBoneKeyFrames() { return boneKeyFrames; }
        const float GetDuration() { return duration; }


    };

    /* Contains all the keyframes for a single animation. */
    struct Animation
    {
        std::vector<KeyFrame> keyframes;
    };

    /* Used for meshes that are animated. Contains indices to the bones
     * the vertex is attached to and gives that bone a weighting value,
     * which decides how much the bone influences the vertex's position. */
    struct BlendVertex : public Vertex
    {

        static const unsigned int maxBlendWeights = 3; // Maximum bones/weights the vertex can be attached to
        static const int noBone = -1; // Constant used to determine when no bone is attached

        float weights[maxBlendWeights]; // Stores weighting values for every attached bone
        int boneIndices[maxBlendWeights]; // Stores index of bone the vertex is attached to

        BlendVertex() : Vertex()
        {
            // Fills bone indices and weights with blank values
            for (unsigned int i = 0; (i < maxBlendWeights); ++i)
            {
                boneIndices[i] = noBone;
                weights[i] = 0.0f;
            }
        }

        BlendVertex(const maths::vector3f& vertPosition, const maths::vector2f& vertTexCoord,
            const maths::vector3f& vertNormal) : Vertex(vertPosition, vertTexCoord, vertNormal)
        {
            // Fills bone indices and weights with blank values
            for (unsigned int i = 0; (i < maxBlendWeights); ++i)
            {
                boneIndices[i] = noBone;
                weights[i] = 0.0f;
            }
        }

    };

}

}

#endif
