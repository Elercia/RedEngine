#include "RedEngine/Core/Configuration/UserInputHelper.hpp"

#include "RedEngine/Input/InputModule.hpp"

#include "RedEngine/Core/Configuration/IniReader.hpp"
#include "RedEngine/Core/Container/String.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Core/Engine.hpp"
#include "RedEngine/Filesystem/Path.hpp"

#include <filesystem>

namespace red
{
namespace utils
{
    UserActionMapping UserInputHelper::LoadActionMapping()
    {
        Path p = Path::Resource(L"userInput.ini");
        auto iniConfig = utils::IniReader::ReadFromFile(p);

        UserActionMapping mappings{};

        for (auto& configLine : iniConfig)
        {
            // auto& cat = std::get<0>(configLine);
            auto& key = std::get<1>(configLine);
            auto& value = std::get<2>(configLine);

            KeyMapping mapping = GetMappingFromConfigLine(value);

            auto mappingIt = mappings.find(key);
            if (mappingIt != mappings.end())
            {
                RED_LOG_WARNING("Redefining key mapping for {}", key);
                mappingIt->second = mapping;
            }
            else
            {
                mappings.insert({key, mapping});
            }
        }

        return mappings;
    }

    KeyMapping UserInputHelper::GetMappingFromConfigLine(const String& valueString)
    {
        KeyMapping m;

        auto indexOfSemicolon = valueString.find_first_of(';');
        String keyString = valueString.substr(0, indexOfSemicolon);
        String modifierString = valueString.substr(indexOfSemicolon + 1, valueString.length());

        m.modifiers = GetModifiersFromConfigLine(modifierString);

        auto keyCode = GetKeyCode(keyString);

        if (keyCode == KeyCodes::MAX)
            RED_LOG_WARNING("Key mapping {} has a invalid key", keyString);

        m.mapping = keyCode;

        return m;
    }

    ModifierKeyBitSet UserInputHelper::GetModifiersFromConfigLine(const String& modifierString)
    {
        ModifierKeyBitSet modifiers;

        if (modifierString.find("lctrl+") != String::npos)
        {
            modifiers.set(ModifierKey::LCTRL);
        }
        if (modifierString.find("rctrl+") != String::npos)
        {
            modifiers.set(ModifierKey::RCTRL);
        }

        if (modifierString.find("lalt+") != String::npos)
        {
            modifiers.set(ModifierKey::LALT);
        }
        if (modifierString.find("ralt+") != String::npos)
        {
            modifiers.set(ModifierKey::RALT);
        }

        if (modifierString.find("lshift+") != String::npos)
        {
            modifiers.set(ModifierKey::LSHIFT);
        }
        if (modifierString.find("rshift+") != String::npos)
        {
            modifiers.set(ModifierKey::RSHIFT);
        }

        return modifiers;
    }

    KeyCodes::Enum UserInputHelper::GetKeyCode(const String& keyString)
    {
        auto codes = GetKeyCodeReadableDb();

        for (size_t i = 0; i < codes.size(); i++)
        {
            auto& readable = codes[i];
            if (keyString == readable)
            {
                return static_cast<KeyCodes::Enum>(i);
            }
        }

        return KeyCodes::MAX;
    }

}  // namespace utils
}  // namespace red