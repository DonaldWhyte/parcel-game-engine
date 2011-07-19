/*
 * File:   Sound.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on October 11, 2008, 8:04 PM
 * Modified to adhere to new coding styles, add 3D sounds and special
 * effects as well as offer a sound deletion feature on
 * July 5, 2009, 2:43 PM
 */

#include "Sound.h"

namespace parcel
{

namespace sound
{

    SoundManager::SoundManager()
    {
        // Creates sound engine
        engine = irrklang::createIrrKlangDevice();
        // TODO: find out if effects are enabled
        soundEffectsEnabled = true;
    }
    SoundManager::~SoundManager()
    {
        // Deletes every sound that has been loaded
        DeleteAll();
        // Closes the sound engine too
        engine->drop();
    }

    void SoundManager::LoadSound(const std::string& id, const std::string& filename,
        bool loop, bool startPaused, bool track, const maths::vector3f& position)
    {
        // Makes sure a sound with the given ID doesn't already exist...
        if (IsSound(id))
        {
            // ...if it does, throw an exception
            throw SoundException("SoundManager::LoadSound - Sound with ID '" +
                id + "' already exists.");
        }

        // Attempts to load the sound file into a sound source object
        irrklang::ISoundSource* source = engine->addSoundSourceFromFile(filename.c_str());
        // If it succesfully loads the file
        if (source)
        {
            // Stores given parameters and sound source into structure
            Sound newSound = { source, position, loop, startPaused, track };
            // Adds structure to the map, using 'id' as the key
            sounds[id] = newSound;
        }
        // If it doesn't, throw an exception
        else
        {
            throw SoundException("SoundManager::LoadSound - "
                "Could not load sound source from file '" + filename + "'.");
        }
    }

    void SoundManager::CreateSoundAlias(const std::string& originalID, const std::string& alias)
    {
        // Makes sure ID for given sound does exist in the map
        if (!IsSound(originalID))
        {
            throw SoundException("SoundManager::CreateSoundAlias - "
                "Cannot create alias of sound. Cannot find original sound '"
                + originalID + "'.");
        }
        // Makes sure given alias does not already exist
        if (IsSound(alias))
        {
            throw SoundException("SoundManager::CreateSoundAlias - "
                "Sound with alias '" + alias + "' already exists.");
        }

        // Creates new alias and adds it to the map
        sounds[alias] = sounds[originalID];
    }

    void SoundManager::DeleteSound(const std::string& id)
    {
        // Throws exception if the sound doesn't exist
        if (!IsSound(id))
        {
            throw SoundException("SoundManager::DeleteSound - Sound with ID '"
                + id + "' does not exist, so it cannot be deleted.");
        }

        // Makes sure the requested sound is not playing before deleting it
        StopSound(id);
        // Removes sound data from the engine and erases it from the map
        engine->removeSoundSource(sounds[id].source);
        sounds.erase(sounds.find(id));
    }

    void SoundManager::DeleteAll()
    {
        // Makes sure all the sounds are not playing
        StopAllSounds();
        // Deletes and releases resources being used by the engine
        engine->removeAllSoundSources();
        // Empties the map and gets rid of every sound and ID
        sounds.clear();
    }

    void SoundManager::PlaySound(const std::string& id, bool playIn2D)
    {
        // Makes sure sound with given ID exists before using it
        if (!IsSound(id))
        {
            throw SoundException("SoundManager::PlaySound - Sound with ID '"
                + id + "' does not exist, so it cannot be played.");
        }
        // Just return and do nothing if the given sound is already playing
        if (IsPlaying(id))
        {
            return;
        }

        // Gets sound structure from iterator
        Sound sound = sounds[id];
        // Plays the sound, storing the playing sound in ISound* pointer
        irrklang::ISound* playingSound = NULL;
        // If flag is set, play sound in 2D and don't use its position to alter the volume
        if (playIn2D)
        {
            playingSound = engine->play2D(sound.source, sound.loop, sound.startPaused,
                sound.track, soundEffectsEnabled);
        }
        // Otherwise, use the position and play the sound in 3D space
        else
        {
            playingSound = engine->play3D(sound.source, ToIrrklangVector(sound.position),
                sound.loop, sound.startPaused, sound.track, soundEffectsEnabled);
        }
        // Throws exception if no sound object was returned
        if (!playingSound)
        {
            throw SoundException("SoundManager::PlaySound - Could not play"
                "sound. The sound file may be missing or corrupt.");
        }

        // Applies the effects stored in Sound to the playing sound
        irrklang::ISoundEffectControl* effects = playingSound->getSoundEffectControl();
        /* If it returned NULL, special effects aren't supported by the computer,
         * we throw an exception. */
        if (!effects)
        {
            throw SoundException("SoundManager::PlaySound - Special effects"
                "on sounds are not supported by this machine's hardware.");
        }

        // Finally, add the sound to map of playing sounds
        playingSounds[id] = playingSound;
    }

