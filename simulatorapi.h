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

    struct Register {
        typedef quint8 Type;

    };

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

#endif // SIMULATORAPI_H
