#include "MainMenu.h"
#include "CharacterSelection.h"
#include "Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainMenu menu;
    menu.show();
    menu.playBgm();

    Widget* game = nullptr;
    CharacterSelection* selection = new CharacterSelection();

    QObject::connect(&menu, &MainMenu::randomBattleSelected, [&](){
        game = new Widget();
        game->show();
        menu.hide();
        menu.stopBgm();
        QObject::connect(game, &Widget::returnToMainMenu, [&]() {
            menu.show();
            game->deleteLater();
            menu.playBgm();
        });
    });

    QObject::connect(&menu, &MainMenu::characterSelectSelected, [&](){
        selection->show();
        menu.hide();
        menu.stopBgm();
        selection->playBgm();
    });

    QObject::connect(selection, &CharacterSelection::charactersSelected, [&](CharacterType p1, CharacterType p2){
        game = new Widget(p1, p2);
        game->show();
        selection->resetSelection();
        selection->hide();
        selection->stopBgm();
        QObject::connect(game, &Widget::returnToMainMenu, [&]() {
            menu.show();
            game->deleteLater();
            menu.playBgm();
        });
    });

    return a.exec();
}
