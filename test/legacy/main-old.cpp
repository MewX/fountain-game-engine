#include <fountain.h>

ftTime::Clock mainClock(60.0);
ftScene::SceneSelector SS;
ftRender::ShaderProgram SP("resources/shader/vs.vert", "resources/shader/fs.frag");
ftType::FontMan fm;
ftAudio::Channel cha;
ftUI::NineSprite nineS;

bool useShader = false;

#include "testscene-old.h"
#include <string>

class testButton : public ftUI::Button
{
public:
	void update()
	{
		Button::update();
		int st = Button::getState();
		if (st == FT_isOn) {
			setBackColor(FT_White);
			setForeColor(FT_Black);
		}
		if (st == FT_isDown) {
			setBackColor(FT_Yellow);
			setForeColor(FT_White);
		}
		if (st == FT_None) {
			setBackColor(FT_Orange);
			setForeColor(FT_White);
		}
	}
};

testButton tBut;

void fountain::setBasicVarible()
{
	mainWin.setSize(800, 600);
	mainWin.title = std::string("fountain-prototype ") + std::string(FOUNTAIN_VERSION);
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = false;
//	mainWin.isFullScreen = true;
	mainWin.hideCursor = false;
//	mainWin.hideCursor = true;
}

void fountain::gameInit()
{

	mainClock.init();

	SS.registerScene(&startScene, FT_StartScene);
	SS.registerScene(&gameScene, FT_Scene2);
	SS.registerScene(&modelScene, FT_Scene3);

	ftFile lData("resources/data/letters.dat");
	fm.loadFont("resources/font/test.ttc");
	fm.genStringTable(lData.getStr(), 32);

	ftUI::setDefaultFont(&fm);

	SP.init();
	SP.setUniform("time", 0.0f);
	SP.use();

	cha.init();
	cha.load("resources/sound/test.wav");
	cha.play();

	tBut.setPosition(0, -100);
	tBut.setRectSize(ftVec2(200, 50));
	tBut.setCaption("按钮");

	nineS = ftUI::NineSprite("resources/image/nine.png");
	nineS.setSize(ftVec2(200, 200));
	nineS.setPosition(-200, 0);
}

void fountain::singleFrame()
{
	//TODO: move this internal(fountainMain)
	mainClock.tick();

	if (fountain::sysKeyboard.getState(FT_S) == FT_KeyDown)
		useShader = !useShader;
	if (useShader)
		SP.use();
	else
		ftRender::useFFP();
	if (fountain::sysKeyboard.getState(FT_R) == FT_KeyDown)
		SP.reload();

	ftVec2 mp = fountain::sysMouse.getPos();
	SP.setUniform("time", mainClock.secondsFromInit());
	SP.setUniform("resolution", fountain::getWinSize());
	SP.setUniform("mouse", mp);

	if (fountain::sysKeyboard.getState(FT_X) == FT_KeyDown)
		cha.play();

	ftRender::useColor(FT_White);
	SS.sceneSolve();
	SS.update();
	SS.draw();

	//test
	ftRender::useColor(FT_White);
	ftRender::transformBegin();
	ftRender::ftTranslate(-300, 230);
	fm.drawString("Welcome to Fountain Game Engine!");
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftTranslate(-300, 180);
	fm.drawString("欢迎！这里是Fountain游戏引擎！");
	ftRender::transformEnd();

	tBut.update();
	tBut.draw();

	nineS.draw();
}
