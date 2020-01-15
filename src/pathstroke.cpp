#include "arthurstyle.h"
#include "arthurwidgets.h"
#include "pathstroke.h"

#include <stdio.h>

namespace Curves {
  class BezierCurve {
  public:
    std::vector<double> FactorialLookup;

    BezierCurve()
    {
      CreateFactorialTable();
    }

    // just check if n is appropriate, then return the result
    double factorial(int n)
    {
      if (n < 0) { throw "n is less than 0"; }
      if (n > 32) { throw "n is greater than 32"; }

      return FactorialLookup[n]; /* returns the value n! as a SUMORealing point number */
    }

    void CreateFactorialTable()
    {
      // fill untill n=32. The rest is too high to represent
      std::vector<double> a(33);
      a[0] = 1.0;
      a[1] = 1.0;
      a[2] = 2.0;
      a[3] = 6.0;
      a[4] = 24.0;
      a[5] = 120.0;
      a[6] = 720.0;
      a[7] = 5040.0;
      a[8] = 40320.0;
      a[9] = 362880.0;
      a[10] = 3628800.0;
      a[11] = 39916800.0;
      a[12] = 479001600.0;
      a[13] = 6227020800.0;
      a[14] = 87178291200.0;
      a[15] = 1307674368000.0;
      a[16] = 20922789888000.0;
      a[17] = 355687428096000.0;
      a[18] = 6402373705728000.0;
      a[19] = 121645100408832000.0;
      a[20] = 2432902008176640000.0;
      a[21] = 51090942171709440000.0;
      a[22] = 1124000727777607680000.0;
      a[23] = 25852016738884976640000.0;
      a[24] = 620448401733239439360000.0;
      a[25] = 15511210043330985984000000.0;
      a[26] = 403291461126605635584000000.0;
      a[27] = 10888869450418352160768000000.0;
      a[28] = 304888344611713860501504000000.0;
      a[29] = 8841761993739701954543616000000.0;
      a[30] = 265252859812191058636308480000000.0;
      a[31] = 8222838654177922817725562880000000.0;
      a[32] = 263130836933693530167218012160000000.0;
      FactorialLookup = a;
    }

    double Ni(int n, int i)
    {
      double ni;
      double a1 = factorial(n);
      double a2 = factorial(i);
      double a3 = factorial(n - i);
      ni = a1 / (a2 * a3);
      return ni;
    }

    // Calculate Bernstein basis
    double Bernstein(int n, int i, double t)
    {
      double basis;
      double ti; /* t^i */
      double tni; /* (1 - t)^i */

      /* Prevent problems with pow */

      if (t == 0.0 && i == 0)
        ti = 1.0;
      else
        ti = std::pow(t, i);

      if (n == i && t == 1.0)
        tni = 1.0;
      else
        tni = std::pow((1 - t), (n - i));

      //Bernstein basis
      basis = Ni(n, i) * ti * tni;
      return basis;
    }

    void Bezier2D(std::vector<double> b, int cpts, std::vector<double> p)
    {
      size_t npts = b.size() / 2;
      int icount, jcount;
      double step, t;

      // Calculate points on curve

      icount = 0;
      t = 0;
      step = (double)1.0 / (cpts - 1u);

      for (int i1 = 0; i1 != cpts; i1++)
      {
        if ((1.0 - t) < 5e-6)
          t = 1.0;

        jcount = 0;
        p[icount] = 0.0;
        p[icount + 1u] = 0.0;
        for (int i = 0; i != npts; i++)
        {
          double basis = Bernstein(npts - 1, i, t);
          p[icount] += basis * b[jcount];
          p[icount + 1u] += basis * b[jcount + 1u];
          jcount = jcount + 2;
        }

        icount += 2;
        t += step;
      }
    }
  };
}

extern void draw_round_rect(QPainter *p, const QRect &bounds, int radius);

