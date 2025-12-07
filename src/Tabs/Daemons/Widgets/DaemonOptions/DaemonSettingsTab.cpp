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
#include <QLineEdit>

#include "DaemonSettingsTab.h"
#include "pwtShared/DaemonSettings.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI {
    DaemonSettingsTab::DaemonSettingsTab() {
        daemonSettings.reset(new PWTS::DaemonSettings);

        startProfile = new QComboBox();
        batteryProfile = new QComboBox();
        psupplyProfile = new QComboBox();
        applyInterval = new SliderWidget();
        listenAdr = new QLineEdit();
        tcpPort = new QSpinBox();
        udpPort = new QSpinBox();
        applyOnWake = new QCheckBox("Re-apply on wake from sleep");
        ignoreBatteryEvt = new QCheckBox("Ignore battery events");
        logLevel = new QComboBox();
        maxLogFiles = new QSpinBox();
        applyBtn = new QPushButton("Apply");
        reloadBtn = new QPushButton("Reload");

        applyInterval->setRange(PWTS::DaemonSettings::MinApplyInterval, PWTS::DaemonSettings::MaxApplyInterval);
        applyInterval->setLabel("Once [0 sec]");
        listenAdr->setPlaceholderText("any (address|localhost|any)");
        tcpPort->setRange(1000, UINT16_MAX);
        udpPort->setRange(1000, UINT16_MAX);
        tcpPort->setValue(PWTS::DaemonSettings::DefaultTCPPort);
        udpPort->setValue(PWTS::DaemonSettings::DefaultUDPPort);
        ignoreBatteryEvt->setToolTip("Do not automatically switch profile on battery status change");
        logLevel->addItems({"Info", "Warning", "Error", "Service", "None"});
        maxLogFiles->setRange(1, 40);

        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Daemon start profile"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), startProfile);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Battery profile"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), batteryProfile);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Power supply profile"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), psupplyProfile);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Re-apply interval"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), applyInterval);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Listen address"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), listenAdr);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("TCP Port"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), tcpPort);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("UDP Port"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), udpPort);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Log level"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), logLevel);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Max log files"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), maxLogFiles);
        scrollWidgLyt->insertWidget(nextInsertIdx(), applyOnWake);
        scrollWidgLyt->insertWidget(nextInsertIdx(), ignoreBatteryEvt);
        lyt->addWidget(applyBtn);
        lyt->addWidget(reloadBtn);

        QObject::connect(applyInterval, &SliderWidget::valueChanged, this, &DaemonSettingsTab::onApplyIntervalChanged);
        QObject::connect(applyBtn, &QPushButton::clicked, this, &DaemonSettingsTab::onApplyBtnClicked);
        QObject::connect(reloadBtn, &QPushButton::clicked, this, &DaemonSettingsTab::onReloadBtnClicked);
    }

    void DaemonSettingsTab::init(const PWTS::DeviceInfoPacket &packet) const {
        if (daemonSettings->load(packet.daemonSettings))
            refreshTab();
        else
            UILogger::getInstance()->writeLog(QStringLiteral("Failed to load daemon settings, using defaults"));
    }

    void DaemonSettingsTab::setData(const PWTS::DaemonPacket &packet) const {
        updateProfilesList(packet.profilesList);
    }

    void DaemonSettingsTab::updateProfilesList(const QList<QString> &list) const {
        for (QComboBox *combo: {startProfile, batteryProfile, psupplyProfile}) {
            combo->clear();
            combo->addItem("");
            combo->addItems(list);
        }

        startProfile->setCurrentIndex(startProfile->findText(daemonSettings->getOnStartProfile()));
        batteryProfile->setCurrentIndex(batteryProfile->findText(daemonSettings->getOnBatteryProfile()));
        psupplyProfile->setCurrentIndex(psupplyProfile->findText(daemonSettings->getOnPowerSupplyProfile()));
    }

    bool DaemonSettingsTab::updateSettings(const QByteArray &data) const {
        if (!daemonSettings->load(data))
            return false;

        refreshTab();
        return true;
    }

    void DaemonSettingsTab::clearTab() const {
        startProfile->clear();
        batteryProfile->clear();
        psupplyProfile->clear();
        applyInterval->setValue(0);
        listenAdr->clear();
        tcpPort->setValue(PWTS::DaemonSettings::DefaultTCPPort);
        udpPort->setValue(PWTS::DaemonSettings::DefaultUDPPort);
        applyOnWake->setChecked(false);
        ignoreBatteryEvt->setChecked(false);
        logLevel->setCurrentIndex(2);
        maxLogFiles->setValue(5);
    }

    void DaemonSettingsTab::refreshTab() const {
        startProfile->setCurrentIndex(startProfile->findText(daemonSettings->getOnStartProfile()));
        batteryProfile->setCurrentIndex(batteryProfile->findText(daemonSettings->getOnBatteryProfile()));
        psupplyProfile->setCurrentIndex(psupplyProfile->findText(daemonSettings->getOnPowerSupplyProfile()));
        applyInterval->setValue(daemonSettings->getApplyInterval());
        listenAdr->setText(daemonSettings->getAddress());
        tcpPort->setValue(daemonSettings->getSocketTcpPort());
        udpPort->setValue(daemonSettings->getSocketUdpPort());
        applyOnWake->setChecked(daemonSettings->getApplyOnWakeFromSleep());
        ignoreBatteryEvt->setChecked(daemonSettings->getIgnoreBatteryEvent());
        logLevel->setCurrentIndex(static_cast<int>(daemonSettings->getLogLevel()));
        maxLogFiles->setValue(daemonSettings->getMaxLogFiles());
    }

    void DaemonSettingsTab::onApplyIntervalChanged(const int value) const {
        const int val = value > 59 ? (value / 60) : value;
        const QString unit = value > 59 ? "min":"sec";
        const QString time = value == 0 ? "Once":"Every";

        applyInterval->setLabel(QString("%1 [%2 %3]").arg(time).arg(val).arg(unit));
    }

    void DaemonSettingsTab::onApplyBtnClicked() {
        daemonSettings->setOnStartProfile(startProfile->currentText());
        daemonSettings->setOnBatteryProfile(batteryProfile->currentText());
        daemonSettings->setOnPowerSupplyProfile(psupplyProfile->currentText());
        daemonSettings->setApplyInterval(applyInterval->getValue());
        daemonSettings->setAddress(listenAdr->text());
        daemonSettings->setApplyOnWakeFromSleep(applyOnWake->isChecked());
        daemonSettings->setIgnoreBatteryEvent(ignoreBatteryEvt->isChecked());
        daemonSettings->setLogLevel(static_cast<PWTS::LogLevel>(logLevel->currentIndex()));
        daemonSettings->setMaxLogFiles(maxLogFiles->value());
        daemonSettings->setSocketTcpPort(tcpPort->value());
        daemonSettings->setSocketUdpPort(udpPort->value());

        emit applySettings(daemonSettings->getData());
    }

    void DaemonSettingsTab::onReloadBtnClicked() {
        emit reloadSettings();
    }
}
