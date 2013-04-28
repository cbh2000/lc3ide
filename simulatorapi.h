#ifndef SIMULATORAPI_H
#define SIMULATORAPI_H

#include <QObject>
#include <QSet>
#include <QProcess>
#include "settings.h"

class SimulatorAPI : public QObject
{
    Q_OBJECT
public:
    explicit SimulatorAPI(QObject *parent = 0);

    enum SimulatorState {
        Initializing,
        Running,
        Paused,
        Halted,
        Error,
        BreakpointReached
    };

    class Register;

    // All LC-3 Addresses may use either this class or Address::Type.
    struct Address {
        typedef qint32 Type;
        Type value;

        Address(const Type initValue);
        QString hexString();

        operator Type() const;
        Address operator +(const Address& other) const;
        Address operator -(const Address& other) const;
        bool operator ==(const Address& other) const;
    };

    const QSet<Address>& breakpoints() const;
    bool isBreakpointAt(const Address& addr) const;
    
signals:
    void stateChanged(SimulatorAPI *simulator, SimulatorState state);
    
public slots:
    void addBreakpoint(const Address& addr);
    void removeBreakpoint(const Address& addr);
    void toggleBreakpoint(const Address& addr);

private:
    SimulatorState previousState;
    SimulatorState state;
    QSet<Address> p_breakpoints;
//    Register p_registers[LC3_REGISTER_COUNT];
    QProcess process;

    struct Simulator : public QObject {
        Simulator(QObject* parent = 0)
            : QObject(parent),
              lumetta_lc3(this),
              executablePath("/home/bryan/Downloads/lc3tools/lc3sim")
        {

        }

        QProcess lumetta_lc3;
        void start() {

        }

        QString executablePath;
    };
};

//// FIXME: This class should not contain a copy of the value.
////  It should get it from the simulator.
//class SumulatorAPI::Register : public QObject
//{
//    Q_OBJECT

//public:
//    typedef LC3_WORD_TYPE ValueType;
//    Register() {}
    
//    ValueType value() { return 0; }
    
//public slots:
//    void setValue(const ValueType& value) { emit valueChanged(this); return; }
    
//signals:
//    void valueChanged(Register *reg);
//};

#endif // SIMULATORAPI_H
