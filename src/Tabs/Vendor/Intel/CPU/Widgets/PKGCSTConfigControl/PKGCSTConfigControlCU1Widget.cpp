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

#include "PKGCSTConfigControlCU1Widget.h"
#include "../../../../../Include/FlowLayout.h"

namespace PWT::UI::INTEL {
    PKGCSTConfigControlCU1Widget::PKGCSTConfigControlCU1Widget() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QVBoxLayout *pkgCstateLyt = new QVBoxLayout();
        QVBoxLayout *maxCoreCstateLyt = new QVBoxLayout();
        QHBoxLayout *cstateLimLyt = new QHBoxLayout();
        FlowLayout *optsLyt = new FlowLayout();

        pkgCStateLimit = new QComboBox();
        maxCoreCState = new QComboBox();
        ioMwaitRedirectionEnable = new QCheckBox("I/O MWAIT redirection enable");
        cfgLock = new QCheckBox("CFG lock");
        c3StateAutodemotionEnable = new QCheckBox("C3 state auto-demotion enable");
        c1StateAutodemotionEnable = new QCheckBox("C1 state auto-demotion enable");
        c3UndemotionEnable = new QCheckBox("C3 un-demotion enable");
        c1UndemotionEnable = new QCheckBox("C1 un-demotion enable");
        pkgcAutodemotionEnable = new QCheckBox("Package C-State auto-demotion enable");
        pkgcUndemotionEnable = new QCheckBox("Package C-State un-demotion enable");
        timedMwaitEnable = new QCheckBox("Timed MWAIT feature enable");

        pkgCStateLimit->addItem("C0/C1 (no package C-sate support)", 0b0000);
        pkgCStateLimit->addItem("C2", 0b0001);
        pkgCStateLimit->addItem("C3", 0b0010);
        pkgCStateLimit->addItem("C6", 0b0011);
        pkgCStateLimit->addItem("C7", 0b0100);
        pkgCStateLimit->addItem("C7s", 0b0101);
        pkgCStateLimit->addItem("C8", 0b0110);
        pkgCStateLimit->addItem("C9", 0b0111);
        pkgCStateLimit->addItem("C10", 0b1000);
        maxCoreCState->addItem("C1", 0b0001);
        maxCoreCState->addItem("C3", 0b0010);
        maxCoreCState->addItem("C6", 0b0011);

        pkgCstateLyt->addWidget(new QLabel("Package C-State Limit"));
        pkgCstateLyt->addWidget(pkgCStateLimit);
        maxCoreCstateLyt->addWidget(new QLabel("Max core C-State"));
        maxCoreCstateLyt->addWidget(maxCoreCState);
        cstateLimLyt->addLayout(pkgCstateLyt);
        cstateLimLyt->addSpacing(4);
        cstateLimLyt->addLayout(maxCoreCstateLyt);
        cstateLimLyt->addStretch();
        optsLyt->addWidget(ioMwaitRedirectionEnable);
        optsLyt->addWidget(cfgLock);
        optsLyt->addWidget(c3StateAutodemotionEnable);
        optsLyt->addWidget(c1StateAutodemotionEnable);
        optsLyt->addWidget(c3UndemotionEnable);
        optsLyt->addWidget(c1UndemotionEnable);
        optsLyt->addWidget(pkgcAutodemotionEnable);
        optsLyt->addWidget(pkgcUndemotionEnable);
        optsLyt->addWidget(timedMwaitEnable);
        lyt->addLayout(cstateLimLyt);
        lyt->addLayout(optsLyt);

        setLayout(lyt);

        QObject::connect(cfgLock, &QCheckBox::checkStateChanged, this, &PKGCSTConfigControlCU1Widget::onCfgLockStateChanged);
    }

    void PKGCSTConfigControlCU1Widget::setData(const PWTS::Intel::PkgCstConfigControl &data, const bool isBiosLocked) const {
        const QSignalBlocker sblock {cfgLock};

        pkgCStateLimit->setCurrentIndex(pkgCStateLimit->findData(data.packageCStateLimit));
        maxCoreCState->setCurrentIndex(maxCoreCState->findData(data.maxCoreCState));
        ioMwaitRedirectionEnable->setChecked(data.ioMwaitRedirectionEnable);
        cfgLock->setChecked(data.cfgLock);
        c3StateAutodemotionEnable->setChecked(data.c3StateAutodemotionEnable);
        c1StateAutodemotionEnable->setChecked(data.c1StateAutodemotionEnable);
        c3UndemotionEnable->setChecked(data.c3UndemotionEnable);
        c1UndemotionEnable->setChecked(data.c1UndemotionEnable);
        pkgcAutodemotionEnable->setChecked(data.pkgcAutodemotionEnable);
        pkgcUndemotionEnable->setChecked(data.pkgcUndemotionEnable);
        timedMwaitEnable->setChecked(data.timedMwaitEnable);
        applyCFGLock(isBiosLocked);
    }

    PWTS::Intel::PkgCstConfigControl PKGCSTConfigControlCU1Widget::getData() const {
        return {
            .packageCStateLimit = static_cast<short>(pkgCStateLimit->currentData().toInt()),
            .maxCoreCState = static_cast<short>(maxCoreCState->currentData().toInt()),
            .ioMwaitRedirectionEnable = ioMwaitRedirectionEnable->isChecked(),
            .cfgLock = cfgLock->isChecked(),
            .c3StateAutodemotionEnable = c3StateAutodemotionEnable->isChecked(),
            .c1StateAutodemotionEnable = c1StateAutodemotionEnable->isChecked(),
            .c3UndemotionEnable = c3UndemotionEnable->isChecked(),
            .c1UndemotionEnable = c1UndemotionEnable->isChecked(),
            .pkgcAutodemotionEnable = pkgcAutodemotionEnable->isChecked(),
            .pkgcUndemotionEnable = pkgcUndemotionEnable->isChecked(),
            .timedMwaitEnable = timedMwaitEnable->isChecked()
        };
    }

    void PKGCSTConfigControlCU1Widget::applyCFGLock(const bool biosLock) const {
        const bool unlocked = !cfgLock->isChecked() && !biosLock;

        cfgLock->setEnabled(!biosLock);
        pkgCStateLimit->setEnabled(unlocked);
        maxCoreCState->setEnabled(unlocked);
        ioMwaitRedirectionEnable->setEnabled(unlocked);
    }

    void PKGCSTConfigControlCU1Widget::onCfgLockStateChanged(const Qt::CheckState state) const {
        const bool locked = state == Qt::Checked;

        pkgCStateLimit->setEnabled(!locked);
        maxCoreCState->setEnabled(!locked);
        ioMwaitRedirectionEnable->setEnabled(!locked);
    }
}
