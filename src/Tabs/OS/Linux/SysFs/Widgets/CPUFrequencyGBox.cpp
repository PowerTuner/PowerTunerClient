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
#include "CPUFrequencyGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX {
    CPUFrequencyGBox::CPUFrequencyGBox(const int threadCount): QGroupBox("CPU frequency") {
        const auto unitVCB = [](QLabel *unitV, const int v) { unitV->setNum(v); };
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *cpuLyt = new QHBoxLayout();

        minAll = new SliderUnitWidget("MHz", unitVCB);
        maxAll = new SliderUnitWidget("MHz", unitVCB);
        cpuSelect = new QComboBox();
        minFreq = new SliderUnitWidget("MHz", unitVCB);
        maxFreq = new SliderUnitWidget("MHz", unitVCB);

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addItem(QString("CPU %1").arg(i));

        minAll->setPageStep(500);
        minAll->setStaticLabel("Minimum (All)");
        maxAll->setPageStep(500);
        maxAll->setStaticLabel("Maximum (All)");
        minFreq->setPageStep(500);
        minFreq->setStaticLabel("Minimum");
        maxFreq->setPageStep(500);
        maxFreq->setStaticLabel("Maximum");

        cpuLyt->addStretch();
        cpuLyt->addWidget(cpuSelect);
        lyt->addWidget(minAll);
        lyt->addSpacing(6);
        lyt->addWidget(maxAll);
        lyt->addSpacing(10);
        lyt->addLayout(cpuLyt);
        lyt->addSpacing(6);
        lyt->addWidget(minFreq);
        lyt->addWidget(maxFreq);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(minAll, &SliderUnitWidget::valueChanged, this, &CPUFrequencyGBox::onMinAllChanged);
        QObject::connect(maxAll, &SliderUnitWidget::valueChanged, this, &CPUFrequencyGBox::onMaxAllChanged);
        QObject::connect(minFreq, &SliderUnitWidget::valueChanged, this, &CPUFrequencyGBox::onMinFreqChanged);
        QObject::connect(maxFreq, &SliderUnitWidget::valueChanged, this, &CPUFrequencyGBox::onMaxFreqChanged);
        QObject::connect(cpuSelect, &QComboBox::currentIndexChanged, this, &CPUFrequencyGBox::onCPUSelectChanged);
    }

    void CPUFrequencyGBox::resetAllSliders() const {
        const QSignalBlocker sblockMinAll {minAll};
        const QSignalBlocker sblockMaxAll {maxAll};

        minAll->setValue(minAll->getMinumum());
        minAll->setUnitValue(minAll->getMinumum());
        maxAll->setValue(maxAll->getMinumum());
        maxAll->setUnitValue(maxAll->getMinumum());
    }

    void CPUFrequencyGBox::setRelatedCPUs(const bool minimum, const int freq) {
        for (const QString &relatedCPU: data[cpuSelect->currentIndex()].limits.relatedCPUs) {
            bool res;
            const int cpuN = relatedCPU.toInt(&res);

            if (!res || !data[cpuN].valid)
                continue;
            else if (minimum)
                data[cpuN].freq.min = freq;
            else
                data[cpuN].freq.max = freq;
        }
    }

    void CPUFrequencyGBox::setData(const PWTS::DaemonPacket &packet) {
        const QList<PWTS::LNX::LinuxThreadData> &thdData = packet.linuxData->threadData;

        setEnabled(cpuSelect->count() && cpuSelect->count() == thdData.size());
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const QSignalBlocker sblockMinAll {minAll};
        const QSignalBlocker sblockMaxAll {maxAll};
        int minAllLimit = INT_MAX;
        int maxAllLimit = 0;

        for (const PWTS::LNX::LinuxThreadData &thd: thdData) {
            const PWTS::LNX::CPUFrequencyLimits limits = thd.cpuFrequencyLimits.getValue();
            const bool limitsValid = thd.cpuFrequencyLimits.isValid();
            const bool isCpuOn = thd.cpuOnlineStatus.isValid() && thd.cpuOnlineStatus.getValue() == 1;

            if (limitsValid) {
                if (limits.limit.min < minAllLimit)
                    minAllLimit = limits.limit.min;

                if (limits.limit.max > maxAllLimit)
                    maxAllLimit = limits.limit.max;
            }

            data.append({
                .valid = isCpuOn && limitsValid && thd.cpuFrequency.isValid(),
                .limits = limits,
                .freq = thd.cpuFrequency.getValue()
            });
        }

        minAll->setRange(minAllLimit, maxAllLimit);
        minAll->setEnabled(minAllLimit != INT_MAX && maxAllLimit > 0);
        maxAll->setRange(minAllLimit, maxAllLimit);
        maxAll->setEnabled(minAllLimit != INT_MAX && maxAllLimit > 0);
        onCPUSelectChanged(cpuSelect->currentIndex());
    }

    void CPUFrequencyGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (int i=0,l=data.size(); i<l; ++i) {
            if (!data[i].valid)
                continue;

            packet.linuxData->threadData[i].cpuFrequency.setValue({
                .min = data[i].freq.min,
                .max = data[i].freq.max
            }, true);
        }
    }

    void CPUFrequencyGBox::onMinAllChanged(const int v) {
        const QSignalBlocker sblock {minFreq};

        for (WData &wd: data) {
            if (wd.valid)
                wd.freq.min = v;
        }

        if (!minFreq->isEnabled())
            return;

        minFreq->setValue(v);
        minFreq->setUnitValue(v);
    }

    void CPUFrequencyGBox::onMaxAllChanged(const int v) {
        const QSignalBlocker sblock {maxFreq};

        for (WData &wd: data) {
            if (wd.valid)
                wd.freq.max = v;
        }

        if (!maxFreq->isEnabled())
            return;

        maxFreq->setValue(v);
        maxFreq->setUnitValue(v);
    }

    void CPUFrequencyGBox::onMinFreqChanged(const int v) {
        if (!data[cpuSelect->currentIndex()].valid)
            return;

        data[cpuSelect->currentIndex()].freq.min = v;
        setRelatedCPUs(true, v);
        minFreq->setUnitValue(v);
        resetAllSliders();
    }

    void CPUFrequencyGBox::onMaxFreqChanged(const int v) {
        if (!data[cpuSelect->currentIndex()].valid)
            return;

        data[cpuSelect->currentIndex()].freq.max = v;
        setRelatedCPUs(false, v);
        maxFreq->setUnitValue(v);
        resetAllSliders();
    }

    void CPUFrequencyGBox::onCPUSelectChanged(const int idx) {
        const QSignalBlocker sblockMin {minFreq};
        const QSignalBlocker sblockMax {maxFreq};
        const bool isValid = data[idx].valid;

        resetAllSliders();
        minFreq->setEnabled(isValid);
        maxFreq->setEnabled(isValid);

        if (!isValid)
            return;

        minFreq->setRange(data[idx].limits.limit.min, data[idx].limits.limit.max);
        minFreq->setValue(data[idx].freq.min);
        minFreq->setUnitValue(data[idx].freq.min);
        maxFreq->setRange(data[idx].limits.limit.min, data[idx].limits.limit.max);
        maxFreq->setValue(data[idx].freq.max);
        maxFreq->setUnitValue(data[idx].freq.max);
    }
}
