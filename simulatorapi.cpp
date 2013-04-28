#include "simulatorapi.h"

SimulatorAPI::SimulatorAPI(QObject *parent) :
    QObject(parent),
    p_breakpoints()
{
//    for (int i = 0; i < sizeof(p_registers) / sizeof(p_registers[0]); ++i) {
//        p_register[i] = Register;
//    }
}

SimulatorAPI::Address::Address(const Type initValue)
    : value(initValue)
{
}

SimulatorAPI::Address::operator Type() const
{
    return value;
}

SimulatorAPI::Address
SimulatorAPI::Address::operator +(const Address& other) const
{
    return value + other.value;
}

SimulatorAPI::Address
SimulatorAPI::Address::operator -(const Address& other) const
{
    return Address(value - other.value);
}

bool SimulatorAPI::Address::operator ==(const Address& other) const
{
    return value == other.value;
}

QString SimulatorAPI::Address::hexString()
{
    QString out = QString::number(value, 16); // To hex
    while (out.length() < 4) out.prepend('0');
    return 'x' + out;
}

const QSet<SimulatorAPI::Address>&
SimulatorAPI::breakpoints() const
{
    return p_breakpoints;
}

bool SimulatorAPI::isBreakpointAt(const Address &addr) const
{
    return p_breakpoints.contains(addr);
}

void SimulatorAPI::addBreakpoint(const Address &addr)
{
    p_breakpoints.insert(addr);
}

void SimulatorAPI::removeBreakpoint(const Address &addr)
{
    p_breakpoints.remove(addr);
}

void SimulatorAPI::toggleBreakpoint(const Address &addr)
{
    if (p_breakpoints.contains(addr)) {
        p_breakpoints.remove(addr);
    } else {
        p_breakpoints.insert(addr);
    }
}
