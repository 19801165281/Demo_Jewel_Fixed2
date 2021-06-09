#include "Jewel.h"
#include "GameScene.h"
#include "GameLogic.h"
#include <vector>
using namespace std;


int main()
{
	if (Game::init(L"Demo1", 1024, 640))
	{
		// 修改节点默认中心点，便于让图片居中显示
		Node::setDefaultAnchor(0.5f, 0.5f);
		//动画界面（默认为第一关）
		auto scene = new GameScene(1, Mode::Fast);
		SceneManager::enter(scene);

		
		Game::start();
	}
	Game::destroy();
	return 0;
}