#pragma once

#include "Indexable.h"

namespace webcam
{

	class VendorModelIndices
	{
	public:
		VendorModelIndices(const Indexable & vendor_indexable_, const Indexable & model_indexable_);
		~VendorModelIndices();

		const Indexable & get_vendor_id()const;
		const Indexable & get_model_id()const;
		
	private:
		Indexable vendor_indexable;
		Indexable model_indexable;
	};

}