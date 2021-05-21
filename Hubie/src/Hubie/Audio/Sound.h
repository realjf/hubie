#pragma once

#include "Hubie/Core/Core.h"
#include "AudioData.h"

namespace Hubie
{
    class HB_EXPORT Sound
    {
        friend class SoundManager;

    public:
        static Sound* Create(const std::string& name, const std::string& extension);
        virtual ~Sound();

        unsigned char* GetData() const
        {
            return m_Data.Data;
        }
        int GetBitRate() const
        {
            return m_Data.BitRate;
        }
        float GetFrequency() const
        {
            return m_Data.FreqRate;
        }
        int GetChannels() const
        {
            return m_Data.Channels;
        }
        int GetSize() const
        {
            return m_Data.Size;
        }
        bool IsStreaming() const
        {
            return m_Streaming;
        }
        double GetLength() const;
        virtual double StreamData(unsigned int buffer, double timeLeft)
        {
            return 0.0f;
        }

        const std::string& GetFilePath() const { return m_FilePath; }

    protected:
        Sound();
        bool m_Streaming;
        std::string m_FilePath;

        AudioData m_Data;
    };
}
