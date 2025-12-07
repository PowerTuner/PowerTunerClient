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

#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

#include "../../Include/ScrollableTab.h"
#include "../../../Classes/ClientSettings.h"

namespace PWT::UI {
    class ClientSettingsWidget final: public ScrollableTab {
        Q_OBJECT

    private:
        QSharedPointer<ClientSettings> settings;
        QPushButton *saveBtn = nullptr;
        QPushButton *resetDefaultsBtn = nullptr;
        QComboBox *appQuitBehaviour = nullptr;
        QComboBox *autoConnectDaemon = nullptr;
        QCheckBox *alwaysOnTop = nullptr;
        QCheckBox *rememberWindowState = nullptr;
        QCheckBox *runAtStartup = nullptr;
        QSpinBox *daemonUDPPort = nullptr;

    public:
        explicit ClientSettingsWidget(const QSharedPointer<ClientSettings> &clientSettings);

        void initAutoConnectDaemonList() const;
        void loadSettings() const;

    private slots:
        void onQuitBehaviourChanged(int idx) const;
        void onSaveBtnClicked();
        void onResetDefaultsBtnClicked();

    signals:
        void settingsSaved();
    };
}