    void SoundManager::StopSound(const std::string& id)
    {
        // If that sound is not playing, just return and do nothing
        if (!IsPlaying(id))
        {
            return;
        }
        // Stops the playing sound and erases it from the map
        playingSounds[id]->stop();
        playingSounds[id]->drop(); // Releases resources used by sound
        playingSounds.erase(playingSounds.find(id));
    }

    void SoundManager::StopAllSounds()
    {
        // Stops all sounds currently playing
        engine->stopAllSounds();
        // Goes through playing sounds, releasing resources used by each
        for (PlayingSounds::iterator it = playingSounds.begin();
            (it != playingSounds.end()); ++it)
        {
            it->second->drop();
        }
        // Clears playing sounds map
        playingSounds.clear();
    }

    bool SoundManager::IsSound(const std::string& id)
    {
        // Attempts to find sound in the table
        SoundTable::iterator it = sounds.find(id);
        // Returns success of the find
        return (it != sounds.end());
    }

    bool SoundManager::IsPlaying(const std::string& id)
    {
        /* Attempts to find sound with the given ID in the playing sounds
         * table. This will check if that sound is being played. */
        PlayingSounds::iterator it = playingSounds.find(id);
        // Returns success of the find
        return (it != playingSounds.end());
    }

    void SoundManager::PauseSound(const std::string& id)
    {
        if (IsPlaying(id))
        {
            playingSounds[id]->setIsPaused(true);
        }
    }

    void SoundManager::ResumeSound(const std::string& id)
    {
        if (IsPlaying(id))
        {
            playingSounds[id]->setIsPaused(false);
        }
    }

    void SoundManager::TogglePause(const std::string& id)
    {
        if (IsPlaying(id))
        {
            // Gets state of sound - paused or not paused and inverts it
            bool paused = playingSounds[id]->getIsPaused();
            playingSounds[id]->setIsPaused(!paused);
        }
    }

    void SoundManager::PauseAll()
    {
        // Goes through every playing sound and pauses them
        for (PlayingSounds::iterator it = playingSounds.begin();
            (it != playingSounds.end()); ++it)
        {
            it->second->setIsPaused(true);
        }
    }

    void SoundManager::ResumeAll()
    {
        // Same as above, but resumes them instead
        for (PlayingSounds::iterator it = playingSounds.begin();
            (it != playingSounds.end()); ++it)
        {
            it->second->setIsPaused(false);
        }
    }

    void SoundManager::ToggleAll()
    {
        // Geos through playing sounds, pausing or resuming them
        for (PlayingSounds::iterator it = playingSounds.begin();
            (it != playingSounds.end()); ++it)
        {
            bool paused = it->second->getIsPaused();
            it->second->setIsPaused(!paused);
        }
    }

    float SoundManager::GetSoundVolume(const std::string& id)
    {
        // If sound is playing, return the volume.
        if (IsPlaying(id))
        {
            return playingSounds[id]->getVolume();
        }
        // If it isn't, throw an exception
        else
        {
            throw SoundException("SoundManager::GetSoundVolume - Sound with ID '"
                + id + "' is not playing, so you cannot get its properties.");
        }
    }

    float SoundManager::GetSoundSpeed(const std::string& id)
    {
        if (IsPlaying(id)) return playingSounds[id]->getPlaybackSpeed();
        else throw SoundException("SoundManager::GetSoundSpeed - Sound with ID '"
                + id + "' is not playing, so you cannot get its properties.");
    }

    float SoundManager::GetSoundPan(const std::string& id)
    {
        if (IsPlaying(id)) return playingSounds[id]->getPan();
        else throw SoundException("SoundManager::GetSoundPan - Sound with ID '"
                + id + "' is not playing, so you cannot get its properties.");
    }

    void SoundManager::SetSoundVolume(const std::string& id, float volume)
    {
        if (IsPlaying(id))
        {
            playingSounds[id]->setVolume(volume);
        }
        else
        {
            throw SoundException("SoundManager::SetSoundVolume - Sound with ID '"
                + id + "' is not playing, so you cannot set its properties.");
        }
    }

    void SoundManager::SetSoundSpeed(const std::string& id, float speed)
    {
        if (IsPlaying(id))
        {
            /* setPlaybackSpeed returns false if the hardware doesn't support
             * such a feature. If it returns false, an exception is thrown. */
            if (!playingSounds[id]->setPlaybackSpeed(speed))
            {
                throw SoundException("SoundManager::SetSoundSpeed - This hardware"
                    "does not support variant playback speeds. Cannot set speed.");
            }
        }
        else
        {
            throw SoundException("SoundManager::SetSoundSpeed - Sound with ID '"
                + id + "' is not playing, so you cannot set its properties.");
        }
    }

