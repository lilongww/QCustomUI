#pragma once

#include "Object.h"

#include <vector>

namespace OpenVisa
{
class Object::CommonCommand
{
public:
    void cls();
    void setEse(int status);
    int ese();
    int esr();
    std::string idn();
    void setOpc();
    bool opc();
    std::vector<std::string> opt();
    void rst();
    void setSre(int status);
    int sre();
    int stb();
    bool tst();
    void wai();
    void rcl(int i);

private:
    inline CommonCommand(Object* parent) : m_parent(parent) {};
    friend Object;
    Object* m_parent;
};
} // namespace OpenVisa
