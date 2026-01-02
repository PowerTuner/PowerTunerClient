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
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QFile>
#include <QDir>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "pwtShared/Utils.h"
#ifdef WITH_INTEL
#include "Tabs/Vendor/Intel/CPU/CPUTab.h"
#include "Tabs/Vendor/Intel/HWP/HWPTab.h"
#include "Tabs/Vendor/Intel/MCHBAR/MCHBARTab.h"
#endif
#ifdef WITH_AMD
#include "Tabs/Vendor/AMD/RYCPU/RYCPUTab.h"
#include "Tabs/Vendor/AMD/CPU/CPUTab.h"
#endif
#include "../version.h"
#include "Tabs/OS/Linux/SysFs/SysFsTab.h"
#include "Tabs/OS/Linux/IntelGPU/IntelGPUTab.h"
#include "Tabs/OS/Linux/AMDGPU/AMDGPUTab.h"
#include "Tabs/OS/Windows/PowerScheme/PowerSchemeTab.h"
#include "Tabs/FanControl/FanControlTab.h"
#include "pwtClientCommon/CommonUtils.h"
#include "pwtClientCommon/UILogger.h"

MainWindow::~MainWindow() {
    if (!trayIcon.isNull())
        delete trayIcon;

    delete ui;
    delete profileActions;
    delete uiActions;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(QString("PowerTuner %1.%2").arg(CLIENT_VER_MAJOR).arg(CLIENT_VER_MINOR));
    setWindowIcon(QIcon(":/ico/pwt"));

    const QString globalDataPath = QString("%1/PowerTuner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    QString appDataPath = QString("%1/desktop").arg(globalDataPath);
    QVBoxLayout *lyt = new QVBoxLayout();
    QFont appFont = font();
    const QDir qdir;

    if (!qdir.exists(appDataPath) && !qdir.mkpath(appDataPath))
        appDataPath.clear();

    appFont.setPointSize(10);
    setFont(appFont);

    service.reset(new PWTCS::ClientService);
    windowEventTimer.reset(new QTimer);

    clientSettings = QSharedPointer<PWT::ClientSettings>::create(appDataPath);
    inputRanges = PWT::UI::InputRanges::getInstance();
    tabWidget = new QTabWidget();
    profileActions = new PWT::UI::ProfileActions();
    uiActions = new PWT::UI::UIActions();
    statusBar = new PWT::UI::StatusBar();
    homeTab = new PWT::UI::HomeTab(clientSettings);
    daemonsTab = new PWT::UI::DaemonsTab();
    logTab = new PWT::UI::LogTab();

    inputRanges->setAppDataPath(globalDataPath);
    tabWidget->addTab(homeTab, "Home");
    tabWidget->addTab(logTab, "Log");
    tabWidget->addTab(daemonsTab, "Daemons");
    tabWidget->setCurrentIndex(tabWidget->indexOf(homeTab));
    windowEventTimer->setSingleShot(true);
    windowEventTimer->setInterval(650);

    lyt->addWidget(tabWidget);
    lyt->addLayout(uiActions->getLayout());
    lyt->addLayout(profileActions->getLayout());
    ui->centralwidget->setLayout(lyt);
    setStatusBar(statusBar);

    if (!appDataPath.isEmpty())
        onLogMessageSent(QString("Client data path: %1").arg(appDataPath));

    QObject::connect(windowEventTimer.get(), &QTimer::timeout, this, &MainWindow::onWindowEventTimerTimeout);
    QObject::connect(uiActions, &PWT::UI::UIActions::reloadFromDaemon, this, &MainWindow::onReloadFromDaemon);
    QObject::connect(uiActions, &PWT::UI::UIActions::applyCurrentSettings, this, &MainWindow::onApplyCurrentSettings);
    QObject::connect(profileActions, &PWT::UI::ProfileActions::logMessageSent, this, &MainWindow::onLogMessageSent);
    QObject::connect(profileActions, &PWT::UI::ProfileActions::profileCreateRequest, this, &MainWindow::onProfileCreateRequested);
    QObject::connect(profileActions, &PWT::UI::ProfileActions::profileDeleteRequest, this, &MainWindow::onProfileDeleteRequested);
    QObject::connect(profileActions, &PWT::UI::ProfileActions::profileSaveRequested, this, &MainWindow::onProfileSaveRequested);
    QObject::connect(profileActions, &PWT::UI::ProfileActions::profileLoadRequested, this, &MainWindow::onProfileLoadRequested);
    QObject::connect(profileActions, &PWT::UI::ProfileActions::profileApplyRequested, this, &MainWindow::onProfileApplyRequested);
    QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    QObject::connect(homeTab, &PWT::UI::HomeTab::clientSettingsSaved, this, &MainWindow::onClientSettingsSaved);
    QObject::connect(PWT::UI::UILogger::getInstance().get(), &PWT::UI::UILogger::logWritten, this, &MainWindow::onLogMessageSent);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::daemonAdded, this, &MainWindow::onDaemonAdded);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::daemonDeleted, this, &MainWindow::onDaemonDeleted);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::daemonConnectionRequest, this, &MainWindow::onDaemonConnectionRequested);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::daemonDisconnectionRequest, this, &MainWindow::onDaemonDisconnectionRequested);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::daemonSettingsReloadRequested, this, &MainWindow::onDaemonSettingsReloadRequested);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::daemonSettingsApplyRequested, this, &MainWindow::onDaemonSettingsApplyRequested);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::exportProfileRequested, this, &MainWindow::onDaemonOptionsExportProfileRequested);
    QObject::connect(daemonsTab, &PWT::UI::DaemonsTab::importProfilesRequested, this, &MainWindow::onDaemonOptionsImportProfilesRequested);
    QObject::connect(service.get(), &PWTCS::ClientService::logMessageSent, this, &MainWindow::onLogMessageSent);
    QObject::connect(service.get(), &PWTCS::ClientService::serviceConnected, this, &MainWindow::onServiceConnected);
    QObject::connect(service.get(), &PWTCS::ClientService::serviceError, this, &MainWindow::onServiceError);
    QObject::connect(service.get(), &PWTCS::ClientService::serviceDisconnected, this, &MainWindow::onServiceDisconnected);
    QObject::connect(clientSettings.get(), &PWT::ClientSettings::logMessageSent, this, &MainWindow::onLogMessageSent);
    QObject::connect(inputRanges.get(), &PWT::UI::InputRanges::logMessageSent, this, &MainWindow::onLogMessageSent);

    loadClientSettings();
}

