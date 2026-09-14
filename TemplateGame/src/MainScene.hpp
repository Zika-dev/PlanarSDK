#include "env.hpp"

class MainScene : public rt::Scene {

public:

	MainScene() = default;

	void onLoad() override;
	void onUpdate(float dt) override;

private:

	gfx::TextureHandle rayTex;
	rt::GameObject rayObject;
};
