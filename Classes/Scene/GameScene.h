#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

const clock_t addMoneyDelay = 1000 * 10;

#include <time.h>
#include "cocos2d.h"
#include "Data/UnitData.h"
#include "Manager/GameManager.h"
#include "Panel/Panel.h"
#include "NetWork/Client.h"
#include "Data/ExternData.h"
#include "Data/LevelData.h"

class Manager;   //解决头文件互相包含时带来的问题
class Panel;     //解决头文件互相包含时带来的问题


class GameScene : public cocos2d::Layer
{
public:
	cocos2d::Sprite*           small_map;

	LevelData*                 _inputData;           ///选择人物时的数据
	GameScene*                 _thisScene;           ///等于This指针
	std::vector<PlayerData>    _playerList;          ///所有远程玩家的信息


	cocos2d::TMXTiledMap*      _tileMap;

	Client*                    _client;              ///服务端指针
	std::string                _localPlayerName;     ///本地玩家的名字
    int                        _localPlayerID;

	int _loserCnt = 0;                  //已经失败的玩家数量

    Manager * _manager;
    
    Panel * panel;

	std::queue<std::string>    _commands;             ///每次读取的命令

    int _unitIndex;

    int _unitIndexDied[5000];

    int _buildingIndex;

    int _buildingIndexDied[5000];

private:
	
	cocos2d::TMXLayer* _barrier;
    int MAPX;
    int MAPY;

    bool _keyUp = false;
    bool _keyDown = false;
    bool _keyRight = false;
    bool _keyLeft = false;                 // C++ 11 的初始化方法

	cocos2d::Point _cursorPosition;
	void scrollMap();

	cocos2d::Vector<Unit*> _selectedSoldiers;

	cocos2d::Vector<Unit*>     _enemySoldiers1;        //第一个敌人的士兵队列.
    cocos2d::Vector<Building*> _enemyBuildings1;       //第一个敌人的建筑队列.
    cocos2d::Vector<Unit*>     _enemySoldiers2;        //第二个敌人的士兵队列.
    cocos2d::Vector<Building*> _enemyBuildings2;       //第二个敌人的建筑队列.
    cocos2d::Vector<Unit*>     _enemySoldiers3;        //第三个敌人的士兵队列.
    cocos2d::Vector<Building*> _enemyBuildings3;       //第三个敌人的建筑队列.
    cocos2d::Vector<Unit*>     _enemySoldiers4;        //第四个敌人的士兵队列.
    cocos2d::Vector<Building*> _enemyBuildings4;       //第四个敌人的建筑队列.

	//my soldiers and buildings
	cocos2d::Vector<Unit*> _soldiers;
	cocos2d::Vector<Building*> _buildings;

	cocos2d::Point _touchBegan;
	cocos2d::Point _touchEnd;

    //显示金币
    char _moneyStr[8];
    char _timeStr[10];
    cocos2d::Label* _moneyCount;
    cocos2d::Label* _timeCount;

    //时间
    long _time = 0;

    //sell building
    cocos2d::Menu* _sellBuildingMenu;
    Building* _sellBuilding = nullptr;
    bool _isSellMenuExit = false;

public:

    //power bar
    cocos2d::ProgressTimer* _powerBar;

	cocos2d::EventListenerTouchOneByOne* _gameListener;
	cocos2d::EventDispatcher* _gameEventDispatcher;

public:

	
	
	CC_SYNTHESIZE(int, _money, Money);

	// 总电力
	CC_SYNTHESIZE(int, _totalPower, TotalPower);

	// 剩余电力
	CC_SYNTHESIZE(int, _power, Power);

	// 电力是否足够
	CC_SYNTHESIZE(bool, _isPowerEnough, IsPowerEnough);

	// 是否有基地
	CC_SYNTHESIZE(bool, _isBaseExist, IsBaseExist);

	CC_SYNTHESIZE(int, _barracksNum, BarracksNum);

	CC_SYNTHESIZE(int, _mineNum, MineNum);

	CC_SYNTHESIZE(int, _powerPlantNum, PowerPlantNum);

	CC_SYNTHESIZE(int, _carFactoryNum, CarFactoryNum);

	CC_SYNTHESIZE(int, _satelliteNum, SatelliteNum);

    // 待造坦克数
    CC_SYNTHESIZE(int, _tankNum, TankNum);
    // 待造狗数
    CC_SYNTHESIZE(int, _dogNum, DogNum);
    // 待造步兵数
    CC_SYNTHESIZE(int, _infantryNum, InfantryNum);

	CC_SYNTHESIZE(cocos2d::Vec2, _carFactoryPosition, CarFactoryPosition);

	CC_SYNTHESIZE(cocos2d::Vec2, _barracksPosition, BarracksPosition);

	static cocos2d::Scene* createScene(LevelData &data, Client* client, std::string playerName);

	virtual bool init();

    virtual void onEnter();

    //virtual void onExitTransitionDidStart();

	// 初始化数据
	void dataInit();

