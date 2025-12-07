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
#include <QDateTime>
#include <QScroller>

#include "LogTab.h"

namespace PWT::UI {
    LogTab::LogTab() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QVBoxLayout *btnLyt = new QVBoxLayout();

        logs = new QTextEdit();
        clearBtn = new QPushButton("Clear");

        logs->setWordWrapMode(QTextOption::NoWrap);
        logs->setReadOnly(true);
        logs->setAcceptRichText(true);
        logs->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
        clearBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        QScroller::grabGesture(logs->viewport(), QScroller::LeftMouseButtonGesture);

        btnLyt->addWidget(clearBtn);
        btnLyt->setAlignment(Qt::AlignRight);
        lyt->addWidget(logs);
        lyt->addLayout(btnLyt);

        setLayout(lyt);

        QObject::connect(clearBtn, &QPushButton::clicked, logs, &QTextEdit::clear);
    }

    void LogTab::writeLog(const QString &msg) const {
        logs->append(QString("%1 %2").arg(QDateTime::currentDateTime().toString("[ddd MMMM d yyyy hh:mm:ss] "), msg));
    }
}
