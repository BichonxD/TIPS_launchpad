#ifndef SON_H
#define SON_H

#include "fmod.hpp"
#include <QObject>

/**
 * @brief The Sound_mgr class will provide an interface to use FMOD library in this project
 */
class Sound_mgr : public QObject
{
    Q_OBJECT
public:
    explicit Sound_mgr(FMOD::System *sys, QString fileName, QObject *parent = 0);

signals:

public slots:

private:
    FMOD::System *_system;
    FMOD::Sound *_sound;
    QString _fileName;
};

#endif // SON_H
