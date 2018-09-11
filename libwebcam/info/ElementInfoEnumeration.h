#pragma once

#include <vector>

namespace webcam
{
	template<typename E>
	class ElementInfoEnumeration
	{
	public:
		void put(const E & e_){

			elements.push_back(e_);
		}
		const E & get(std::size_t index_)const{

			return elements.at(index_);
		}
		std::size_t count()const{
			
			return elements.size();
		}
		
		bool is_empty()const{
		
			return elements.is_empty();
		}
	protected:
		std::vector<E> elements;
	};

}
