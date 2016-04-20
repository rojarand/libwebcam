#pragma once

#include "indexable.h"

namespace webcam
{

	class vendor_model_indices
	{
	public:
		vendor_model_indices(const indexable & vendor_indexable_, const indexable & model_indexable_);
		~vendor_model_indices();

		const indexable & get_vendor_id()const;
		const indexable & get_model_id()const;
		
	private:
		indexable vendor_indexable;
		indexable model_indexable;
	};

}