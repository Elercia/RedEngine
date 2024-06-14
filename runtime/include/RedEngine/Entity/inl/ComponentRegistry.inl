namespace red
{
template <typename COMP>
bool ComponentRegistry::RegisterComponent()
{
    static_assert(std::is_polymorphic<COMP>::value == false, "Registering a virtual component, this is not permited");

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

    using CompTraits = ComponentTraits<COMP>;

    return RegisterComponent(TypeInfo<COMP>(), CompTraits::IsSingleton, sizeof(COMP), construct, destruct);
}

template <typename COMP>
const ComponentMetadata* ComponentRegistry::GetRegisteredComponentMetadata() const
{
    return GetRegisteredComponentMetadata(TypeInfo<COMP>().typeId);
}

}  // namespace red