#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <QObject>
#include <QMediaPlayer>

class SoundEffect : public QObject
{
    Q_OBJECT
public:
    explicit SoundEffect(QObject *parent = nullptr);
    ~SoundEffect();

    // 基本控制接口
    void load(const QString& filePath);    // 加载音效文件
    void play();                            // 开始播放
    void stop();                            // 停止播放

    // 参数控制接口
    void setVolume(int volume);             // 设置音量 (0-100)
    void setPlaybackSpeed(qreal speed);     // 设置播放速度 (1.0=正常速度)
    void setLoop(bool loop);                // 设置循环播放

    // 状态查询接口
    qint64 duration() const;               // 获取音效总时长（毫秒）

private slots:
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer* m_mediaPlayer;
    bool m_loop = false;                   // 循环播放标志
};

#endif // SOUNDEFFECT_H
