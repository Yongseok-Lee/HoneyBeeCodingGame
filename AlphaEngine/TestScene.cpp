#include "TestScene.h"
#include "Grid.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "Error.h"
#include <iostream>
#include <algorithm>
#include "SoundManager.h"
#include "GameApp.h"

static void HelpMarker()
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(
			u8"Go(number) - 숫자만큼 현재 방향으로 전진\n"
			"TurnLeft(number) - 숫자만큼 왼쪽으로 회전\n"
			"TurnRight(number) - 숫자만큼 오른쪽으로 회전\n"
			"var i = 0 - 변수 i 생성후 0으로 초기화\n"
			"loop number { ... } - 숫자만큼 반복\n"
			"for i=0; i<5; i=i+1 { ... } - i를 5가 될때까지 반복\n"
			"if (i<5) { ... } - i가 5보다 작으면 실행\n"
		);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static bool picking(glm::vec2 pos, glm::vec2 size)
{
	static auto app = GameApp::GetInstance();
	double x = app->_mouseX;
	double y = app->_mouseY;

	if (pos.x - size.x / 2.0f <= x && pos.x + size.x/2.0f >= x)
		if (pos.y - size.y / 2.0f <= y && pos.y + size.y/2.0f >= y)
			return true;
	return false;
}

int TestScene::_stageNumber = 0;
bool TestScene::_isStart = false;
static std::vector<std::vector<StageBlockType> > _mapdata[] = {
	{	/* stage 0 */
		{Start, Default, Default, Object},
	},
	{	/* stage 1 */
		{Start, Default, Default},
		{ Null,    Null, Default},
		{ Null,    Null,  Object}
	},
	{	/* stage 2 */
		{Start, Default, PiranhaPlant},
		{ Null, Default,      Default},
		{ Null,    Null,       Object}
	},
	{	/* stage 3 */
		{  Start,     Pond, Default},
		{Default, PondFrog, Default},
		{Default,  Default,  Object}
	},
	{	/* stage 4 */
		{Pond,    Pond,    Pond,     Pond,    Pond,         Pond, Pond},
		{Pond,   Start, Default,  Default, Default,       Object, Pond},
		{Pond, Default,    Pond,     Pond,    Pond,      Default, Pond},
		{Pond, Default,    Pond, PondFrog,    Pond, PiranhaPlant, Pond},
		{Pond, Default,    Pond,     Pond,    Pond,      Default, Pond},
		{Pond,  Object, Default,  Default, Default,       Object, Pond},
		{Pond,    Pond,    Pond,     Pond,    Pond,         Pond, Pond}
	},
	{	/* stage 5 */
		{ Object, Default,  Object,     Pond,    Pond},
		{Default,    Pond, Default,     Pond,    Pond},
		{ Object, Default,   Start,  Default,  Object},
		{   Pond,    Pond, Default, PondFrog, Default},
		{   Pond,    Pond,  Object,  Default,  Object}
	},
	{	/* stage 6 */
		{ Object, Default,  Object, Default,  Object},
		{Default,    Pond, Default,    Pond, Default},
		{ Object, Default,   Start, Default,  Object},
		{Default,    Pond, Default,    Pond, Default},
		{ Object, Default,  Object, Default,  Object}
	},
	{	/* stage 7 */
		{Start,   Object,     Pond, PondFrog,    Pond,     Pond},
		{ Pond,  Default,     Pond,     Pond,    Pond,     Pond},
		{ Pond,   Object,  Default,  Default,  Object, PondFrog},
		{ Pond,     Pond,     Pond,     Pond, Default,     Pond},
		{ Pond, PondFrog,     Pond,     Pond, Default,     Pond},
		{ Pond,     Pond,     Pond,     Pond, Default,     Pond},
		{ Pond,     Pond, PondFrog,     Pond,  Object,     Pond}
	},
	{	/* stage 8 */
		{       Strat,   Object,  Default,  Default,  Default,  Default,  Default,  Default},
		{     Default,     Pond, PondFrog,     Pond,     Pond, PondFrog,     Pond,   Object},
		{     Default, PondFrog,  Deafult,   Object,  Default,  Default, PondFrog,  Default},
		{     Default,     Pond,  Default,     Pond,     Pond,   Object,     Pond,  Default},
		{     Default,     Pond,  Default,     Pond,   Object,  Default, PondFrog,  Default},
		{     Default,     Pond,   Object,     Pond,     Pond, PondFrog,     Pond,  Default},
		{      Object, PondFrog,  Deafult,  Default,  Default,  Default,   Object,  Default},
		{PiranhaPlant,     Pond, PondFrog,     Pond,     Pond,     Pond,     Pond, PondFrog}
	},
	{	/* stage 9 */
		{Pond,    Start,  Default,  Object, Pond},
		{Pond,     Pond, PondFrog, Default, Pond},
		{Pond,   Object,  Deafult, Default, Pond},
		{Pond,  Default, PondFrog,    Pond, Pond},
		{Pond,  Default,  Default,  Object, Pond},
		{Pond,     Pond, PondFrog, Default, Pond},
		{Pond,   Object,  Deafult, Default, Pond},
		{Pond,  Default, PondFrog,    Pond, Pond},
		{Pond,  Default,  Default,  Object, Pond}
	},
	{	/* stage END */
		{Object, Pond, Object},
		{Pond, Start, Pond},
		{Object, Pond, Object}
	}
};

