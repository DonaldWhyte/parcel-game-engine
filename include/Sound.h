/*
 * File:   Sound.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on October 11, 2008, 7:08 PM
 * Modified to adhere to new coding styles, add 3D sounds and special
 * effects as well as offer a sound deletion feature on
 * July 5, 2009, 2:43 PM
 */

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <string>
#include <map>
#include <irrKlang.h>
#include "Vector.h"
#include "Exceptions.h"

/* Just in case the windows library's macro, PlaySound, inteferes
 * with any code that calls SoundManager::PlaySound, we'll undefine
 * here. */
#undef PlaySound

namespace parcel
{

namespace sound
{

    /* Stores a single sound's properties. */
    struct Sound
    {
        irrklang::ISoundSource* source; // Pointer to object that contains actual sound data
        maths::vector3f position; // The position of the sound in 3D space
        bool loop; // If true, sound starts playing at the beginning when finished
        bool startPaused; // If true, sound starts paused when first played
        bool track; // If true, engine is able to track progress of playback
    };

    /* This class handles all sound processing through irrKlang, a sound library. */
    class SoundManager
    {


    private:

        // Used to make code easier to read
        typedef std::map<std::string, Sound> SoundTable;
        typedef std::map<std::string, irrklang::ISound*> PlayingSounds;


        // Stores the irrklang sound engine, which plays/creates every sound
        irrklang::ISoundEngine* engine;
        /* All sounds are stored in this map, where they can be retrived by
         * giving a string ID. */
        SoundTable sounds;
        /* Sames as above, but this stores every sound and its ID that is
         * currently playing. */
        PlayingSounds playingSounds;

        /* If true, special effects can be applied to playing sounds.
         * This flag is checked whenever an effect is applied to a sound. */
        bool soundEffectsEnabled;


        /* Converts Parcel vector to the vector object used for IrrKlang functions. */
        irrklang::vec3df ToIrrklangVector(const maths::vector3f vec);


    public:

        /* Constructor, creates sound engine and checks if special sound effects are
         * availible. */
        SoundManager();
        /* Destructor, stops all sounds and then deletes them. Also deletes sound engine. */
        ~SoundManager();

        /* Loads a sound from a file and stores it with the given ID. Other optional
         * parameters can be given to alter the way the sound will be played. */
        void LoadSound(const std::string& id, const std::string& filename, bool loop = false,
            bool startPaused = false, bool track = false, const maths::vector3f& position =
            maths::vector3f(0, 0, 0));
        /* This copies an existing stored sound, with the ID 'originalID'. The copy is
         * stored in a sound with the ID 'alias'. */
        void CreateSoundAlias(const std::string& originalID, const std::string& alias);
        /* Uses given ID to find the requested sound and delete it from the list. */
        void DeleteSound(const std::string& id);
        /* Deletes every sound loaded in the manager. */
        void DeleteAll();

        /* Plays sound with given ID. If 'playIn2D' is true, the sound and listener's
         * position will change the volume and panning of the sound. If it is false,
         * then the sound will play normally. */
        void PlaySound(const std::string& id, bool playIn2D = false);
        /* Stops the sound with the given ID. If the sound is not playing, this
         * method just does nothing. */
        void StopSound(const std::string& id);
        /* Stop every currently playing sound, clearing 'playingSounds' map in the process. */
        void StopAllSounds();

        /* Returns true if a sound with the given ID is loaded into this manager. */
        bool IsSound(const std::string& id);
        /* Returns true if the sound with the given ID is already playing. */
        bool IsPlaying(const std::string& id);

        /* Pauses and resumes a playing sound respectively. Simply does nothing
         * if the sound with the given ID is not playing. */
        void PauseSound(const std::string& id);
        void ResumeSound(const std::string& id);
        /* This pauses a sound if it is playing and resumes it if it is paused. */
        void TogglePause(const std::string& id);
        /* Pauses, resumes and toggles playback on all playing sounds respectively. */
        void PauseAll();
        void ResumeAll();
        void ToggleAll();

        /* These methods can get and set a single sound's volume, speed (pitch)
         * and panning. If the given ID does not belong to any loaded sound,
         * an exception will be thrown. */
        float GetSoundVolume(const std::string& id);
        float GetSoundSpeed(const std::string& id);
        float GetSoundPan(const std::string& id);
        void SetSoundVolume(const std::string& id, float volume);
        void SetSoundSpeed(const std::string& id, float speed);
        void SetSoundPan(const std::string& id, float pan);
        /* Gets and sets the master volume, the volume of every sound. */
        float GetMasterVolume();
        void SetMasterVolume(float volume);

        /* These methods apply effects to any sounds currently playing. This includes
         * reverb, echo, changing the tempo, etc. Each method takes parameters unique
         * to that effect. */
        void DisableEffects(const std::string& id);

        void ApplyDistortion(const std::string& id, float gain = -18,float edge = 15,
            float postEQCenterFrequency = 2400, float postEQBandwidth = 2400,
            float preLowpassCutoff = 8000);

        void ApplyEcho(const std::string& id, float wetDryMix = 50, float feedback = 50,
            float leftDelay = 500, float rightDelay = 500, int panDelay = 0);

        void ApplyWaveReverb(const std::string& id, float inGain = 0,
            float reverbMix = 0, float reverbTime = 1000, float highFreqRTRatio = 0.001f);

        void ApplyParamEqualizer(const std::string& id, float venter = 8000,
            float bandwidth = 12, float gain = 0);

        void ApplyGargle(const std::string& id, int rateHz = 20, bool sinusWaveForm = true);

        void ApplyChorus(const std::string& id, float wetDryMix = 50, float depth = 10,
            float feedback = 25, float frequency = 1.1, bool sinusWaveForm = true,
			float delay = 16, int phase = 90);

        void ApplyCompressor(const std::string& id, float gain = 0,	float attack = 10,
            float release = 200, float threshold = -20, float ratio = 3, float predelay = 4);

        void ApplyFlanger(const std::string& id, float wetDryMix = 50, float depth = 100,
            float feedback = -50, float frequency = 0.25f, bool triangleWaveForm = true,
            float delay = 2, int phase = 0);

        void ApplyI3DL2Reverb(const std::string& id, int room = -1000, int roomHF = -100,
            float roomRolloffFactor = 0, float decayTime = 1.49f, float decayHFRatio = 0.83f,
            int reflections = -2602, float reflectionsDelay = 0.007f, int reverb = 200,
            float reverbDelay = 0.011f, float diffusion = 100.0f, float density = 100.0f,
            float hFReference = 5000.0f);

        /* Sets the new position, direction and velocity of the user to determine
         * how the sounds play. */
        void SetListenerPosition(const maths::vector3f& position,
            const maths::vector3f& direction = maths::vector3f(0, 0, 1),
            const maths::vector3f& velocityPerSecond = maths::vector3f(0, 0, 0),
            const maths::vector3f& up = maths::vector3f(0, 1, 0));


    };

    /* Exception thrown when a sound related problem occurs. */
    class SoundException : public debug::Exception
    {
    public:
        SoundException(const std::string& errorMessage) : Exception(errorMessage) {}

    };

}

}

#endif
