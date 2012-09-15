﻿#include "HouseUI.h"

#include <fstream>
#include "ui/Config/DivaUIConfig.h"
#include "luaclass.h"

#include "soraguiimage.hpp"
#include "SoraSprite.h"

#include "ui/DivaNetwork/DivaNetwork.h"
#include "ui/Player/PlayerManager.h"
#include "SoraFontManager.h"



namespace diva
{
	namespace HouseUI
	{
		HouseUI::HouseUI()
		{
			top = new gcn::Container();
			top->setSize(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());
			top->setOpaque(false);
			Enter();

			roomTop = new gcn::ContainerEx();
			roomTop->setSize(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());
			roomTop->setOpaque(false);
			roomTop->setEnabled(false);
			roomTop->setVisible(true);

			// lua init
			//sora::SoraLuaObject lo;
			//gge::LuaClass<HouseUI>(lo.getState(), "UI", this).
			//	def("ParseJson", &HouseUI::ParseJson).
			//	def("CreateStaticImage", &HouseUI::CreateStaticImage).
			//	def("CreateButton", &HouseUI::CreateButton).
			//	def("CreateLoginWindow", &HouseUI::CreateLoginWindow);
			//lo.doScript(L"HouseUI.lua");
			
			state = STATE_ROOM;

			

			// parse json
			ParseJson(L"uiconfig/house.json", L"uiconfig/stage.json", L"uiconfig/RoomList_PlayerList.json");

			// back ground
			roomTop->add(CreateStaticImage(conf, L"TestBackground"));

			// --------- room
			top->add(roomTop, 0, 0);

			// shop button
			shopButton = CreateButton(conf, L"ToolButtons/Normal/Shop_Normal", L"ToolButtons/MouseOn/Shop_MouseOn", L"ToolButtons/MouseDown/Shop_MouseDown", L"ToolButtons/Normal/Shop_Normal");
			roomTop->add(shopButton);

			// stage button
			stageButton = CreateButton(conf, L"ToolButtons/Normal/Stage_Normal", L"ToolButtons/MouseOn/Stage_MouseOn", L"ToolButtons/MouseDown/Stage_MouseDown", L"ToolButtons/Normal/Stage_Normal");
			roomTop->add(stageButton);
			stageButton->addMouseListener(new LoginButton_MouseListener());

			// option button
			optionButton = CreateButton(conf, L"ToolButtons/Normal/Option_Normal", L"ToolButtons/MouseOn/Option_MouseOn", L"ToolButtons/MouseDown/Option_MouseDown", L"ToolButtons/Normal/Option_Normal");
			roomTop->add(optionButton);

			// modify button
			modifyButton = CreateButton(conf, L"ToolButtons/Normal/Modify_Normal", L"ToolButtons/MouseOn/Modify_MouseOn", L"ToolButtons/MouseDown/Modify_MouseDown", L"ToolButtons/Normal/Modify_Normal");
			roomTop->add(modifyButton);

			// clothes button
			clothesButton = CreateButton(conf, L"ToolButtons/Normal/Clothes_Normal", L"ToolButtons/MouseOn/Clothes_MouseOn", L"ToolButtons/MouseDown/Clothes_MouseDown", L"ToolButtons/Normal/Clothes_Normal");
			roomTop->add(clothesButton);

			// exit button
			exitButton = CreateButton(conf, L"ToolButtons/Normal/Exit_Normal", L"ToolButtons/MouseOn/Exit_MouseOn", L"ToolButtons/MouseDown/Exit_MouseDown", L"ToolButtons/Normal/Exit_Normal");
			roomTop->add(exitButton);

			// message area
			messagePanel = CreateMessagePanel(conf);
			roomTop->add(messagePanel);


			//gcn::TextBox * bb = new gcn::TextBox("sdfsdfjjkwefkjndfkjsndfjksdnfkjsnfkjsdnfk");
			//bb->setSize(100, 100);
			//roomTop->add(bb, 400, 400);

			///////////////////////////////////////////////// stage buttons
			// up down button
			udButton = CreateButton(sconf, L"ToolBar/Normal/btn_UD_normal", L"ToolBar/MouseOn/btn_UD_mouseon", L"ToolBar/MouseDown/btn_UD_mousedown", L"ToolBar/Normal/btn_UD_normal");
			roomTop->add(udButton);
			udButton->setVisible(false);

			// settings button
			settingsButton = CreateButton(sconf, L"ToolBar/Normal/btn_settings_normal", L"ToolBar/MouseOn/btn_settings_mouseon", L"ToolBar/MouseDown/btn_settings_mousedown", L"ToolBar/Normal/btn_settings_normal");
			roomTop->add(settingsButton);
			settingsButton->setVisible(false);

			// exitStageButton button
			exitStageButton = CreateButton(sconf, L"ToolBar/Normal/btn_exitstage_normal", L"ToolBar/MouseOn/btn_exitstage_mouseon", L"ToolBar/MouseDown/btn_exitstage_mousedown", L"ToolBar/Normal/btn_exitstage_normal");
			roomTop->add(exitStageButton);
			exitStageButton->setVisible(false);
			exitStageButton->addMouseListener(new LoginButton_MouseListener());

			// select music button
			selectMusicButton = CreateButton(sconf, L"ToolBar/Normal/btn_selectmusic_normal", L"ToolBar/MouseOn/btn_selectmusic_mouseon", L"ToolBar/MouseDown/btn_selectmusic_mousedown", L"ToolBar/Normal/btn_selectmusic_normal");
			roomTop->add(selectMusicButton);
			selectMusicButton->setVisible(false);

			// decorate button
			decorateButton = CreateButton(sconf, L"ToolBar/Normal/btn_decorate_normal", L"ToolBar/MouseOn/btn_decorate_mouseon", L"ToolBar/MouseDown/btn_decorate_mousedown", L"ToolBar/Normal/btn_decorate_normal");
			roomTop->add(decorateButton);
			decorateButton->setVisible(false);

			// login 
			loginPanel = CreateLoginWindow(conf, L"Login");
			top->add(loginPanel);
			//loginPanel->addModifier(new

			//  ----- status panel
			statusPanel = CreateStatusPanel(conf);
			roomTop->add(statusPanel);
			
			//Player list
			sPlayerListPanel = CreatePlayerListPanel(sconf);
			sPlayerListPanel->setVisible(false);
			roomTop->add(sPlayerListPanel);

			//sPlayerListPanel->setVisible(false);

			usernameInput->setText(L"sonicmisora");
			passwordInput->setText(L"123456");
			
			roomTop->setEnabled(false);

			//

			roomTop->add(CreateThingList(sconf));

			roomTop->add(CreateTeamList(sconf));

			// ---------------

			roomListPanel = CreateRoomListWindow(rconf);
			roomListPanel->setVisible(false);
			top->add(roomListPanel);
		
			//LoginButtonClicked();
			// ------------- Refresh All
			//RefreshStatus();

			//Refresh_hostInfo();

			//LabelEx* lb = new LabelEx(L"¹þ¹þ¹þ¹þ¹þ¹þ");
			//lb->setHeight(lb->getFont()->getHeight());
			//lb->setWidth(50);
			//roomTop->add(lb, 0, 0);
		}

