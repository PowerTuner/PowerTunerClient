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

#include <QCloseEvent>
#include <QMainWindow>
#include <QScopedPointer>
#include <QTimer>

#include "Classes/ClientSettings.h"
#include "pwtClientCommon/InputRanges/InputRanges.h"
#include "pwtClientService/ClientService.h"
#include "Widgets/ProfileActions.h"
#include "Widgets/UIActions.h"
#include "Widgets/StatusBar.h"
#include "Widgets/TrayIcon.h"
#include "Tabs/Include/AddonTab.h"
#include "Tabs/Home/HomeTab.h"
#include "Tabs/Daemons/DaemonsTab.h"
#include "Tabs/Log/LogTab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final: public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QSharedPointer<PWT::ClientSettings> clientSettings;
    QSharedPointer<PWT::UI::InputRanges> inputRanges;
    QScopedPointer<PWTCS::ClientService> service;
    QList<QPointer<PWT::UI::AddonTab>> addonTabList;
    QPointer<PWT::TrayIcon> trayIcon;
    QScopedPointer<QTimer> windowEventTimer;
    QString exportedProfilesOut;
    QTabWidget *tabWidget = nullptr;
    PWT::UI::ProfileActions *profileActions = nullptr;
    PWT::UI::UIActions *uiActions = nullptr;
    PWT::UI::StatusBar *statusBar = nullptr;
    PWT::UI::HomeTab *homeTab = nullptr;
    PWT::UI::DaemonsTab *daemonsTab = nullptr;
    PWT::UI::LogTab *logTab = nullptr;
    PWTS::CPUVendor cpuVendor = PWTS::CPUVendor::Unknown;
    PWTS::OSType osType = PWTS::OSType::Unknown;
    bool appMaximized = false;
    int cpuCoreCount = 0;
    int cpuThreadCount = 0;
    int insertTabIdx = 1;
    int unreadLogs = 0;

    [[nodiscard]] int nextTabInsertIdx() { return insertTabIdx++; }

    void printErrors(const QSet<PWTS::DError> &errors);
    void loadClientSettings();
    void setAlwaysOnTopFlag(bool enable);
    void initTrayIcon();
    void initMain(const PWTS::DeviceInfoPacket &packet);
    void initLinuxIntelGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, int index, const QString &marketName);
    void initLinuxAMDGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, int index, const QString &marketName);
    void initLinuxTabs(const PWTS::DeviceInfoPacket &packet);
	void initWindowsTabs(const PWTS::DeviceInfoPacket &packet);
#ifdef WITH_INTEL
    void initIntelTabs(const PWTS::DeviceInfoPacket &packet);
#endif
#ifdef WITH_AMD
    void initAMDTabs(const PWTS::DeviceInfoPacket &packet);
#endif
    void initFanControlTab(const PWTS::DeviceInfoPacket &packet);
    void deleteAddonTabs();
    void connectServiceSignals() const;
    void disconnectServiceSignals() const;
    void completeServiceDisconnection();
    void lockUI(bool lock) const;
    void refreshTabsData(const PWTS::DaemonPacket &packet) const;
    PWTS::ClientPacket createClientPacket() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void closeEvent(QCloseEvent *e) override;

private slots:
    void onTabChanged(int idx);
    void onLogMessageSent(const QString &msg);
    void onWindowEventTimerTimeout() const;
    void onReloadFromDaemon() const;
    void onApplyCurrentSettings() const;
    void onClientSettingsSaved();
    void onDaemonAdded(const PWT::Daemon &data) const;
    void onDaemonDeleted(const PWT::Daemon &data) const;
    void onDaemonConnectionRequested(const QString &adr, quint16 port);
    void onDaemonDisconnectionRequested();
    void onDaemonSettingsReloadRequested() const;
    void onDaemonSettingsApplyRequested(const QByteArray &data) const;
    void onDaemonOptionsExportProfileRequested(const QString &name);
    void onDaemonOptionsImportProfilesRequested();
    void onServiceError();
    void onServiceCommandFailed() const;
    void onServiceConnected();
    void onServiceDisconnected();
    void onServiceDeviceInfoPacketReceived(const PWTS::DeviceInfoPacket &packet);
    void onServiceDaemonPacketReceived(const PWTS::DaemonPacket &packet);
    void onServiceSettingsApplied(const QSet<PWTS::DError> &errors);
    void onServiceProfileWritten(bool result);
    void onServiceProfileDeleted(bool result);
    void onServiceProfileApplied(const QSet<PWTS::DError> &errors, const QString &name);
    void onServiceExportedProfilesReceived(const QHash<QString, QByteArray> &exported);
    void onServiceImportedProfiles(bool result);
    void onServiceDaemonSettingsReceived(const QByteArray &data) const;
    void onServiceDaemonSettingsApplied(bool diskSaveResult);
    void onServiceBatteryStatusChanged(const QSet<PWTS::DError> &errors, const QString &profile);
    void onServiceWakeFromSleepEvent(const QSet<PWTS::DError> &errors);
    void onServiceApplyTimerTick(const QSet<PWTS::DError> &errors);
    void onProfileListReceived(const QList<QString> &profileList);
    void onProfileCreateRequested(const QString &name, bool askOverwrite);
    void onProfileDeleteRequested(const QString &name);
    void onProfileSaveRequested(const QString &name);
    void onProfileLoadRequested(const QString &name) const;
    void onProfileApplyRequested(const QString &name) const;
    void onTrayShowMainWindow();
};
