#pragma once

namespace red
{
struct System
{
    virtual void Init(){};
    virtual void Finalize(){};
    virtual void Update(){};
};
}  // namespace red