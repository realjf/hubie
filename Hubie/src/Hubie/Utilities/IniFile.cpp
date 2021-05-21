#include "Precompiled.h"
#include "IniFile.h"
#include "Hubie/Core/OS/FileSystem.h"
#include "Hubie/Core/StringUtilities.h"

#include <filesystem>
#include <fstream>

Hubie::IniFile::IniFile(const std::string& filePath)
    : m_FilePath(filePath)
{
    Load();
}

void Hubie::IniFile::Reload()
{
    RemoveAll();
    Load();
}

bool Hubie::IniFile::Remove(const std::string& key)
{
    if(IsKeyExisting(key))
    {
        m_Data.erase(key);
        return true;
    }

    return false;
}

void Hubie::IniFile::RemoveAll()
{
    m_Data.clear();
}

bool Hubie::IniFile::IsKeyExisting(const std::string& key) const
{
    return m_Data.find(key) != m_Data.end();
}

void Hubie::IniFile::RegisterPair(const std::string& key, const std::string& value)
{
    RegisterPair(std::make_pair(key, value));
}

void Hubie::IniFile::RegisterPair(const std::pair<std::string, std::string>& pair)
{
    m_Data.insert(pair);
}

std::vector<std::string> Hubie::IniFile::GetFormattedContent() const
{
    std::vector<std::string> result;

    for(const auto& [key, value] : m_Data)
        result.push_back(key + "=" + value);

    return result;
}

void Hubie::IniFile::Load()
{
    auto fileString = Hubie::FileSystem::ReadTextFile(m_FilePath);
    auto lines = Hubie::StringUtilities::GetLines(fileString);

    for(auto& line : lines)
    {
        if(IsValidLine(line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            RegisterPair(ExtractKeyAndValue(line));
        }
    }
}

void Hubie::IniFile::Rewrite() const
{
    std::stringstream stream;
    for(const auto& [key, value] : m_Data)
        stream << key << "=" << value << std::endl;

    FileSystem::WriteTextFile(m_FilePath, stream.str());
}

std::pair<std::string, std::string> Hubie::IniFile::ExtractKeyAndValue(const std::string& p_line) const
{
    std::string key;
    std::string value;

    std::string* currentBuffer = &key;

    for(auto& c : p_line)
    {
        if(c == '=')
            currentBuffer = &value;
        else
            currentBuffer->push_back(c);
    }

    return std::make_pair(key, value);
}

bool Hubie::IniFile::IsValidLine(const std::string& attributeLine) const
{
    if(attributeLine.size() == 0)
        return false;

    if(attributeLine[0] == '#' || attributeLine[0] == ';' || attributeLine[0] == '[')
        return false;

    if(std::count(attributeLine.begin(), attributeLine.end(), '=') != 1)
        return false;

    return true;
}

bool Hubie::IniFile::StringToBoolean(const std::string& value) const
{
    return (value == "1" || value == "T" || value == "t" || value == "True" || value == "true");
}