		HouseUI::~HouseUI()
		{
			if (playerListFont)
			{
				delete playerListFont;
			}
			if (statusPanelFont)
			{
				delete statusPanelFont;
			}
			if (messageAreaFont)
			{
				delete messageAreaFont;
			}
		}

		void HouseUI::Enter()
		{
			sora::GCN_GLOBAL->getTop()->add(top, 0, 0);
		}
		
		void HouseUI::Leave()
		{
			sora::GCN_GLOBAL->getTop()->remove(top);
		}

		void HouseUI::Render()
		{
		}

		void HouseUI::Update(float dt)
		{
			RecvMsg();
		}

		void HouseUI::RecvMsg()
		{
			while (!Net::Network::isEmpty())
			{
				std::wstring msg, arg;
				Net::Network::Recv(msg, arg);

				if (msg == L"LOGIN_RET")
				{
					if (arg[0] == '0')
					{
						//throw "LOGIN FAILED";
					}
					else
					{
						PlayerInfo info;
						int t;
						wchar_t un[100], nm[100];
						swscanf(arg.c_str(), L"%d%s%s%d", &t, un, nm, &info.id);
						info.username = un;
						info.nickname = nm;
						PlayerManager::Instance()->SetHostInfo(info);
						state = STATE_ROOM;
						loginPanel->setVisible(false);
						roomTop->setEnabled(true);
						
						//PlayerManager::Instance()->GetStageGuests().push_back("SonicMisora");
						PlayerManager::Instance()->SetOnline(true);
						//RefreshStatus();
						Refresh_hostInfo();
					}
				}
			}
		}

		void HouseUI::StateChange_ROOM_STAGE()
		{
			state = STATE_STAGE;
			sPlayerListPanel->setVisible(true);

			// set all the room buttons to invisible
			shopButton->setVisible(false);
			stageButton->setVisible(false);
			optionButton->setVisible(false);
			modifyButton->setVisible(false);
			clothesButton->setVisible(false);
			exitButton->setVisible(false);

			// set all the stage buttons to visible

			udButton->setVisible(true);
			settingsButton->setVisible(true);
			exitStageButton->setVisible(true);
			selectMusicButton->setVisible(true);
			decorateButton->setVisible(true);
			thingList->setVisible(true);
			teamList->setVisible(true);

			// exit button
			/*exitButton = CreateButton(conf, "ToolButtons/Normal/Exit_Normal", "ToolButtons/MouseOn/Exit_MouseOn", "ToolButtons/MouseDown/Exit_MouseDown", "ToolButtons/Normal/Exit_Normal");
			roomTop->add(exitButton);*/

			Refresh_sPlayerList();
		}

