/*
 * This file is part of PowerTunerClient.
 * Copyright (C) 2025 kylon
 *
 * PowerTunerClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PowerTunerClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "StatusBar.h"

namespace PWT::UI {
    StatusBar::StatusBar() {
        daemonStatus = new QLabel(" Daemon disconnected ");
        cmdStatus = new QLabel(" Command disconnected");

        addPermanentWidget(new QLabel("GPLv3 "));
        addPermanentWidget(new QLabel(" PowerTuner Client  -  kylon "));
        addPermanentWidget(daemonStatus);
        addPermanentWidget(cmdStatus);

        setDaemonConnected(false);
        setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    }

    void StatusBar::setDaemonConnected(const bool connected) const {
        QPalette pal = daemonStatus->palette();
        QColor color;

        if (connected) {
            color = Qt::darkGreen;
            daemonStatus->setText(" Daemon connected ");

        } else {
            color = Qt::red;
            daemonStatus->setText(" Daemon disconnected ");
            setCMDStatus(CMDStatus::Unknown);
        }

        pal.setColor(QPalette::WindowText, color);
        daemonStatus->setPalette(pal);
    }

    void StatusBar::setCMDStatus(const CMDStatus status) const {
        QPalette pal = cmdStatus->palette();
        QColor cmdStatusColor;

        switch (status) {
            case CMDStatus::Pending: {
                cmdStatus->setText("Command pending ");
                cmdStatusColor = QColor(214, 61, 0);
            }
                break;
            case CMDStatus::Ready: {
                cmdStatus->setText("Command ready ");
                cmdStatusColor = Qt::darkGreen;
            }
                break;
            default: {
                cmdStatus->setText("Command disconnected ");
                cmdStatusColor = Qt::red;
            }
                break;
        }

        pal.setColor(QPalette::WindowText, cmdStatusColor);
        cmdStatus->setPalette(pal);
    }
}
