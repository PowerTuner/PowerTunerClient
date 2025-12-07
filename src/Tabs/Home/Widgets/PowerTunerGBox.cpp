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

#include "PowerTunerGBox.h"
#include "pwtShared/Utils.h"
#include "pwtClientService/ClientService.h"
#include "pwtClientCommon/CommonUtils.h"
#include "../../../version.h"

namespace PWT::UI {
    PowerTunerGBox::PowerTunerGBox(): QGroupBox("PowerTuner") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *clientLyt = new QHBoxLayout();
        QHBoxLayout *serviceLyt = new QHBoxLayout();
        QHBoxLayout *pwtsLyt = new QHBoxLayout();
        QHBoxLayout *commonLyt = new QHBoxLayout();
        QHBoxLayout *daemonLyt = new QHBoxLayout();
        QHBoxLayout *daemonPwtsLyt = new QHBoxLayout();

        daemonVer = new QLabel("0");
        daemonPwtsVer = new QLabel("0");

        clientLyt->addWidget(new QLabel("Client version:"));
        clientLyt->addStretch();
        clientLyt->addWidget(new QLabel(QString("%1.%2").arg(CLIENT_VER_MAJOR).arg(CLIENT_VER_MINOR)));
        serviceLyt->addWidget(new QLabel("libPWTService version:"));
        serviceLyt->addStretch();
        serviceLyt->addWidget(new QLabel(QString("%1.%2").arg(PWTCS::ClientService::getMajorVersion()).arg(PWTCS::ClientService::getMinorVersion())));
        pwtsLyt->addWidget(new QLabel("libPWTShared version:"));
        pwtsLyt->addStretch();
        pwtsLyt->addWidget(new QLabel(QString("%1.%2").arg(PWTS::getLibMajorVersion()).arg(PWTS::getLibMinorVersion())));
        commonLyt->addWidget(new QLabel("libClientCommon version:"));
        commonLyt->addStretch();
        commonLyt->addWidget(new QLabel(QString("%1.%2").arg(getCommonMajorVersion()).arg(getCommonMinorVersion())));
        daemonLyt->addWidget(new QLabel("Daemon version:"));
        daemonLyt->addStretch();
        daemonLyt->addWidget(daemonVer);
        daemonPwtsLyt->addWidget(new QLabel("Daemon libPWTShared version:"));
        daemonPwtsLyt->addStretch();
        daemonPwtsLyt->addWidget(daemonPwtsVer);
        lyt->addLayout(clientLyt);
        lyt->addLayout(serviceLyt);
        lyt->addLayout(commonLyt);
        lyt->addLayout(pwtsLyt);
        lyt->addLayout(daemonLyt);
        lyt->addLayout(daemonPwtsLyt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
    }

    void PowerTunerGBox::setDaemonVersion(const int major, const int minor) const {
        daemonVer->setText(QString("%1.%2").arg(major).arg(minor));
    }

    void PowerTunerGBox::setDaemonPWTSVersion(const int major, const int minor) const {
        daemonPwtsVer->setText(QString("%1.%2").arg(major).arg(minor));
    }

    void PowerTunerGBox::clearWidget() const {
        daemonVer->setNum(0);
        daemonPwtsVer->setNum(0);
    }
}
