#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMessageBox>

#include "gamecontroller.h"
#include "food.h"
#include "snake.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(new Snake(*this)),
    isPause(false)
{
    timer.start( 1000/33 );

	Food *a1 = new Food(0, -50);        
    scene.addItem(a1);

    scene.addItem(snake);
    scene.installEventFilter(this);

    resume();
}

GameController::~GameController()
{
}

void GameController::snakeAteFood(Food *food)
{
    scene.removeItem(food);
    snake->number_of_points++;
    if(snake->number_of_points % 4 == 0)snake->score+=3;
    else if(snake->number_of_points % 13 == 0)snake->score+=7;
    else snake->score++;

    if((snake->number_of_points + 1) % 4 == 0)addNewFood(2);
    else if((snake->number_of_points + 1) % 13 == 0)addNewFood(3);
    else addNewFood(1);
}

//void GameController::snakeHitWall(Snake *snake, Wall *wall)
//{
//}

void GameController::snakeAteItself()
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::handleKeyPressed(QKeyEvent *event)
{
    if (!isPause)
        switch (event->key()) {
            case Qt::Key_Left:
                snake->setMoveDirection(Snake::MoveLeft);
                break;
            case Qt::Key_Right:
                snake->setMoveDirection(Snake::MoveRight);
                break;
            case Qt::Key_Up:
                snake->setMoveDirection(Snake::MoveUp);
                break;
            case Qt::Key_Down:
                snake->setMoveDirection(Snake::MoveDown);
                break;
            case Qt::Key_Space:
                pause();
                break;
        }
    else resume();
}

void GameController::addNewFood(int tipe)
{
    int x, y;

    do {
		x = (int)(qrand() % 200) / 10 - 10;
		y = (int)(qrand() % 200) / 10 - 10;

        x *= 10;
        y *= 10;
	} while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))));

    Food *food = new Food(qreal(x), qreal(y));
    food->tipe = tipe;
    scene.addItem(food);
}

void GameController::gameOver()
{
    disconnect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    if (QMessageBox::Yes == QMessageBox::information(NULL,
                            tr("Game Over"),
                            tr("Your score:")+QString::number(snake->score)+
                            tr("\nPoints taken:")+QString::number(snake->number_of_points)+
                            tr("\nPlay Again?"),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::Yes)) {
        connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
        scene.clear();

        snake = new Snake(*this);
        scene.addItem(snake);
        addNewFood(1);
    } else {
        exit(0);
    }
}

void GameController::pause()
{
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
    isPause = true;
}

void GameController::resume()
{
    connect(&timer, SIGNAL(timeout()),
            &scene, SLOT(advance()));
    isPause = false;
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}
