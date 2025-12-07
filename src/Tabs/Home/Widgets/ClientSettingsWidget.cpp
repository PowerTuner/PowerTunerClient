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
#include <QLabel>

#include "ClientSettingsWidget.h"
#include "pwtShared/DaemonSettings.h"
#include "pwtClientCommon/CommonUtils.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI {
    ClientSettingsWidget::ClientSettingsWidget(const QSharedPointer<ClientSettings> &clientSettings) {
        settings = clientSettings;
        appQuitBehaviour = new QComboBox();
        autoConnectDaemon = new QComboBox();
        alwaysOnTop = new QCheckBox("Always on top window");
        rememberWindowState = new QCheckBox("Remember window state and position");
        runAtStartup = new QCheckBox("Run at startup");
        daemonUDPPort = new QSpinBox();
        saveBtn = new QPushButton("Save");
        resetDefaultsBtn = new QPushButton("Reset to defaults");

        appQuitBehaviour->addItems({"Quit application", "Close to tray icon"});
        daemonUDPPort->setRange(1000, UINT16_MAX);
        daemonUDPPort->setValue(PWTS::DaemonSettings::DefaultUDPPort);

        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Application quit behaviour"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), appQuitBehaviour);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Connect daemon on start"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), autoConnectDaemon);
        scrollWidgLyt->insertWidget(nextInsertIdx(), alwaysOnTop);
        scrollWidgLyt->insertWidget(nextInsertIdx(), rememberWindowState);
        scrollWidgLyt->insertWidget(nextInsertIdx(), runAtStartup);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Daemon UDP port"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), daemonUDPPort);
        lyt->addWidget(saveBtn);
        lyt->addWidget(resetDefaultsBtn);

        QObject::connect(appQuitBehaviour, &QComboBox::currentIndexChanged, this, &ClientSettingsWidget::onQuitBehaviourChanged);
        QObject::connect(saveBtn, &QPushButton::clicked, this, &ClientSettingsWidget::onSaveBtnClicked);
        QObject::connect(resetDefaultsBtn, &QPushButton::clicked, this, &ClientSettingsWidget::onResetDefaultsBtnClicked);
    }

    void ClientSettingsWidget::initAutoConnectDaemonList() const {
        autoConnectDaemon->clear();
        autoConnectDaemon->addItem("");

        for (const Daemon &dm: settings->getDaemonList())
            autoConnectDaemon->addItem(dm.name);
    }

    void ClientSettingsWidget::loadSettings() const {
        const QSignalBlocker quitBehavBlock {appQuitBehaviour};

        appQuitBehaviour->setCurrentIndex(settings->getCloseToTray());
        alwaysOnTop->setChecked(settings->getAlwaysOnTop());
        rememberWindowState->setChecked(settings->getRememberWindowState());
        runAtStartup->setChecked(getStartupUtil("powertuner")->isSet());
        autoConnectDaemon->setCurrentIndex(autoConnectDaemon->findText(settings->getAutoConnectDaemonName()));

        onQuitBehaviourChanged(appQuitBehaviour->currentIndex());
    }

    void ClientSettingsWidget::onQuitBehaviourChanged(const int idx) const {
        runAtStartup->setEnabled(idx == 1);
        runAtStartup->setChecked(idx == 1 && runAtStartup->isChecked());
    }

    void ClientSettingsWidget::onSaveBtnClicked() {
        saveBtn->setEnabled(false);

        const std::unique_ptr<StartupUtil> startupUtil = getStartupUtil("powertuner");

        settings->setCloseToTray(appQuitBehaviour->currentIndex());
        settings->setAlwaysOnTop(alwaysOnTop->isChecked());
        settings->setRememberWindowState(rememberWindowState->isChecked());
        settings->setAutoConnectDaemonName(autoConnectDaemon->currentText());

        if (runAtStartup->isChecked()) {
            if (!startupUtil->setup())
                UILogger::getInstance()->writeLog(QStringLiteral("Failed to create startup file"));

        } else {
            if (!startupUtil->unset())
                UILogger::getInstance()->writeLog(QStringLiteral("Failed to remove from startup"));
        }

        settings->save();
        emit settingsSaved();
        saveBtn->setEnabled(true);
    }

    void ClientSettingsWidget::onResetDefaultsBtnClicked() {
        resetDefaultsBtn->setEnabled(false);
        settings->reset();
        loadSettings();
        settings->save();
        resetDefaultsBtn->setEnabled(true);
        emit settingsSaved();
    }
}
