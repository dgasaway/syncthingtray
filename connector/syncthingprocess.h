#ifndef DATA_SYNCTHINGPROCESS_H
#define DATA_SYNCTHINGPROCESS_H

#include "./global.h"

#include <c++utilities/chrono/datetime.h>

#include <QProcess>
#include <QTimer>

namespace Data {

class LIB_SYNCTHING_CONNECTOR_EXPORT SyncthingProcess : public QProcess {
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning)
    Q_PROPERTY(CppUtilities::DateTime activeSince READ activeSince)
    Q_PROPERTY(bool manuallyStopped READ isManuallyStopped)

public:
    explicit SyncthingProcess(QObject *parent = nullptr);
    bool isRunning() const;
    CppUtilities::DateTime activeSince() const;
    bool isActiveFor(unsigned int atLeastSeconds) const;
    bool isManuallyStopped() const;
    static SyncthingProcess *mainInstance();
    static void setMainInstance(SyncthingProcess *mainInstance);
    static QStringList splitArguments(const QString &arguments);

Q_SIGNALS:
    void confirmKill();

public Q_SLOTS:
    void restartSyncthing(const QString &program, const QStringList &arguments);
    void startSyncthing(const QString &program, const QStringList &arguments);
    void stopSyncthing();
    void killSyncthing();

private Q_SLOTS:
    void handleStarted();
    void handleFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void killToRestart();

private:
    QString m_program;
    QStringList m_arguments;
    CppUtilities::DateTime m_activeSince;
    QTimer m_killTimer;
    bool m_manuallyStopped;
    static SyncthingProcess *s_mainInstance;
};

inline bool SyncthingProcess::isRunning() const
{
    return state() != QProcess::NotRunning;
}

inline CppUtilities::DateTime SyncthingProcess::activeSince() const
{
    return m_activeSince;
}

inline bool SyncthingProcess::isActiveFor(unsigned int atLeastSeconds) const
{
    return !m_activeSince.isNull() && (CppUtilities::DateTime::gmtNow() - m_activeSince).totalSeconds() > atLeastSeconds;
}

inline bool SyncthingProcess::isManuallyStopped() const
{
    return m_manuallyStopped;
}

inline SyncthingProcess *SyncthingProcess::mainInstance()
{
    return s_mainInstance;
}

inline void SyncthingProcess::setMainInstance(SyncthingProcess *mainInstance)
{
    s_mainInstance = mainInstance;
}

} // namespace Data

#endif // DATA_SYNCTHINGPROCESS_H