TestScene::TestScene()
{
	_startButton = new Sprite("Start.png");
	_logo = new ObjModel("logo.obj");
	_player = new ObjModel("Combee.obj");
	_clear = new ObjModel("Clear.obj");

	_interpolator = Interpolator::GetInstance();
}

TestScene::~TestScene()
{
	delete[] _rawScript;
}

void TestScene::Init()
{
	Camera::GetInstance()->Init();

	delete[] _rawScript;
	_rawScript = new char[512];
	memset(_rawScript, 0, 512);

	//_objectList.push_back(new Grid());

	_player->Init();
	std::cout << "stage n: " << _stageNumber << std::endl;
	_stageData.mapData = _mapdata[_stageNumber];
	_stageData.xSize = _stageData.mapData[0].size();
	_stageData.ySize = _stageData.mapData.size();

	delete _blockManager;
	_blockManager = new BlockManager();
	_blockManager->SetStageData(_stageData);

	_objectList.push_back(_blockManager);

	_playerMatrix = glm::mat4(1.0f);
	_playerMatrix = glm::translate(_playerMatrix, glm::vec3(0.0f, 0.2f, 0.0f));
	//_playerMatrix = glm::rotate(_playerMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_playerMatrix = glm::scale(_playerMatrix, glm::vec3(0.3f));

	_dir = glm::vec2(1, 0);
	
	_objectCount = _stageData.GetObjectCount();
	_buttonY = 550.0f;

	_errorMsg = "";
	_sign = _logo;
	_logo->Init();
	_clear->Init();
	_logoY = 3.0f;
	_drawLogo = true;
	_logoScale = 2.0f;
	
	_interpolator->AddTween({ CircInOut, &_logoY, 5.0f, 0.0f, 4.2f, [this](Tween t)
	{
		*t._value = t._end;
		if (_isStart)
		{
			_interpolator->AddTween({ CircIn, &_logoY, 1.0f, 4.2f, -7.0f, [this](Tween t)
			{
				*t._value = t._end;
				_drawLogo = false;
			} });
		}
	} });

	SoundManager::GetInstance()->StopChannel(Background);
	SoundManager::GetInstance()->PlaySound(BG_Stage);

	_buttonPos = glm::vec2(1200 / 2.f, 800 / 4.f * 3);
	_buttonSize = glm::vec2(520.0f, 222.0f)/2.0f;
	_startButton->Init();
	GameScene::Init();
}

void TestScene::Update()
{
	GameScene::Update();
	static auto app = GameApp::GetInstance();
	static bool _isClicked = false;
	// 마우스 클릭때 버튼 안쪽에 있으면.
	if (!_isClicked && app->_isClick)
	{
		if (picking(_buttonPos, _buttonSize)) // picking
		{
			_isStart = true;
			_interpolator->AddTween({ CircIn, &_logoY, 1.0f, 4.2f, -7.0f, [this](Tween t)
			{
				*t._value = t._end;
				_drawLogo = false;
			} });
			_isClicked = true;
		}
	}

	if (_isPlay)
	{
		PlayUpdate();
	}
}

void TestScene::PreRender()
{
	GameScene::PreRender();
}

