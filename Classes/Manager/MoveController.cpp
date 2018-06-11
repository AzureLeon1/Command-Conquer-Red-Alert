/*
*  @file     MoveController.cpp
*  @brief    MoveController
*/
#include "MoveController.h"
#include "../Data/Building.h"
#include "../Data/UnitData.h"
#include <time.h>
#include <algorithm>

USING_NS_CC;

MoveController* MoveController::createWithGameScene(GameScene* gameScene)
{
    MoveController* move_controller = new MoveController();
    if (move_controller->initWithGameScene(gameScene))
    {
        move_controller->autorelease();
        return move_controller;
    }

    CC_SAFE_DELETE(move_controller);
    return nullptr;
}

bool MoveController::initWithGameScene(GameScene* gameScene)
{
    _selectedSoldiers = gameScene->getSelectedSoldiers();
    _gameScene = gameScene;

    return true;
}

void MoveController::selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint)
{
    _selectedSoldiers->clear();
    float rect_width = fabs(mouseUpPoint.x - mouseDownPoint.x);
    float rect_height = fabs(mouseUpPoint.y - mouseDownPoint.y);
    Rect rect(MIN(mouseDownPoint.x, mouseUpPoint.x), MIN(mouseDownPoint.y, mouseUpPoint.y), rect_width, rect_height);
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (rect.containsPoint(soldier->getPosition()))
        {
            soldier->setIsSelected(true);
            _selectedSoldiers->pushBack(soldier);
        }
    }
}

void MoveController::selectSoldiersWithTag(Tag tag)
{
    _selectedSoldiers->clear();
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (soldier->getTag() == tag)
        {
            soldier->setIsSelected(true);
            _selectedSoldiers->pushBack(soldier);
        }
    }
}

void MoveController::setDestination(cocos2d::Vec2 position)
{
 //===========laji duixin===================
    int col;                     // column of duixin
    int row = 0;                     // row of duixin
    int soldierSize = 30;            // To Do
    // put the cars first   
    col = 4;
    for (auto& car : *_selectedSoldiers)
    {
        if (car->getUnitTag() == TANK_TAG)
        {
            while (!canPut(position + Vec2(col * soldierSize, -row * soldierSize)))
            {
                col -= 2;              
                if (col < -4)
                {
                    col = 4;
                    row += 1;          
                }
                //log("%d %d", col, row);
            }
            car->setDestination(position + Vec2(col * soldierSize, -row * soldierSize));
            //log("%d %d car %f %f", col, row, car->getDestination().x, car->getDestination().y);
            // find the best way
            findRroute(car, car->_route);
            // the first position in the way          
            car->setDestination(car->_route.front());
            (car->_route).erase((car->_route).begin());
            car->setGetDestination(false);

            col -= 2;             // temporarily think car is double size than infantry
            if (col < -4)
            {
                col = 4;
                row += 1;          // temporarily think car is double size than infantry
            }
        }
    }
    // then put the soldiers   
    for (auto& soldier : *_selectedSoldiers)
    {
        switch (soldier->getUnitTag())
        {
        case TANK_TAG:
            continue;
        case BASE_CAR_TAG:
            soldier->setDestination(position);
            soldier->setGetDestination(false);
            return;
        default:
            while (!canPut(position + Vec2(col * soldierSize, -row * soldierSize)))
            {
                --col;
                if (col < -3)
                {
                    col = 3;
                    ++row;
                }
                //log("%d %d", col, row);
            }
            soldier->setDestination(position + Vec2(col * soldierSize, -row * soldierSize));
            //log("%d %d soldier %f %f",col, row, soldier->getDestination().x, soldier->getDestination().y);
            // find the best way
            findRroute(soldier, soldier->_route);
            // the first position at the way       
            soldier->setDestination(soldier->_route.front());
            (soldier->_route).erase((soldier->_route).begin());
            soldier->setGetDestination(false);

            --col;
            if (col < -3)
            {
                col = 3;
                ++row;
            }

            break;
        }
    }
}

void MoveController::moveSoldiers()
{
    static clock_t preT = clock();
    clock_t nowT = clock();
    float interval = nowT - preT;
	preT = nowT;
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (!soldier->getGetDestination())
        {
			float speed = soldier->getUnitSpeed() * interval;
				//如果没找过路，就去找路
			if (!soldier->isFindRoad) {
					findRroute(soldier, soldier->_route);
					soldier->isFindRoad = true;
			}
			if(soldier->_route.size()){
				//这一帧移动的方向
				Vec2 moveDirection = soldier->_route.front() - soldier->getPosition();
				//这一寻路节点还能否使用
				float moveDistance = moveDirection.length();
				if (moveDistance < speed) {
					(soldier->_route).erase((soldier->_route).begin());
				}
				else {
					moveDirection = soldier->_route.front() - soldier->getPosition();
					moveDistance = moveDirection.length();
				}
				moveDirection.normalize();
				//还有要走的路
				if (soldier->_route.size()) {
					soldier->moveTo(moveDirection*soldier->getUnitSpeed() * interval+ soldier->getPosition());
				}
				else {
					soldier->setGetDestination(true);
				}
				}
			}
			}
			
            
  }

   


