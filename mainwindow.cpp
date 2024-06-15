/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: mainwindow.cpp                                             #
#                                                                  #
# Author:                                                          #
#   First name: Fitim                                              #
#   Last name: Abazi                                               #
#   Student id: 150417967                                          #
#   email: fitim.abazi@tuni.fi                                     #
####################################################################
*
* Description:
*   Defines a class representing the graphical gameboard. Contains
* the functionality of the graphical userinterface as well as the
* implementation of background API's game logic into a graphical
* version.
*/

#include "mainwindow.hh"
#include "gameboard.hh"
#include "ui_mainwindow.h"
#include "point.hh"

#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setLcdBackground();

    setupSpinboxes();
    disableSpinboxes();

    connectPushButtons();

    // user shouldn't be able to pause the game before it even starts
    disablePauseButton();

    setInstructionText();
}

MainWindow::~MainWindow()
{
    const unsigned int snakeBodySize = snakeBody_.size();

    for (unsigned int i = 0; i < snakeBodySize; i++) {
        delete snakeBody_.back();
        snakeBody_.pop_back();
    }

    delete ui;
}

void MainWindow::setLcdBackground()
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::white);
    ui->lcdScoreNumber->setAutoFillBackground(true);
    ui->lcdScoreNumber->setPalette(pal);
    ui->lcdTimeNumber->setAutoFillBackground(true);
    ui->lcdTimeNumber->setPalette(pal);
}

void MainWindow::setupSpinboxes()
{
    // setup initial value spinboxes
    ui->spinBoxWidth->setMinimum(DEFAULT_WIDTH);
    ui->spinBoxWidth->setMaximum(10);

    ui->spinBoxHeight->setMinimum(DEFAULT_HEIGTH);
    ui->spinBoxHeight->setMaximum(10);

    ui->spinBoxSeed->setMinimum(DEFAULT_SEED);
    ui->spinBoxSeed->setMaximum(100);

    // target value is always two smaller than the area of the gameboard
    // so that no overlap is done in game modes
    ui->spinBoxTarget->setMinimum(1);
    ui->spinBoxTarget->setMaximum((ui->spinBoxHeight->value()
                                   *ui->spinBoxWidth->value())-2);
}

void MainWindow::connectPushButtons()
{
    // connecting play button signal to slot
    connect(ui->playButton, &QPushButton::clicked, this,
            &MainWindow::onPlayButtonClicked);

    // connecting timer to move the snake
    connect(&timerSnake_, &QTimer::timeout, this,
            &MainWindow::snakeMoveTimeOut);

    // connecting close button signal to slot
    connect(ui->closeButton, &QPushButton::clicked, this,
            &MainWindow::onCloseButtonClicked);

    // connecting pause button signal to slot
    connect(ui->pauseButton, &QPushButton::clicked, this,
            &MainWindow::onPauseButtonClicked);

    // connecting timer to gametimer
    connect(&timerGame_, &QTimer::timeout, this,
            &MainWindow::gameTimerTimeOut);

    // connecting game modes
    connect(ui->classicModePushButton, &QPushButton::clicked, this,
            &MainWindow::onClassicModeButtonClicked);
    connect(ui->targetModePushButton, &QPushButton::clicked, this,
            &MainWindow::onTargetModeButtonClicked);

}

// snake, apple and gameboard are drawn and generated only when play button
// is pressed
void MainWindow::onPlayButtonClicked()
{
    initializeGameRoundVisual();

    float scalar = 0.3; // is used to make sure snake stays within borders
    int height = ui->spinBoxHeight->value();
    int width = ui->spinBoxWidth->value();
    int seed = ui->spinBoxSeed->value();

    // setting up the scene in which to draw the snake and points
    gameScene_ = new QGraphicsScene(this);
    ui->playArea->setGeometry(10, 270, (width+scalar)*STEP,
                              (height+scalar)*STEP);
    gameScene_->setSceneRect(0, 0, width*STEP, height*STEP);
    ui->playArea->setScene(gameScene_);

    // creating the current game board as unique pointer
    currentBoard_ = std::make_unique<GameBoard>(width, height, seed);

    drawInitialSnakeAndApple();
    resetGameValues();
}