		void HouseUI::StateChange_STAGE_ROOM()
		{
			state = STATE_ROOM;
			sPlayerListPanel->setVisible(false);

			// set all the room buttons to visible
			shopButton->setVisible(true);
			stageButton->setVisible(true);
			optionButton->setVisible(true);
			modifyButton->setVisible(true);
			clothesButton->setVisible(true);
			exitButton->setVisible(true);

			// set all the stage buttons to invisible

			udButton->setVisible(false);
			settingsButton->setVisible(false);
			exitStageButton->setVisible(false);
			selectMusicButton->setVisible(false);
			decorateButton->setVisible(false);
			thingList->setVisible(false);
			teamList->setVisible(false);
		}

		void HouseUI::StateChange_LOGINWINDOW_ROOM()
		{
			state = STATE_ROOM;
		}

		void HouseUI::StateChange_ROOM_ROOMLIST()
		{
			state = STATE_ROOMLIST;
			
			roomTop->setEnabled(false);
			
			roomListPanel->setVisible(true);
		}

		void HouseUI::StateChange_ROOMLIST_ROOM()
		{
			state = STATE_ROOM;

			roomTop->setEnabled(true);

			roomListPanel->setVisible(false);
		}

		void HouseUI::StateChange_ROOMLIST_STAGE()
		{
			state = STATE_STAGE;
			sPlayerListPanel->setVisible(true);

			// set all the room buttons to invisible
			shopButton->setVisible(false);
			stageButton->setVisible(false);
			optionButton->setVisible(false);
			modifyButton->setVisible(false);
			clothesButton->setVisible(false);
			exitButton->setVisible(false);

			// set all the stage buttons to visible

			udButton->setVisible(true);
			settingsButton->setVisible(true);
			exitStageButton->setVisible(true);
			selectMusicButton->setVisible(true);
			decorateButton->setVisible(true);
			thingList->setVisible(true);
			teamList->setVisible(true);

			//
			roomListPanel->setVisible(false);
			roomTop->setEnabled(true);
			Refresh_sPlayerList();
		}

		void HouseUI::setState(int des)
		{
			if (state == STATE_ROOM && des == STATE_STAGE)
			{
				StateChange_ROOM_STAGE();
				return;
			}
			if (state == STATE_STAGE && des == STATE_ROOM)
			{
				StateChange_STAGE_ROOM();
				return;
			}
			if (state == STATE_LOGINWINDOW && des == STATE_ROOM)
			{
				StateChange_LOGINWINDOW_ROOM();
				return;
			}
			if (state == STATE_ROOM && des == STATE_ROOMLIST)
			{
				StateChange_ROOM_ROOMLIST();
				return;
			}
			if (state == STATE_ROOMLIST && des == STATE_ROOM)
			{
				StateChange_ROOMLIST_ROOM();
				return;
			}
			if (state == STATE_ROOMLIST && des == STATE_STAGE)
			{
				StateChange_ROOMLIST_STAGE();
				return;
			}
		}

		void HouseUI::ParseJson(const std::wstring& filename, const std::wstring& stage, const std::wstring& room)
		{
			//std::ifstream fs(filename);
			WJson::Reader reader;
			if (!reader.parse(ReadJsonFile(filename), conf))
				throw "failed reading WJson.";

			//std::ifstream fs2(stage);
			if (!reader.parse(ReadJsonFile(stage), sconf))
				throw "failed.";

			if (!reader.parse(ReadJsonFile(room), rconf))
				throw "failed.";
		}

