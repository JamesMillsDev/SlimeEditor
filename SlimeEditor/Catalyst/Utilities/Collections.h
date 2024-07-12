#pragma once

#include <xutility>
#include <map>

#include "Catalyst/Types.h"

using std::reverse_iterator;
using std::map;

namespace Catalyst::Utilities
{
	class Collections
	{
	public:
		template<class COLLECTION, typename T>
		static bool All(COLLECTION _collection, bool(*_predicate)(T))
		{
			bool output = true;

			for (T& t : _collection)
			{
				output &= _predicate(t);
			}

			return output;
		}

		template<class COLLECTION, typename T>
		static bool Any(COLLECTION _collection, bool(*_predicate)(T))
		{
			bool output = false;

			for (T& t : _collection)
			{
				output |= _predicate(t);
			}

			return output;
		}

		template<class COLLECTION, typename T>
		static COLLECTION Where(COLLECTION _collection, bool(*_predicate)(T))
		{
			COLLECTION output;

			for (T& t : _collection)
			{
				if (_predicate(t))
					output.emplace_back(t);
			}

			return output;
		}

		template<class COLLECTION, typename T>
		static T First(COLLECTION _collection, bool(*_predicate)(T))
		{
			for (T& t : _collection)
			{
				if (_predicate(t))
					return t;
			}

			return 0;
		}

		template<class COLLECTION, typename T>
		static T Last(COLLECTION _collection, bool(*_predicate)(T))
		{
			for (reverse_iterator iter = _collection.rbegin(); iter != _collection.rend(); ++iter)
			{
				if (_predicate(*iter))
					return *iter;
			}

			return 0;
		}

		template<class COLLECTION>
		static COLLECTION Reverse(COLLECTION _collection)
		{
			COLLECTION output;

			for (reverse_iterator iter = _collection.rbegin(); iter != _collection.rend(); ++iter)
			{
				output.emplace_back(*iter);
			}

			return output;
		}

		template<class OUTPUT, class COLLECTION, typename T, typename S>
		static OUTPUT Select(COLLECTION _collection, S(*_predicate)(T))
		{
			OUTPUT output;

			for (T& t : _collection)
			{
				output.emplace_back(_predicate(t));
			}

			return output;
		}

		template<class COLLECTION, typename T>
		static int Count(COLLECTION _collection, bool(*_predicate)(T))
		{
			int output = 0;

			for (T& t : _collection)
			{
				output += _predicate(t);
			}

			return output;
		}

		template<typename T, class COLLECTION>
		static T ElementAt(COLLECTION _collection, uint _index)
		{
			auto iter = _collection.begin();
			std::advance(iter, _index);

			return *iter;
		}

		template<class COLLECTION, typename KEY, typename VALUE>
		static COLLECTION Values(map<KEY, VALUE> _map)
		{
			COLLECTION output;

			for (auto& [key, value] : _map)
				output.emplace_back(value);

			return output;
		}

		template<class COLLECTION, typename KEY, typename VALUE>
		static COLLECTION Keys(map<KEY, VALUE> _map)
		{
			COLLECTION output;

			for (auto& [key, value] : _map)
				output.emplace_back(key);

			return output;
		}

		template<class COLLECTION>
		static COLLECTION Take(COLLECTION _collection, uint _count)
		{
			if (_count >= _collection.size())
				return _collection;

			COLLECTION output;

			for (uint i = 0; i < _count; ++i)
			{
				auto iter = _collection.begin();
				std::advance(iter, i);

				output.emplace_back(*iter);
			}

			return output;
		}

		template<class COLLECTION>
		static COLLECTION TakeWhile(COLLECTION _collection, bool(*_predicate)())
		{
			COLLECTION output;

			for (uint i = 0; i < _collection.size(); ++i)
			{
				if (!_predicate())
					break;

				auto iter = _collection.begin();
				std::advance(iter, i);

				output.emplace_back(*iter);
			}

			return output;
		}

		template<class COLLECTION, typename T>
		static void ForEach(COLLECTION _collection, void(*_predicate)(T, uint))
		{
			for (uint i = 0; i < _collection.size(); ++i)
			{
				auto iter = _collection.begin();
				std::advance(iter, i);

				_predicate(*iter, i);
			}
		}

	};
}
