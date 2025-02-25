#ifndef VALUEBAR_H
#define VALUEBAR_H

#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QVector>

class ValueBar : public QObject
{
    Q_OBJECT
public:
    ValueBar(QGraphicsScene* scene,
             const QPointF& pos,
             const QColor& color,
             const QString& label,
             bool isBlood,
             int maxValue = 100,
             int originalValue=100);

    void updateValue(int Value);
    void createTicks();

private:
    QGraphicsRectItem* m_background;  // 背景
    QGraphicsRectItem* m_foreground;  // 前景
    QGraphicsTextItem* m_label;       // 标签
    const int m_maxValue;            // 最大值
    const QColor m_color;             // 颜色
    QRectF m_barRect{0, 0, 300, 20}; // 尺寸
    bool m_isEnemy;                   // 是否是敌人
    bool m_isBlood;                   //是否是血条
    QVector<QGraphicsLineItem*> m_ticks; //刻度线
};

#endif // VALUEBAR_H
