#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "DivaPlayerItem.h"
#include "ui/DivaNetwork/DivaNetwork.h"
#include <string>
#include <vector>

namespace diva
{
	namespace MusicUI
	{
		using namespace gcn;

		class DivaPlayerList :
			public ListBoxEx
		{
		private:
			DivaRoomInfo info;
			std::vector<std::wstring> iconList;

		public:
			DivaPlayerList();
			~DivaPlayerList();

			void setPlayerReady(int id, bool v);

			DivaRoomInfo& getRoomInfo();

			DivaRoomInfo login();
			void init();

			void ModifyPlayer(int index, const DivaPlayerInfo& info);

		};
	}
}