void MainWindow::closeEvent(QCloseEvent *e) {
    if (!clientSettings->getCloseToTray()) {
        QApplication::quit();
        return;
    }

    appMaximized = windowState() == Qt::WindowMaximized;

    e->ignore();
    hide();

    trayIcon->setUnreadLogs(unreadLogs);
    trayIcon->setStatus(service->isConnected(), statusBar->getStatus());
    trayIcon->setActiveProfile(homeTab->getActiveProfile());
    trayIcon->show();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::WindowStateChange:
        case QEvent::Move:
        case QEvent::Resize: {
            if (!clientSettings->getRememberWindowState())
                break;

            windowEventTimer->start();
        }
            break;
        default:
            break;
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::printErrors(const QSet<PWTS::DError> &errors) {
    for (const PWTS::DError &e: errors)
        onLogMessageSent(PWTS::getErrorStr(e));
}

void MainWindow::loadClientSettings() {
    clientSettings->load();
    homeTab->loadClientSettings();
    daemonsTab->loadDaemons(clientSettings->getDaemonList());
    setAlwaysOnTopFlag(clientSettings->getAlwaysOnTop());
    initTrayIcon();

    if (clientSettings->getRememberWindowState()) {
        const PWT::UI::PWTWindowState wstate = clientSettings->getWindowState();
        const QPoint pos = mapToGlobal(wstate.position);

        if (wstate.windowState != Qt::WindowNoState)
            setWindowState(wstate.windowState);

        if (pos != QPoint(-1, -1))
            move(pos);

        if (wstate.width >= minimumWidth() && wstate.height >= minimumHeight())
            resize(wstate.width, wstate.height);
    }

    if (!clientSettings->getAutoConnectDaemonName().isEmpty()) {
        const QString autoConnName = clientSettings->getAutoConnectDaemonName();

        for (const PWT::Daemon &d: clientSettings->getDaemonList()) {
            if (d.name != autoConnName)
                continue;

            daemonsTab->setActiveDaemon(d.adr, d.port);
            onDaemonConnectionRequested(d.adr, d.port);
            break;
        }
    }
}

void MainWindow::setAlwaysOnTopFlag(const bool enable) {
    if (enable)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);

    show();
}

