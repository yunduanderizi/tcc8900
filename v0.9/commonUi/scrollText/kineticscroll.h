/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: openBossa - INdT (renato.chencarek@openbossa.org)
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** the openBossa stream from INdT (renato.chencarek@openbossa.org).
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef KINETICSCROLL_H
#define KINETICSCROLL_H

#include <QObject>

namespace chinatsp_common_gui_ui
{
    class KineticScrollPrivate;

    class KineticScroll : public QObject
    {
        Q_OBJECT

    public:
        KineticScroll(QObject *parent = 0);
        ~KineticScroll();

        void mouseUp(int value);
        bool mouseDown(int value);
        void mouseMove(int value);
        void mouseCancel();
        void kineticStart(qreal speed);
        void kineticStop();

    public Q_SLOTS:
        void animator();

    Q_SIGNALS:
        void signalMoveOffset(int);
        void signalKineticStop();

    private:
        KineticScrollPrivate *d;
    };
}
#endif
