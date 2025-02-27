#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);

signals:
    void randomBattleSelected();
    void characterSelectSelected();

private:
    QPushButton* createMenuButton(const QString& text);
};

#endif // MAINMENU_H
