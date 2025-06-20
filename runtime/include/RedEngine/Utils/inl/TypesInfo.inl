namespace red
{
template <typename T>
constexpr TypeTraits TypeInfo()
{
    static_assert(std::is_pointer_v<T> == false && std::is_const_v<T> == false && std::is_reference_v<T> == false);

    constexpr StringView full_name{RED_FUNCNAME};
    constexpr StringView left_marker{RED_FUNCNAME_START};
    constexpr StringView right_marker{RED_FUNCNAME_END};
    constexpr StringView function_struct_start{RED_FUNCNAME_STRUCT_START};
    constexpr StringView function_class_start{RED_FUNCNAME_CLASS_START};

    constexpr auto left_marker_index = full_name.find(left_marker);
    static_assert(left_marker_index != StringView::npos, "Type T is not starting with left_marker (unsupported compiler ?)");

    constexpr auto start_index = left_marker_index + left_marker.size();

    constexpr auto end_index = full_name.find(right_marker, left_marker_index);
    static_assert(end_index != StringView::npos, "Type T is not ending with right_marker (unsupported compiler ?)");

    constexpr auto length = end_index - start_index;

    constexpr auto typeName = full_name.substr(start_index, length);

    constexpr auto structSlicedTypeName =
        function_struct_start.empty() == false && typeName.find(function_struct_start) != StringView::npos
            ? typeName.substr(function_struct_start.length())
            : typeName;

    constexpr auto classSlicedTypeName =
        function_class_start.empty() == false && structSlicedTypeName.find(function_class_start) != StringView::npos
            ? structSlicedTypeName.substr(function_class_start.length())
            : structSlicedTypeName;

    constexpr auto hash = fnv1a32(classSlicedTypeName);

    constexpr bool triviallyConstructible = std::is_trivially_constructible<T>::value;
    constexpr bool triviallyDestructible = std::is_trivially_destructible<T>::value;

    constexpr TypeTraits typeTraits{.name = classSlicedTypeName,
                                    .typeId = hash,
                                    .size = sizeof(T),
                                    .triviallyConstructible = triviallyConstructible,
                                    .triviallyDestructible = triviallyDestructible};

    return typeTraits;
}

// TODO what to do with this parial initialisation of the type
TypeTraits GetTypeInfoFromTypeName(StringView name)
{
    auto hash = fnv1a32(name);

    TypeTraits typeTraits{name, hash};

    return typeTraits;
}
}  // namespace red