/*
 * File:   Animator.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on July 9, 2009, 8:29 PM
 */

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation.h"

namespace parcel
{

namespace graphics
{

    /* Loads bone and animation data for skeletal animation and uses it to generate
     * the correct orientations for each bone at a specific stage of the animation. */
    class Animator
    {


    private:

        /* Typedef'd to make code easier to read. */
        typedef std::map<std::string, Animation> AnimationTable;


        /* String constant that means no animation. Used when no animations
         * are availible. */
        static const std::string noAnimation;

        // Contains all the bones the animations use
        std::vector<Bone> bones;
        // Contains every animation. They can be looked up using a string key
        AnimationTable animations;

        // Data on currently playing animation
        Animation* currentAnimation; // Pointer to the currently playing animation
        // Contains each bone's LOCAL orientation at the current part of the animation
        std::vector<maths::quaternionf> currentOrientations;
        /* Contains each bone's FINAL current orientation, which takes into
         * account parent bones and world space. */
        std::vector<maths::matrixf> finalOrientations;
        /* Stores the position of every bone. */
        std::vector<maths::vector3f> bonePositions;

        unsigned int keyframeIndex; // Index to current keyframe in the current animation
        KeyFrame* currentKeyframe; // Pointer to the current keyframe
        KeyFrame* nextKeyframe; // Pointer to the next keyframe (NULL if there isn't a next keyframe)
        /* Progress of keyframe, values range from 0 to 1. Used to interpolate
         * between the two keyframe's orientations. */
        float progressOnKeyframe;
        // If true, animation will not continue
        bool animationPaused;

        // String ID of the animation which is the bind pose
        std::string bindPose;
        /* If true, current animation starts from beginning when it
         * has finished playing. */
        bool repeat;
        // Speed of the animation, initial vbalue is 0.01
        float speed;


        /* Determines if an animation with the given ID exists in the animation table. */
        bool IsAnimation(const std::string& id);
        /* Overload of play, which uses a pointer to an animation instead of its ID.
         * Used internally by this class to repeat an animation once it has finished. */
        void Play(Animation* animation);


    public:

        /* Constructor, initalises values. */
        Animator();
        /* Virtual just in case another derives from this one. */
        virtual ~Animator() {}

        /* Stores the given bones and animations and sets up the animator's
         * state for playback. */
        void Load(const std::vector<Bone>& newBones, const AnimationTable& newAnimations);

        /* Finds an animation with the given ID and starts playing it. This
         * ignores the fact that an animation may already be playing, so it
         * interupts the currently playing animation and starts playing this
         * one from the beginning. */
        void Play(const std::string& id);
        /* Stops currently playiong animation and makes the current animation
         * the bind pose. */
        void Stop();
        /* These three methods pause, resume and toggle playback of the
         * currently playing animation. */
        void Pause();
        void Resume();
        void TogglePause();

        /* Updates the currently playing animation; calculates new orientations
         * for the bone's, moves to the next keyframe if current has ended
         * and decides whether or not the whole animation is played. Should be
         * called every frame. */
        void Update(const maths::matrixf& matrix);
        /* Transforms the given vertices using the bone's final orientations.
         * It's best to call this straight after a call to Update(). */
        void BlendVertices(std::vector<BlendVertex>& vertices);

        /* Getters and setters. */
        bool GetRepeat() { return repeat; }
        void SetRepeat(bool willRepeat) { repeat = willRepeat; }
        float GetSpeed() { return speed; }
        void SetSpeed(float newSpeed) { speed = newSpeed; }
        const std::string& GetBindPose() { return bindPose; }
        void SetBindPose(const std::string& id);


    };

}

}

#endif
