/*
 *  gnet.h
 *
 *  Created by tempbuffer on 8/6/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef _GNET_H_
#define _GNET_H_

#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <deque>

namespace gnet
{
	using std::vector;
	using std::string;
	using std::stringstream;

	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
	typedef unsigned short word;
	typedef unsigned int dword;
	typedef unsigned long long qword;
	typedef signed char int8;
	typedef signed int int32;
	typedef signed long long int64; 
	typedef std::vector<byte> Bytes;
	const int TYPE_LENGTH = 4;

	class ItemBase;
	typedef std::vector<ItemBase*> Tuple;
	typedef std::string Binary;
	typedef const char* RawBinary;
	class List {
	public:
		Tuple data;
		List():data() {}
		List(const Tuple &data):data(data) {}
	};
	class Atom {
	public:
		Binary data;
		Atom():data() {}
		Atom(const Binary &data):data(data) {}
	};

	enum ItemType{
		GNET_TYPE_NULL,
		GNET_TYPE_UINT_32,
		GNET_TYPE_INT_32,
		GNET_TYPE_UINT_64,
		GNET_TYPE_INT_64,
		GNET_TYPE_INT_8,
		GNET_TYPE_UINT_8,
		GNET_TYPE_BINARY,
		GNET_TYPE_ATOM,
		GNET_TYPE_TUPLE,
		GNET_TYPE_LIST,
		GNET_TYPE_DOUBLE};


	/*
	* BinaryUtility
	*/
	namespace BinaryUtility
	{
		Bytes operator+(const Bytes &a, const Bytes &b);

		// convertToBytes series	
		Bytes convertToBytes(uint32 data);
		Bytes convertToBytes(int32 data);
		Bytes convertToBytes(uint64 data);
		Bytes convertToBytes(int64 data);
		Bytes convertToBytes(uint8 data);
		Bytes convertToBytes(int8 data);
		Bytes convertToBytes(const Binary &data);
		Bytes convertToBytes(double data);

		// convertToHost series
		template<typename T>
		T convertToHost(Bytes &bytes);
		template<>
		uint8 convertToHost<uint8>(Bytes &bytes);
		template<>
		int8 convertToHost<int8>(Bytes &bytes);
		template<>
		uint32 convertToHost<uint32>(Bytes &bytes);
		template<>
		int32 convertToHost<int32>(Bytes &bytes);
		template<>
		uint64 convertToHost<uint64>(Bytes &bytes);
		template<>
		int64 convertToHost<int64>(Bytes &bytes);
		template<>
		std::string convertToHost<Binary>(Bytes &bytes);
		template<>
		double convertToHost<double>(Bytes &bytes);
	};

	using BinaryUtility::operator+;

	/*
	* TypeID_Traitor
	*/
	template<typename T>
	class TypeID_Traitor
	{
	public:
		static const ItemType type = GNET_TYPE_NULL; 
	};
	template<>
	class TypeID_Traitor<int8>
	{
	public:
		static const ItemType type = GNET_TYPE_INT_8;
	};
	template<>
	class TypeID_Traitor<uint8>
	{
	public:
		static const ItemType type = GNET_TYPE_UINT_8;
	};
	template<>
	class TypeID_Traitor<int32>
	{
	public:
		static const ItemType type = GNET_TYPE_INT_32;
	};
	template<>
	class TypeID_Traitor<uint32>
	{
	public:
		static const ItemType type = GNET_TYPE_UINT_32;
	};
	template<>
	class TypeID_Traitor<int64>
	{
	public:
		static const ItemType type = GNET_TYPE_INT_64;
	};
	template<>
	class TypeID_Traitor<uint64>
	{
	public:
		static const ItemType type = GNET_TYPE_UINT_64;
	};
	template<>
	class TypeID_Traitor<Binary>
	{
	public:
		static const ItemType type = GNET_TYPE_BINARY;
	};
	template<>
	class TypeID_Traitor<RawBinary>
	{
	public:
		static const ItemType type = GNET_TYPE_BINARY;
	};
	template<>
	class TypeID_Traitor<Atom>
	{
	public:
		static const ItemType type = GNET_TYPE_ATOM;
	};
	template<>
	class TypeID_Traitor<Tuple>
	{
	public:
		static const ItemType type = GNET_TYPE_TUPLE;
	};
	template<>
	class TypeID_Traitor<List>
	{
	public:
		static const ItemType type = GNET_TYPE_LIST;
	};
	template<>
	class TypeID_Traitor<double>
	{
	public:
		static const ItemType type = GNET_TYPE_DOUBLE;
	};
	/*template<int ITEM_TYPE>
	class Type_Traitor
	{
	public:
	typedef void type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_INT_8>
	{
	public:
	typedef int8 type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_UINT_8>
	{
	public:
	typedef uint8 type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_INT_32>
	{
	public:
	typedef int32 type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_UINT_32>
	{
	public:
	typedef uint32 type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_INT_64>
	{
	public:
	typedef int64 type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_UINT_64>
	{
	public:
	typedef uint64 type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_BINARY>
	{
	public:
	typedef binary type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_TUPLE>
	{
	public:
	typedef Tuple type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_ATOM>
	{
	public:
	typedef binary type;
	};
	template<>
	class Type_Traitor<GNET_TYPE_LIST>
	{
	public:
	typedef Tuple type;
	};*/

	/*
	* Items
	*/
	class ItemBase
	{
	public:
		virtual ItemType getType() {
			return GNET_TYPE_NULL;
		}

		std::string getHex();
		virtual std::string getDescription() {return "";}
	
		Bytes getItem() {
			return _getType()+_getItem();
		}
		uint64 getUInt();
		int64 getInt();
		double getValue();
		std::string getString();

		template<typename T>
		T* as() {
			return dynamic_cast<T*>(this);
		}
		template<typename T>
		T* asValue() {
			Item<TypeID_Traitor<T>::type> *item = dynamic_cast<Item<TypeID_Traitor<T>::type>*>(this);
			assert(item!=NULL);
			return item->getValue();
		}
	protected:
		Bytes _getType() {return BinaryUtility::convertToBytes(static_cast<uint32>(getType()));}
		virtual Bytes _getItem() = 0;
	};

	template<typename T>
	class Item : public ItemBase
	{
		T mData;
	public:
		Item():T() {}
		Item(T data):mData(data) {}
		Item(Bytes &bytes) {mData = BinaryUtility::convertToHost<T>(bytes);}
		
		std::string getDescription()
		{
			stringstream ss;
			if(sizeof(T)==sizeof(uint8)||sizeof(T)==sizeof(int8))
				ss << '\'' << mData << '\'';
			else
				ss << mData;
			return ss.str();
		}

		T getData() {return mData;}
		ItemType getType() {return TypeID_Traitor<T>::type;}
	protected:
		Bytes _getItem() {
			return BinaryUtility::convertToBytes(mData);
		} 
	};

	template<>
	class Item<Binary> : public ItemBase
	{
		std::string mData;
	public:
		Item():mData() {}
		Item(const std::string &data):mData(data) {}
		Item(Bytes &bytes) {mData = BinaryUtility::convertToHost<Binary>(bytes);}

		std::string getDescription() {return "\""+mData+"\"";}

		std::string getData() {return mData;}
		ItemType getType() {return GNET_TYPE_BINARY;}
	protected:
		Bytes _getItem() {
			return BinaryUtility::convertToBytes(mData);
		}
	};

	template<>
	class Item<RawBinary> : public Item<Binary>{
	public:
		Item() {}
		Item(const std::string &data):Item<Binary>(data) {}
		Item(Bytes &bytes):Item<Binary>(bytes) {}

		ItemType getType() {return GNET_TYPE_BINARY;}
	};

	template<>
	class Item<Atom> : public Item<Binary>{
	public:
		Item() {}
		Item(const std::string &data):Item<Binary>(data) {}
		Item(Bytes &bytes):Item<Binary>(bytes) {}
		Item(const Atom atom):Item<Binary>(atom.data) {} 

		ItemType getType() {return GNET_TYPE_ATOM;}
	};

	template<>
	class Item<Tuple> : public ItemBase
	{
		Tuple mData;
	public:
		Item():mData() {}
		Item(const Tuple &data):mData(data) {}
		~Item() {clear();}

		std::string getDescription() {return _getStrWithBracket('{','}');}

		ItemType getType() {return GNET_TYPE_TUPLE;}

		Tuple getData() const {return mData;}

		ItemBase* getItem(size_t index)
		{
			if(index>=mData.size())
				throw "Index out of range";
			return mData[index];
		}
		void deleteItem(size_t index)
		{
			if(index>=mData.size())
				throw "Index out of range";
			mData.erase(mData.begin()+index);
		}
		void clear(bool isWeak = false)
		{
			if(!isWeak)
				for(Tuple::iterator ptr = mData.begin(); ptr != mData.end(); ptr++)
					delete *ptr;
			mData.clear();
		}
		bool empty() {return mData.empty();}
		size_t size() {return mData.size();}

		template<typename T>
		void appendAhead(T data) {appendAheadItem(new Item<T>(data));}
		template<typename T>
		void append(T data) {appendItem(new Item<T>(data));}
		void appendAheadItem(ItemBase *item) { mData.insert(mData.begin(),item); }
		void appendItem(ItemBase* item) { mData.push_back(item); }
		void removeLast() {if(!mData.empty()){ delete *mData.rbegin(); mData.pop_back();}}
		
		ItemBase* operator[](size_t index) {return getItem(index);}
		template <typename T>
		Item<Tuple>& operator+=(T data) {append<T>(data); return *this;}
		Item<Tuple>& operator--(int) {removeLast(); return *this;}

	protected:
		Bytes _getItem() {
			Bytes bytes = BinaryUtility::convertToBytes(mData.size());
			for(Tuple::iterator ptr = mData.begin(); ptr != mData.end(); ptr++)
				bytes = bytes + (*ptr)->getItem();
			return bytes;
		}

		std::string _getStrWithBracket(char left, char right)
		{
			std::string ret = "";
			ret += left;
			if(!mData.empty())
			{
				ret += (*mData.begin())->getDescription();
				for(Tuple::iterator ptr = mData.begin()+1; ptr != mData.end(); ptr++)
					ret += ", " + (*ptr)->getDescription();
			}
			ret += right;
			return ret;
		}
	};

	template<>
	class Item<List> : public Item<Tuple>
	{
	public:
		Item() {}
		Item(const Tuple &data):Item<Tuple>(data) {}
		Item(const List &list):Item<Tuple>(list.data) {}
		Item(const Item<Tuple> &tuple):Item<Tuple>(tuple.getData()) {}

		std::string getDescription() {return _getStrWithBracket('[',']');}

		ItemType getType() {return GNET_TYPE_LIST;}
	};

	class ItemUtility
	{
	public:
		static Item<Tuple>* formatTuple(const char* format, va_list	ArgPtr);
		static Item<Tuple>* formatTuple(const char* format, ...);
		static void formatReadTuple(Item<Tuple> *tuple, const char *format, va_list ArgPtr);
		static void formatReadTuple(Item<Tuple> *tuple, const char *format, ...);
		static uint64 getUInt(ItemBase *item);
		static int64 getInt(ItemBase *item);
		static double getValue(ItemBase *item);
		static std::string getString(ItemBase *item);
	};

	typedef Item<Tuple> TupleItem;
	typedef Item<List> ListItem;
}

#endif
