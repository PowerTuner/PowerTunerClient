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
#include <QLabel>

#include "PowerSettingGBox.h"

namespace PWT::UI::WIN {
	PowerSettingGBox::PowerSettingGBox(const QString &title, const QString &groupTitle, const QString &guid): QGroupBox(title) {
		QHBoxLayout *modeLyt = new QHBoxLayout();
		QLabel *modeLbl = new QLabel(QStringLiteral("Mode:"));

		lyt = new QVBoxLayout();
		mode = new QComboBox();
		settingTitle = title;
		settingGroupTitle = groupTitle.isEmpty() ? QStringLiteral("No group") : groupTitle;

		mode->addItems({QStringLiteral("Plugged In"), QStringLiteral("Battery")});
		modeLbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

		modeLyt->addWidget(new QLabel(guid));
		modeLyt->addStretch();
		modeLyt->addWidget(modeLbl);
		modeLyt->addWidget(mode);
		lyt->addLayout(modeLyt);
		lyt->addSpacing(8);
		lyt->addWidget(new QLabel(QString("Group: %1").arg(settingGroupTitle)));
		lyt->addSpacing(8);

		setAlignment(Qt::AlignCenter);
		setLayout(lyt);

		QObject::connect(mode, &QComboBox::currentIndexChanged, this, &PowerSettingGBox::onModeChanged);
	}

	void PowerSettingGBox::setData(const QString &scheme, const PWTS::WIN::PowerSchemeSetting &setting) {
		settingGroupGuid = setting.groupGuid;
		settings.insert(scheme, setting.value);
	}

	void PowerSettingGBox::switchScheme(const QString &scheme) {
		currentScheme = scheme;

		onModeChanged(mode->currentIndex());
	}

	void PowerSettingGBox::duplicateScheme(const QString &current, const QString &duplicate) {
		settings.insert(duplicate, settings.value(current));
	}

	void PowerSettingGBox::deleteScheme(const QString &scheme) {
		settings.remove(scheme);
	}

    void PowerSettingGBox::setMode(const int idx) const {
        mode->setCurrentIndex(idx);
    }

    void PowerSettingGBox::copyValue(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) {
        if (toMode == 0)
            settings[toScheme].ac = fromMode == 0 ? settings[fromScheme].ac : settings[fromScheme].dc;
        else if (toMode == 1)
            settings[toScheme].dc = fromMode == 0 ? settings[fromScheme].ac : settings[fromScheme].dc;
        else
            settings[toScheme] = settings[fromScheme];

        if (fromScheme == toScheme) // refresh ui
            onModeChanged(mode->currentIndex());
    }

	void PowerSettingGBox::onSettingValueChanged(const int v) {
		PWTS::WIN::PowerSettingValue sett = settings.value(currentScheme);

		if (mode->currentIndex() == 0)
			sett.ac = v;
		else
			sett.dc = v;

		settings.insert(currentScheme, sett);
	}
}
