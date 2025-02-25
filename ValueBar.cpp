#include "ValueBar.h"

ValueBar::ValueBar(QGraphicsScene* scene,
                     const QPointF& pos,
                     const QColor& color,
                     const QString& label,
                     bool isBlood,
                     int maxValue,
                     int originalValue)
    : m_maxValue(maxValue),
      m_color(color),
      m_isEnemy(label == "ENEMY"),
      m_isBlood(isBlood)
{
    if(!m_isBlood){
        m_barRect=QRectF {0, 0, 150, 20};
    }
    // 创建背景条
    m_background = new QGraphicsRectItem(m_barRect);
    m_background->setPos(pos);
    m_background->setBrush(QColor(50, 50, 50));
    m_background->setPen(Qt::NoPen);
    scene->addItem(m_background);

    // 创建前景条
    m_foreground = new QGraphicsRectItem(m_barRect);
    m_foreground->setPos(pos);
    m_foreground->setBrush(color);
    m_foreground->setPen(Qt::NoPen);
    scene->addItem(m_foreground);

    // 创建标签
    if(m_isBlood){
        m_label = new QGraphicsTextItem(label);
        m_label->setPos(pos.x(), pos.y() - 20);
        m_label->setDefaultTextColor(Qt::white);
        m_label->setFont(QFont("Times New Roman", 16));
        scene->addItem(m_label);
    }else{
        createTicks();
    }

    updateValue(originalValue);
}

void ValueBar::createTicks(){
    const int tickCount = m_maxValue / 5;  // 每5点一个刻度
    const qreal tickSpacing = m_barRect.width() / (tickCount);
    for (int i = 1; i < tickCount; ++i) {
        QGraphicsLineItem* tick = new QGraphicsLineItem();

        // 设置刻度样式
        QPen pen(QColor(50, 50, 50), 3);
        pen.setCosmetic(true);  // 保持线条粗细不受缩放影响
        tick->setPen(pen);

        // 根据血条类型设置位置
        if (m_isEnemy) {
            tick->setLine(m_barRect.right() - i * tickSpacing, 1,
                          m_barRect.right() - i * tickSpacing, m_barRect.height()-2);
        } else {
            tick->setLine(i * tickSpacing, 1,
                          i * tickSpacing, m_barRect.height()-2);
        }

        tick->setParentItem(m_foreground); // 相对于血条背景定位
        m_ticks.append(tick);
    }
}

void ValueBar::updateValue(int Value)
{
    qreal widthFactor = static_cast<qreal>(Value) / m_maxValue;
    QRectF rect = m_barRect;

    if (m_isEnemy) {
        // 敌人从右侧收缩
        qreal newWidth = rect.width() * widthFactor;
        rect.setLeft(rect.width() - newWidth);
        rect.setWidth(newWidth);
        m_foreground->setRect(rect);
        m_foreground->setPos(m_background->pos().x() + (rect.width() - newWidth),
                             m_background->pos().y());
    } else {
        // 玩家正常收缩
        rect.setWidth(rect.width() * widthFactor);
        m_foreground->setRect(rect);
    }
}
