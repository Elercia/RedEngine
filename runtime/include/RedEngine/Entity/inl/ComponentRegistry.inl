namespace red
{
template <typename COMP>
bool ComponentRegistry::RegisterComponent(bool isSingleton)
{
    return RegisterComponent(TypeInfo<COMP>(), isSingleton);
}

template <typename COMP>
const ComponentMetadata* ComponentRegistry::GetRegisteredComponentMetadata() const
{
    return GetRegisteredComponentMetadata(TypeInfo<COMP>().typeId);
}
}  // namespace red