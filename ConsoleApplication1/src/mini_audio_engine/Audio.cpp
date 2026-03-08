#define MINIAUDIO_IMPLEMENTATION
#include <mini_audio_engine/Audio.h>

#include <third-party/audio_lib/miniaudio.h>

#include <utility>  // std::swap
#include <iostream>

ma_engine Audio::s_engine{};
bool      Audio::s_initialized = false;

bool Audio::Init()
{
    if (s_initialized)
        return true;

    if (ma_engine_init(nullptr, &s_engine) != MA_SUCCESS)
    {
        std::cerr << "Failed to initialize miniaudio engine\n";
        return false;
    }

    s_initialized = true;
    return true;
}

void Audio::Shutdown()
{
    if (!s_initialized)
        return;

    ma_engine_uninit(&s_engine);
    s_initialized = false;
}

Audio::~Audio()
{
    Unload();
}

bool Audio::Load(const std::string& path)
{
    Unload();

    if (!s_initialized) {
        std::cerr << "Audio::Load called before Audio::Init\n";
        return false;
    }

    m_sound = new ma_sound{};
    if(ma_sound_init_from_file( &s_engine,
						        path.c_str(),
						        0,
						        nullptr,
						        nullptr,
						        m_sound) != MA_SUCCESS )
    {
        std::cerr << "Failed to load sound: " << path << '\n';
        delete m_sound;
        m_sound = nullptr;
        m_loaded = false;
        return false;
    }

    m_loaded = true;
    return true;
}

void Audio::Unload()
{
    if (m_loaded && m_sound) 
    {
        ma_sound_uninit(m_sound);
        delete m_sound;
        m_sound = nullptr;
        m_loaded = false;
    }
}

void Audio::Play(bool restart) const
{
    if (!m_loaded || !m_sound)
    {
        return;
    }

    if(restart)
    {
        ma_sound_stop(m_sound);
    }

    ma_sound_start(m_sound);
}

void Audio::Stop() const
{
    if (!m_loaded || !m_sound)
    {
        return;
    }

    ma_sound_stop(m_sound);
}

void Audio::SetLooping(bool loop) const
{
    if (!m_loaded || !m_sound)
    {
        return;
    }

    ma_sound_set_looping(m_sound, loop ? MA_TRUE : MA_FALSE);
}

void Audio::Rewind() const
{
    if (!m_loaded || !m_sound) return;
    ma_sound_seek_to_pcm_frame(m_sound, 0);
}


Audio::Audio(Audio&& other) noexcept
{
    std::swap(m_sound, other.m_sound);
    std::swap(m_loaded, other.m_loaded);
}

Audio& Audio::operator=(Audio&& other) noexcept
{
    if (this != &other) 
    {
        Unload();
        std::swap(m_sound, other.m_sound);
        std::swap(m_loaded, other.m_loaded);
    }
    return *this;
}
