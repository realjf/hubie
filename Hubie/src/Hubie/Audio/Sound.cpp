#include "Precompiled.h"
#include "Sound.h"
#include "Hubie/Core/VFS.h"

#ifdef HB_OPENAL
#include "Hubie/Platform/OpenAL/ALSound.h"
#endif

namespace Hubie
{
    Sound::Sound()
        : m_Streaming(false)
        , m_Data(AudioData())
    {
    }

    Sound::~Sound()
    {
        delete[] m_Data.Data;
    }

    Sound* Sound::Create(const std::string& name, const std::string& extension)
    {
#ifdef HB_OPENAL
        return new ALSound(name, extension);
#else
        return nullptr;
#endif
    }

    double Sound::GetLength() const
    {
        return m_Data.Length;
    }
}
