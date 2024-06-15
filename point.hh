/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: point.hh                                                   #
#                                                                  #
# Program author:                                                  #
#    Name: Fitim Abazi                                             #
#    Student number: 150417967                                     #
#    UserID: qbfiab                                                #
#    E-Mail: fitim.abazi@tuni.fi                                   #
####################################################################
* Description:
*   Declares a class representing a point in the gameboard.
*
* Notes:
*   This file is kept as close to original file as possible. Only
* change is that getters were used to get x and y locations of the
* items from the private section to public section
*/

#ifndef POINT_HH
#define POINT_HH

#include <string>

// Constants for directions
const char UP = 'w';
const char LEFT = 'a';
const char DOWN = 's';
const char RIGHT = 'd';

class Point
{
public:
    // Default constructor, creates a new point at (0, 0).
    Point();

    // Constructor, creates a new point at the given coordinates.
    Point(int x, int y);

    // Destructor
    ~Point();

    // Comparison operators
    bool operator==(const Point& rhs) const;
    bool operator!=(const Point& rhs) const;

    // Sets a new position for the point
    void setPosition(int x, int y);

    // Moves the point into the given direction, direction can be one of
    // constants above.
    void move(const std::string& dir);

    // Tells if the point is inside the square specified by the given
    // parameters.
    bool isInside(int left_top_x, int left_top_y,
                  int right_bottom_x, int right_bottom_y) const;

    int x() const;

    int y() const;

private:
    // Coordinates of the point
    int x_;
    int y_;
};

#endif // POINT_HH
