/*
    Collisions - a real-time simulation program of colliding particles.
    Copyright (C) 2011 - 2015  G. Endignoux

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.txt
*/

#include <QApplication>
#include <QMessageBox>
#include "main_window.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.showMaximized();

    try
    {
        app.exec();
    }
    catch (const std::exception& except)
    {
        QMessageBox::critical(0, "Error", QString::fromUtf8(except.what()));
    }
    catch (...)
    {
        QMessageBox::critical(0, "Error", "An unknown error occurred !");
    }

    return 0;
}
