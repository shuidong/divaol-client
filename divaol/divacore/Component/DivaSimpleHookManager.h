/*
 *  DivaSimpleHookManager.h
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_SIMPLE_HOOK_MANAGER
#define DIVA_SIMPLE_HOOK_MANAGER

#include "Core/DivaHookManager.h"
#include "Hook/DivaAutoplay.h"
#include "Hook/DivaCTMode.h"
#include "Hook/DivaPVMode.h"
#include "thread/SoraMutex.h"

namespace divacore
{
	class SimpleHookManager : public HookManager
	{
		typedef std::map<std::string,HookPtr> HOOK_MAP;

		int mHookInfo;
		HookQueue renderHooks;
		HookQueue stateHooks;
		HookQueue inputHooks;
		HookQueue soundHooks;
		HookQueue noteHooks;
		HookQueue hooks;
		HOOK_MAP hookMap;
		sora::SoraMutex mutex;

	public:
		void gameReset()
		{
			LOGGER->msg("Set","SimpleHookManager");
		}

		void gameStop()
		{
			renderHooks.clear();
			stateHooks.clear();
			inputHooks.clear();
			soundHooks.clear();
			noteHooks.clear();
			hooks.clear(true);
			hookMap.clear();
		}

		void update(float dt)
		{
			sora::SoraMutexGuard lock(mutex);
			hooks.update(dt);
		}

		void render()
		{
			sora::SoraMutexGuard lock(mutex);
			hooks.render();
		}

		virtual void insert(HookPtr hook)
		{
			if(hook->hasAbility(Hook::RENDER))
				renderHooks += hook;
			if(hook->hasAbility(Hook::STATE))
				stateHooks += hook;
			if(hook->hasAbility(Hook::INPUT))
				inputHooks += hook;
			if(hook->hasAbility(Hook::SOUND))
				soundHooks += hook;
			if(hook->hasAbility(Hook::NOTE))
				noteHooks += hook;

			mutex.lock();
			hooks += hook;
			mutex.unlock();

			if(hook->getName()!="")
			{
				if(hookMap.find(hook->getName())==hookMap.end())
					hookMap[hook->getName()] = hook;
				else
					DIVA_EXCEPTION_MODULE("Hook "+hook->getName()+" already exists","SimpleHookManager");
			}
		}
		virtual void del(HookPtr hook)
		{
			if(hook==0)
				return;

			if(hook->hasAbility(Hook::RENDER))
				renderHooks -= hook;
			if(hook->hasAbility(Hook::STATE))
				stateHooks -= hook;
			if(hook->hasAbility(Hook::INPUT))
				inputHooks -= hook;
			if(hook->hasAbility(Hook::SOUND))
				soundHooks -= hook;
			if(hook->hasAbility(Hook::NOTE))
				noteHooks -= hook;
			hooks -= hook;
			for(HOOK_MAP::iterator ptr = hookMap.begin(); ptr != hookMap.end(); ptr++)
				if(ptr->second==hook)
				{
					hookMap.erase(ptr);
					break;
				}
		}

		virtual HookPtr getHook(const std::string &name) 
		{
			if(hookMap.find(name)!=hookMap.end())
				return hookMap[name];
			return NULL;
		}

		virtual bool hook(StateEvent &key) 
		{
			for(int i = 0; i < stateHooks.size(); i++)
				if(stateHooks[i]->isActive())
					if(stateHooks[i]->hook(key))
					{
						mHookInfo = stateHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(float x, float y, SoraSprite*sprite, const std::string&tag) 
		{
			for(int i = 0; i < renderHooks.size(); i++)
				if(renderHooks[i]->isActive())
					if(renderHooks[i]->hook(x, y, sprite, tag))
					{
						mHookInfo = renderHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(KeyEvent &key) 
		{
			for(int i = 0; i < inputHooks.size(); i++)
				if(inputHooks[i]->isActive())
					if(inputHooks[i]->hook(key))
					{
						mHookInfo = inputHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(const std::string &ID, const std::string &channel, const std::string &tag) 
		{
			for(int i = 0; i < soundHooks.size(); i++)
				if(soundHooks[i]->isActive())
					if(soundHooks[i]->hook(ID,channel,tag))
					{
						mHookInfo = soundHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(NotePtr note)
		{
			for(int i = 0; i < soundHooks.size(); i++)
				if(soundHooks[i]->isActive())
					if(soundHooks[i]->hook(note))
					{
						mHookInfo = noteHooks[i]->hookInfo();
						return true;
					}
					return false;
		}
		int hookInfo() {return mHookInfo;}

		HookPtr createHook(const std::string &hookName)
		{
			if(hookName=="PVMode")
				return new PVMode;
			else if(hookName=="CTMode")
				return new CTMode;
			else if(hookName=="AutoPlay")
				return new AutoPlay;
			DIVA_EXCEPTION_MODULE("Hook "+hookName+" not found!","SimpleHookManager");
			return NULL;
		}
	};
};

#endif