		gcn::ContainerEx* HouseUI::CreateStaticImage(const WJson::Value& conf, const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv;
			tv = conf[name];
			ContainerEx* con = new ContainerEx();
			con->load(tv[L"filename"].asString(), 
				gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
			con->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			con->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return con;
		}

		//gcn::ContainerEx* HouseUI::CreateStaticImage(const std::wstring& name)
		//{
		//	using namespace gcn;
		//	Json::Value tv;
		//	tv = conf[name];
		//	ContainerEx* con = new ContainerEx();
		//	con->load(tv[L"filename"].asString(), 
		//		gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
		//	con->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		//	top->add(con,  tv[L"desX"].asInt(), tv[L"desY"].asInt());
		//	return con;
		//}

		//gcn::SuperButtonEx* HouseUI::CreateButton(const std::wstring& normal, const std::wstring& on,
		//	const std::wstring& down, const std::wstring& disable)
		//{
		//	using namespace gcn;
		//	SuperButtonEx* b = _CreateButton(normal, on, down, disable);
		//	top->add(b);
		//	return b;
		//}

		gcn::SuperButtonEx* HouseUI::CreateButton(const WJson::Value& conf, const std::wstring& normal, const std::wstring& on,
			const std::wstring& down, const std::wstring& disable)
		{
			using namespace gcn;
			WJson::Value tv1 = conf[normal], tv2 =conf[on], tv3 = conf[down], tv4 = conf[disable];
			SuperButtonEx* b = new SuperButtonEx();
			b->setLook(tv1[L"filename"].asString(),
				gcn::Rectangle(tv1[L"srcX"].asInt(), tv1[L"srcY"].asInt(), tv1[L"width"].asInt(), tv1[L"height"].asInt()),
				gcn::Rectangle(tv2[L"srcX"].asInt(), tv2[L"srcY"].asInt(), tv2[L"width"].asInt(), tv2[L"height"].asInt()),
				gcn::Rectangle(tv3[L"srcX"].asInt(), tv3[L"srcY"].asInt(), tv3[L"width"].asInt(), tv3[L"height"].asInt()),
				gcn::Rectangle(tv4[L"srcX"].asInt(), tv4[L"srcY"].asInt(), tv4[L"width"].asInt(), tv4[L"height"].asInt()),
				0, 0,
				tv2[L"desX"].asInt() - tv1[L"desX"].asInt(), tv2[L"desY"].asInt() - tv1[L"desY"].asInt(),
				tv3[L"desX"].asInt() - tv1[L"desX"].asInt(), tv3[L"desY"].asInt() - tv1[L"desY"].asInt(),
				tv4[L"desX"].asInt() - tv1[L"desX"].asInt(), tv4[L"desY"].asInt() - tv1[L"desY"].asInt());
			b->setSize(tv1[L"width"].asInt(), tv1[L"height"].asInt());
			b->setPosition(tv1[L"desX"].asInt(), tv1[L"desY"].asInt());
			
			return b;
		}

		RoomListItem* HouseUI::CreateRoomListItem(const WJson::Value conf, const std::wstring& normal, const std::wstring& on, const std::wstring& down)
		{
			using namespace gcn;
			WJson::Value tv1 = conf[normal], tv2 =conf[on], tv3 = conf[down];
			RoomListItem* b = new RoomListItem();
			b->setLook(tv1[L"filename"].asString(),
				gcn::Rectangle(tv1[L"srcX"].asInt(), tv1[L"srcY"].asInt(), tv1[L"width"].asInt(), tv1[L"height"].asInt()),
				tv2[L"filename"].asString(),
				gcn::Rectangle(tv2[L"srcX"].asInt(), tv2[L"srcY"].asInt(), tv2[L"width"].asInt(), tv2[L"height"].asInt()),
				tv2[L"filename"].asString(),
				gcn::Rectangle(tv3[L"srcX"].asInt(), tv3[L"srcY"].asInt(), tv3[L"width"].asInt(), tv3[L"height"].asInt()));
			Network::RoomInfo info;
			info.maxPlayerNum = 10;
			info.owner = L"次音速豆豆";
			info.playerNum = 8;
			info.selectedSong.push_back(L"私の恋を許してくれ");
			info.stageName = L"超级音速豆豆";
			b->setInfo(info);
			return b;
		}

		gcn::WTextField* HouseUI::CreateInput(const WJson::Value& conf, const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv = conf[name];
			gcn::WTextField* text = new WTextField();
			text->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			text->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return text;
		}

		gcn::LabelEx* HouseUI::CreateLabel(const WJson::Value& conf, const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv = conf[name];
			gcn::LabelEx* text = new LabelEx(tv[L"text"].asString());
			text->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			text->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return text;
		}


		gcn::ContainerEx* HouseUI::CreateLoginWindow(const WJson::Value& conf, const std::wstring& prefix)
		{
			using namespace gcn;
			WJson::Value tv;
			
			// panel
			ContainerEx* con = CreateStaticImage(conf, prefix + L"/BackGround");
			int topX = con->getX(), topY = con->getY();

			//tv = conf[L"Login/Config"];
			//con->setVisible(true);
			//con->setPosition(tv[L"desX_1"].asInt(), tv[L"desY_1"].asInt());
			//con->addModifier(new GUIAnimation_Position(gcn::Point(tv[L"desX_2"].asInt(), tv[L"desY_2"].asInt()),
			//	tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearSin));

			// username text box
			WTextField* username = CreateInput(conf, prefix + L"/textBox_1");
			con->add(username, username->getX() - topX, username->getY() - topY);
			sora::GCN_GLOBAL->getTop()->focusNext();
			usernameInput = username;

			// pass word textbox
			WTextField* password = CreateInput(conf, prefix + L"/textBox_2");
			con->add(password, password->getX() - topX, password->getY() - topY);
			passwordInput = password;

			// login button
			SuperButtonEx* lb = CreateButton(conf, prefix + L"/LoginButton_Normal",
				prefix + L"/LoginButton_MouseOn",
				prefix + L"/LoginButton_MouseDown",
				prefix + L"/LoginButton_Normal");
			con->add(lb, lb->getX() - topX, lb->getY() - topY);
			lb->addMouseListener(new LoginButton_MouseListener());
			loginButton = lb;

			//loginButton->addModifier(new GUIAnimation_Position(gcn::Point(0, 0), 120, GUIAnimation_Float_Linear, GUIAnimation_Mode::NONE));

			// user login label
			LabelEx* lb1 = CreateLabel(conf, prefix + L"/Label_1");
			con->add(lb1, lb1->getX() - topX, lb1->getY() - topY);
			

			// username label
			LabelEx* lb2 = CreateLabel(conf, prefix + L"/Label_2");
			con->add(lb2, lb2->getX() - topX, lb2->getY() - topY);

			// password label
			LabelEx* lb3 = CreateLabel(conf, prefix + L"/Label_3");
			con->add(lb3, lb3->getX() - topX, lb3->getY() - topY);

			con->setVisible(true);
			
			
			//top->add(con);
			return con;
		}

		ContainerEx* HouseUI::CreateRoomListWindow(const WJson::Value conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"RoomList/Config"];

			ContainerEx* panel = CreateStaticImage(conf, L"RoomList/background");

			//////////////////////////////////////////////////////////////////////////
			
			SuperButtonEx* closeButton = CreateButton(conf, L"RoomList/btn_close/btn_close_normal", L"RoomList/btn_close/btn_close_on",
				L"RoomList/btn_close/btn_close_down", L"RoomList/btn_close/btn_close_normal");
			closeButton->setPosition(closeButton->getX() - panel->getX(), closeButton->getY() - panel->getY());
			closeButton->addMouseListener(new LoginButton_MouseListener());
			panel->add(closeButton);
			roomListCloseButton = closeButton;

			//////////////////////////////////////////////////////////////////////////

			SuperButtonEx* openButton = CreateButton(conf, L"RoomList/btn_openStage/btn_openStage_normal", L"RoomList/btn_openStage/btn_openStage_on",
				L"RoomList/btn_openStage/btn_openStage_down", L"RoomList/btn_openStage/btn_openStage_normal");
			openButton->setPosition(openButton->getX() - panel->getX(), openButton->getY() - panel->getY());
			openButton->setText(tv[L"openStageText"].asString());
			openButton->addMouseListener(new LoginButton_MouseListener());
			panel->add(openButton);
			roomListOpenButton = openButton;
			
			
			//////////////////////////////////////////////////////////////////////////

			ListViewEx* list = new ListViewEx();
			list->setGap(gcn::Rectangle(tv[L"firstX"].asInt(), tv[L"firstY"].asInt(), tv[L"firstWidth"].asInt(), tv[L"firstHeight"].asInt()),
				tv[L"xGap"].asInt(), tv[L"yGap"].asInt());
			list->setMaxItem(tv[L"xItemCount"].asInt(), tv[L"yItemCount"].asInt());
			list->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			list->adjustSize();
			list->setPosition(list->getX() - panel->getX(), list->getY() - panel->getY());
			RoomListItem::setTextPosition(tv[L"x1"].asInt(), tv[L"y1"].asInt(),
				tv[L"x2"].asInt(), tv[L"y2"].asInt(),
				tv[L"x3"].asInt(), tv[L"y3"].asInt(),
				tv[L"x4"].asInt(), tv[L"y4"].asInt());
			panel->add(list);
			for (int i=1; i<=9; i++)
				list->pushItem(CreateRoomListItem(conf, L"RoomList/RoomItem_normal", L"RoomList/RoomItem_on", L"RoomList/RoomItem_down"));

			//////////////////////////////////////////////////////////////////////////

			tv = conf[L"RoomList/scrollBar/Config"];
			SliderEx* slider = new SliderEx();
			//slider->setSize(
			slider->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			slider->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			slider->setLook(CreateButton(conf, L"RoomList/scrollBar/btn_up_normal", L"RoomList/scrollBar/btn_up_on", L"RoomList/scrollBar/btn_up_down", L"RoomList/scrollBar/btn_up_normal"),
				CreateButton(conf, L"RoomList/scrollBar/btn_down_normal", L"RoomList/scrollBar/btn_down_on", L"RoomList/scrollBar/btn_down_down", L"RoomList/scrollBar/btn_down_normal"),
				CreateButton(conf, L"RoomList/scrollBar/slider/slider_normal", L"RoomList/scrollBar/slider/slider_on", L"RoomList/scrollBar/slider/slider_down", L"RoomList/scrollBar/slider/slider_normal"));
			slider->setMarkScale(0, list->getFullMaxPage(), list->getFullMaxPage() + 1); 
			slider->setPosition(slider->getX() - panel->getX(), slider->getY() - panel->getY());
			panel->add(slider);


			return panel;
		}

