/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: mainwindow.hh                                              #
#                                                                  #
# Author:                                                          #
#   First name: Fitim                                              #
#   Last name: Abazi                                               #
#   Student id: 150417967                                          #
#   E-mail: fitim.abazi@tuni.fi                                    #
####################################################################
*
* Description:
*   Declares a class representing graphic userinterface
* of the gameboard. Initializes required variables and functions.
* This class describes the snake's and points locations on the gui
* and these functions are only used receive data from the gameboard
* and point classes. In practice gameboard doesn't know anything
* about this class while this class knows everything (needed) from it
*
* Notes:
*   Most of the functions of this class are used to manipulate the gui
* buttons, spinboxes and the location of the snake. Timers are used to
* implement movement of the snake and the functionality of game timer.
* gameboards "gameboard" is stored into a unique smart pointer so that
* it keeps itself from memory leakage. Other variables regarding the
* gameboards snake and food are stored as QGraphical items and they are
* kept separate from their API counterparts.
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"
#include <QMainWindow>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QTimer>
#include <memory>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// constants used in this gui
const QBrush GREENBRUSH(Qt::green);
const QBrush BLUEBRUSH(Qt::blue);
const QBrush REDBRUSH(Qt::red);
const int STEP = 20; // how much to move at a time
const int GAMETIMER = 1000; // sets the game round timer


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief keyPressEvent void function which reads users keyboard inputs for
     * the spesific keys
     * @param event variable which is used to access the functionality of the
     * inputs
     */
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    /**
     * @brief onPlayButtonClicked void function which
     * starts the game with user given parameters, also resets the game with
     * to initial status
     */
    void onPlayButtonClicked();

    /**
     * @brief snakeMoveTimeOut void function which implements
     * timer to move the snake which is used to call
     * functions which require the snake's movement
     */
    void snakeMoveTimeOut();

    /**
     * @brief onCloseButtonClicked void function which closes the game
     */
    void onCloseButtonClicked();

    /**
     * @brief onPauseButtonClicked void function which pauses and unpauses
     * the game
     */
    void onPauseButtonClicked();

    /**
     * @brief gameTimerTimeOut void function which increments the game round
     * timer on each timeout
     */
    void gameTimerTimeOut();

    /**
     * @brief onClassicModeButtonClicked void function which enables classic
     * game style snake game which has maximum points of full area
     */
    void onClassicModeButtonClicked();

    /**
     * @brief onTargetModeButtonClicked void function which enables alternate
     * game style in which user tries to reach set target value
     */
    void onTargetModeButtonClicked();

private:
    Ui::MainWindow *ui;

    // surface for drawing the snake and circles
    QGraphicsScene* gameScene_;

    // used for drawing the snake
    QGraphicsRectItem* snake_;

    // used for drawing the snake's tail
    QGraphicsRectItem* snakeTail_;

    // vector item which is used to draw the snakes body
    std::vector<QGraphicsRectItem*> snakeBody_ = {};

    //drawing the circle, aka "apple"
    QGraphicsEllipseItem* apple_;

    // for moving the snake
    QTimer timerSnake_;

    // current board as unique pointer, easies the memory handling
    std::unique_ptr<GameBoard> currentBoard_;

    // timer for the game round time
    QTimer timerGame_;

    // initializing variables
    char direction = RIGHT;
    int durationSnake = 1000; // how many ms a move takes
    int gamePoint = 0; // game starts from 0 points
    bool gamePaused = false; // game isn't paused from the start
    int gameTimerSec = 0; // game timer starts from 0 seconds

    /**
     * @brief activateSpinBoxes void function which activates spinboxes for
     * user inputs
     */
    void activateSpinBoxes();

    /**
     * @brief disableSpinboxes void funtion which disables the spinboxes so that
     * user can't crash the game with them during snake movement
     */
    void disableSpinboxes();

    /**
     * @brief stopTimers void function which stops the game's timers on pause,
     * win or lose
     */
    void stopTimers();

    /**
     * @brief startTimers void function which starts the game's timers from
     * pause, win or lose
     */
    void startTimers();

    /**
     * @brief activatePauseButton void funtion which activates the pause button
     * for user usage. Pause button needs to be controlled so that user doesn't
     * crash the game on start
     */
    void activatePauseButton();

    /**
     * @brief disablePauseButton void function which disables the pause button
     */
    void disablePauseButton();

    /**
     * @brief updateSnake void function which draws the snake on it's new
     * location upon snaketimertimeout
     */
    void updateSnake();

    /**
     * @brief updateScore void function which increments the games's score on
     * lcd screen upon snake eating an apple
     */
    void updateScore();

    /**
     * @brief setLcdBackround void function which sets the lcd screen backrounds
     * to white
     */
    void setLcdBackground();

    /**
     * @brief setupSpinboxes void function which setups the spinboxes for user
     * to be able to use them
     */
    void setupSpinboxes();

    /**
     * @brief connectPushButtons void funtion which connects all the push
     * button signals to their slots on ui
     */
    void connectPushButtons();

    /**
     * @brief resetGameValues void function which is used to reset the game's
     * initial values upon reset
     */
    void resetGameValues();

    /**
     * @brief setInstructionText void function which sets instruction text to
     * the text browser
     */
    void setInstructionText();

    /**
     * @brief initializeGameRoundVisual void function which sets the game's
     * backround back OS preset and gives visual cue for user for which buttons
     * are available for use
     */
    void initializeGameRoundVisual();

    /**
     * @brief checkRoundStatus void function which checks if the user has won or
     * lost the current round of snake apple eater
     */
    void checkRoundStatus();

    /**
     * @brief gameRoundEnd void function which stops all the counting and gives
     * visual cue for user that which buttons are available for use
     */
    void gameRoundEnd();

    /**
     * @brief drawInitialSnakeAndApple void function which is used to draw the
     * initial snake and food on the graphical play area
     */
    void drawInitialSnakeAndApple();

};
#endif // MAINWINDOW_HH
