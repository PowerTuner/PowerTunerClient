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
#include <QApplication>
#include <QCommandLineParser>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication::setQuitOnLastWindowClosed(false);

    const QCommandLineOption startupOp {"s", "start minimized"};
    QApplication a(argc, argv);
    QCommandLineParser parser;
    MainWindow w;

    parser.addOption(startupOp);
    a.installEventFilter(&w);
    w.setWindowIcon(QIcon(":/ico/pwt"));
    parser.process(a);

    if (parser.isSet("s")) {
        QCloseEvent evt {};

        w.closeEvent(&evt);

    } else {
        w.show();
    }

    return a.exec();
}
