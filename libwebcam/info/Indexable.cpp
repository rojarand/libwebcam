#include "Indexable.h"

namespace webcam
{

	Indexable::Indexable(int id_)
		:_id(id_)
	{
	}


	Indexable::~Indexable()
	{
	}

	int Indexable::get_id()const
	{
		return _id;
	}

	void Indexable::set_id(int id_)
	{
		_id = id_;
	}

	bool Indexable::operator==(const Indexable & indexable_)const
	{
	
		return (_id == indexable_.get_id());
	}
}