    void SoundManager::SetSoundPan(const std::string& id, float pan)
    {
        if (IsPlaying(id)) playingSounds[id]->setPan(pan);
        else throw SoundException("SoundManager::SetSoundPan - Sound with ID '"
            + id + "' is not playing, so you cannot set its properties.");
    }

    float SoundManager::GetMasterVolume()
    {
        return engine->getSoundVolume();
    }

    void SoundManager::SetMasterVolume(float volume)
    {
        engine->setSoundVolume(volume);
    }

    void SoundManager::DisableEffects(const std::string& id)
    {
        // If speical sound effects are enabled and the sound is playing...
        if (soundEffectsEnabled && IsPlaying(id))
        {
            // ...disable all the effects.
            playingSounds[id]->getSoundEffectControl()->disableAllEffects();
        }

        /* NOTE: Same goes for all effect relative methods. They check if special
         * effects are enabled and the sound is playing, then pass any parameters
         * they received to the required irrKlang method. */
    }

    void SoundManager::ApplyDistortion(const std::string& id, float gain, float edge,
        float postEQCenterFrequency, float postEQBandwidth,
        float preLowpassCutoff)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableDistortionSoundEffect(
                gain, edge, postEQCenterFrequency, postEQBandwidth, preLowpassCutoff);
        }
    }

    void SoundManager::ApplyEcho(const std::string& id, float wetDryMix, float feedback,
        float leftDelay, float rightDelay, int panDelay)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableEchoSoundEffect(
                wetDryMix, feedback, leftDelay, rightDelay, panDelay);
        }
    }

    void SoundManager::ApplyWaveReverb(const std::string& id, float inGain,
        float reverbMix, float reverbTime, float highFreqRTRatio)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableWavesReverbSoundEffect(
                inGain, reverbMix, reverbTime, highFreqRTRatio);
        }
    }

    void SoundManager::ApplyParamEqualizer(const std::string& id, float center,
        float bandwidth, float gain)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableParamEqSoundEffect(
                center, bandwidth, gain);
        }
    }

    void SoundManager::ApplyGargle(const std::string& id, int rateHz, bool sinusWaveForm)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableGargleSoundEffect(
                rateHz, sinusWaveForm);
        }
    }

    void SoundManager::ApplyChorus(const std::string& id, float wetDryMix, float depth,
        float feedback, float frequency, bool sinusWaveForm, float delay, int phase)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableChorusSoundEffect(
                wetDryMix, depth, feedback, frequency, sinusWaveForm, delay, phase);
        }
    }

    void SoundManager::ApplyCompressor(const std::string& id, float gain, float attack,
        float release, float threshold, float ratio, float predelay)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableCompressorSoundEffect(
                gain, attack, release, threshold, ratio, predelay);
        }
    }

    void SoundManager::ApplyFlanger(const std::string& id, float wetDryMix, float depth,
        float feedback, float frequency, bool triangleWaveForm, float delay, int phase)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableFlangerSoundEffect(
                wetDryMix, depth, feedback, frequency, triangleWaveForm, delay, phase);
        }
    }

    void SoundManager::ApplyI3DL2Reverb(const std::string& id, int room, int roomHF,
        float roomRolloffFactor, float decayTime, float decayHFRatio, int reflections,
        float reflectionsDelay, int reverb, float reverbDelay, float diffusion,
        float density, float hFReference)
    {
        if (soundEffectsEnabled && IsPlaying(id))
        {
            playingSounds[id]->getSoundEffectControl()->enableI3DL2ReverbSoundEffect(
                room, roomHF, roomRolloffFactor, decayTime, decayHFRatio,
                reflections, reflectionsDelay, reverb, reverbDelay, diffusion,
                density, hFReference);
        }
    }

    void SoundManager::SetListenerPosition(const maths::vector3f& position,
        const maths::vector3f& direction, const maths::vector3f& velocityPerSecond,
        const maths::vector3f& up)
    {
        /* Sets the listener's position, direction, velocity and up vectors
         * in the current sound engine. */
        engine->setListenerPosition(
            ToIrrklangVector(position), ToIrrklangVector(direction),
            ToIrrklangVector(velocityPerSecond), ToIrrklangVector(up));
    }

    irrklang::vec3df SoundManager::ToIrrklangVector(const maths::vector3f vec)
    {
        return irrklang::vec3df(vec.x, vec.y, vec.z);
    }

}

}
