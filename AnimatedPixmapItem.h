#ifndef ANIMATEDPIXMAPITEM_H
#define ANIMATEDPIXMAPITEM_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QMap>
#include <QPointF>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDir>

class AnimatedPixmapItem : public QObject
{
    Q_OBJECT
public:
    // 构造函数：接受目标PixmapItem、图片路径和图片数量
    AnimatedPixmapItem(QGraphicsPixmapItem* targetItem,
                      const QString& imagePath,
                      int imageCount,
                      QObject* parent = nullptr);

    // 设置目标PixmapItem（允许运行时更换目标）
    void setTargetItem(QGraphicsPixmapItem* targetItem);

    // 动画控制方法
    void startAnimation();
    void stopAnimation();

    // 动画参数配置
    void setFrameRate(int fps);
    void setLooping(bool loop);
    void setPerFrameMovement(int frameIndex, const QPointF& movement);
    void setPerFrameMovement(const QPointF& movement);
    void setPerFramePos(int frameIndex, const QPointF& pos);
    void setFrameDuration(int frameIndex, int duration);
    void setScaleFactor(double scaleFactor);
    void addpixmap(QPixmap pixmap);
    void setEndFrameNum(int endFrameNum);
    void setcombo(bool iscombo);
    void setFrameCollision(int frameIndex, const QRectF& area);
    void setSegmentLoop(int startFrame, int endFrame, int loopCount);
    void transform(bool isTransform);
    int getCurrentFrame();
    QRectF currentCollisionRect() const;

signals:
    void animationStopped();
    void animationUpdated();

private slots:
    void updateFrame();

private:
    void updatePixmap();
    int getCurrentFrameDuration() const;
    void applyFrameMovement();

    QGraphicsPixmapItem* m_targetItem;  // 目标图形项
    QList<QPixmap> m_frames;            // 动画帧集合
    QTimer* m_animationTimer;           // 动画定时器

    // 动画状态
    int m_currentFrameIndex = 0;
    bool m_isLooping = false;

    // 动画参数
    QMap<int, QPointF> m_perFrameMovements;  // 帧相对位移参数
    QMap<int, int> m_frameDurations;        // 自定义帧持续时间
    QMap<int,QPointF> m_perFramePos;        //帧绝对位移参数
    QMap<int, QRectF> m_frameCollisions; // 帧索引 -> 碰撞区域
    int m_defaultFrameDuration = 100;       // 默认帧持续时间（ms）
    double m_scaleFactor = 1.0;              // 图像缩放因子
    bool m_isAnimating = false;
    int m_countFrame=1;                         //已播放帧数
    int m_endFrameNum=-1;                         //结束帧数
    bool m_iscombo=false;                   //是否连击
    int m_segmentLoopStart = -1;        // 循环起始帧
    int m_segmentLoopEnd = -1;          // 循环结束帧
    int m_segmentLoopCount = 0;         // 总循环次数
    int m_segmentCurrentLoop = 0;       // 当前剩余循环次数
    bool m_isInSegmentLoop = false;     // 是否处于片段循环中
    int m_facing=1;                       //朝右？1：-1
};

#endif // ANIMATEDPIXMAPITEM_H
