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

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

namespace PWT {
    class TrayIcon final: public QObject {
        Q_OBJECT

    private:
        QSystemTrayIcon *tray = nullptr;
        QMenu *contextMenu = nullptr;
        QAction *activeProfile = nullptr;
        QAction *unreadLogs = nullptr;
        QAction *connectionStatus = nullptr;
        QAction *commandStatus = nullptr;
        QAction *closeApp = nullptr;

    public:
        TrayIcon();
        ~TrayIcon() override;

        [[nodiscard]] bool isVisible() const { return tray->isVisible(); }
        void show() const { tray->show(); }
        void close() const { tray->hide(); }

        void reset() const;
        void setActiveProfile(const QString &name) const;
        void unsetActiveProfile() const;
        void setUnreadLogs(int unread) const;
        void setStatus(bool isConnected, const QString &status) const;

    private slots:
        void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
        void onTrayShowApp();

    signals:
        void showMainWindow();
    };
}
