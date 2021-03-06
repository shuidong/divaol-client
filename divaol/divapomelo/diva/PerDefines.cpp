#include "PreDefines.h"

namespace divapomelo {
	const char* EventCode[] =  {
		"onConnect",
		"onLogin",
		"onClose",
		"onEnter",
		"onLeave",
		"onChat",
		"onKick",
		"stage.onSetMode",
		"stage.onSetHook",
		"stage.onSetSong",
		"stage.onDraw",
		"stage.onReady",
		"stage.onUnready",
		"stage.onJoin",
		"stage.onLeave",
		"stage.onCreate",
		"stage.onClose",
		"stage.onAllInfo",
		"stage.onStart",
		"stage.onReturn",
		"stage.onKick",
		"game.onStart",
		"game.onHeartbeat",
		"game.onFailure",
		"game.onRenew",
		"game.relay.onVoidPeriod",
		"game.relay.onChance",
		"game.relay.onNewPlayer",
		"lobby.chatHandler.send",
		"lobby.lobbyHandler.getStageList",
		"lobby.lobbyHandler.createStage",
		"lobby.lobbyHandler.joinStage",
		"lobby.lobbyHandler.leaveStage",
		"lobby.stageHandler.draw",
		"lobby.stageHandler.setSong",
		"lobby.stageHandler.setMode",
		"lobby.stageHandler.setHook",
		"lobby.stageHandler.ready",
		"lobby.stageHandler.start",
		"lobby.stageHandler.kick",
		"game.gameHandler.back",
		"game.gameHandler.link",
		"game.gameHandler.update",
		"game.gameHandler.over",
		"game.gameHandler.eval",
		"game.gameHandler.failure",
		"game.gameHandler.renew",
		"game.gameHandler.relayIamPlaying",
		"game.gameHandler.relayRelayWantToChange",
		"game.gameHandler.relayRelayWantToPlay",
		"local.update.songUI"
	};

	const char* PlayerStatusCode[] = {"load", "ready", "play", "leave", "over", "back"};
}