//i forget what is this
//what's
//the
//fuck
struct node 
{
    int whereX;
    int whereY;
    int cost;
    node* father;
    node(int wx, int wy, int c, node* f) 
    {
        whereX = wx;
        whereY = wy;
        cost = c;
        father = f;
    }
};

//what
//is
//the
//fuck
//i,m dying
bool MoveController::is_find(Vec2 position, Vec2 destination)
{
    Vec2 direction = destination - position;
    //i
    //am 
    for (int i = 0; i < 15; ++i)
    {
        if (_gameScene->isCollision(_gameScene->
            _tileMap->convertToWorldSpace(position + i * direction / 15)))
        {
            return false;
        }
    }

    return true;
}

/**
going
to
die https://blog.csdn.net/jialeheyeshu/article/details/53105810
* @brief you can read more about the algorithm by clicking this url
and this function is written by czd
* @return  void
*/
void MoveController::findRroute(Unit *soldier, std::vector<Point> &route)
{
	int mapNode[50][50];
	memset(mapNode, 0, sizeof(mapNode));
    Vec2 screenNowPosition = soldier->getPosition();
    Vec2 screenDestination = soldier->getDestination();
    //so  
    Vec2 nowPosition = _gameScene->_tileMap->convertToNodeSpace(screenNowPosition);
    Vec2 nowDestination = _gameScene->_tileMap->convertToNodeSpace(screenDestination);

    std::queue<node*> open;
    std::vector<node*> storeNew;
    //std::priority_queue<node*, std::vector<node*>, cmp> close;
    node *head = new node(nowPosition.x, nowPosition.y, 0, NULL);
    storeNew.push_back(head);
    node *myend = head;
    open.push(head);
    int distance = 80;
    //don't    
    //know
    float directX[8] = { 0,0,-1 * distance ,distance,0.7*distance ,0.7* distance ,-0.7* distance ,-0.7* distance };
    float directY[8] = { distance,-1 * distance,0 ,0,0.7* distance ,-0.7* distance ,0.7* distance ,-0.7* distance };
    int is_not_find = 1;
    //what
    if (is_find(nowPosition, nowDestination))
    {
        is_not_find = 0;
    }
    while (is_not_find)
    {
        node *cur = open.front();
        //close.push(cur);
        open.pop();
        //is      
        for (int i = 0; i < 8; i++)
        {
            //this         
            if (!_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace
				//cur->whereX是地图坐标
            (Point(cur->whereX + directX[i], cur->whereY + directY[i]))))
            {
				int nodeX = (cur->whereX + directX[i]) / distance;
				int nodeY = (cur->whereY + directY[i]) / distance;
                //fucking           
                if (mapNode[nodeX][nodeY]!=1)
                {
                    node *n = new node(cur->whereX + directX[i], cur->whereY + directY[i], distance, cur);
					mapNode[nodeX][nodeY] = 1;
                    storeNew.push_back(n);
                    if (is_find(Point(n->whereX, n->whereY), nowDestination))
                    {
                        is_not_find = 0;
                        myend = n;
                        break;
                    }
                    open.push(n);
                }
            }

        }
    }
    //notes
    Point nowPlace{ 0,0 };
    nowPlace.x = static_cast<float>(myend->whereX);
    nowPlace.y = static_cast<float>(myend->whereY);
    Point direction = nowDestination - nowPlace;
    direction.normalize();
    //let   
    while (myend != NULL)
    {
        route.push_back(Point(myend->whereX, myend->whereY));
        myend = myend->father;
    }
    //the 
    std::reverse(route.begin(), route.end());
    //fucking
    while ((nowDestination - nowPlace).length() > distance)
    {
        route.push_back(nowPlace + distance * direction);
        nowPlace += distance * direction;
    }
    route.push_back(nowDestination);
    // delete
    std::vector<node*>::iterator iter = storeNew.end() - 1;
    size_t i = storeNew.size();
    for (size_t cnt = 0; cnt < i; cnt++)
    {
        delete *iter;
        if (cnt < i - 1)
        {
            iter--;
        }
    }
    //vs  
    std::vector<Point>::iterator iter2;
    for (iter2 = route.begin(); iter2 != route.end(); iter2++)
    {
        *iter2 = _gameScene->_tileMap->convertToWorldSpace(*iter2);
    }
}

bool MoveController::canPut(cocos2d::Point position)
{
    // is the collision or not in map
    if (_gameScene->isCollision(position))
    {
        return false;
    }
    // is there any soldier or building
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        //log("soldier position %f %f", soldier->getPosition().x, soldier->getPosition().y);
        Rect rect = Rect(soldier->getPositionX() - soldier->getContentSize().width / 2,
            soldier->getPositionY() - soldier->getContentSize().height / 2,
            soldier->getContentSize().width, soldier->getContentSize().height);
        //log("%f %f %f %f", rect.getMinX(), rect.getMinY(), rect.size.width, rect.size.height);
        //log("put position %f %f", position.x, position.y);
        if (rect.containsPoint(position))
        {
            return false;
        }
    }
    for (auto& building : *(_gameScene->getBuildings()))
    {
        Rect rect = Rect(building->getPositionX() - building->getContentSize().width / 2,
            building->getPositionY() - building->getContentSize().height / 2,
            building->getContentSize().width, building->getContentSize().height);
        if (rect.containsPoint(position))
        {
            return false;
        }
    }
    // yes you can put soldier here   
    return true;
}