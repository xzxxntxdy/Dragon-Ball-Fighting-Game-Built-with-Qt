#include "AnimatedPixmapItem.h"

AnimatedPixmapItem::AnimatedPixmapItem(QGraphicsPixmapItem* targetItem,
                                     const QString& imagePath,
                                     int imageCount,
                                     QObject* parent)
    : QObject(parent)
    , m_targetItem(targetItem)
{
    // 加载动画帧
    for (int i = 0; i < imageCount; ++i) {
        QString path = QString("%1%2.png").arg(imagePath).arg(i);
        QPixmap pixmap(path);
        if (!pixmap.isNull()) {
            m_frames.append(pixmap);
        }
    }

    // 初始化定时器
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &AnimatedPixmapItem::updateFrame);
}

void AnimatedPixmapItem::setTargetItem(QGraphicsPixmapItem* targetItem)
{
    if (m_targetItem != targetItem) {
        stopAnimation();
        m_targetItem = targetItem;
    }
}

void AnimatedPixmapItem::setEndFrameNum(int endFrameNum)
{
    m_endFrameNum=endFrameNum;
}

void AnimatedPixmapItem::setSegmentLoop(int startFrame, int endFrame, int loopCount) {
    // 参数有效性检查
    if (startFrame >= 0 && endFrame < m_frames.size() && startFrame <= endFrame && loopCount >= 0) {
        m_segmentLoopStart = startFrame;
        m_segmentLoopEnd = endFrame;
        m_segmentLoopCount = loopCount;
        m_segmentCurrentLoop = 0;
        m_isInSegmentLoop = false;
    }
}


void AnimatedPixmapItem::startAnimation()
{
    if (!m_isAnimating && m_targetItem && !m_frames.empty()) {
        if(!m_iscombo) m_currentFrameIndex=0;
        m_countFrame=1;
        updatePixmap();
        applyFrameMovement();
        m_animationTimer->start(getCurrentFrameDuration());
        m_isAnimating = true;
        m_isInSegmentLoop = false;
    }
}

void AnimatedPixmapItem::stopAnimation()
{
    if (m_isAnimating) {
        m_animationTimer->stop();
        m_isAnimating = false;
    }
}

void AnimatedPixmapItem::updateFrame()
{
    if (!m_targetItem || m_frames.empty()) return;

    // 更新帧索引
    m_currentFrameIndex++;

    // 处理片段循环逻辑
    if (m_isInSegmentLoop) {
        if (m_currentFrameIndex > m_segmentLoopEnd) {
            m_segmentCurrentLoop--;
            if (m_segmentCurrentLoop > 0) {
                m_currentFrameIndex = m_segmentLoopStart;
            } else {
                m_isInSegmentLoop = false;
                m_currentFrameIndex = m_segmentLoopEnd + 1;
            }
        }
    } else if (m_segmentLoopCount > 0 && m_currentFrameIndex == m_segmentLoopStart) {
        // 进入片段循环
        m_isInSegmentLoop = true;
        m_segmentCurrentLoop = m_segmentLoopCount;
    }

    // 检查是否需要停止动画（非循环模式）
    bool shouldStop = false;
    if (!m_isLooping) {
        if (m_currentFrameIndex >= m_frames.size()) {
            shouldStop = true;
        } else if (m_endFrameNum != -1 && m_countFrame >= m_endFrameNum) {
            shouldStop = true;
        }
    }

    if (shouldStop) {
        m_currentFrameIndex = m_currentFrameIndex % m_frames.size();
        stopAnimation();
        emit animationStopped();
        return;
    }

    // 全局循环处理
    if (m_isLooping) {
        m_currentFrameIndex = m_currentFrameIndex % m_frames.size();
    } else {
        // 确保当前帧不越界
        if (m_currentFrameIndex >= m_frames.size()) {
            m_currentFrameIndex = m_frames.size() - 1;
        }
    }

    // 更新显示和其他处理
    m_countFrame++;
    updatePixmap();
    if(!m_isInSegmentLoop||(m_isInSegmentLoop && m_segmentCurrentLoop == m_segmentLoopCount)){
        applyFrameMovement();
    }

    // 更新定时器间隔
    m_animationTimer->setInterval(getCurrentFrameDuration());
}

void AnimatedPixmapItem::updatePixmap()
{
    QPixmap pixmap = m_frames[m_currentFrameIndex];
    if (m_scaleFactor != 1.0) {
        pixmap = pixmap.scaled(pixmap.size() * m_scaleFactor,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
    }
    m_targetItem->setPixmap(pixmap);
    emit animationUpdated();
}

int AnimatedPixmapItem::getCurrentFrameDuration() const
{
    return m_frameDurations.value(m_currentFrameIndex, m_defaultFrameDuration);
}

void AnimatedPixmapItem::applyFrameMovement()
{
    if (m_perFrameMovements.contains(m_currentFrameIndex)) {
        QPointF movement = m_perFrameMovements[m_currentFrameIndex];
        m_targetItem->moveBy(movement.x()*m_facing, movement.y());
    }
    if (m_perFramePos.contains(m_currentFrameIndex)) {
        QPointF pos = m_perFramePos[m_currentFrameIndex];
        m_targetItem->setPos(pos);
    }
}

void AnimatedPixmapItem::setPerFrameMovement(int frameIndex, const QPointF &movement){
    m_perFrameMovements[frameIndex]=movement;
}

void AnimatedPixmapItem::setPerFrameMovement(const QPointF &movement){
    for(int i=0;i<m_frames.size();i++)
    m_perFrameMovements[i]=movement;
}

void AnimatedPixmapItem::setPerFramePos(int frameIndex, const QPointF &pos){
    m_perFramePos[frameIndex]=pos;
}


void AnimatedPixmapItem::setFrameDuration(int frameIndex, int duration){
    m_frameDurations[frameIndex]=duration;
}

void AnimatedPixmapItem::setLooping(bool loop){
    m_isLooping=loop;
}

void AnimatedPixmapItem::setFrameRate(int fps){
    m_defaultFrameDuration=1000/fps;
}

void AnimatedPixmapItem::setScaleFactor(double scaleFactor){
    m_scaleFactor=scaleFactor;
}

void AnimatedPixmapItem::setcombo(bool iscombo){
    m_iscombo=iscombo;
}

void AnimatedPixmapItem::setFrameCollision(int frameIndex, const QRectF& area) {
    m_frameCollisions[frameIndex] = area;
}

int AnimatedPixmapItem::getCurrentFrame(){
    return m_currentFrameIndex;
}

void AnimatedPixmapItem::addpixmap(QPixmap pixmap){
    m_frames.append(pixmap);
}

void AnimatedPixmapItem::transform(bool isTransform){
    if(isTransform){
        m_facing=-1;
    }else{
        m_facing=1;
    }
}

QRectF AnimatedPixmapItem::currentCollisionRect() const {
    if (!m_targetItem || !m_frameCollisions.contains(m_currentFrameIndex))
        return QRectF();

    // 将局部碰撞区域转换为场景坐标
    QRectF localRect = m_frameCollisions[m_currentFrameIndex];
    return m_targetItem->mapRectToScene(localRect);
}
