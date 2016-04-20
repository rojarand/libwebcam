#include "indexable.h"

namespace webcam
{

	indexable::indexable(int id_)
		:_id(id_)
	{
	}


	indexable::~indexable()
	{
	}

	int indexable::get_id()const
	{
		return _id;
	}

	void indexable::set_id(int id_)
	{
		_id = id_;
	}

	bool indexable::operator==(const indexable & indexable_)const
	{
	
		return (_id == indexable_.get_id());
	}
}