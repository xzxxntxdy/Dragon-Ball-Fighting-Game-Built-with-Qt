#include "SoundEffect.h"
#include <QUrl>
#include <QTimer>

SoundEffect::SoundEffect(QObject *parent)
    : QObject(parent),
      m_mediaPlayer(new QMediaPlayer(this))
{
    // 连接媒体状态变化信号
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, &SoundEffect::handleMediaStatusChanged);
}

SoundEffect::~SoundEffect()
{
    stop();
}

void SoundEffect::load(const QString& filePath)
{
    // 转换为本地文件URL格式
    m_mediaPlayer->setMedia(QMediaContent(QUrl(filePath)));
}

void SoundEffect::play()
{
    if(m_mediaPlayer->mediaStatus() == QMediaPlayer::NoMedia) return;
    m_mediaPlayer->play();
}

void SoundEffect::stop()
{
    m_mediaPlayer->stop();
}

void SoundEffect::setVolume(int volume)
{
    // 确保音量在0-100范围
    m_mediaPlayer->setVolume(qBound(0, volume, 100));
}

void SoundEffect::setPlaybackSpeed(qreal speed)
{
    // 设置合理的速度范围 (0.5x - 2.0x)
    m_mediaPlayer->setPlaybackRate(qBound(0.5, speed, 2.0));
}

void SoundEffect::setLoop(bool loop)
{
    m_loop = loop;
}

qint64 SoundEffect::duration() const
{
    return m_mediaPlayer->duration();
}

void SoundEffect::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    // 循环播放逻辑
    if(status == QMediaPlayer::EndOfMedia && m_loop) {
        m_mediaPlayer->play();
    }
}
