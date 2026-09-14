#include "TemplateGameEntry.hpp"
#include "MainScene.hpp"

void GameEntry(core::runtime::SceneManager& sceneManager)
{
	sceneManager.insert<MainScene>("MainScene");
}
