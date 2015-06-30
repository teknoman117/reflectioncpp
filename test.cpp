#include <iostream>
#include <string>
#include <map>

#include <boost/thread/shared_mutex.hpp>

#include "common.hpp"
#include "type.hpp"
#include "variant.hpp"

using namespace reflectioncpp;

class MetaType
{
public:
	virtual std::string Name() const = 0;
	virtual size_t Size() const = 0;
	virtual TypeCode Identifier() const = 0;
};

template <typename T>
class MetaTypeImpl : public MetaType
{
public:
	size_t Size() const override
	{
		return sizeof(T);
	}

	std::string Name() const override
	{
		return Type<T>::GetName();
	}

	TypeCode Identifier() const override
	{
		return Type<T>::GetTypeCode();
	}
};

class MetaTypeDatabase
{
	std::map<TypeCode, MetaType *> database;
	boost::shared_mutex databaseAccess;

	static MetaTypeDatabase& Instance()
	{
		static MetaTypeDatabase instance;
		return instance;
	}

public:
	static void RegisterType(MetaType *type)
	{
		boost::unique_lock<boost::shared_mutex> lock(Instance().databaseAccess);
		Instance().database[type->Identifier()] = type;
	}

	static MetaType* GetType(TypeCode typeId)
	{
		boost::shared_lock<boost::shared_mutex> lock(Instance().databaseAccess);
		return Instance().database[typeId];
	}

	template <class T>
	static MetaType* Get()
	{
		using RootType = typename reflectioncpp::TypeUtility<T>::Type;

		boost::shared_lock<boost::shared_mutex> lock(Instance().databaseAccess);
		return Instance().database[RootType::GetTypeCode()];
	}
};

struct Herp
{
	int a;
	char b;
	float c;
};
EXPORT_TYPEINFO(Herp, Herp);
EXPORT_TYPEINFO(std::string, std_string);

int main (int argc, char** argv)
{
	reflectioncpp::Variant a;
	a.Set(5);
	int f = 3;
	int* g = &f;
	a = g;

	reflectioncpp::Variant b;
	b.Set(std::string("hello world"));

	MetaTypeDatabase::RegisterType(new MetaTypeImpl<Herp>);
	MetaTypeDatabase::RegisterType(new MetaTypeImpl<int>);

	MetaType *metadata = MetaTypeDatabase::Get<Herp>();
	MetaType *integer = MetaTypeDatabase::Get<int>();

	std::cout << Type<int>::GetName() << ": " << Type<int>::GetTypeCode() << std::endl;

	std::cout << integer->Name() << ": " << integer->Identifier() << std::endl;

	std::cout << metadata->Name() << ": " << metadata->Identifier() << std::endl;

	return 0;
}
