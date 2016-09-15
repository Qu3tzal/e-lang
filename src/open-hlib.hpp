#ifndef CPP_HASKELL_LIB_REPRODUCTION
#define CPP_HASKELL_LIB_REPRODUCTION

/*
	The MIT License (MIT)

	Copyright (c) 2013-2014 Maxime Alvarez

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


	Version : 0.0.3

	Haskel standard lib reproduction in C++ (a try at least..). Lists are represented by std::vector.

	Implememented :
		Functions on numbers :
			succ
			pred
			odd

		Functions on ord-objects :
			max
			min

		Function on std::vector :
			concat					= xs ++ xxs		where xs and xxs are lists.
			push_front				= x:xs			where xs is a list.
			push_back				= xs:x			where xs is a list.
			at						= xs !! i		where xs is a list and i an index.
			head
			last
			tail
			init
			length
			null
			reverse
			maximum
			minimum
			sum
			product
			take
			drop
			elem
			zip

		Classes :
			Pair					= (x, y)		tuple of size 2.

		Functions on Pair objects :
			fst
			snd

*/

#include <vector>
#include <iostream>

namespace hlib
{
	template <typename T, typename C>
	class Pair;

	/**
	 * Num
	 */

	double succ(double x)
	{
		return (x + 1);
	}

	double pred(double x)
	{
		return (x - 1);
	}

	bool odd(int x)
	{
		return (x % 2 == 0);
	}

	/**
	 * Ord
	 */

	template<typename T>
	T max(T x, T y)
	{
		if(x >= y)
			return x;

		return y;
	}

	template<typename T>
	T min(T x, T y)
	{
		if(x <= y)
			return x;

		return y;
	}

	/**
	 * std::vector
	 */

	template<typename T>
	void concat(std::vector<T>& x, std::vector<T> y)
	{
		for(unsigned int i(0) ; i < y.size() ; i++)
			x.push_back(y.at(i));
	}

	template<typename T>
	void push_front(T element, std::vector<T>& x)
	{
		std::vector<T> buf(x);

		x.clear();
		x.push_back(element);

		concat(x, buf);
	}

	template<typename T>
	void push_back(T element, std::vector<T>& x)
	{
		x.push_back(element);
	}

	template<typename T>
	T at(std::vector<T>& x, unsigned int i, T error_value)
	{
		if(i >= 0 && i < x.size())
			return x.at(i);
		else
			return error_value;
	}

	template<typename T>
	T head(const std::vector<T>& x)
	{
		return x.at(0);
	}

	template<typename T>
	T last(const std::vector<T>& x)
	{
		if(x.size() > 0)
			return x.at(x.size() - 1);

		return x.at(0);
	}

	template<typename T>
	std::vector<T> tail(const std::vector<T>& x)
	{
		std::vector<T> empty_base;

		if(x.size() > 1)
			for(unsigned int i(1) ; i < x.size() ; i++)
				empty_base.push_back(x.at(i));

		return empty_base;
	}

	template<typename T>
	std::vector<T> init(const std::vector<T>& x)
	{
		std::vector<T> empty_base;

		if(x.size() > 1)
			for(unsigned int i(0) ; i < x.size() - 1 ; i++)
				empty_base.push_back(x.at(i));

		return empty_base;
	}

	template<typename T>
	size_t length(const std::vector<T>& x)
	{
		return x.size();
	}

	template<typename T>
	bool null(const std::vector<T>& x)
	{
		return x.empty();
	}

	template<typename T>
	std::vector<T> reverse(const std::vector<T>& x)
	{
		std::vector<T> reversed;

		if(x.size() > 0)
		{
			for(unsigned int i(x.size() - 1) ; i > 0 ; i--)
				reversed.push_back(x.at(i));

			reversed.push_back(x.at(0));
		}

		return reversed;
	}

	template<typename T>
	T maximum(const std::vector<T>& x)
	{
		T maximum(x.at(0));

		for(unsigned int i(0) ; i < x.size() ; i++)
			maximum = max<T>(x.at(i), maximum);

		return maximum;
	}

	template<typename T>
	T minimum(const std::vector<T>& x)
	{
		T minimum(x.at(0));

		for(unsigned int i(0) ; i < x.size() ; i++)
			minimum = min<T>(x.at(i), minimum);

		return minimum;
	}

	template<typename T>
	T sum(const std::vector<T>& x, T lastValue = 0)
	{
		if(null(x))
			return lastValue;

		return head<T>(x) + sum<T>(tail<T>(x));
	}

	template<typename T>
	T product(const std::vector<T>& x, T lastValue = 1)
	{
		if(null(x))
			return lastValue;

		return head<T>(x) * product<T>(tail<T>(x));
	}

	template<typename T>
	std::vector<T> take(int numberOfItemsRequired, const std::vector<T>& x)
	{
		std::vector<T> empty_base;

		if(numberOfItemsRequired <= 0)
			return empty_base;

		empty_base.push_back(head<T>(x));

		concat(empty_base, take(numberOfItemsRequired - 1, tail<T>(x)));
		return empty_base;
	}

	template<typename T>
	std::vector<T> drop(int numberOfItemsToDrop, const std::vector<T>& x)
	{
		if(numberOfItemsToDrop == 0)
			return x;

		return drop(numberOfItemsToDrop - 1, tail<T>(x));
	}

	template<typename T>
	bool elem(T element, const std::vector<T>& x)
	{
		for(unsigned int i(0) ; i < x.size() ; i++)
			if(x.at(i) == element)
				return true;

		return false;

		/*
			Recursive way :

			if(null(x))
				return false;
			else if(head<T>(x) == element)
				return true;
			else
				return elem(element, tail<T>(x));
		*/
	}

	template<typename T, typename C>
	std::vector<Pair<T, C> > zip(const std::vector<T>& x, const std::vector<C>& y)
	{
		std::vector<Pair<T, C> > empty_base;

		for(unsigned int i(0) ; i < x.size() && i < y.size() ; i++)
			empty_base.push_back(Pair<T, C>(x.at(i), y.at(i)));

		return empty_base;
	}

	/**
	 * Pair
	 */

	template<typename T, typename C>
	class Pair
	{
		public:
			Pair(T x, C y) : first(x), second(y)
			{}

			T first;
			C second;
	};

	template<typename T, typename C>
	T fst(Pair<T, C> x)
	{
		return x.first;
	}

	template<typename T, typename C>
	C snd(Pair<T, C> x)
	{
		return x.second;
	}
} // hlib namespace.

#endif // CPP_HASKELL_LIB_REPRODUCTION
