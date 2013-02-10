/*
 *  DivaClient.h
 *
 *  Created by tempbuffer on 9/27/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_CLIENT
#define DIVA_CLIENT

#include "DivaNetInfo.h"
#include "DivaGNetSystem.h"
#include "Lib/Base/Base.h"
#include "Lib/Base/Pattern/Observer.h"
#include "SoraAutoUpdate.h"

namespace divanet
{
	using Base::Observer;
	using Base::Notification;
	typedef Base::Thread<void()> ThreadTask;
	typedef Base::Function<void()> Task;

	class Client : public Base::ObserverHandler, public SoraAutoUpdate
	{
	public:
		enum {STATE_DISCONNECT,STATE_CONNECT,STATE_BREAK,STATE_CONNECTING};
<<<<<<< HEAD
		enum {NOTIFY_CONNECT,NOTIFY_DISCONNECT,NOTIFY_TIMEOUT};
=======
		enum {NOTIFY_CONNECT,NOTIFY_DISCONNECT,NOTIFY_TIMEOUT,NOTIFY_PING_RESPONSE};
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87

		bool isLogin() const {return mIsLogin;}
		bool isConnect() const {return mIsConnect;}
		uint32 state() const {return mClientState;}

		Client():mIsConnect(false),mClientState(STATE_DISCONNECT),mTickTask(Base::MakeFunction(&Client::tick_thread, this)) {
		}
<<<<<<< HEAD
		virtual ~Client() throw() {
=======
		virtual ~Client() {
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
			if(isConnect())
				disconnect();
		}

		virtual std::string name() const = 0;
		virtual void setNetworkSystem(NetworkPtr ptr) {mNetSys = ptr;}
		NetworkPtr getSys() {return mNetSys;}
		void refresh() {mNetSys->refresh();}
		virtual bool connect() {
			if(isConnect())
				return true;
			mNetSys->setHostInfo(NET_INFO.server(name()).ip, NET_INFO.server(name()).port);
			try{
				mNetSys->connect();
				
				notify("ok",NOTIFY_CONNECT);

				GNET_RECEIVE_REGISTER(mNetSys,"tick#response", &Client::_gnet_tick);
<<<<<<< HEAD
				mWaitTickTime = NetInfo::TIME_OUT;
				mNextTickTime = 0;
=======
				GNET_RECEIVE_REGISTER(mNetSys,"ping#response", &Client::_gnet_ping_response);
				mWaitTickTime = NetInfo::TIME_OUT;
				mIsWaitingPing = false;
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87

				mIsConnect = true;

				mTickTask.start();
			}
			catch(...)
			{
				notify("failed",NOTIFY_CONNECT);
				return false;
			}

			mClientState = STATE_CONNECT;
			onConnect();
			return true;
		}
		void disconnect() {
			if(!isConnect())
				return;
			mTickTask.stop();

			mClientState = STATE_DISCONNECT;
			mIsConnect = false;
			mNetSys->disconnect();
			notify("ok",NOTIFY_DISCONNECT);

			GNET_RECEIVE_UNREGISTER(mNetSys,"tick#response");
<<<<<<< HEAD
=======
			GNET_RECEIVE_UNREGISTER(mNetSys,"ping#response");
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		}

		virtual void onBreak() {}
		virtual void onConnect() {}
		virtual void onUpdate(float dt) {
			if(isConnect()) {
				mNetSys->update(dt);

<<<<<<< HEAD
				mNextTickTime -= dt;
				if(mNextTickTime<=0)
				{
					mNextTickTime = NetInfo::TICK_TIME;
					//mNetSys->tick();
				}

=======
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
				mWaitTickTime -= dt;
				if(mWaitTickTime<=0) {
					mWaitTickTime = 0;
					notify("timeout",NOTIFY_TIMEOUT);
					disconnect();

					mClientState = STATE_BREAK;
					onBreak();
				}
			}
		}
		void reconnect(const Task &task) {
			if(isConnect()||state()==STATE_CONNECTING)
				return;
			mConnectTask.set(task);
			mConnectTask.start();
		}
<<<<<<< HEAD
=======
		void ping() {
			if(mIsWaitingPing)
				return;
			gnet::Item<gnet::Tuple> *ping = new gnet::Item<gnet::Tuple>();
			ping->appendItem(new gnet::Item<gnet::Atom>("ping"));
			ping->appendItem(new gnet::Item<gnet::uint64>(uint64(Base::TimeUtil::currentTime())));
			mNetSys->send(ping);
			mIsWaitingPing = true;
		}
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87

	protected:
		void _gnet_tick(divanet::GPacket *packet) {
			mWaitTickTime = NetInfo::TIME_OUT;
		}
<<<<<<< HEAD
=======
		void _gnet_ping_response(divanet::GPacket *packet) {
			mIsWaitingPing = false;
			notify("ping", NOTIFY_PING_RESPONSE, packet);
		}
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		void _setLogin(bool login) {
			mIsLogin = login;
		}
		void tick_thread() {
			while(isConnect()) {
				mNetSys->tick();
				Base::TimeUtil::mSleep(NET_INFO.TICK_TIME*1000);
			}
		}
		bool connect_thread() {
			int originState = state();
			mClientState = STATE_CONNECTING;
			if(!isConnect())
				if(connect())
					return true;
			mClientState = originState;
			return false;
		}

	protected:
		ThreadTask mTickTask;
		ThreadTask mConnectTask;
<<<<<<< HEAD
		float mNextTickTime;
		float mWaitTickTime;
=======
		float mWaitTickTime;
		bool mIsWaitingPing;
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		bool mIsConnect;
		bool mIsLogin;
		uint32 mClientState;
		NetworkPtr mNetSys;
	};
}

#endif