void MainWindow::initTrayIcon() {
    if (!trayIcon.isNull() || !clientSettings->getCloseToTray())
        return;

    trayIcon = new PWT::TrayIcon();

    QObject::connect(trayIcon, &PWT::TrayIcon::showMainWindow, this, &MainWindow::onTrayShowMainWindow);
}

void MainWindow::initMain(const PWTS::DeviceInfoPacket &packet) {
    cpuVendor = packet.cpuInfo.vendor;
    osType = packet.sysInfo.osType;
    cpuCoreCount = packet.cpuInfo.numCores;
    cpuThreadCount = packet.cpuInfo.numLogicalCpus;

    onLogMessageSent(QString("App data location: %1").arg(packet.daemonDataPath));
    inputRanges->load(packet.sysInfo.product, packet.cpuInfo.brand);
    homeTab->init(packet);
    daemonsTab->init(packet);

    switch (osType) {
        case PWTS::OSType::Linux:
            initLinuxTabs(packet);
            break;
        case PWTS::OSType::Windows:
            initWindowsTabs(packet);
            break;
        default:
            break;
    }

    switch (cpuVendor) {
#ifdef WITH_INTEL
        case PWTS::CPUVendor::Intel:
            initIntelTabs(packet);
            break;
#endif
#ifdef WITH_AMD
        case PWTS::CPUVendor::AMD:
            initAMDTabs(packet);
            break;
#endif
        default:
            break;
    }

    initFanControlTab(packet);
}

void MainWindow::initLinuxIntelGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, const int index, const QString &marketName) {
    if (features.contains(PWTS::Feature::INTEL_GPU_SYSFS_GROUP)) {
        const QString tabName = QString("%1 [%2]").arg(marketName.isEmpty() ? "Intel GPU" : marketName).arg(index);
        const QPointer<PWT::UI::LNX::Intel::IntelGPUTab> intelGpuTab = new PWT::UI::LNX::Intel::IntelGPUTab(packet, index);

        tabWidget->insertTab(nextTabInsertIdx(), intelGpuTab, tabName);
        addonTabList.append(intelGpuTab);
    }
}

void MainWindow::initLinuxAMDGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, const int index, const QString &marketName) {
    if (features.contains(PWTS::Feature::AMD_GPU_SYSFS_GROUP)) {
        const QString tabName = QString("%1 [%2]").arg(marketName.isEmpty() ? "AMD GPU" : marketName).arg(index);
        const QPointer<PWT::UI::LNX::AMD::AMDGPUTab> amdGpuTab = new PWT::UI::LNX::AMD::AMDGPUTab(packet, index);

        tabWidget->insertTab(nextTabInsertIdx(), amdGpuTab, tabName);
        addonTabList.append(amdGpuTab);
    }
}

void MainWindow::initLinuxTabs(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.cpu.contains(PWTS::Feature::SYSFS_GROUP)) {
        const QPointer<PWT::UI::LNX::SysFsTab> sysFsTab = new PWT::UI::LNX::SysFsTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), sysFsTab, "SysFs");
        addonTabList.append(sysFsTab);
    }

    for (const auto &[gpuIndex, gpuFeatures]: packet.features.gpus.asKeyValueRange()) {
        const PWTS::GpuInfo &info = packet.gpusInfo[gpuIndex];
        const QString marketName = PWT::getGPUMarketName(info.vendor, info.deviceID.sliced(2), info.revisionID.sliced(2));

        switch (gpuFeatures.first) {
            case PWTS::GPUVendor::Intel:
                initLinuxIntelGPUTab(packet, gpuFeatures.second, gpuIndex, marketName);
                break;
            case PWTS::GPUVendor::AMD:
                initLinuxAMDGPUTab(packet, gpuFeatures.second, gpuIndex, marketName);
                break;
            default:
                break;
        }
    }
}

