#include "sound_mgr.h"

Sound_mgr::Sound_mgr(FMOD::System *sys, QString fileName, QObject *parent) : QObject(parent) , _system(sys), _fileName(fileName)
{
    _system->CreateSound(_fileName.toStdString().c_str(), FMOD_CREATESAMPLE, 0, &_sound);
}
