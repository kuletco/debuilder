#ifndef DEBFILE_H
#define DEBFILE_H

#include <QObject>
#include <QPointer>
#include <QFile>
#include <QProcess>

namespace DEBAttrs
{
    Q_NAMESPACE

    enum class Type {
        deb,
        udeb,
        invalid = -1,
    };
    Q_ENUM_NS(Type)

    enum class Architecture {
        all,
        amd64,
        i386,
        arm64,
        armel,
        arm,
        powerpc64,
        powerpc,
        mips64el,
        mips,
        invalid = -1,
    };
    Q_ENUM_NS(Architecture)

    enum class Priority {
        required,
        standard,
        optional,
        extra,
        invalid = -1,
    };
    Q_ENUM_NS(Priority)

    enum class Section {
        database,
        editors,
        electronics,
        embedded,
        fonts,
        games,
        graphics,
        interpreters,
        mail,
        misc,
        net,
        news,
        science,
        sound,
        text,
        utils,
        video,
        web,
        invalid = -1,
    };
    Q_ENUM_NS(Section)
} // namespace DEBAttrs

class DEBFile : public QObject
{
    Q_OBJECT
public:

    QString m_name;
    QString m_version;
    QString m_maintainer;
    QString m_homepage;
    QString m_summary;
    QString m_description;
    QString m_depends;
    QString m_predepends;
    QString m_conflicts;
    QString m_replaces;
    QString m_provides;
    bool m_protected;
    DEBAttrs::Type m_type;
    DEBAttrs::Architecture m_architecture;
    DEBAttrs::Priority m_priority;
    DEBAttrs::Section m_section;

    QString m_contents_control;
    QString m_contents_preinst;
    QString m_contents_postinst;
    QString m_contents_prerm;
    QString m_contents_postrm;

    explicit DEBFile(QObject *parent = nullptr);
    ~DEBFile();

    qint32 error() const { return m_error; }
    const QString &errStr() const { return m_errstr; }

    const QString &filename();

    qint64 CreateControlFile();
    qint64 CreateScriptFiles();

    void ClearBuildDir();
    void CreateBuildDir();
    void CreatePackage(const QString &debfile);

protected:
    qint64 CreateTextFile(const QString &file, QFileDevice::Permissions permission, const QString &contents);
    qint64 CreateScriptFile(const QString &script_file, const QString &contents);

private slots:
    void worker_started();
    void worker_finished(int exitCode, QProcess::ExitStatus exitStatus = QProcess::NormalExit);
    void worker_error(QProcess::ProcessError error);

signals:
    void work_started(QString info);
    void work_finished(QString info);
    void work_failed(QString info);

private:
    qint32 m_error;
    QString m_errstr;

    QString m_build_dir;
    QString m_filename;
    QString m_file;

    QPointer<QProcess> m_worker;
};

#endif // DEBFILE_H