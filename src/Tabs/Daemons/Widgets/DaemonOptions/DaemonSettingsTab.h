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

#include <QCheckBox>
#include <QComboBox>

#include "../../../Include/ScrollableTab.h"
#include "../../../Widgets/SliderWidget.h"
#include "pwtShared/DaemonSettings.h"
#include "pwtShared/Include/Packets/DeviceInfoPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::UI {
    class DaemonSettingsTab final: public ScrollableTab {
        Q_OBJECT

    private:
        QComboBox *startProfile = nullptr;
        QComboBox *batteryProfile = nullptr;
        QComboBox *psupplyProfile = nullptr;
        SliderWidget *applyInterval = nullptr;
        QLineEdit *listenAdr = nullptr;
        QSpinBox *tcpPort = nullptr;
        QSpinBox *udpPort = nullptr;
        QCheckBox *applyOnWake = nullptr;
        QCheckBox *ignoreBatteryEvt = nullptr;
        QComboBox *logLevel = nullptr;
        QSpinBox *maxLogFiles = nullptr;
        QPushButton *applyBtn = nullptr;
        QPushButton *reloadBtn = nullptr;
        QScopedPointer<PWTS::DaemonSettings> daemonSettings;

        void refreshTab() const;

    public:
        DaemonSettingsTab();

        void init(const PWTS::DeviceInfoPacket &packet) const;
        void setData(const PWTS::DaemonPacket &packet) const;
        void updateProfilesList(const QList<QString> &list) const;
        [[nodiscard]] bool updateSettings(const QByteArray &data) const;
        void clearTab() const;

    private slots:
        void onApplyIntervalChanged(int value) const;
        void onApplyBtnClicked();
        void onReloadBtnClicked();

    signals:
        void applySettings(const QByteArray &data);
        void reloadSettings();
    };
}