void MainWindow::initWindowsTabs(const PWTS::DeviceInfoPacket &packet) {
	if (packet.features.cpu.contains(PWTS::Feature::PWR_SCHEME_GROUP)) {
		const QPointer<PWT::UI::WIN::PowerSchemeTab> powerSchemeTab = new PWT::UI::WIN::PowerSchemeTab();

		tabWidget->insertTab(nextTabInsertIdx(), powerSchemeTab, "Power Scheme");
		addonTabList.append(powerSchemeTab);
	}
}

#ifdef WITH_INTEL
void MainWindow::initIntelTabs(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.cpu.contains(PWTS::Feature::INTEL_CPU_GROUP)) {
        const QPointer<PWT::UI::INTEL::CPUTab> CPUTab = new PWT::UI::INTEL::CPUTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), CPUTab, "CPU");
        addonTabList.append(CPUTab);
    }

    if (packet.features.cpu.contains(PWTS::Feature::INTEL_HWP_GROUP)) {
        const QPointer<PWT::UI::INTEL::HWPTab> HWPTab = new PWT::UI::INTEL::HWPTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), HWPTab, "HWP");
        addonTabList.append(HWPTab);
    }

    if (packet.features.cpu.contains(PWTS::Feature::INTEL_MCHBAR_GROUP)) {
        const QPointer<PWT::UI::INTEL::MCHBARTab> mchbarTab = new PWT::UI::INTEL::MCHBARTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), mchbarTab, "MCHBAR");
        addonTabList.append(mchbarTab);
    }
}
#endif
#ifdef WITH_AMD
void MainWindow::initAMDTabs(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.cpu.contains(PWTS::Feature::AMD_CPU_RY_GROUP)) {
        const QPointer<PWT::UI::AMD::RYCPUTab> ryTab = new PWT::UI::AMD::RYCPUTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), ryTab, "RyzenAdj");
        addonTabList.append(ryTab);
    }

    if (packet.features.cpu.contains(PWTS::Feature::AMD_CPU_GROUP)) {
        const QPointer<PWT::UI::AMD::CPUTab> CPUTab = new PWT::UI::AMD::CPUTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), CPUTab, "CPU");
        addonTabList.append(CPUTab);
    }
}
#endif

void MainWindow::initFanControlTab(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.fans.isEmpty())
        return;

    const QPointer<PWT::UI::FANS::FanControlTab> FanControlTab = new PWT::UI::FANS::FanControlTab(packet);

    tabWidget->insertTab(nextTabInsertIdx(), FanControlTab, "FAN Control");
    addonTabList.append(FanControlTab);
}

void MainWindow::deleteAddonTabs() {
    for (const QPointer<PWT::UI::AddonTab> &tab: addonTabList) {
        tabWidget->removeTab(tabWidget->indexOf(tab));
        tab->deleteLater();
    }

    addonTabList.clear();
    insertTabIdx = tabWidget->indexOf(homeTab) + 1;
}

void MainWindow::connectServiceSignals() const {
    QObject::connect(service.get(), &PWTCS::ClientService::commandFailed, this, &MainWindow::onServiceCommandFailed);
    QObject::connect(service.get(), &PWTCS::ClientService::deviceInfoPacketReceived, this, &MainWindow::onServiceDeviceInfoPacketReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::daemonPacketReceived, this, &MainWindow::onServiceDaemonPacketReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::settingsApplied, this, &MainWindow::onServiceSettingsApplied);
    QObject::connect(service.get(), &PWTCS::ClientService::profileListReceived, this, &MainWindow::onProfileListReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::profileWritten, this, &MainWindow::onServiceProfileWritten);
    QObject::connect(service.get(), &PWTCS::ClientService::profileDeleted, this, &MainWindow::onServiceProfileDeleted);
    QObject::connect(service.get(), &PWTCS::ClientService::profileApplied, this, &MainWindow::onServiceProfileApplied);
    QObject::connect(service.get(), &PWTCS::ClientService::daemonSettingsReceived, this, &MainWindow::onServiceDaemonSettingsReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::daemonSettingsApplied, this, &MainWindow::onServiceDaemonSettingsApplied);
    QObject::connect(service.get(), &PWTCS::ClientService::batteryStatusChanged, this, &MainWindow::onServiceBatteryStatusChanged);
    QObject::connect(service.get(), &PWTCS::ClientService::wakeFromSleepEvent, this, &MainWindow::onServiceWakeFromSleepEvent);
    QObject::connect(service.get(), &PWTCS::ClientService::applyTimerTick, this, &MainWindow::onServiceApplyTimerTick);
    QObject::connect(service.get(), &PWTCS::ClientService::profilesExported, this, &MainWindow::onServiceExportedProfilesReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::profilesImported, this, &MainWindow::onServiceImportedProfiles);
}

