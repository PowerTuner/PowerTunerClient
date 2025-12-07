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
#include <QLabel>

#include "PKGCSTConfigControlSBWidget.h"
#include "../../../../../Include/FlowLayout.h"

namespace PWT::UI::INTEL {
    PKGCSTConfigControlSBWidget::PKGCSTConfigControlSBWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *cstateLimLyt = new QHBoxLayout();
        FlowLayout *optsLyt = new FlowLayout();

        pkgCStateLimit = new QComboBox();
        ioMwaitRedirectionEnable = new QCheckBox("I/O MWAIT redirection enable");
        cfgLock = new QCheckBox("CFG lock");
        c3StateAutodemotionEnable = new QCheckBox("C3 state auto-demotion enable");
        c1StateAutodemotionEnable = new QCheckBox("C1 state auto-demotion enable");
        c3UndemotionEnable = new QCheckBox("C3 un-demotion enable");
        c1UndemotionEnable = new QCheckBox("C1 un-demotion enable");

        pkgCStateLimit->addItem("C0/C1 (no package C-sate support)", 0b000);
        pkgCStateLimit->addItem("C2", 0b001);
        pkgCStateLimit->addItem("C6 no retention", 0b010);
        pkgCStateLimit->addItem("C6 retention", 0b011);
        pkgCStateLimit->addItem("C7", 0b100);
        pkgCStateLimit->addItem("C7s", 0b101);
        pkgCStateLimit->addItem("No package C-state limit", 0b111);

        cstateLimLyt->addWidget(new QLabel("Package C-State Limit:"));
        cstateLimLyt->addWidget(pkgCStateLimit);
        cstateLimLyt->addStretch();
        optsLyt->addWidget(ioMwaitRedirectionEnable);
        optsLyt->addWidget(cfgLock);
        optsLyt->addWidget(c3StateAutodemotionEnable);
        optsLyt->addWidget(c1StateAutodemotionEnable);
        optsLyt->addWidget(c3UndemotionEnable);
        optsLyt->addWidget(c1UndemotionEnable);
        lyt->addLayout(cstateLimLyt);
        lyt->addLayout(optsLyt);

        setLayout(lyt);

        QObject::connect(cfgLock, &QCheckBox::checkStateChanged, this, &PKGCSTConfigControlSBWidget::onCfgLockStateChanged);
    }

    void PKGCSTConfigControlSBWidget::setData(const PWTS::Intel::PkgCstConfigControl &data, const bool isBiosLocked) const {
        const QSignalBlocker sblock {cfgLock};

        pkgCStateLimit->setCurrentIndex(pkgCStateLimit->findData(data.packageCStateLimit));
        ioMwaitRedirectionEnable->setChecked(data.ioMwaitRedirectionEnable);
        cfgLock->setChecked(data.cfgLock);
        c3StateAutodemotionEnable->setChecked(data.c3StateAutodemotionEnable);
        c1StateAutodemotionEnable->setChecked(data.c1StateAutodemotionEnable);
        c3UndemotionEnable->setChecked(data.c3UndemotionEnable);
        c1UndemotionEnable->setChecked(data.c1UndemotionEnable);
        applyCFGLock(isBiosLocked);
    }

    PWTS::Intel::PkgCstConfigControl PKGCSTConfigControlSBWidget::getData() const {
        return {
            .packageCStateLimit = static_cast<short>(pkgCStateLimit->currentData().toInt()),
            .ioMwaitRedirectionEnable = ioMwaitRedirectionEnable->isChecked(),
            .cfgLock = cfgLock->isChecked(),
            .c3StateAutodemotionEnable = c3StateAutodemotionEnable->isChecked(),
            .c1StateAutodemotionEnable = c1StateAutodemotionEnable->isChecked(),
            .c3UndemotionEnable = c3UndemotionEnable->isChecked(),
            .c1UndemotionEnable = c1UndemotionEnable->isChecked()
        };
    }

    void PKGCSTConfigControlSBWidget::applyCFGLock(const bool biosLock) const {
        const bool unlocked = !cfgLock->isChecked() && !biosLock;

        cfgLock->setEnabled(!biosLock);
        pkgCStateLimit->setEnabled(unlocked);
        ioMwaitRedirectionEnable->setEnabled(unlocked);
    }

    void PKGCSTConfigControlSBWidget::onCfgLockStateChanged(const Qt::CheckState state) const {
        const bool locked = state == Qt::Checked;

        pkgCStateLimit->setEnabled(!locked);
        ioMwaitRedirectionEnable->setEnabled(!locked);
    }
}
