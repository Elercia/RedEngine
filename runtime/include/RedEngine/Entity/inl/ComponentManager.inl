namespace red
{
template <typename COMP>
bool ComponentRegistry::RegisterComponent()
{
    return RegisterComponent(TypeInfo<COMP>());
}

template <typename COMP>
const ComponentMetadata* ComponentRegistry::GetRegisteredComponentMetadata() const
{
    return GetRegisteredComponentMetadata(TypeInfo<COMP>().typeId);
}
}  // namespace red