void MainWindow::disconnectServiceSignals() const {
    QObject::disconnect(service.get(), &PWTCS::ClientService::commandFailed, this, &MainWindow::onServiceCommandFailed);
    QObject::disconnect(service.get(), &PWTCS::ClientService::deviceInfoPacketReceived, this, &MainWindow::onServiceDeviceInfoPacketReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::daemonPacketReceived, this, &MainWindow::onServiceDaemonPacketReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::settingsApplied, this, &MainWindow::onServiceSettingsApplied);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileListReceived, this, &MainWindow::onProfileListReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileWritten, this, &MainWindow::onServiceProfileWritten);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileDeleted, this, &MainWindow::onServiceProfileDeleted);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileApplied, this, &MainWindow::onServiceProfileApplied);
    QObject::disconnect(service.get(), &PWTCS::ClientService::daemonSettingsReceived, this, &MainWindow::onServiceDaemonSettingsReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::daemonSettingsApplied, this, &MainWindow::onServiceDaemonSettingsApplied);
    QObject::disconnect(service.get(), &PWTCS::ClientService::batteryStatusChanged, this, &MainWindow::onServiceBatteryStatusChanged);
    QObject::disconnect(service.get(), &PWTCS::ClientService::wakeFromSleepEvent, this, &MainWindow::onServiceWakeFromSleepEvent);
    QObject::disconnect(service.get(), &PWTCS::ClientService::applyTimerTick, this, &MainWindow::onServiceApplyTimerTick);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profilesExported, this, &MainWindow::onServiceExportedProfilesReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profilesImported, this, &MainWindow::onServiceImportedProfiles);
}

void MainWindow::lockUI(const bool lock) const {
    uiActions->enableButtons(!lock);
    profileActions->enableButtons(!lock);
    daemonsTab->enableDaemonOptionsTabs(!lock);
}

void MainWindow::refreshTabsData(const PWTS::DaemonPacket &packet) const {
    homeTab->refreshTab(packet);
    daemonsTab->refreshTab(packet);

    for (const QPointer<PWT::UI::AddonTab> &tab: addonTabList)
        tab->refreshTab(packet);
}

PWTS::ClientPacket MainWindow::createClientPacket() const {
    PWTS::ClientPacket packet;

    packet.os = osType;
    packet.vendor = cpuVendor;

    switch (osType) {
        case PWTS::OSType::Linux: {
            packet.linuxData = QSharedPointer<PWTS::LNX::LinuxData>::create();
            packet.linuxData->threadData.fill({}, cpuThreadCount);
        }
            break;
        case PWTS::OSType::Windows: {
            packet.windowsData = QSharedPointer<PWTS::WIN::WindowsData>::create();
        }
            break;
        default:
            break;
    }

    switch (cpuVendor) {
#ifdef WITH_INTEL
        case PWTS::CPUVendor::Intel: {
            packet.intelData = QSharedPointer<PWTS::Intel::IntelData>::create();
            packet.intelData->coreData.fill({}, cpuCoreCount);
            packet.intelData->threadData.fill({}, cpuThreadCount);
        }
            break;
#endif
#ifdef WITH_AMD
        case PWTS::CPUVendor::AMD: {
            packet.amdData = QSharedPointer<PWTS::AMD::AMDData>::create();
            packet.amdData->coreData.fill({}, cpuCoreCount);
            packet.amdData->threadData.fill({}, cpuThreadCount);

            switch (osType) {
                case PWTS::OSType::Linux: {
                    packet.linuxAmdData = QSharedPointer<PWTS::LNX::AMD::LinuxAMDData>::create();
                    packet.linuxAmdData->threadData.fill({}, cpuThreadCount);
                }
                    break;
                default:
                    break;
            }
        }
#endif
        default:
            break;
    }

    for (const QPointer<PWT::UI::AddonTab> &tab: addonTabList)
        tab->setDataForPacket(packet);

    return packet;
}