void MainWindow::resetGameValues()
{
    // variables regarding the game timer
    gameTimerSec = 0;
    timerGame_.setInterval(GAMETIMER);
    timerGame_.start();

    // variables regarding the game point
    gamePoint = 0;
    ui->lcdScoreNumber->display(gamePoint);

    // variables regarding the snake and it's movement
    durationSnake = 1000;
    timerSnake_.setInterval(durationSnake);
    timerSnake_.start();
    direction = RIGHT;
    gamePaused = false;
}

void MainWindow::setInstructionText()
{
    ui->instructionBrowser->setText(QString("Welcome to Snek apple eater! "
    "instructions as follow: \n"
    "-Choose game mode \n"
    "--target by default\n"
    "controls:\n"
    "w-up \ns-down \na-left \nd-right \nr-reset/play"));
}

void MainWindow::initializeGameRoundVisual()
{
    ui->classicModePushButton->setDisabled(true);
    ui->targetModePushButton->setDisabled(true);
    setStyleSheet(";"); // resets the ui background to OS specific one
    setAutoFillBackground(false);
    ui->gameStatusLabel->setText(QString("Snek has begun its journey!"));
    activatePauseButton();
    disableSpinboxes();
}

void MainWindow::gameRoundEnd()
{
    ui->gameStatusLabel->setVisible(true);
    stopTimers();
    activateSpinBoxes();
    disablePauseButton();
    setAutoFillBackground(true);
    ui->classicModePushButton->setDisabled(false);
    ui->targetModePushButton->setDisabled(false);
}

void MainWindow::drawInitialSnakeAndApple()
{
    // defining the outline of the snake and apple
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    // creating and fixing coordinates for the intial food (apple)
    apple_ = new QGraphicsEllipseItem(0, 0, 20, 20);
    apple_->setPos(currentBoard_->food().x() * STEP,
                   currentBoard_->food().y() * STEP);
    apple_->setBrush(REDBRUSH);
    apple_->setPen(blackPen);
    gameScene_->addItem(apple_);

    // creating and fixing coordinates for the intial snake's head
    snakeBody_.push_back(new QGraphicsRectItem(0, 0, 20, 20));
    snakeBody_.back()->setPos(currentBoard_->snake().back().x() * STEP,
                   currentBoard_->snake().back().y() * STEP);
    snakeBody_.back()->setBrush(GREENBRUSH);
    snakeBody_.back()->setPen(blackPen);
    gameScene_->addItem(snakeBody_.back());
}

void MainWindow::updateScore()
{
    // when snake's head hits an apple
    if (snakeBody_.at(snakeBody_.size()-1)->pos() == apple_->pos()) {
        if (durationSnake > 250) {
            durationSnake -= 50;
            timerSnake_.setInterval(durationSnake);
        }
        if (gamePoint <= ui->spinBoxWidth->value()*ui->spinBoxHeight->value()) {
            gamePoint ++;
            ui->lcdScoreNumber->display(gamePoint);
        }
    }
}

void MainWindow::checkRoundStatus()
{
    // win or lose status checking
    if (currentBoard_->gameLost()) {
        ui->gameStatusLabel->setText(QString("Snek has died q.q"));
        setStyleSheet("background-color:red;"); // coloring the background
        gameRoundEnd();
    }

    else if (currentBoard_->gameWon()) {
        ui->gameStatusLabel->setText(QString("Snek is fat and won :)"));
        setStyleSheet("background-color:green;");
        gameRoundEnd();
    }

    else if (gamePoint == ui->spinBoxTarget->value()) {
        ui->gameStatusLabel->setText(QString("Snek won the game! :3"));
        setStyleSheet("background-color:yellow;");
        gameRoundEnd();
    }

    // if game wasn't won a new apple is drawn somewhere within the gameboard
    else {
        apple_->setPos(currentBoard_->food().x() * STEP,
                       currentBoard_->food().y() * STEP);
    }
}

