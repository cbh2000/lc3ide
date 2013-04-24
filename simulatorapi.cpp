#include "simulatorapi.h"

SimulatorAPI::SimulatorAPI(QObject *parent) :
    QObject(parent),
    p_breakpoints()
{
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

void SimulatorAPI::addBreakpoint(const Address &addr)
{
    p_breakpoints.insert(addr);
}

void SimulatorAPI::removeBreakpoint(const Address &addr)
{
    p_breakpoints.remove(addr);
}
