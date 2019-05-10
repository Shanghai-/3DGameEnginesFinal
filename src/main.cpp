#include <QApplication>
#include <QWindow>
#include <QScreen>
#include <iostream>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // We cannot use w.showFullscreen() here because on Linux that creates the
    // window behind all other windows, so we have to set it to fullscreen after
    // it has been shown. 
    w.show();

    // We want to show the game on the widest horizontal monitor available, so
    // we need to pick the most suitable one somehow. We will store that screen
    // pointer here.
    QScreen *targetScreen = nullptr;

    // Loop over all screens the app could possibly be on
    QListIterator<QScreen *> it(qApp->screens());
    while (it.hasNext()) {
        QScreen *s = it.next();

        if (targetScreen == nullptr) {
            // If we don't already have a screen we're considering, this one is our candidate
            targetScreen = s;

        } else if (s->isLandscape(s->orientation())) {
            if (targetScreen->isLandscape(targetScreen->orientation())) {
                // If we find a landscape screen, but our current candidate is also landscape,
                // we check to see which one is wider. If the new one is wider, we keep it.
                if (s->availableSize().width() > targetScreen->availableSize().width()) {
                    targetScreen = s;
                }
            } else {
                // If our old candidate wasn't landscape, we automatically choose the landscape one
                targetScreen = s;
            }
        }
    }

    if (targetScreen == nullptr) {
        // If we somehow don't have any screens on our computer (ssh maybe?), this will keep
        // our game from attempting to run.
        std::cerr << "ERROR: Did not find any screens which could run the game." << std::endl;
        a.quit();

    } else {
        // Otherwise, we set the game to belong to the target screen with setScreen,
        // then use setGeometry to actually place it on the target.
        w.windowHandle()->setScreen(targetScreen);
        w.setGeometry(targetScreen->geometry());

        // Finally, we showFullScreen to make it large.
        // If this call is commented out, the window will fill the screen but still
        // have a top bar with an x button
        w.showFullScreen();

        return a.exec();
    }

    return 1;
}

