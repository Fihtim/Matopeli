/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: main.cpp                                                   #
#                                                                  #
# Author:                                                          #
#   First name: Fitim                                              #
#   Last name: Abazi                                               #
#   Student id: 150417967                                          #
#   email: fitim.abazi@tuni.fi                                     #
####################################################################
*
* Description:
*   Runs the projects code and graphical ui
*/

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