void MainWindow::completeServiceDisconnection() {
    disconnectServiceSignals();
    profileActions->clear();
    homeTab->clearWidget();
    daemonsTab->clearWidget();
    deleteAddonTabs();
    statusBar->setDaemonConnected(false);

    if (!trayIcon.isNull() && trayIcon->isVisible())
        trayIcon->reset();
}

void MainWindow::onTabChanged(const int idx) {
    if (unreadLogs > 0) {
        const int logTabIdx = tabWidget->indexOf(logTab);

        if (idx == logTabIdx) {
            tabWidget->setTabText(logTabIdx, "Log");
            unreadLogs = 0;
        }
    }
}

void MainWindow::onLogMessageSent(const QString &msg) {
    const int logTabIdx = tabWidget->indexOf(logTab);

    if (tabWidget->currentIndex() != logTabIdx) {
        tabWidget->setTabText(logTabIdx, QString("Log (%1)").arg(++unreadLogs));

        if (!trayIcon.isNull() && trayIcon->isVisible())
            trayIcon->setUnreadLogs(unreadLogs);
    }

    logTab->writeLog(msg);
}

void MainWindow::onWindowEventTimerTimeout() const {
    clientSettings->setWindowState({
        .windowState = windowState(),
        .position = pos(),
        .width = width(),
        .height = height()
    });
    clientSettings->save(true);
}

void MainWindow::onReloadFromDaemon() const {
    lockUI(true);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
    service->sendGetDaemonPacketRequest();
}

