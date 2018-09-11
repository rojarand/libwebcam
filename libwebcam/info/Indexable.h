#pragma once

namespace webcam
{

	class Indexable
	{
	public:
		Indexable(int id_);
		~Indexable();
		int get_id()const;
		void set_id(int id_);
		bool operator==(const Indexable & indexable_)const;
	protected:
		int _id;
	};

}