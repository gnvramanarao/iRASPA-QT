/*************************************************************************************************************
 Copyright (c) 2006-2019 David Dubbeldam, Sofia Calero, Thijs J.H. Vlugt.

    D.Dubbeldam@uva.nl            http://www.acmm.nl/molsim/users/dubbeldam/index.html
    scaldia@upo.es                http://www.upo.es/raspa/sofiacalero.php
    t.j.h.vlugt@tudelft.nl        http://homepage.tudelft.nl/v9k6y

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 *************************************************************************************************************/


#pragma once

// https://github.com/GoldenCheetah/GoldenCheetah

#include <QWidget>


class QMenu;
class QStyleOption;

class QtSegmentControlPrivate;

class QtSegmentControl : public QWidget
{
    Q_OBJECT
    Q_ENUMS(SelectionBehavior)
    Q_PROPERTY(SelectionBehavior selectionBehavior READ selectionBehavior WRITE setSelectionBehavior)
    Q_PROPERTY(int selectedSegment READ selectedSegment NOTIFY segmentSelected)
    Q_PROPERTY(int count READ count WRITE setCount)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
public:
    enum SelectionBehavior { SelectOne, SelectAll, SelectNone };

    QtSegmentControl(QWidget *parent = 0);
    ~QtSegmentControl();

    int count() const;
    void setCount(int newConut);

    bool isSegmentSelected(int index) const;
    int selectedSegment() const;
    void setSegmentSelected(int index, bool selected);

    void setSegmentEnabled(int index, bool enable);
    bool segmentEnabled(int index) const;

    void setSelectionBehavior(SelectionBehavior behavior);
    SelectionBehavior selectionBehavior() const;

    void setSegmentText(int index, const QString &text);
    QString segmentText(int index) const;

    void setSegmentIcon(int index, const QIcon &icon);
    QIcon segmentIcon(int index) const;

    void setIconSize(const QSize &size);
    QSize iconSize() const;

    void setSegmentMenu(int segment, QMenu *menu);
    QMenu *segmentMenu(int segment) const;

    void setSegmentToolTip(int segment, const QString &tipText);
    QString segmentToolTip(int segment) const;

    void setSegmentWhatsThis(int segment, const QString &whatsThisText);
    QString segmentWhatsThis(int segment) const;

    virtual QSize segmentSizeHint(int segment) const;
    QSize sizeHint() const;

    QRect segmentRect(int index) const;
    int segmentAt(const QPoint &pos) const;

protected:
    void initStyleOption(int segment, QStyleOption *option) const;
    void paintEvent(QPaintEvent *pe);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    bool event(QEvent *event);

Q_SIGNALS:
    void segmentSelected(int selected);

private:
    QtSegmentControlPrivate *d;
};
