#ifndef ARTHURWIDGETS_H
#define ARTHURWIDGETS_H

#include "arthurstyle.h"
#include <QBitmap>
#include <QPushButton>
#include <QGroupBox>

QT_FORWARD_DECLARE_CLASS(QOpenGLWindow)
QT_FORWARD_DECLARE_CLASS(QTextDocument)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)

class ArthurFrame : public QWidget

{
    Q_OBJECT
public:
    ArthurFrame(QWidget *parent);
    virtual void paint(QPainter *) {}

    bool preferImage() const { return m_prefer_image; }

public slots:
    void setPreferImage(bool pi) { m_prefer_image = pi; }

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent* e) override {
      QWidget::resizeEvent(e);
    }

    QPixmap m_tile;

    bool m_show_doc;
    bool m_prefer_image;
    QTextDocument *m_document;

    QString m_sourceFileName;

};

#endif
