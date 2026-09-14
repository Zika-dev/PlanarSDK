#include "MainScene.hpp"

#include "Log.hpp"

using namespace env;

void MainScene::onLoad()
{
	APP_LOG_INFO("Hello from MainScene!");

	auto projectDir = resources().getProjectDir();

	rayTex = graphics().createTexture(projectDir / "assets/raySmol.png");
 
	rayObject.setTextureHandle(rayTex);
	rayObject.setSize({ 16, 16 });

	rayObject.pos = { core::WIDTH / 2 + 50, core::HEIGHT / 2 };
}

void MainScene::onUpdate(float dt)
{
	graphics().drawGameObject(rayObject);
}