		ContainerEx* HouseUI::CreateTeamList(const WJson::Value& conf)
		{
			using namespace gcn;

			WJson::Value tv = conf[L"TeamSelection/Config"];
			int itemCount = tv[L"itemCount"].asInt();

			WJson::Value t2 = conf[L"TeamSelection/Normal/btn_Team1_Normal"];
			WJson::Value t3 = conf[L"TeamSelection/Normal/btn_Team" + s2ws(sora::int_to_str(itemCount)) + (L"_Normal")];

			teamList = new ContainerEx;
			teamList->setOpaque(false);
			teamList->setSize(t3[L"desX"].asInt() - t2[L"desX"].asInt() + t3[L"width"].asInt(),
				t3[L"height"].asInt());
			teamList->setPosition(t2[L"desX"].asInt(), t2[L"desY"].asInt());

			
			for (int i = 0; i < itemCount; i++)
			{
				SuperButtonEx* b = CreateButton(conf, 
					L"TeamSelection/Normal/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_Normal"),
					L"TeamSelection/MouseOn/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_mouseon"),
					L"TeamSelection/Selected/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_Selected"),
					L"TeamSelection/Normal/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_Normal"));
				b->setSelected(i == 2);
				b->setPosition(b->getX() - teamList->getX(), b->getY() - teamList->getY());
				b->addMouseListener(new TeamSelect_MouseListener());
				b->userData = (void*)i;
				teamListButtons.push_back(b);
				
				teamList->add(b);
				//teamList->pushItem(new ThingListItem(t2[L"filename"].asString(),
				//	gcn::Rectangle(t2[L"srcX"].asInt(), t2[L"srcY"].asInt(), t2[L"width"].asInt(), t2[L"height"].asInt()),
				//	t3[L"filename"].asString(),
				//	gcn::Rectangle(t3[L"srcX"].asInt(), t3[L"srcY"].asInt(), t3[L"width"].asInt(), t3[L"height"].asInt())));
			}
			teamList->setVisible(false);

			return teamList;
		}

		void HouseUI::Refresh_sPlayerList()
		{
			sPlayerList->clearItems();
			WJson::Value tv = sconf[L"RoomInfo/Config"];
			std::vector<PlayerInfo>& stageGuests= PlayerManager::Instance()->GetStageGuests();
			int c = 0;
			for (std::vector<PlayerInfo>::iterator i = stageGuests.begin(); i!=stageGuests.end(); i++, c++)
				sPlayerList->pushItem(new HouseUIRoomInfoListItem(i->nickname, tv[L"colorList"][c%(tv[L"maxItem"].asInt())].asUInt()));
		}

		void HouseUI::Refresh_hostInfo()
		{
			PlayerInfo pf = PlayerManager::Instance()->GetHostInfo();
			wchar_t temp[200];
			_swprintf(temp, L"昵称：%s\nID：%d", pf.nickname.c_str(), pf.id);
			hostInfoLabel->setText(temp);
		}


		void HouseUI::RefreshStatus()
		{
			// Host Info Refresh
			

			if (state == STATE_ROOM)
			{
				roomTop->setEnabled(true);
				sPlayerListPanel->setVisible(false);
				loginPanel->setVisible(false);
			}
			else if (state == STATE_STAGE)
			{
				// visible set
				roomTop->setEnabled(true);
				loginPanel->setVisible(false);
				sPlayerListPanel->setVisible(true);

				// playerlist refresh
				Refresh_sPlayerList();
			}
			else if (state == STATE_LOGINWINDOW)
			{
				roomTop->setEnabled(false);
				sPlayerListPanel->setVisible(false);
				loginPanel->setVisible(true);
			}
		}

		ThingList* HouseUI::CreateThingList(const WJson::Value& conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"itemlist/Config"];
			WJson::Value t2 = conf[L"itemlist/item_itemback_normal"];
			WJson::Value t3 = conf[L"itemlist/item_itemback_selected"];

			thingList = new ThingList;
			thingList->setMaxItem(6);
			thingList->setGap(gcn::Rectangle(0, 0, t2[L"width"].asInt(), t2[L"height"].asInt()), tv[L"itemGap"].asInt());
			thingList->setSize(tv[L"items"].size() * (t2[L"width"].asInt() + tv[L"itemGap"].asInt()) - tv[L"itemGap"].asInt(), 
				t2[L"height"].asInt());
			thingList->setPosition(t3[L"desX"].asInt(), t3[L"desY"].asInt());

			for (WJson::Value::iterator i = tv[L"items"].begin(); i!= tv[L"items"].end(); i++)
			{
				thingList->pushItem(new ThingListItem(t2[L"filename"].asString(),
					gcn::Rectangle(t2[L"srcX"].asInt(), t2[L"srcY"].asInt(), t2[L"width"].asInt(), t2[L"height"].asInt()),
					t3[L"filename"].asString(),
					gcn::Rectangle(t3[L"srcX"].asInt(), t3[L"srcY"].asInt(), t3[L"width"].asInt(), t3[L"height"].asInt())));
			}
			thingList->setVisible(false);
			
			return thingList;
		}

		gcn::ContainerEx* HouseUI::CreateStatusPanel(const WJson::Value& conf)
		{
			using namespace gcn;
			ContainerEx* panel = CreateStaticImage(conf, L"StatusArea");
			WJson::Value tv = conf[L"StatusPanel/Config"];
			//bool b = conf.isMember("StatusPanel/Config");
			
			LabelEx* lb = new LabelEx(L"");
			lb->setSize(tv[L"lbWidth"].asInt(), tv[L"lbHeight"].asInt());
			panel->add(lb, tv[L"lbX"].asInt(), tv[L"lbY"].asInt());
			statusPanelFont = new gcn::SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			lb->setFont(statusPanelFont);
			hostInfoLabel = lb;

			return panel;
		}

		gcn::ContainerEx* HouseUI::CreatePlayerListPanel(const WJson::Value& conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"RoomInfo/Config"];
			ContainerEx* panel = CreateStaticImage(conf, L"RoomInfo/background");
			playerListFont = new gcn::SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			
			RoomInfoList* list = new RoomInfoList();
			list->setMaxItem(tv[L"maxItem"].asInt());
			list->setSize(panel->getWidth(), panel->getHeight());
			list->setGap(gcn::Rectangle(tv[L"gap"][L"x"].asInt(),
				tv[L"gap"][L"y"].asInt(),
				tv[L"gap"][L"width"].asInt(),
				tv[L"gap"][L"height"].asInt()), 
				tv[L"gap"][L"itemGap"].asInt());
			//Font* font = new gcn::SoraGUIFont(L"res/msyh.ttf", 16);
			list->setFont(playerListFont);
			//list->setHorizontal(true);
			panel->add(list, 0, 0);
			sPlayerList = list;

			SuperButtonEx* b1 = CreateButton(conf, L"RoomInfo/buttons/btn_left", 
				L"RoomInfo/buttons/btn_left_mouseon", L"RoomInfo/buttons/btn_left_mousedown", L"RoomInfo/buttons/btn_left");
			b1->setPosition(b1->getX() - panel->getX(), b1->getY() - panel->getY());
			panel->add(b1);

			SuperButtonEx* b2 = CreateButton(conf, L"RoomInfo/buttons/btn_right", 
				L"RoomInfo/buttons/btn_right_mouseon", L"RoomInfo/buttons/btn_right_mousedown", L"RoomInfo/buttons/btn_right");
			b2->setPosition(b2->getX() - panel->getX(), b2->getY() - panel->getY());
			panel->add(b2);

			ContainerEx* pagenum = CreateStaticImage(conf, L"RoomInfo/buttons/label_pagenum");
			pagenum->setPosition(pagenum->getX() - panel->getX(), pagenum->getY() - panel->getY());
			panel->add(pagenum);
			playerListPagenumFont = new SoraGUIFont(L"res/msyh.ttf", tv[L"pagenumFontSize"].asInt());
			pagenum->setFont(playerListPagenumFont);
			pagenum->setText(L"1/1");

			//for (int i=1; i<=9; i++)
			//list->pushItem(new HouseUIRoomInfoListItem("SonicMisora",  (tv[L"colorList"][2]).asInt()));
			//list->pushItem(new HouseUIRoomInfoListItem("SMBean",  (tv[L"colorList"][1]).asInt()));
			
			return panel;
		}

		gcn::SliderEx* HouseUI::CreateMessageSlider(const WJson::Value& conf)
		{
			using namespace gcn;
			//conf[""
			return new SliderEx();
		}

		gcn::ContainerEx* HouseUI::CreateMessagePanel(const WJson::Value& conf)
		{
			using namespace gcn;
			//WJson::Value tv = conf[];

			ContainerEx* panel = CreateStaticImage(conf, L"MessageArea/InputArea_BackGround");

			WJson::Value tv;

			// chat box
			tv = conf[L"MessageArea/TextBox"];
			HouseMessageBox* chatBox = new HouseMessageBox();
			chatBox->setMaxLine(tv[L"maxLine"].asInt());
			chatBox->setWidth(tv[L"width"].asInt());
			chatBox->setPosition(tv[L"desX"].asInt() - panel->getX(), tv[L"desY"].asInt() - panel->getY());
			messageAreaFont = new SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			chatBox->setFont(messageAreaFont);
			chatBox->adjust();
			chatBox->setText(tv[L"testText"].asString());
			panel->add(chatBox);
			messagePanelChatBox = chatBox;

			// message channel
			tv = conf[L"MessageArea/ChannelList"];
			MessageChannelList* list = new MessageChannelList();
			list->setFirstIndex(0);
			list->setMaxItem(tv[L"items"].size());
			list->setWidth(tv[L"width"].asInt());
			list->setHeight(tv[L"height"].asInt() * tv[L"items"].size());
			list->setGap(gcn::Rectangle(0, 0, list->getWidth(), tv[L"height"].asInt()), 0);
			list->setPosition(tv[L"desX_2"].asInt() - panel->getX(), tv[L"desY_2"].asInt() - panel->getY());
			gcn::Rectangle rect(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt());
			for (WJson::Value::iterator i = tv[L"items"].begin(); i != tv[L"items"].end(); i++)
			{
				list->pushItem(new MessageChannelListItem(tv[L"filename"].asString(), rect, (*i).asString()));
			}
			list->setVisible(false);
			panel->add(list);

			messageChannelList = list;

			// message to
			ContainerEx* messageToBox = CreateStaticImage(conf, L"MessageArea/MessageTo/ToSomeOne");
			messageToBox->setPosition(messageToBox->getX() - panel->getX(), messageToBox->getY() - panel->getY());
			panel->add(messageToBox);
			messageToSomeOne = messageToBox;

			// channel
			ContainerEx* channelBox = CreateStaticImage(conf, L"MessageArea/MessageTo/Channel");
			channelBox->setPosition(channelBox->getX() - panel->getX(), channelBox->getY() - panel->getY());
			//channelBox->setId("MessagePanel_Channel");
			//sora::SoraGUI::Instance()->registerGUIResponser(channelBox, this, "MessagePanel_Channel", sora::RESPONSEMOUSE);
			channelBox->addMouseListener(new LoginButton_MouseListener());
			panel->add(channelBox);
			messageChannel = channelBox;

			// input box
			tv = conf[L"MessageArea/InputBox"];
			TextFieldEx* inputBox = new TextFieldEx();
			inputBox->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			inputBox->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			inputBox->setImage(tv[L"filename"].asString(), 
				gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
			messageInputFont = new SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			inputBox->setFont(messageInputFont);
			inputBox->setForegroundColor(tv[L"fontColor"].asUInt());
			inputBox->setPosition(inputBox->getX() - panel->getX(), inputBox->getY() - panel->getY());
			inputBox->setId("MessagePanel_InputBox");
			sora::SoraGUI::Instance()->registerGUIResponser(inputBox, this, "MessagePanel_InputBox", sora::RESPONSEACTION);
			panel->add(inputBox);
			messagePanelInputBox = inputBox;
			





			return panel;
		}

		void HouseUI::LoginButtonClicked()
		{
			using namespace Net;
			Network::Send(L"LOGIN", usernameInput->getText() + L" " + passwordInput->getText());
		}



		void HouseUI::SetWidgetInvisible(gcn::Widget* widget)
		{
			widget->setVisible(false);
		}

		void HouseUI::MouseClicked(gcn::MouseEvent& mouseEvent)
		{
			//using namespace gcn;
			if (mouseEvent.getSource() == (gcn::Widget*) loginButton && loginButton->checkIsEnabled())
			{
				LoginButtonClicked();
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) stageButton) //&& stageButton->checkIsEnabled())
			{
				//LoginButtonClicked();
				if (state == STATE_ROOM)
				{
					setState(STATE_ROOMLIST);
					//RefreshStatus();
				}
				else if (state == STATE_STAGE)
				{
					setState(STATE_ROOM);
					//RefreshStatus();
				}
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) exitStageButton && exitStageButton->checkIsEnabled())
			{
				setState(STATE_ROOM);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) messageChannel)
			{
				MessagePanelChannelClicked();
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) roomListCloseButton)
			{
				setState(STATE_ROOM);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) roomListOpenButton)
			{
				setState(STATE_STAGE);
				return;
			}
		}

		void HouseUI::MessagePanelChannelListClicked(int index)
		{
			WJson::Value tv = conf[L"MessageArea/ChannelList"];
			if (index != -1)
				messageChannel->setText(conf[L"MessageArea/ChannelList"][L"items"][index].asString());

			messageChannelList->setPosition(tv[L"desX_2"].asInt() - messagePanel->getX(), tv[L"desY_2"].asInt() - messagePanel->getY());
			messageChannelList->addModifier(new GUIAnimation_Position(gcn::Point(tv[L"desX_1"].asInt() - messagePanel->getX(),
				tv[L"desY_1"].asInt() - messagePanel->getY()),
				tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearCos, NONE, NULL, 
				sora::Bind(this, &HouseUI::SetWidgetInvisible)));
			messageChannelList->setAlpha(255);
			messageChannelList->addModifier(new GUIAnimation_Alpha(0, tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearCos));
		}

		void HouseUI::MessagePanelChannelClicked()
		{
			if (messageChannelList->isVisible())
			{
				MessagePanelChannelListClicked(-1);
				return;
			}
			WJson::Value tv = conf[L"MessageArea/ChannelList"];
			messageChannelList->setVisible(true);
			messageChannelList->setAlpha(0);
			messageChannelList->setPosition(tv[L"desX_1"].asInt() - messagePanel->getX(), tv[L"desY_1"].asInt() - messagePanel->getY());
			messageChannelList->addModifier(new GUIAnimation_Position(gcn::Point(tv[L"desX_2"].asInt() - messagePanel->getX(),
				tv[L"desY_2"].asInt() - messagePanel->getY()),
				tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearSin));
			messageChannelList->addModifier(new GUIAnimation_Alpha(255, tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearSin));
		}

		void HouseUI::MessagePanelInputBoxEnterPressed()
		{
			if (messagePanelInputBox->getText() == L"")
				return;
			messagePanelChatBox->addText(PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
			messagePanelInputBox->setText(L"");
		}

		void HouseUI::action()
		{
			if (getID() == "MessagePanel_InputBox")
			{
				MessagePanelInputBoxEnterPressed();
			}
		}

		void HouseUI::mouseClicked(const gcn::MouseEvent& mouseEvent)
		{
			if (getID() == "MessagePanel_Channel")
			{
				//MessagePanelChannelClicked();
			}
		}

		void HouseUI::RoomLInfoListClicked(int index)
		{
			messageToSomeOne->setText(L"To " + PlayerManager::Instance()->GetStageGuests()[index].nickname);
		}

		void HouseUI::TeamListClicked(gcn::MouseEvent& mouseEvent)
		{
			int t = (int)((ButtonEx*)mouseEvent.getSource())->userData;
			for (int i=0; i<teamListButtons.size(); i++)
				teamListButtons[i]->setSelected(t == i);
		}


		// ------------------------------------ Event ----------------------------------

		using namespace gcn;

		void LoginButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->MouseClicked(mouseEvent);
		}

		void TeamSelect_MouseListener::mouseClicked(gcn::MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->TeamListClicked(mouseEvent);
		}
	}
}