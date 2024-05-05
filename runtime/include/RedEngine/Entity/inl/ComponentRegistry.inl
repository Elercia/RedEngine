namespace red
{
template <typename COMP>
bool ComponentRegistry::RegisterComponent(bool isSingleton)
{
    auto construct = [](void* address) -> void
    {
        COMP* ptr = (COMP*)address;

        new (ptr) COMP();
    };

    auto destruct = [](void* address) -> void
    {
        COMP* ptr = (COMP*)address;

        (ptr)->~COMP();
    };


    return RegisterComponent(TypeInfo<COMP>(), isSingleton, sizeof(COMP), construct, destruct);
}

template <typename COMP>
const ComponentMetadata* ComponentRegistry::GetRegisteredComponentMetadata() const
{
    return GetRegisteredComponentMetadata(TypeInfo<COMP>().typeId);
}

}  // namespace red