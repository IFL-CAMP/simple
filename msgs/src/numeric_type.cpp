
#include "simple_msgs\numeric_type.hpp"

namespace simple_msgs {

	
	inline const char* NumericType<int>::getTopic() {
		return IntFbsIdentifier();
	}

	inline const char* NumericType<double>::getTopic() {
		return DoubleFbsIdentifier();
	}

	inline const char* NumericType<float>::getTopic() {
		return FloatFbsIdentifier();
	}
}

