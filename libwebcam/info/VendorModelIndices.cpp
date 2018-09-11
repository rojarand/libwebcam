#include "VendorModelIndices.h"

namespace webcam
{

	VendorModelIndices::VendorModelIndices(const Indexable & vendor_indexable_, const Indexable & model_indexable_)
		:vendor_indexable(vendor_indexable_)
		,model_indexable(model_indexable_)
	{
	}

	VendorModelIndices::~VendorModelIndices()
	{
	}

	const Indexable & VendorModelIndices::get_vendor_id()const{

		return vendor_indexable;
	}

	const Indexable & VendorModelIndices::get_model_id()const{
		
		return model_indexable;
	}
}