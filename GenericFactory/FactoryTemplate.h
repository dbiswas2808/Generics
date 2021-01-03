#pragma once
#include <map>

// ProductIdType should be of type that can be used as key in std::map or a custom map
// Suggest using a flat map for this to help
template <class ProductIdType, class AbstractProduct, class ProductCreator = AbstractProduct* (*)(), 
	template <class, class> class FactoryErrorPolicy, 
	template<class id, class type, class...> class MapType = std::map>
class Factory {
	using AssocMap = MapType<ProductIdType, ProductCreator>;
	using ErrorPolicy = FactoryErrorPolicy<ProductIdType, AbstractProduct>;
	using P_AbstractProduct = std::shared_ptr<AbstractProduct>;

public:
	bool Register(ProductIdType id, ProductCreator func) {
		return assocMap.emplace(id, ProductCreator).second;
	}

	bool unregister(const ProductIdType& id) {
		return assocMap.erase(id) == 1;
	}

	P_AbstractProduct CreateObject(const ProductIdType& id) {
		const auto it = assocMap.find(id);
		if (it != assocMap.cend()) {
			return (*it)();
		}
		
		return unknownIdHandler(id);
	}

private:
	ErrorPolicy unknownTypeIdHandler;
	AssocMap assocMap;
};