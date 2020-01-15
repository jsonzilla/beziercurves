#ifndef PATHSTROKE_H
#define PATHSTROKE_H

#include "arthurwidgets.h"

#include <QtWidgets>

//----------------------------------------------------------------

class PathStrokeRenderer : public ArthurFrame
{
    Q_OBJECT
public:
    explicit PathStrokeRenderer(QWidget *parent);

    void paint(QPainter *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent* e) override {};
    bool event(QEvent *e) override;

    QSize sizeHint() const override { return QSize(500, 500); }

signals:
    void clicked();

private:
    void initializePoints();
    void updatePoints();

    int m_pointCount;
    int m_pointSize;
    int m_activePoint;
    QVector<QPointF> m_points;
    QVector<QPointF> m_vectors;

    qreal m_penWidth = 20;
    Qt::PenJoinStyle m_joinStyle = Qt::BevelJoin;
    Qt::PenCapStyle m_capStyle = Qt::RoundCap;
    Qt::PenStyle m_penStyle;

    QPoint m_mousePress;
    bool m_mouseDrag;

    QHash<int, int> m_fingerPointMapping;
};

//----------------------------------------------------------------

class PathStrokeWidget : public QWidget
{
    Q_OBJECT

public:
    PathStrokeWidget();
    void setStyle(QStyle* style) {
      QWidget::setStyle(style);
    }

private:
    PathStrokeRenderer *m_renderer;
};

#endif // PATHSTROKE_H
