/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QFrame>

class SearchResult;
class TitleBarWidgetPrivate;
class TitleBarWidget : public QFrame
{
    Q_OBJECT
public:
    explicit TitleBarWidget(QWidget *parent = 0);
    ~TitleBarWidget();

    void exitSearch();
    void clearSearch();
    void setSearchEnable(bool enable = true);
    void setResultWidget(SearchResult *);
    void setViewname(const QString &viewname);
signals:
    void searchExited();
    void search(const QString &text);
    void locateMusicInAllMusiclist(const QString &hash);

protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QScopedPointer<TitleBarWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), TitleBarWidget)
};
