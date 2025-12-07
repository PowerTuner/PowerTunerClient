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
#include <QVBoxLayout>

#include "PKGCSTConfigControlGBox.h"
#include "PKGCSTConfigControlSBWidget.h"
#include "PKGCSTConfigControlCU1Widget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::INTEL {
    PKGCSTConfigControlGBox::PKGCSTConfigControlGBox(const int coreCount, const QSet<PWTS::Feature> &features): QGroupBox("C-State Configuration Control") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *cpuSelectLyt = new QHBoxLayout();

        cpuSelect = new QComboBox();
        applyToAll = new QCheckBox("Apply to all cores");

        for (int i=0; i<coreCount; ++i)
            cpuSelect->addItem(QString("Core %1").arg(i));

        initPkgCstConfigControlWidget(features);

        cpuSelectLyt->addStretch();
        cpuSelectLyt->addWidget(applyToAll);
        cpuSelectLyt->addWidget(cpuSelect);
        lyt->addLayout(cpuSelectLyt);
        lyt->addWidget(pkgCstCfgControlWidget);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(applyToAll, &QCheckBox::checkStateChanged, this, &PKGCSTConfigControlGBox::onApplyToAllStateChanged);
        QObject::connect(cpuSelect, &QComboBox::currentIndexChanged, this, &PKGCSTConfigControlGBox::onCpuSelectChanged);
    }

    void PKGCSTConfigControlGBox::initPkgCstConfigControlWidget(const QSet<PWTS::Feature> &features) {
        if (features.contains(PWTS::Feature::INTEL_PKG_CST_CONFIG_CONTROL_SB))
            pkgCstCfgControlWidget = new PKGCSTConfigControlSBWidget();
        else if (features.contains(PWTS::Feature::INTEL_PKG_CST_CONFIG_CONTROL_CU1))
            pkgCstCfgControlWidget = new PKGCSTConfigControlCU1Widget();
    }

    void PKGCSTConfigControlGBox::updatePkgCstConfigControlWidget(const int idx) const {
        pkgCstCfgControlWidget->setEnabled(data[idx].valid);

        if (!data[idx].valid)
            return;

        pkgCstCfgControlWidget->setData(data[idx].cstConfig, data[idx].cfgLock);
    }

    void PKGCSTConfigControlGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.intelData->coreData.size());
        data.clear();

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (const PWTS::Intel::IntelCoreData &core: packet.intelData->coreData) {
            const PWTS::Intel::PkgCstConfigControl cfg = core.pkgCstConfigControl.getValue();

            data.append({
                .valid = core.pkgCstConfigControl.isValid(),
                .cfgLock = cfg.cfgLock,
                .cstConfig = cfg
            });
        }

        updatePkgCstConfigControlWidget(cpuSelect->currentIndex());
    }

    void PKGCSTConfigControlGBox::setDataForPacket(const PWTS::ClientPacket &packet) {
        if (!isEnabled())
            return;

        const PWTS::Intel::PkgCstConfigControl cstConfigData = pkgCstCfgControlWidget->getData();

        if (applyToAll->isChecked()) {
            for (WData &wd: data) {
                if (!wd.valid)
                    continue;

                wd.cstConfig = cstConfigData;
            }
        } else if (data[cpuSelect->currentIndex()].valid) {
            data[cpuSelect->currentIndex()].cstConfig = cstConfigData;
        }

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.intelData->coreData[i].pkgCstConfigControl.setValue(data[i].cstConfig, true);
        }
    }

    void PKGCSTConfigControlGBox::onCpuSelectChanged(const int idx) {
        if (data[prevCpuSelect].valid)
            data[prevCpuSelect].cstConfig = pkgCstCfgControlWidget->getData();

        prevCpuSelect = idx;

        updatePkgCstConfigControlWidget(idx);
    }

    void PKGCSTConfigControlGBox::onApplyToAllStateChanged(const Qt::CheckState state) const {
        cpuSelect->setEnabled(state != Qt::Checked);
    }
}
