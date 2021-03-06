#pragma once

#include "ui/Config/DivaUIConfig.h"
#include "app/SoraGameState.h"
#include "app/SoraGameApp.h"
#include "SoraEvent.h"
#include "SoraCanvas.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "guichansetup.h"

#include "StageUI.h"

namespace diva
{
	namespace StageUI
	{
		class StageGameState: public sora::SoraGameState, public sora::SoraEventHandler 
		{
		public:
			bool initialized;
			sora::SoraBaseCanvas* uiCanvas;
			StageUI* stageUI;

			StageGameState()
			{
				initialized = false;
			}

			void onEnter()
			{
				if (!initialized)
				{
					uiCanvas = new sora::SoraBaseCanvas(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());
					stageUI = StageUI::Instance();

					initialized = true;
				}
				sora::SoraCore::Instance()->setFPS(60);
			}

			void onUpdate(float dt) {   
				stageUI->Update(dt);
			}

			void onRender() {

				uiCanvas->beginRender(0);

				sora::GCN_GLOBAL->gcnDraw();
				stageUI->Render();

				uiCanvas->finishRender();

				// ------------------------------

				sora::SoraGameApp::BeginScene();

				sora::SoraSprite* sprite = uiCanvas->getCanvasSprite();
				sprite->setScale(double(setConfig[L"windowWidth"].asInt()) / setConfig[L"gameWidth"].asInt(), double(setConfig[L"windowHeight"].asInt()) / setConfig[L"gameHeight"].asInt());
				sprite->render(0, 0);

				sora::SoraGameApp::EndScene();
			}
		};

	}

}