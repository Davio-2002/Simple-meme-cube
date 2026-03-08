// audio/Audio.h
#pragma once

#include <string>

// Forward declarations from miniaudio
struct ma_engine;
struct ma_sound;

class Audio
{
public:
    // Initialize / shutdown global audio engine
    static bool Init();
    static void Shutdown();

    Audio() = default;
    explicit Audio(const std::string& path) { Load(path); }
    ~Audio();

    Audio(const Audio&) = delete;
    Audio& operator=(const Audio&) = delete;

    Audio(Audio&& other) noexcept;
    Audio& operator=(Audio&& other) noexcept;

    bool Load(const std::string& path);
    void Unload();

    void Play(bool restart = true) const;
    void Stop() const;
    void Rewind() const;
    void SetLooping(bool loop) const;

    bool IsValid() const noexcept { return m_loaded; }

private:
    static ma_engine s_engine;
    static bool      s_initialized;

    ma_sound* m_sound{ nullptr };
    bool      m_loaded{ false };
};


