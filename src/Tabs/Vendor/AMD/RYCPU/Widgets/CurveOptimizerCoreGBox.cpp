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
#include "CurveOptimizerCoreGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::AMD {
    CurveOptimizerCoreGBox::CurveOptimizerCoreGBox(const int coreCount): QGroupBox("Curve Optimizer") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *cpuLyt = new QHBoxLayout();

        cpuSelect = new QComboBox();
        enableChk = new QCheckBox("Enable setting");
        applyToAll = new QCheckBox("Apply to all cores");
        co = new SliderUnitWidget("", [](QLabel *unitV, const int v) { unitV->setNum(v); });

        for (int i=0; i<coreCount; ++i)
            cpuSelect->addItem(QString("Core %1").arg(i));

        enableChk->setToolTip("If unchecked, this setting is ignored and wont be applied");
        co->setStaticLabel("Offset");
        co->setPageStep(10);

        cpuLyt->addWidget(enableChk);
        cpuLyt->addWidget(applyToAll);
        cpuLyt->addStretch();
        cpuLyt->addWidget(cpuSelect);

        lyt->addLayout(cpuLyt);
        lyt->addStretch();
        lyt->addWidget(co);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(applyToAll, &QCheckBox::checkStateChanged, this, &CurveOptimizerCoreGBox::onApplyToAllStateChanged);
        QObject::connect(cpuSelect, &QComboBox::currentIndexChanged, this, &CurveOptimizerCoreGBox::onCpuSelectChanged);
        QObject::connect(enableChk, &QCheckBox::checkStateChanged, this, &CurveOptimizerCoreGBox::onEnableStateChanged);
    }

    void CurveOptimizerCoreGBox::updateCOSlider(const int idx) const {
        co->setEnabled(data[idx].valid);
        co->setValue(data[idx].co);
    }

    void CurveOptimizerCoreGBox::setRanges(const PWTS::MinMax &range) const {
        co->setRange(range.min, range.max);
    }

    void CurveOptimizerCoreGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.amdData->coreData.size());
        data.clear();

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const QSignalBlocker sblock {enableChk};

        for (const PWTS::AMD::AMDCoreData &cdt: packet.amdData->coreData) {
            data.append({
                .valid = cdt.curveOptimizer.isValid(),
                .co = cdt.curveOptimizer.getValue()
            });
        }

        updateCOSlider(cpuSelect->currentIndex());
        enableChk->setChecked(packet.hasProfileData ? !packet.amdData->coreData[0].curveOptimizer.isIgnored() : enableChecked);
    }

    void CurveOptimizerCoreGBox::setDataForPacket(const PWTS::ClientPacket &packet) {
        if (!isEnabled())
            return;

        const bool isIgnored = !enableChk->isChecked();
        const int curve = co->getValue();

        if (applyToAll->isChecked()) {
            for (WData &wd: data) {
                if (wd.valid)
                    wd.co = curve;
            }
        } else {
            data[cpuSelect->currentIndex()].co = curve;
        }

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.amdData->coreData[i].curveOptimizer.setValue(data[i].co, true, isIgnored);
        }
    }

    void CurveOptimizerCoreGBox::onCpuSelectChanged(const int idx) {
        if (data[prevCPU].valid)
            data[prevCPU].co = co->getValue();

        prevCPU = idx;

        updateCOSlider(idx);
    }

    void CurveOptimizerCoreGBox::onApplyToAllStateChanged(const Qt::CheckState state) const {
        cpuSelect->setEnabled(state != Qt::Checked);
    }

    void CurveOptimizerCoreGBox::onEnableStateChanged(const Qt::CheckState state) {
        enableChecked = state == Qt::Checked;
    }
}
