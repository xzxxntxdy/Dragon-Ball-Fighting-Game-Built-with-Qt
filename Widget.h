#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <QString>
#include <QDebug>
#include <QEvent>
#include <bits/stdc++.h>
#include "AnimatedPixmapItem.h"
#include "ValueBar.h"
#include "CharacterBase.h"
#include "GokuRed.h"
#include "GokuBlue.h"
#include "Frieza.h"
#include "Gokussthree.h"
#include "CharacterSelection.h"
#include "Gogetass.h"
#include "Buu.h"
#include "SoundEffect.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(CharacterType heroType=Random, CharacterType enemyType=Random, QWidget *parent = nullptr);
    ~Widget();
    void updateWidget();
signals:
    void returnToMainMenu();
private:
    void rolemove();
    void enemymove();
    QGraphicsScene m_scene;
    QGraphicsView m_view;
    QGraphicsPixmapItem *m_background;
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    QTimer* moveTimer;
    CharacterBase *hero;
    CharacterBase *enemy;
    QList<int> m_keyList;
    QList<int> m_enemyKeyList;
    ValueBar* m_heroHealth;
    ValueBar* m_enemyHealth;
    ValueBar* m_heroEnergy;
    ValueBar* m_enemyEnergy;
    //回合系统
    void startNewRound();
    void showRoundNumber();
    void showCountDown();
    void checkRoundWinner();
    void checkRoundStart();
    void showFinalWinner();

    int m_currentRound = 1;
    int m_heroWins = 0;
    int m_enemyWins = 0;
    bool m_roundActive = false;
    QGraphicsTextItem* m_roundText = nullptr;
    QGraphicsTextItem* m_countdownText = nullptr;
    QTimer* m_roundStartTimer;
    CharacterType m_heroType;
    CharacterType m_enemyType;
    bool m_gameOver = false;
    SoundEffect* bgm;
};
#endif // WIDGET_H
