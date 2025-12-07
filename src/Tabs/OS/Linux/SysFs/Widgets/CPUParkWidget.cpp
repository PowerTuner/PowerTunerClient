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
#include "CPUParkWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX {
    CPUParkWidget::CPUParkWidget(const int threadCount): GridWidget(4) {
        for (int i=0; i<threadCount; ++i) {
            QLabel *lbl = new QLabel("CPU");
            QVBoxLayout *cpuLyt = new QVBoxLayout();
            QComboBox *combo = new QComboBox();

            combo->addItems({"Disabled", "Enabled"});
            combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            cpuLyt->addWidget(lbl);
            cpuLyt->addWidget(combo);

            setNextSlot();
            gridLyt->addLayout(cpuLyt, row, column);

            cpuList.append({lbl, combo});

            QObject::connect(combo, &QComboBox::currentIndexChanged, this, &CPUParkWidget::onCPUStateChange);
        }
    }

    void CPUParkWidget::setCPULabelColor(const int idx, const int enabled) const {
        QPalette pal = cpuList[idx].combo->palette();

        pal.setColor(QPalette::WindowText, enabled ? Qt::darkGreen : Qt::red);
        cpuList[idx].lbl->setPalette(pal);
    }

    void CPUParkWidget::setData(const PWTS::DaemonPacket &packet) {
        const QList<PWTS::LNX::LinuxThreadData> &thdData = packet.linuxData->threadData;

        setEnabled(!cpuList.isEmpty() && cpuList.size() == thdData.size());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        onlineCount = 0;

        for (int i=0,l=thdData.size(); i<l; ++i) {
            const PWTS::LNX::LinuxThreadData &thd = thdData[i];
            const bool isValidData = thd.cpuOnlineStatus.isValid();

            cpuList[i].combo->setEnabled(isValidData && thd.cpuLogicalOffAvailable.isValid() && thd.cpuLogicalOffAvailable.getValue());

            if (!isValidData)
                continue;

            const QSignalBlocker sblock {cpuList[i].combo};
            const int status = thd.cpuOnlineStatus.getValue();
            const QString coreID = thd.coreID.isValid() ? QString(" [core %1]").arg(thd.coreID.getValue()) : "";

            cpuList[i].lbl->setText(QString("CPU %1%2").arg(i).arg(coreID));
            cpuList[i].combo->setCurrentIndex(status);
            setCPULabelColor(i, status);

            if (status == 1)
                ++onlineCount;
        }
    }

    void CPUParkWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        int i=0;

        for (const CPU &cpu: cpuList)
            packet.linuxData->threadData[i++].cpuOnlineStatus.setValue(cpu.combo->currentIndex(), true);
    }

    void CPUParkWidget::updateOnlineCPUs(int newOnlineCount) {
        for (int i=0,l=cpuList.size(); i<l; ++i) {
            QComboBox *cpu = cpuList[i].combo;

            if (!cpu->isEnabled()) {
                --newOnlineCount;
                continue;
            }

            const int enable = newOnlineCount > 0;

            if (cpu->currentIndex() != enable) {
                const QSignalBlocker sblock {cpu};

                cpu->setCurrentIndex(enable);
                setCPULabelColor(i, enable);
                onlineCount += enable ? 1 : -1;
            }

            --newOnlineCount;
        }
    }

    void CPUParkWidget::onCPUStateChange(const int enable) {
        const QComboBox *combo = qobject_cast<QComboBox *>(QObject::sender());

        if (combo == nullptr)
            return;

        for (int i=0,l=cpuList.size(); i<l; ++i) {
            if (cpuList[i].combo != combo)
                continue;

            onlineCount += enable ? 1 : -1;
            setCPULabelColor(i, enable);
            emit statusChanged();
            break;
        }
    }
}
