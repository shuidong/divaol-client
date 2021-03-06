/****************************************************
 *  Path.cpp
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of funs that deal with path
 ****************************************************/

#include "Path.h"

#include <fstream>

namespace Base
{
	Path Path::GetEnvironment(const String &env) {
		DWORD len = GetEnvironmentVariableW(env.unicode_str(), 0, 0);
		if (len != 0) {
			wchar* buffer = new wchar[len];
			GetEnvironmentVariableW(env.unicode_str(), buffer, len);
			String result(buffer);
			delete [] buffer;
			return result;
		}
		return "./";
	}
	Path Path::GetApplicationPath(bool full) {
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetApplicationPath(full);
#endif
		return "./";
	}

	Path Path::GetFontPath() {	
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetFontPath();
#endif
		return "./";
	}

	Path Path::GetCurrentPath() {
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetCurrentPath();
#endif
		return "./";
	}
	Path Path::GetHome() {
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetHome();
#endif
		return "./";
	}
	Path Path::GetRoot() {
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetRoot();
#endif
		return "./";
	}
	Path Path::GetTempPath() {
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetTempPath();
#endif
		return "./";
	}
	Path Path::GetWrittablePath() {
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetWrittablePath();
#endif
		return "./";
	}
	Path Path::GetResourcePath() {
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetResourcePath();
#endif
		return "./";
	}
	std::vector<String> Path::GetDriveList()
	{
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::GetDriveList();
#endif
		return std::vector<String>();
	}
	bool Path::SetCurrentPath(const String &path)
	{
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::SetCurrentPath(path);
#endif
		return false;
	}
	Path Path::ExpandPath(const String& path)
	{
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::ExpandPath(path);
#endif
		return "./";
	}
	String Path::FormatPath(const String &path) {
		String::IndexVar index = 0;
		String ret = "";
		bool flag = false;
		while(index<path.size())
		{
			if(path[index]=='\\' || path[index] =='/')
			{
				if(!flag)
					ret += '/', flag = true;
			}
			else
				ret += path[index], flag = false;
			index++;
		}
		return ret;
	}
	String Path::GetFileName(const String &path, bool hasExt) {
		String::IndexVar index = path.size()-1;
		while(index>=0)
		{
			if(path[index]==  L'\\' || path[index] == L'/')
				break;
			index--;
		}
		String filename = path(index+1,-1);
		if(!hasExt) {
			if((index=filename.rfind('.'))!=String::npos)
				filename = filename(0,index);
		}
		return filename;
	}
	Path Path::GetFilePath(const String &path) {
		String::IndexVar index = path.size()-1;
		while(index>0)
		{
			if(path[index]==  L'\\' || path[index] == L'/')
				break;
			index--;
		}
		String filePath = path(0,index);
		if(filePath.empty())
			filePath = "./";
		return filePath;
	}
	Path Path::GetFolderPath(const String &path) {
		if(CheckIsFolder(path))
			return path;
		else
			return GetFilePath(path);
	}
	Path Path::CombinePath(const Path &a, const Path &b) {
		return a.mPath + "/" + b.mPath;
	}
	String Path::GetExtension(const String &path) {
		String filename = GetFileName(path);
		String::IndexVar pos;
		if((pos=filename.rfind('.'))!=String::npos)
			return filename(pos+1,-1);
		return "";
	}
	Path Path::GetFullPath(const String &path) {
		if(CheckAbsolutePath(path))
			return path;
		else
			return CombinePath(GetCurrentPath(),path);
	}
	String Path::GetPathRoot(const Path &path)
	{
		const String data = path;
		if(data.find(":")==1)
			return data(0,2)+"/";
		else if(data[0]=='/' || data[0]=='~')
			return data[0];
		else
			return "";
	}
	Path Path::ChangeExtension(const Path &path, const String &ext)
	{
		return GetFilePath(path).str() + GetFileName(path,false) + "." + ext;
	}
	Path Path::PickoutFinalSlash(const String &path)
	{
		String newPath = Path(path).str();
		if(!newPath.empty()&&newPath[newPath.size()-1]=='/')
			newPath = newPath(0,newPath.size()-1);
		return newPath;
	}
	bool Path::CheckAbsolutePath(const Path &path)
	{
		return GetPathRoot(path)!="";
	}
	bool Path::CheckHasEntension(const Path &path)
	{
		return GetExtension(path)!="";
	}
	bool Path::CheckIsFolder(const Path &path)
	{
		Path newPath = PickoutFinalSlash(path);
		return FileUtil::IsFolder(newPath);
	}
	DirectoryIterator Path::beginEnum()
	{
		return DirectoryIterator(folderPath());
	}
	std::vector<Path> Path::EnumFolder(const String &folderPath, bool recursion)
	{
		std::vector<Path> files;
		DirectoryIterator ptr = Path(folderPath).beginEnum();
		while(!ptr.isEnd()) {
			if(!ptr.isFolder())
				files.push_back(ptr.fullPath());
			else if(recursion)
				EnumFolder(ptr.path(), files, recursion);
			++ptr;
		}
		return files;
	}
	std::vector<Path> Path::EnumFolder(const String &folderPath, std::vector<Path> &files, bool recursion)
	{
		DirectoryIterator ptr = Path(folderPath).beginEnum();
		while(!ptr.isEnd()) {
			if(!ptr.isFolder())
				files.push_back(ptr.fullPath());
			else if(recursion)
				EnumFolder(ptr.path(), files, recursion);
			++ptr;
		}
		return files;
	}

	DirectoryIterator::DirectoryIterator() {
		mIsEnd = false;
	}

	DirectoryIterator::DirectoryIterator(const DirectoryIterator& iter):
		mFolderPath(iter.mFolderPath),mFile(iter.mFile),mImpl(iter.mImpl),mIsEnd(iter.mIsEnd) {
	}

	DirectoryIterator::DirectoryIterator(const String& path):
		mFolderPath(path+"/"), mImpl(new DirectoryIteratorImpl(path)), mIsEnd(false) {
		mFile = mImpl->get();
	}

	DirectoryIterator::~DirectoryIterator() {
	}

	DirectoryIterator& DirectoryIterator::operator =(const DirectoryIterator& rhs) {
		if(this != &rhs) {
			mImpl = rhs.mImpl;
		}
		return *this;
	}


	DirectoryIterator& DirectoryIterator ::operator++() {
		if(mImpl) {
			std::string n = mImpl->next();
			if(n.empty())
				mIsEnd = true;
			else {
				mFile = mImpl->get();
			}
		}
		return *this;
	}
}