PathStrokeWidget::PathStrokeWidget()
{
    setWindowTitle(tr("Path Stroking"));

    // Widget construction and property setting
    m_renderer = new PathStrokeRenderer(this);

    // Layouting
    QHBoxLayout *viewLayout = new QHBoxLayout(this);
    viewLayout->addWidget(m_renderer);
}

PathStrokeRenderer::PathStrokeRenderer(QWidget *parent)
    : ArthurFrame(parent)
{
    m_pointSize = 10;
    m_activePoint = -1;
    m_capStyle = Qt::FlatCap;
    m_joinStyle = Qt::BevelJoin;
    m_penWidth = 1;
    m_penStyle = Qt::SolidLine;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_AcceptTouchEvents);
}

void PathStrokeRenderer::paint(QPainter *painter)
{
    if (m_points.isEmpty())
        initializePoints();

    painter->setRenderHint(QPainter::Antialiasing);

    QPalette pal = palette();
    painter->setPen(Qt::NoPen);

    // Construct the path
    QPainterPath path;
    path.moveTo(m_points.at(0));

    int i=1;
    while (i + 2 < m_points.size()) {
        path.cubicTo(m_points.at(i), m_points.at(i+1), m_points.at(i+2));
        i += 3;
    }
    while (i < m_points.size()) {
        path.lineTo(m_points.at(i));
        ++i;
    }


    // Draw the path
    {
        QColor lg = Qt::blue;

        // The "custom" pen
        if (m_penStyle == Qt::NoPen) {
            QPainterPathStroker stroker;
            stroker.setWidth(m_penWidth);
            stroker.setJoinStyle(m_joinStyle);
            stroker.setCapStyle(m_capStyle);

            QVector<qreal> dashes;
            qreal space = 4;
            dashes << 1 << space
                   << 3 << space
                   << 9 << space
                   << 27 << space
                   << 9 << space
                   << 3 << space;
            stroker.setDashPattern(dashes);
            QPainterPath stroke = stroker.createStroke(path);
            painter->fillPath(stroke, lg);

        } else {
            QPen pen(lg, m_penWidth, m_penStyle, m_capStyle, m_joinStyle);
            painter->strokePath(path, pen);
        }
    }

    if (1) {
        // Draw the control points
        painter->setPen(QColor(50, 100, 120, 200));
        painter->setBrush(QColor(200, 200, 210, 120));
        for (int i=0; i<m_points.size(); ++i) {
            QPointF pos = m_points.at(i);
            painter->drawEllipse(QRectF(pos.x() - m_pointSize,
                                       pos.y() - m_pointSize,
                                       m_pointSize*2, m_pointSize*2));
        }
        painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawPolyline(m_points);
    }
}

void PathStrokeRenderer::initializePoints()
{
    const int count = 7;
    m_points.clear();
    m_vectors.clear();

    QMatrix m;
    qreal rot = 360.0 / count;
    QPointF center(width() / 2, height() / 2);
    QMatrix vm;
    vm.shear(2, -1);
    vm.scale(3, 3);

    for (int i=0; i<count; ++i) {
        m_vectors << QPointF(.1f, .25f) * (m * vm);
        m_points << QPointF(0, 100) * m + center;
        m.rotate(rot);
    }
}

void PathStrokeRenderer::updatePoints()
{
    qreal pad = 10;
    qreal left = pad;
    qreal right = width() - pad;
    qreal top = pad;
    qreal bottom = height() - pad;

    Q_ASSERT(m_points.size() == m_vectors.size());
    for (int i=0; i<m_points.size(); ++i) {
        QPointF pos = m_points.at(i);
        QPointF vec = m_vectors.at(i);
        pos += vec;
        if (pos.x() < left || pos.x() > right) {
            vec.setX(-vec.x());
            pos.setX(pos.x() < left ? left : right);
        } if (pos.y() < top || pos.y() > bottom) {
            vec.setY(-vec.y());
            pos.setY(pos.y() < top ? top : bottom);
        }
        m_points[i] = pos;
        m_vectors[i] = vec;
    }
    update();
}