void MainWindow::onApplyCurrentSettings() const {
    lockUI(true);
    service->sendApplySettingsRequest(createClientPacket());
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onClientSettingsSaved() {
    if (clientSettings->getCloseToTray())
        initTrayIcon();
    else if (!trayIcon.isNull())
        delete trayIcon;

    setAlwaysOnTopFlag(clientSettings->getAlwaysOnTop());
}

void MainWindow::onDaemonAdded(const PWT::Daemon &data) const {
    clientSettings->addDaemon(data);
    clientSettings->save();
    homeTab->initAutoConnectDaemonList();
}

void MainWindow::onDaemonDeleted(const PWT::Daemon &data) const {
    clientSettings->removeDaemon(data);
    clientSettings->save();
    homeTab->initAutoConnectDaemonList();
}

void MainWindow::onDaemonConnectionRequested(const QString &adr, const quint16 port) {
    lockUI(true);

    if (service->isConnected()) {
        service->disconnectFromDaemon();
        completeServiceDisconnection();
    }

    service->connectToDaemon(adr, port);
}

void MainWindow::onDaemonDisconnectionRequested() {
    lockUI(true);
    service->disconnectFromDaemon();
    daemonsTab->setDaemonAsDisconnected();
    completeServiceDisconnection();
}

void MainWindow::onDaemonSettingsReloadRequested() const {
    service->sendGetDaemonSettingsRequest();
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
    lockUI(true);
}

void MainWindow::onDaemonSettingsApplyRequested(const QByteArray &data) const {
    service->sendApplyDaemonSettingsRequest(data);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
    lockUI(true);
    daemonsTab->optionsFocusSettingsTab();
}

void MainWindow::onDaemonOptionsExportProfileRequested(const QString &name) {
    const QString dir = QFileDialog::getExistingDirectory(this, "Export in folder..");

    if (dir.isEmpty())
        return;

    exportedProfilesOut = dir;
    service->sendExportProfilesRequest(name);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
    lockUI(true);
    daemonsTab->optionsFocusOptionsTab();
}

void MainWindow::onDaemonOptionsImportProfilesRequested() {
    const QStringList files = QFileDialog::getOpenFileNames(this, "Import profiles", "", "PowerTuner profile (*.pwt)");

    if (files.isEmpty())
        return;

    QProgressDialog progressDialog(this);
    QHash<QString, QByteArray> imports;
    int fileNo = 0;

    progressDialog.setCancelButton(nullptr);
    progressDialog.setMinimum(0);
    progressDialog.setMaximum(files.size());
    progressDialog.setValue(0);

    for (const QString &file: files) {
        progressDialog.setValue(++fileNo);

        if (!PWTS::isValidProfileForImport(file))
            continue;

        const QFileInfo finfo {file};
        QFile f {file};

        if (f.open(QFile::ReadOnly))
            imports.insert(finfo.baseName(), f.readAll());
        else
            onLogMessageSent(QString("cannot import profile '%1', skip").arg(file));
    }

    progressDialog.setValue(progressDialog.maximum());
    service->sendImportProfilesRequest(imports);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
    lockUI(true);
    daemonsTab->optionsFocusOptionsTab();
}

void MainWindow::onServiceError() {
    lockUI(true);
    daemonsTab->setDaemonAsDisconnected();
    completeServiceDisconnection();
}

void MainWindow::onServiceCommandFailed() const {
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceConnected() {
    daemonsTab->setDaemonAsConnected();
    statusBar->setDaemonConnected(true);
    connectServiceSignals();
    service->sendGetDeviceInfoPacketRequest();
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
    onLogMessageSent(QString("[%1] Daemon connection successful").arg(service->getDaemonAddress()));
}

void MainWindow::onServiceDisconnected() {
    lockUI(true);
    daemonsTab->setDaemonAsDisconnected();
    completeServiceDisconnection();
}

void MainWindow::onServiceDeviceInfoPacketReceived(const PWTS::DeviceInfoPacket &packet) {
    initMain(packet);
    service->sendGetDaemonPacketRequest();
}

void MainWindow::onServiceDaemonPacketReceived(const PWTS::DaemonPacket &packet) {
    inputRanges->refresh();
    refreshTabsData(packet);
    profileActions->updateProfileList(packet.profilesList);
    printErrors(packet.errors);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceSettingsApplied(const QSet<PWTS::DError> &errors) {
    printErrors(errors);
    homeTab->unsetAppliedProfile();

    if (!trayIcon.isNull() && trayIcon->isVisible())
        trayIcon->unsetActiveProfile();

    onReloadFromDaemon();
}

void MainWindow::onServiceProfileWritten(const bool result) {
    onLogMessageSent(result ? QStringLiteral("Profile saved successfully") : QStringLiteral("Failed to write profile to disk"));
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceProfileDeleted(const bool result) {
    onLogMessageSent(result ? QStringLiteral("Profile deleted successfully") : QStringLiteral("Failed to delete profile"));
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceProfileApplied(const QSet<PWTS::DError> &errors, const QString &name) {
    if (errors.isEmpty()) {
        homeTab->setAppliedProfile(name);
        onLogMessageSent(QString("\"%1\" profile applied successfully").arg(name));

        if (!trayIcon.isNull() && trayIcon->isVisible())
            trayIcon->setActiveProfile(name);

    } else {
        onLogMessageSent(QString("Failed to apply all \"%1\" profile settings").arg(name));
        onLogMessageSent(QString("== '%1' error log start ==").arg(name));
        printErrors(errors);
        onLogMessageSent(QString("== '%1' error log end ==").arg(name));
    }

    onReloadFromDaemon();
}

void MainWindow::onServiceExportedProfilesReceived(const QHash<QString, QByteArray> &exported) {
    if (exportedProfilesOut.isEmpty())
        return;

    QHashIterator<QString, QByteArray> it {exported};
    const QDir outDir {exportedProfilesOut};

    if (!outDir.exists("PowerTunerExported") && !outDir.mkdir("PowerTunerExported")) {
        onLogMessageSent("Failed to create export folder");
        exportedProfilesOut.clear();
        statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
        lockUI(false);
        return;
    }

    while (it.hasNext()) {
        it.next();

        QFile profile {QString("%1/PowerTunerExported/%2").arg(exportedProfilesOut, it.key())};
        qint64 written;

        if (!profile.open(QFile::WriteOnly | QFile::Truncate)) {
            onLogMessageSent(QString("Failed to open file for write, skip \"%1\"").arg(it.key()));
            continue;
        }

        written = profile.write(it.value());

        if (written == -1 || written != it.value().size())
            onLogMessageSent(QString("Failed to write exported \"%1\" profile to disk").arg(it.key()));
        else
            onLogMessageSent(QString("exported profile \"%1\"").arg(it.key()));
    }

    exportedProfilesOut.clear();
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceImportedProfiles(const bool result) {
    if (!result)
        onLogMessageSent("Import profiles: failed to import some profiles");
    else
        onLogMessageSent("Profiles imported successfully");

    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceDaemonSettingsReceived(const QByteArray &data) const {
    daemonsTab->updateDaemonSettings(data);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceDaemonSettingsApplied(const bool diskSaveResult) {
    if (!diskSaveResult)
        onLogMessageSent(QStringLiteral("Failed to write new daemon settings to disk"));
    else
        onLogMessageSent(QStringLiteral("Daemon settings saved successfully"));

    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceBatteryStatusChanged(const QSet<PWTS::DError> &errors, const QString &profile) {
    if (errors.isEmpty()) {
        onLogMessageSent(QString("Battery status changed: applied \"%1\" profile").arg(profile));
        homeTab->setAppliedProfile(profile);

    } else {
        onLogMessageSent(QString("Battery status changed: failed to apply all \"%1\" profile settings").arg(profile));
        onLogMessageSent(QString("== '%1' error log start ==").arg(profile));
        printErrors(errors);
        onLogMessageSent(QString("== '%1' error log end ==").arg(profile));
        homeTab->unsetAppliedProfile();
    }

    onReloadFromDaemon();
}

void MainWindow::onServiceWakeFromSleepEvent(const QSet<PWTS::DError> &errors) {
    onLogMessageSent(QStringLiteral("Wake from sleep event"));

    if (errors.isEmpty()) {
        onLogMessageSent(QStringLiteral("Settings re-applied successfully"));

    } else {
        onLogMessageSent(QStringLiteral("== Wake from sleep error log start =="));
        printErrors(errors);
        onLogMessageSent(QStringLiteral("== Wake from sleep error log end =="));
    }

    onReloadFromDaemon();
}

void MainWindow::onServiceApplyTimerTick(const QSet<PWTS::DError> &errors) {
    printErrors(errors);
}

void MainWindow::onProfileListReceived(const QList<QString> &profileList) {
    lockUI(true);
    profileActions->updateProfileList(profileList);
    daemonsTab->updateProfileList(profileList);
    onLogMessageSent(QStringLiteral("Updated profile list received from daemon"));
    lockUI(false);
}

void MainWindow::onProfileCreateRequested(const QString &name, const bool askOverwrite) {
    QMessageBox::StandardButton res = QMessageBox::question(this, "Create profile", QString("Save current settings to \"%1\" profile?").arg(name));

    if (res == QMessageBox::StandardButton::No)
        return;

    if (askOverwrite) {
        res = QMessageBox::question(this, "Create profile", QString("\"%1\" profile already exists, overwrite?").arg(name));
        if (res == QMessageBox::StandardButton::No)
            return;
    }

    lockUI(true);
    service->sendWriteProfileRequest(name, createClientPacket());
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileDeleteRequested(const QString &name) {
    QMessageBox::StandardButton res;

    res = QMessageBox::question(this, "Delete profile", QString("\"%1\" profile will be deleted").arg(name));
    if (res == QMessageBox::StandardButton::No)
        return;

    lockUI(true);
    service->sendDeleteProfileRequest(name);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileSaveRequested(const QString &name) {
    const QMessageBox::StandardButton res = QMessageBox::question(this, "Save profile", QString("Save current settings to \"%1\" profile?").arg(name));

    if (res == QMessageBox::StandardButton::No)
        return;

    lockUI(true);
    service->sendWriteProfileRequest(name, createClientPacket());
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileLoadRequested(const QString &name) const {
    lockUI(true);
    service->sendLoadProfileRequest(name);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileApplyRequested(const QString &name) const {
    lockUI(true);
    service->sendApplyProfileRequest(name);
    statusBar->setCMDStatus(PWT::UI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onTrayShowMainWindow() {
    if (appMaximized)
        showMaximized();
    else
        showNormal();
}
