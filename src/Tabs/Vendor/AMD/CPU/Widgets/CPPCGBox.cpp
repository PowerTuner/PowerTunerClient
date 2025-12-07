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
#include "CPPCGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    CPPCGBox::CPPCGBox(const QSet<PWTS::Feature> &cpuFeatures, const int threadCount): QGroupBox("CPPC Request") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *enableLyt = new QHBoxLayout();
        QHBoxLayout *cpuSelLyt = new QHBoxLayout();

        enable = new QCheckBox("CPPC enable");
        cpuSelect = new QComboBox();
        applyToAll = new QCheckBox("Apply to all CPUs");
        cppcWidget = new CPPCWidget();

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addItem(QString("CPU %1").arg(i));

        if (cpuFeatures.contains(PWTS::Feature::AMD_PSTATE_SYSFS) || cpuFeatures.contains(PWTS::Feature::PWR_SCHEME_GROUP)) {
            preferOSOpt = new QCheckBox("Prefer OS setting");

            preferOSOpt->setChecked(true);
            preferOSOpt->setToolTip("If checked, this option is ignored and the OS setting will be applied instead, for example SysFs in Linux or power scheme option in Windows.");
            enableLyt->addWidget(preferOSOpt);
        }

        enableLyt->addWidget(enable);
        enableLyt->addStretch();
        cpuSelLyt->addStretch();
        cpuSelLyt->addWidget(applyToAll);
        cpuSelLyt->addWidget(cpuSelect);
        lyt->addLayout(enableLyt);
        lyt->addLayout(cpuSelLyt);
        lyt->addWidget(cppcWidget);

        setLayout(lyt);
        setAlignment(Qt::AlignCenter);
        setEnabled(false);

        QObject::connect(applyToAll, &QCheckBox::checkStateChanged, this, &CPPCGBox::onApplyToAllChanged);
        QObject::connect(cpuSelect, &QComboBox::currentIndexChanged, this, &CPPCGBox::onCpuSelectChanged);
    }

    void CPPCGBox::updateCPPCWidget(const int idx) const {
        cppcWidget->setEnabled(data[idx].valid);

        if (!cppcWidget->isEnabled())
            return;

        cppcWidget->setRanges(data[idx].caps1);
        cppcWidget->setData(data[idx].request);
    }

    void CPPCGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.amdData->threadData.size());
        data.clear();

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const int enableBitValid = packet.amdData->cppcEnableBit.isValid();
        const int enableBit = packet.amdData->cppcEnableBit.getValue();

        enable->setEnabled(enableBitValid && enableBit == 0);
        enable->setChecked(enableBitValid && enableBit == 1);

        for (const PWTS::AMD::AMDThreadData &thd: packet.amdData->threadData) {
            data.append({
                .valid = thd.cppcCapability1.isValid() && thd.cppcRequest.isValid(),
                .caps1 = thd.cppcCapability1.getValue(),
                .request = thd.cppcRequest.getValue()
            });
        }

        updateCPPCWidget(cpuSelect->currentIndex());
    }

    void CPPCGBox::setDataForPacket(const PWTS::ClientPacket &packet) {
        if (!isEnabled() || (!preferOSOpt.isNull() && preferOSOpt->isChecked()))
            return;

        const PWTS::AMD::CPPCRequest reqData = cppcWidget->getData();

        packet.amdData->cppcEnableBit.setValue(enable->isChecked(), enable->isEnabled());

        if (applyToAll->isChecked()) {
            for (WData &wd: data) {
                if (!wd.valid)
                    continue;

                wd.request = reqData;

                // fix bounds
                wd.request.maxPerf = qBound(wd.caps1.lowestPerf, wd.request.maxPerf, wd.caps1.highestPerf);
                wd.request.minPerf = qBound(wd.caps1.lowestPerf, wd.request.minPerf, wd.caps1.highestPerf);
                wd.request.desPerf = wd.request.desPerf > 0 ? qBound(wd.caps1.lowestPerf, wd.request.desPerf, wd.caps1.highestPerf) : 0;
            }
        } else {
            data[cpuSelect->currentIndex()].request = reqData;
        }

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.amdData->threadData[i].cppcRequest.setValue(data[i].request, true);
        }
    }

    void CPPCGBox::onApplyToAllChanged(const Qt::CheckState state) const {
        cpuSelect->setEnabled(state != Qt::Checked);
    }

    void CPPCGBox::onCpuSelectChanged(const int idx) {
        if (data[prevCpuSelect].valid)
            data[prevCpuSelect].request = cppcWidget->getData();

        prevCpuSelect = idx;

        updateCPPCWidget(idx);
    }
}
