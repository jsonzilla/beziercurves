#include "pathstroke.h"

#include <QApplication>

//----------------------------------------------------------------

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(pathstroke);

    QApplication app(argc, argv);

    PathStrokeWidget pathStrokeWidget;
    QStyle *arthurStyle = new ArthurStyle();
    pathStrokeWidget.setStyle(arthurStyle);
    QList<QWidget *> widgets = pathStrokeWidget.findChildren<QWidget *>();
    foreach (QWidget *w, widgets) {
        w->setStyle(arthurStyle);
        w->setAttribute(Qt::WA_AcceptTouchEvents);
    }
    pathStrokeWidget.show();

#ifdef QT_KEYPAD_NAVIGATION
    QApplication::setNavigationMode(Qt::NavigationModeCursorAuto);
#endif
    return app.exec();
}

//----------------------------------------------------------------