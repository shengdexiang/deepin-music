/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "shortcutedit.h"

#include <QPainter>
#include <QKeySequence>

namespace
{

const int MAX_WIDTH = 238;
const int MAX_HEIGHT = 22 + 2 * 2;
const double BORDER_RADIUS = 3.0;
const int ACTIVE_BORDER_WIDTH = 1;
const int NORMAL_BORDER_WIDTH = 1;
const QColor ACTIVE_BORDER_COLOR = QColor("#2ca7f8");
const QColor NORMAL_BORDER_COLOR = QColor(0, 0, 0, 255 * 0.08);

}


class ShortcutEditPrivate
{
public:
    ShortcutEditPrivate(ShortcutEdit *parent) : q_ptr(parent) {}

    QSize stringSize(const QString &str) const;
    QRectF drawTextRect(const QRect &lastRect, const QString &str, QPainter &painter) const;

    QList<Qt::Key>          keys;

    QColor          borderColor = NORMAL_BORDER_COLOR;

    ShortcutEdit *q_ptr;
    Q_DECLARE_PUBLIC(ShortcutEdit)
};

ShortcutEdit::ShortcutEdit(QList<Qt::Key> keys, QWidget *parent) : QWidget(parent), d_ptr(new ShortcutEditPrivate(this))
{
    Q_D(ShortcutEdit);
    d->keys = keys;
    d->keys << Qt::Key_F1 << Qt::Key_F2;
    setFixedSize(MAX_WIDTH, MAX_HEIGHT);
    setFocusPolicy(Qt::ClickFocus);
}

ShortcutEdit::~ShortcutEdit()
{

}

void ShortcutEdit::paintEvent(QPaintEvent *e)
{
    Q_D(const ShortcutEdit);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    // Draw key
    // "Return" and "Enter" should show as "Enter" only
//    QString s = m_shortcut.replace("Return", "Enter");
//    QStringList keys = s.split("+", QString::SkipEmptyParts);
    QStringList keys;
    for (auto key : d->keys) {
        keys << QKeySequence(key).toString();
    }

    if (keys.isEmpty()) {
        // Draw tips
        QRect tR(6, 5, width(), height());
        QFont f;
        f.setPixelSize(11);
        painter.setFont(f);
        painter.setPen(QPen(QColor(48, 48, 48, 0.4 * 255)));
        painter.drawText(tR, tr("Please enter a new shortcut"));
    } else {
        QRectF lastRect(1, 0, 0, 0);
        for (QString key : keys) {
            painter.save();
            lastRect = d->drawTextRect(lastRect.toRect(), key, painter);
            painter.restore();
        }
    }

    auto borderRadius = BORDER_RADIUS;
    auto penWidthf = 1.0;
    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    QMarginsF borderMargin(penWidthf / 2, penWidthf / 2, penWidthf / 2, penWidthf / 2);
    borderRadius -= penWidthf / 2;
    borderRect = borderRect.marginsRemoved(borderMargin);
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);
    QPen borderPen(d->borderColor);
    borderPen.setWidthF(penWidthf);
    painter.strokePath(borderPath, borderPen);
}

void ShortcutEdit::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    Q_D(ShortcutEdit);
    d->borderColor = ACTIVE_BORDER_COLOR;
    this->update();
}

void ShortcutEdit::focusOutEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    Q_D(ShortcutEdit);
    d->borderColor = NORMAL_BORDER_COLOR;
    this->update();

//    m_shortcut = m_shortcut.isEmpty() ? defaultValue() : m_shortcut;
}

QSize ShortcutEditPrivate::stringSize(const QString &str) const
{
    QFont f;
    f.setPixelSize(10);
    QFontMetrics fm(f);
    int w = fm.boundingRect(str).width();
    int h = fm.height();

    return QSize(w, h);
}

QRectF ShortcutEditPrivate::drawTextRect(const QRect &lastRect, const QString &str, QPainter &painter) const
{
    const QSize ss = stringSize(str);
    QRectF r(lastRect.x() + lastRect.width() + 5, 2 + 2,
             ss.width() + 6 * 2, 18);

    QPainterPath bp;
    bp.addRoundedRect(r, BORDER_RADIUS, BORDER_RADIUS);
    painter.setClipPath(bp);

    painter.fillRect(r, QColor(105, 170, 255, 255 * 0.15));

    // Draw inside border
    painter.setPen(QPen(QColor(95, 159, 217, 255 * 0.3), 1));
    QPainterPathStroker stroker;
    stroker.setWidth(1);
    stroker.setJoinStyle(Qt::RoundJoin);
    QPainterPath borderPath = stroker.createStroke(bp);
    painter.drawPath(borderPath);

    // Draw text
    QRectF tR(r.x() + 6, 5, r.width(), r.height());
    QFont f;
    f.setPixelSize(10);
    painter.setFont(f);
    painter.setPen(QPen(QColor("#434343")));
    painter.drawText(tR, str);

    return r;
}
