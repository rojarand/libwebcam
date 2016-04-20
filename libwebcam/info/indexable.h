#pragma once

namespace webcam
{

	class indexable
	{
	public:
		indexable(int id_);
		~indexable();
		int get_id()const;
		void set_id(int id_);
		bool operator==(const indexable & indexable_)const;
	protected:
		int _id;
	};

}