void PathStrokeRenderer::mousePressEvent(QMouseEvent *e)
{
    if (!m_fingerPointMapping.isEmpty())
        return;
    m_activePoint = -1;
    qreal distance = -1;
    for (int i=0; i<m_points.size(); ++i) {
        qreal d = QLineF(e->pos(), m_points.at(i)).length();
        if ((distance < 0 && d < 8 * m_pointSize) || d < distance) {
            distance = d;
            m_activePoint = i;
        }
    }

    if (m_activePoint != -1) {
        mouseMoveEvent(e);
    }

    // If we're not running in small screen mode, always assume we're dragging
    m_mouseDrag = true;
    m_mousePress = e->pos();
}

void PathStrokeRenderer::mouseMoveEvent(QMouseEvent *e)
{
    if (!m_fingerPointMapping.isEmpty())
        return;
    // If we've moved more then 25 pixels, assume user is dragging
    if (!m_mouseDrag && QPoint(m_mousePress - e->pos()).manhattanLength() > 25)
        m_mouseDrag = true;

    if (m_mouseDrag && m_activePoint >= 0 && m_activePoint < m_points.size()) {
        m_points[m_activePoint] = e->pos();
        update();
    }
}

void PathStrokeRenderer::mouseReleaseEvent(QMouseEvent *)
{
    if (!m_fingerPointMapping.isEmpty())
        return;
    m_activePoint = -1;

    if (!m_mouseDrag)
        emit clicked();
}

bool PathStrokeRenderer::event(QEvent *e)
{
    bool touchBegin = false;
    switch (e->type()) {
    case QEvent::TouchBegin:
        touchBegin = true;
        Q_FALLTHROUGH();
    case QEvent::TouchUpdate:
    {
        const QTouchEvent *const event = static_cast<const QTouchEvent*>(e);
        const QList<QTouchEvent::TouchPoint> points = event->touchPoints();
        foreach (const QTouchEvent::TouchPoint &touchPoint, points) {
            const int id = touchPoint.id();
            switch (touchPoint.state()) {
            case Qt::TouchPointPressed:
            {
                // find the point, move it
                QSet<int> activePoints = QSet<int>::fromList(m_fingerPointMapping.values());
                int activePoint = -1;
                qreal distance = -1;
                const int pointsCount = m_points.size();
                for (int i=0; i<pointsCount; ++i) {
                    if (activePoints.contains(i))
                        continue;

                    qreal d = QLineF(touchPoint.pos(), m_points.at(i)).length();
                    if ((distance < 0 && d < 12 * m_pointSize) || d < distance) {
                        distance = d;
                        activePoint = i;
                    }
                }
                if (activePoint != -1) {
                    m_fingerPointMapping.insert(touchPoint.id(), activePoint);
                    m_points[activePoint] = touchPoint.pos();
                }
                break;
            }
            case Qt::TouchPointReleased:
            {
                // move the point and release
                QHash<int,int>::iterator it = m_fingerPointMapping.find(id);
                m_points[it.value()] = touchPoint.pos();
                m_fingerPointMapping.erase(it);
                break;
            }
            case Qt::TouchPointMoved:
            {
                // move the point
                const int pointIdx = m_fingerPointMapping.value(id, -1);
                if (pointIdx >= 0)
                    m_points[pointIdx] = touchPoint.pos();
                break;
            }
            default:
                break;
            }
        }
        if (m_fingerPointMapping.isEmpty()) {
            e->ignore();
            return false;
        } else {
            update();
            return true;
        }
    }
        break;
    case QEvent::TouchEnd:
        if (m_fingerPointMapping.isEmpty()) {
            e->ignore();
            return false;
        }
        m_fingerPointMapping.clear();
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}