void MainWindow::updateSnake()
{
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    // snakebodysize is set to snakebodys' size as constant so that the original
    // snakebodysize isn't altered upon eating apples. Otherwise the snake would
    // leave little cubes on the screen
    const unsigned int snakeBodySize = snakeBody_.size();

    // The snake is deleted on each timertimeout
    for (unsigned int i = 0; i < snakeBodySize; i++) {
        delete snakeBody_.back();
        snakeBody_.pop_back();
    }

    // The snake is created in the new location determined by the direction
    for (unsigned int part = 0; part < currentBoard_->snake().size(); part++) {

        QGraphicsRectItem* snakeBodyPart = new QGraphicsRectItem(0, 0, 20, 20);
        snakeBodyPart->setPos(currentBoard_->snake().at(part).x() * STEP,
                                  currentBoard_->snake().at(part).y() * STEP);
        snakeBodyPart->setPen(blackPen);

        // setting the tail
        if (part == 0 && part != currentBoard_->snake().size()-1) {
            snakeBodyPart->setBrush(BLUEBRUSH);
        }

        // setting the head
        else if (part == currentBoard_->snake().size()-1) {
            snakeBodyPart->setBrush(GREENBRUSH);
        }

        // setting the body
        else {
            snakeBodyPart->setBrush(REDBRUSH);
        }
        gameScene_->addItem(snakeBodyPart);
        snakeBody_.push_back(snakeBodyPart);
    }

}

void MainWindow::snakeMoveTimeOut()
{
    // initial direction for the snake
    currentBoard_->moveSnake(std::string(1, direction));

    // calling functions which require the snake timer to timeout
    updateSnake();
    updateScore();
    checkRoundStatus();
}

void MainWindow::gameTimerTimeOut()
{
    gameTimerSec ++;
    ui->lcdTimeNumber->display(gameTimerSec);
}

void MainWindow::onClassicModeButtonClicked()
{
    // setting target values unreachable for the user
    ui->spinBoxTarget->setDisabled(true);
    ui->spinBoxTarget->setVisible(false);
    ui->targetLabel->setVisible(false);

    // minimum value is set over the area of the gameboard so that the user
    // can never reach it
    ui->spinBoxTarget->setMinimum(((ui->spinBoxHeight->value()
                                    *ui->spinBoxWidth->value())+1));
    activateSpinBoxes();
}

void MainWindow::onTargetModeButtonClicked()
{
    // setting target values reachable for the user
    ui->spinBoxTarget->setVisible(true);
    ui->targetLabel->setVisible(true);
    setupSpinboxes();
    activateSpinBoxes();
}

void MainWindow::onCloseButtonClicked()
{
    close();
}

void MainWindow::onPauseButtonClicked()
{
    // pause button functionality added
    if (!gamePaused) {
        stopTimers();
        ui->gameStatusLabel->setText(QString("Snek is taking a break~~"));
        ui->gameStatusLabel->setVisible(true);
        gamePaused = true;
    }

    else {
        startTimers();
        ui->gameStatusLabel->setText(QString("Snek is working hard >:3"));
        ui->gameStatusLabel->setVisible(true);
        gamePaused = false;
    }
}

void MainWindow::activateSpinBoxes()
{
    ui->spinBoxHeight->setDisabled(false);
    ui->spinBoxWidth->setDisabled(false);
    ui->spinBoxSeed->setDisabled(false);
    ui->spinBoxTarget->setDisabled(false);
}

void MainWindow::disableSpinboxes()
{
    ui->spinBoxHeight->setDisabled(true);
    ui->spinBoxWidth->setDisabled(true);
    ui->spinBoxSeed->setDisabled(true);
    ui->spinBoxTarget->setDisabled(true);
}

void MainWindow::stopTimers()
{
    timerSnake_.stop();
    timerGame_.stop();
}

void MainWindow::startTimers()
{
    timerSnake_.start();
    timerGame_.start();
}

void MainWindow::activatePauseButton()
{
    ui->pauseButton->setDisabled(false);
}

void MainWindow::disablePauseButton()
{
    ui->pauseButton->setDisabled(true);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    // pressing r key starts and resets the game
    if (event->key() == Qt::Key_R) {
        onPlayButtonClicked();
    }


    else if(!gamePaused) {
        //moving snake up
        if(direction != DOWN && event->key() == Qt::Key_W) {
                direction = UP;
        }
        //moving snake down
        else if(direction != UP && event->key() == Qt::Key_S) {
                direction = DOWN;
        }

        //moving snake left
        else if(direction != RIGHT && event->key() == Qt::Key_A) {
            direction = LEFT;
        }

        //moving snake up
        else if(direction != LEFT && event->key() == Qt::Key_D) {
            direction = RIGHT;
         }
    }

    // used for ignoring user inputs during the game's pause
    else {
        event->ignore();
    }
}
