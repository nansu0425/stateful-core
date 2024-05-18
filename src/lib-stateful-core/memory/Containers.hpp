// lib-stateful-core/memory/Containers.hpp

#pragma once

namespace StatefulCore
{
	namespace Memory
	{
		template<typename Type, uint32 Size>
		using Array = std::array<Type, Size>;

		template<typename Type>
		using Vector = std::vector<Type, StlAllocator<Type>>;

		template<typename Type>
		using List = std::list<Type, StlAllocator<Type>>;

		template<typename KeyType, typename Type, typename Pred = std::less<KeyType>>
		using TreeMap = std::map<KeyType, Type, Pred, StlAllocator<std::pair<const KeyType, Type>>>;

		template<typename KeyType, typename Pred = std::less<KeyType>>
		using TreeSet = std::set<KeyType, Pred, StlAllocator<KeyType>>;

		template <typename KeyType, typename Type, typename Hasher = std::hash<KeyType>, typename KeyEq = std::equal_to<KeyType>>
		using HashMap = std::unordered_map<KeyType, Type, Hasher, KeyEq, StlAllocator<std::pair<const KeyType, Type>>>;

		template <typename KeyType, typename Hasher = std::hash<KeyType>, typename KeyEq = std::equal_to<KeyType>>
		using HashSet = std::unordered_set<KeyType, Hasher, KeyEq, StlAllocator<KeyType>>;

		template<typename Type>
		using Deque = std::deque<Type, StlAllocator<Type>>;

		template<typename Type, typename Container = Deque<Type>>
		using Queue = std::queue<Type, Container>;

		template<typename Type, typename Container = Deque<Type>>
		using Stack = std::stack<Type, Container>;

		template<typename Type, typename Container = Vector<Type>, typename Pred = std::less<typename Container::value_type>>
		using PriorityQueue = std::priority_queue<Type, Container, Pred>;

		using String = std::basic_string<char, std::char_traits<char>, StlAllocator<char>>;
		using WString = std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t>>;
	}
}
