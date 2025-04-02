#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include "SoundEffect.h"

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);
    void stopBgm();
    void playBgm();

signals:
    void randomBattleSelected();
    void characterSelectSelected();

private:
    QPushButton* createMenuButton(const QString& text);
    SoundEffect* bgm;
};

#endif // MAINMENU_H