	virtual void update(float time);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void menuBackCallback(Ref *pSender);

    void pushEnemyUnitByID(Unit* u, int id);

    void pushEnemyBuildingByID(Building* b, int id);

	/**
	* @brief getSelectedSoldiers
	* @return the address of _selectedSoldiers
	*/
	cocos2d::Vector<Unit*>* getSelectedSoldiers();

	/**
	* @brief getSoldiers
	* @return the address of _soldiers
	*/
	cocos2d::Vector<Unit*>* getSoldiers();

    Unit* getSoldierByIndex(int index);

   /*
    * @brief getEnemySoldiers
    * @return the address of enemy_soldiers
    */
    cocos2d::Vector<Unit*> * getEnemySoldiersByID(int id);

    Unit* getEnemySoldierByIdIndex(int id, int index);

	/**
	* @brief getBuildings
	* @return the address of _buildings
	*/
	cocos2d::Vector<Building*>* getBuildings();

     /*
    * @brief getEnemyBuildings
    * @return the address of enemy_soldiers
    */
    cocos2d::Vector<Building*> * getEnemyBuildingsByID(int id);

	/**
	* @brief addMoney
	* @param 增加的钱
	* @return void
	*/
	void addMoney(int money);

	/**
	* @brief decreaseMoney
	* @param 减少的钱
	* @return void
	*/
	void decreaseMoney(int money);

	/**
	* @brief addPower 增加电
	* @return void
	*/
	void addPower(int power);

	/**
	* @brief decreasePower
	* @return void
	*/
	void decreasePower(int power);

	/**
	* @brief addTotalPower 增加总电
	* @return void
	*/
	void addTotalPower(int power);

	/**
	* @brief decreaseTotalPower
	* @return void
	*/
	void decreaseTotalPower(int power);

	/*
	* @brief 电厂数量加一
	* @return void
	*/
	void addPowerPlant() { _powerPlantNum++; }

	/*
	* @brief 电厂数量减一
	* @return void
	*/
	void decreasePowerPlant() { _powerPlantNum--; }

	/*
	* @brief 车厂数量加一
	* @return void
	*/
	void addCarFactory() { _carFactoryNum++; }

	/*
	* @brief 车厂数量减一
	* @return void
	*/
	void decreaseCarFactory() { _carFactoryNum--; }

	/*
	* @brief 卫星数量加一
	* @return void
	*/
	void addSatellite() { _satelliteNum++; }

	/*
	* @brief 卫星数量减一
	* @return void
	*/
	void decreaseSatellite() { _satelliteNum--; }

	/*
	* @brief 兵营数量加一
	* @return void
	*/
	void addBarracks() { _barracksNum++; }

	/*
	* @brief 兵营数量减一
	* @return void
	*/
	void decreaseBarracks() { _barracksNum--; }

	/*
	* @brief 矿厂数量加一
	* @return void
	*/
	void addMine() { _mineNum++; }

	/*
	* @brief 矿产数量减一
	* @return void
	*/
	void decreaseMine() { _mineNum--; }

    /*
    * @brief 待造狗加一
    */
    void addDog() { _dogNum++; }

    /*
    * @brief 待造兵加一
    */
    void addInfantry() { _infantryNum++; }

    /*
    * @brief 待造坦克加一
    */
    void addTank() { _tankNum++; }

    /*
    * @brief 待造狗减一
    */
    void decreaseDog() { _dogNum--; }

    /*
    * @brief 待造兵减一
    */
    void decreaseInfantry() { _infantryNum--; }

    /*
    * @brief 待造坦克减一
    */
    void decreaseTank() { _tankNum--; }

	/*
	* @brief isCollision
	* @param the position
	* @return bool
	*/
	bool isCollision(cocos2d::Vec2 position);

	/*
	* @brief getTileSize
	* @return the size of tile
	*/
	float getTileSize();

    /*
    *@brief 移动所有士兵建筑 包括目的地
    */
    void moveSpritesWithMap(cocos2d::Vec2 direction);

    /*
    * @brief print time every second
    */
    void printTime(float dt);

    /*
    *@brief sell building call back function
    */
    void sellBuildingCallBack();

    /*
    *@brief 得到点相对于中心点的位置,转换到第一象限,利用函数
    *关系判断点是否在菱形内
    *@param 菱形中心点
    *@parma 菱形宽度一半
    *@param 菱形高度一半
    *@param 目的点
    */
    bool inDiamond(cocos2d::Point center, float width, 
        float height, cocos2d::Point position);
	/*
	@brief 把士兵显示在小地图上
	*/
	void showOnSmallMap();
	DrawNode* drawNode = DrawNode::create();
	DrawNode* drawNode2 = DrawNode::create();
	DrawNode* drawNode3 = DrawNode::create();
	DrawNode* drawNode4 = DrawNode::create();
	/*
	brief 战争迷雾
	*/
	int fog[40][40];
	void makeFog();
	bool hasFog;
};

#endif // __Welcome_SCENE_H__
