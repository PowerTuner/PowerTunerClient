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
#pragma once

#include <QGroupBox>
#include <QPushButton>

namespace PWT::UI {
    class DaemonBox final: public QGroupBox {
        Q_OBJECT

    private:
        QPushButton *connectBtn = nullptr;
        QPushButton *deleteBtn = nullptr;
        bool daemonConnected = false;
        QString dname;
        QString dadr;
        quint16 dport;

    public:
        DaemonBox(const QString &name, const QString &adr, quint16 port);

        [[nodiscard]] QString getAddress() const { return dadr; }
        [[nodiscard]] quint16 getPort() const { return dport; }
        [[nodiscard]] QString getName() const { return dname; }
        [[nodiscard]] bool isPending() const { return !connectBtn->isEnabled(); }
        [[nodiscard]] bool isConnected() const { return daemonConnected; }

        void setConnectionResult(bool connected);

    private slots:
        void connectBtnClicked();
        void deleteBtnClicked();

    signals:
        void connectDaemon(const QString &adr, quint16 port);
        void disconnectDaemon();
        void deleteDaemon();
    };
}