void TestScene::Render()
{
	GameScene::Render();

	if (_drawLogo)
	{
		glm::mat4 logoT(1.0f);
		logoT = glm::translate(logoT, glm::vec3(1.0f, 5.0f-_logoY, 1.0f));
		logoT = glm::rotate(logoT, glm::radians(20.0f), glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0, 1, 0))[2]));
		logoT = glm::scale(logoT, glm::vec3(_logoScale));
		_sign->Update(logoT);
		_sign->Render();
	}
	
	if (_isPlay)
	{
		glm::mat4 playerTween(1.0f);

		switch (*_iter)
		{
		case PlayerAction::Go:
			playerTween = glm::translate(playerTween, _playerGo * normalize(glm::vec3(_playerMatrix[0])));
			break;
		case PlayerAction::TurnLeft:
		case PlayerAction::TurnRight:
			playerTween = glm::translate(playerTween, glm::vec3(_playerMatrix[3]));
			playerTween = glm::rotate(playerTween, _playerTurn, glm::vec3(0, 1.0, 0));
			playerTween = glm::translate(playerTween, glm::vec3(-_playerMatrix[3]));
			break;
		}

		_player->Update(playerTween*_playerMatrix);
	}
	else
	{
		_player->Update(_playerMatrix);
	}
	_player->Render();


	if (!_isStart)
	{
		glm::mat4 model(1.0f);

		//model = glm::translate(model, glm::vec3(0.5f * _buttonSize.x, 0.5f * _buttonSize.y, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(_buttonPos, 200.0f));
		model = glm::translate(model, glm::vec3(-0.5f * _buttonSize.x, -0.5f * _buttonSize.y, 0.0f));

		model = glm::scale(model, glm::vec3(_buttonSize, 1.0f));
		_startButton->Update(model);
		_startButton->Render();
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();

	if (!_drawLogo && ImGui::Begin(u8"꿀벌 대소동", nullptr, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::Text(u8"프로그래밍 공간");
		ImGui::SameLine();
		HelpMarker();

		ImGui::SetNextItemWidth(-1);
		ImGui::InputTextMultiline("", _rawScript, 512, ImVec2(-1, 300), ImGuiInputTextFlags_AllowTabInput);
		
		if (ImGui::Button(u8"시작!"))
		{
			// Play something.
			delete _script;
			_script = new script::Script(_rawScript);
			try {
				_script->Exec();
				_isPlay = true;
				PlayInit();
			}
			catch (script::LexicalError& e)
			{
				std::cout << "Lexer Error : " << e.what() << std::endl;
				_errorMsg = e.what();
			}
			catch (std::invalid_argument& e)
			{
				std::cout << "Error(invalid_argument) : " << e.what() << std::endl;
				_errorMsg = e.what();
			}
			catch (std::out_of_range& e)
			{
				std::cout << "Error(out_of_range) : " << e.what() << std::endl;
				_errorMsg = e.what();
			}
			catch (std::range_error& e)
			{
				std::cout << "Error(range_error) : " << e.what() << std::endl;
				_errorMsg = e.what();
			}
			catch (std::system_error& e)
			{
				std::cout << "Error(system_error) code:" << e.code() << " " << e.what() << std::endl;
				_errorMsg = e.what();
			}
			catch (script::ParsingError& e)
			{
				std::cout << "Error : " << e.what() << std::endl;
				_errorMsg = e.what();
			}
			catch (script::Error& e)
			{
				std::cout << "Error : " << e.what() << std::endl;
				_errorMsg = e.what();
			}
			catch (std::bad_alloc)
			{
				std::cout << "Error : bad_alloc" << std::endl;
				_errorMsg = "bad_alloc Error";
			}
			catch (...)
			{
				std::cout << "Error : Unexcepted Error" << std::endl;
				_errorMsg = "Unexpected Error";
			}
		}
		
		ImGui::SameLine();
		if (ImGui::Button(u8"초기화"))
		{
			PlayRelease();
			PlayInit();
		}
#ifdef _DEBUG
		if (ImGui::Button("PrevStage"))
		{
			_stageNumber--;
			PlayRelease();
			Release();
			Init();
		}
		ImGui::SameLine();
		if (ImGui::Button("NextStage"))
		{
			_stageNumber++;
			PlayRelease();
			Release();
			Init();
		}
#endif
		
		if (!_errorMsg.empty())
		{
			ImGui::Text(_errorMsg.c_str());
		}
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestScene::PostRender()
{
	GameScene::PostRender();
}

void TestScene::Release()
{
	GameScene::Release();
}

void TestScene::PlayInit()
{
	_playerGo = 0.0f;
	_playerTurn = 0.0f;

	_nextAction = true;

	_curTween = nullptr;
	_actions = _script->_symbolTable.GetActions();
	_iter = _actions.begin();
	_errorMsg = "";

	_objectCount = _stageData.GetObjectCount();

	_playerX = _blockManager->GetStartX();
	_playerY = _blockManager->GetStartY();
	_dir = glm::vec2(1, 0);

	_blockManager->SetStageData(_stageData);
}


void TestScene::PlayUpdate()
{
	static auto callback = [this](Tween t) -> void
	{
		_nextAction = true;
		glm::mat4 playerTween(1.0f);

		if (!this->_isPlay) return;
		
		switch (*_iter)
		{
		case PlayerAction::Go:
			playerTween = glm::translate(playerTween, t._end * normalize(glm::vec3(_playerMatrix[0])));
			break;
		case PlayerAction::TurnLeft:
		case PlayerAction::TurnRight:
			playerTween = glm::translate(playerTween, glm::vec3(_playerMatrix[3]));
			playerTween = glm::rotate(playerTween, t._end, glm::vec3(0, 1.0, 0));
			playerTween = glm::translate(playerTween, glm::vec3(-_playerMatrix[3]));
			break;
		}
		_playerMatrix = playerTween * _playerMatrix;

		++_iter;
		*t._value = t._start;
	};

	static glm::mat<2, 2, int> leftMat(0, -1, 1, 0);
	static glm::mat<2, 2, int> rightMat(0, 1, -1, 0);

	if (_nextAction)
	{
		// 메트릭스 반영.
		_nextAction = false;
		if (_objectCount == 0)
		{
			_isPlay = false;
			_drawLogo = true;
			_sign = _clear;
			_logoScale = 0.5f;
			_errorMsg = u8"클리어!";
			SoundManager::GetInstance()->StopChannel(Background);
			SoundManager::GetInstance()->PlaySound(FX_Clear);
			_interpolator->AddTween({ CircInOut, &_logoY, 3.0f, 0.0f, 4.2f, [this](Tween t)
			{
				this->_interpolator->AddTween({ CircIn, &_logoY, 2.0f, 4.2f, -5.0f, [this](Tween t)
				{
					_drawLogo = false;

					if (_sign == _clear)
					{
						_stageNumber++;
						Release();
						Init();
					}
				} });
			} });
			
			PlayRelease();
			return;
		}
		if (_iter != _actions.end())
		{
			SoundManager::GetInstance()->PlaySound(FX_Move);
			if (*_iter == PlayerAction::Go)
			{
				auto blockType = _blockManager->GetBlock(_playerX + _dir.x, _playerY + _dir.y);

				switch (blockType)
				{
				case Start: case Default: break;
				case Object:
					_blockManager->SetBlock(_playerX + _dir.x, _playerY + _dir.y, Default);
					_objectCount--;
					SoundManager::GetInstance()->PlaySound(FX_Acquire);
					break;
				default: 
					_isPlay = false;
					_errorMsg = u8"더 이상 앞으로 갈 수 없습니다!";
					_blockManager->SetStageData(_stageData);
					PlayRelease();
					return;
				}
			}
			
			switch (*_iter)
			{
			case PlayerAction::Go:
				_goTween = { ease_function::CircInOut, &_playerGo, 0.5f, 0.0f, 0.2f, callback };
				_curTween = _interpolator->AddTween(_goTween);
				// _playerX, _playerY change.
				_playerX += _dir.x;
				_playerY += _dir.y;
				break;
			case PlayerAction::TurnLeft:
				_turnTween = { CircInOut, &_playerTurn, 0.5f, 0.0f, glm::radians(90.0f), callback };
				_curTween = _interpolator->AddTween(_turnTween);
				// dir change;
				_dir = leftMat * _dir;
				break;
			case PlayerAction::TurnRight:
				_turnTween = { CircInOut, &_playerTurn, 0.5f, 0.0f, glm::radians(-90.0f), callback };
				_curTween = _interpolator->AddTween(_turnTween);
				// dir change;
				_dir = rightMat * _dir;
				break;
			}
			//std::cout << "player: x-" << _playerX << ", y-" << _playerY << std::endl;
			//std::cout << "dir: x-" << _dir.x << ", y-" << _dir.y << std::endl;
		}
		else
		{
			_isPlay = false;
			PlayRelease();
		}
	}
}

void TestScene::PlayRelease()
{
	_isPlay = false;
	_playerMatrix = glm::mat4(1.0f);
	_playerMatrix = glm::translate(_playerMatrix, glm::vec3(0.0f, 0.2f, 0.0f));
	//_playerMatrix = glm::rotate(_playerMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_playerMatrix = glm::scale(_playerMatrix, glm::vec3(0.3f));

	_playerX = _blockManager->GetStartX();
	_playerY = _blockManager->GetStartY();
	_dir = glm::vec2(1, 0);
	
	_iter = _actions.end();
	_actions.clear();
	Camera::GetInstance()->Init();
}

/*
Solution - 4

var i
for i = 1; i < 5; i = i + 1 {
	Go(i)
	if (i % 2 == 0) {
		TurnLeft(1)
	}
	else {
		TurnRight(1)
	}
}

Solution - 1

Go(4)
TurnLeft(2)
Go(4)
loop 2 {
	TurnLeft(1)
	Go(4)
}

Solution - 2

var i
for i = 0; i < 5; i = i + 1 {
	if (i < 2) {
		Go(2)
		TurnRight(1)
	}
	else {
		Go(4)
		TurnRight(1)
	}
}
*/
