#pragma once

#include "RedEngine/Core/Container/String.hpp"
#include "RedEngine/Math/Hash.hpp"
#include "RedEngine/Utils/Types.hpp"

namespace red
{
// Representation of a path
// Can be build using static methods from the game resources or from the user game path (usually MyDocuments or ~
// followed by the game name) The path status is refreshed on creation but can be refresh at anytime
class Path
{
public:
    // API to construct a path inside the user directory
    static Path User(const WString& path);
    // API to construct a path inside the game resources
    static Path Resource(const WString& path);

    // c-string like creators
    static Path Resource(const String& path);
    static Path User(const String& path);

private:
    static WString GetUserBasePath();
    static WString GetResourceBasePath();

public:
    Path();
    explicit Path(WString path);
    ~Path();

    WStringView GetPath() const;
    String GetAscciiPath() const;

    bool Exist() const;
    bool IsDirectory() const;
    bool IsFile() const;

    uint64 GetHash() const;

    Path& Append(const WStringView& str);

    void ForceRefreshStatus();  // Force the refresh of the path

    bool operator==(const Path& other) const;
    bool operator!=(const Path& other) const;

private:
    void SetUnicodePath(const WString& unicodeStr);

private:
    enum PathState
    {
        VALID = 1 << 0,      // The Path is a valid physical path or a valid virtual path (doesn't guarantee existence)
        EXIST = 1 << 1,      // The path exist on the disk
        TYPE_FILE = 1 << 2,  // 1 => file, 0 => directory (folder). Invalid if not exist
    };
    using PathStates = uint32;

private:
    WString m_unicodePath;
    PathStates m_states;
    uint32 m_hash;
};

template <>
inline uint64 Hash(const Path& value)
{
    return value.GetHash();
}
}  // namespace red

namespace std
{
template <>
struct less<red::Path>
{
    bool operator()(const red::Path& lhs, const red::Path& rhs) const
    {
        return lhs.GetHash() < rhs.GetHash();
    }
};
}  // namespace std
