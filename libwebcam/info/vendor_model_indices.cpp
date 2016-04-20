#include "vendor_model_indices.h"

namespace webcam
{

	vendor_model_indices::vendor_model_indices(const indexable & vendor_indexable_, const indexable & model_indexable_)
		:vendor_indexable(vendor_indexable_)
		,model_indexable(model_indexable_)
	{
	}

	vendor_model_indices::~vendor_model_indices()
	{
	}

	const indexable & vendor_model_indices::get_vendor_id()const{

		return vendor_indexable;
	}

	const indexable & vendor_model_indices::get_model_id()const{
		
		return model_indexable;
	}
}