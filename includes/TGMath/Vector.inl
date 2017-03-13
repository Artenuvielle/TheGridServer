template <class  ValueTypeT, UInt32 SizeI>
const Vector<ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::Null;

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>::Vector() {
	_values = std::vector<ValueTypeT>(SizeI);
	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] = ValueTypeT(0);
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>::Vector(const Vector &source) {
	_values = std::vector<ValueTypeT>(SizeI);
	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] = source._values[i];
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>::Vector(const ValueTypeT rVal1) {
	_values = std::vector<ValueTypeT>(SizeI);
	_values[0] = rVal1;

	for(UInt32 i = 1; i < SizeI; i++) {
		_values[i] = ValueTypeT(0);
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>::Vector(const ValueTypeT rVal1, const ValueTypeT rVal2) {
	_values = std::vector<ValueTypeT>(SizeI);
	_values[0] = rVal1;
	_values[1] = rVal2;

	for(UInt32 i = 2; i < SizeI; i++) {
		_values[i] = ValueTypeT(0);
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>::Vector(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3) {
	_values = std::vector<ValueTypeT>(SizeI);
	_values[0] = rVal1;
	_values[1] = rVal2;
	_values[2] = rVal3;

	for(UInt32 i = 3; i < SizeI; i++) {
		_values[i] = ValueTypeT(0);
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>::Vector(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3, const ValueTypeT rVal4) {
	_values = std::vector<ValueTypeT>(SizeI);
	_values[0] = rVal1;
	_values[1] = rVal2;
	_values[2] = rVal3;
	_values[3] = rVal4;

	for(UInt32 i = 4; i < SizeI; i++) {
		_values[i] = ValueTypeT(0);
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::setValues(const ValueTypeT rVal1) {
	_values[0] = rVal1;
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::setValues(const ValueTypeT rVal1, const ValueTypeT rVal2) {
	_values[0] = rVal1;
	_values[1] = rVal2;
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::setValues(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3) {
	_values[0] = rVal1;
	_values[1] = rVal2;
	_values[2] = rVal3;
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::setValues(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3, const ValueTypeT rVal4) {
	_values[0] = rVal1;
	_values[1] = rVal2;
	_values[2] = rVal3;
	_values[3] = rVal4;
}


/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>::~Vector() {
	//delete [] _values;
}

/*-------------------------------------------------------------------------*/
/*                          Common Math                                    */

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::x() const {
	return _values[0];
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::y() const {
	return _values[1];
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::z() const {
	return _values[2];
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::getSeparateValues(ValueTypeT &x) const {
	x = _values[0];
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::getSeparateValues(ValueTypeT &x, ValueTypeT &y) const {
	x = _values[0];
	y = _values[1];
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::getSeparateValues(ValueTypeT &x, ValueTypeT &y, ValueTypeT &z) const {
	x = _values[0];
	y = _values[1];
	z = _values[2];
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::getSeparateValues(ValueTypeT &x, ValueTypeT &y, ValueTypeT &z, ValueTypeT &w) const {
	x = _values[0];
	y = _values[1];
	z = _values[2];
	w = _values[3];
}

template <class ValueTypeT, UInt32 SizeI> inline
bool Vector<ValueTypeT, SizeI>::isZero() const {
	for(UInt32 i = 0; i < SizeI; i++) {
		if (_values[i] != ValueTypeT(0)) {
			return false;
		}
	}

	return true;
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::setZero() {
	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] = ValueTypeT(0);
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::length() const {
	return sqrt(squareLength());
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::squareLength() const {
	ValueTypeT rTmpVal = _values[0] * _values[0];

	for(UInt32 i = 1; i < SizeI; i++) {
		rTmpVal += _values[i] * _values[i];
	}

	return rTmpVal;
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::normalize() {
	ValueTypeT rLength = length();

	if(abs(rLength) < ValueTypeT(0.0001)) {
		rLength = ValueTypeT(1);
	} else {
		rLength = ValueTypeT(1) / rLength;
	}

	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] *= rLength;
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::cross(const Vector &vec) const {
	Vector<ValueTypeT, SizeI> returnValue;

	if(SizeI >= 3) {
		returnValue[0] = _values[1] * vec._values[2] - _values[2] * vec._values[1];
		returnValue[1] = _values[2] * vec._values[0] - _values[0] * vec._values[2];
		returnValue[2] = _values[0] * vec._values[1] - _values[1] * vec._values[0];
	}

	return returnValue;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::operator % (const Vector &vec) const {
	return this->cross(vec);
}

template <class ValueTypeT, UInt32 SizeI> inline
void Vector<ValueTypeT, SizeI>::crossThis(const Vector &vec)
{
	if(SizeI >= 3)
	{
		ValueTypeT rTmp[2];

		rTmp[0] = _values[1] * vec._values[2] - _values[2] * vec._values[1];
		rTmp[1] = _values[2] * vec._values[0] - _values[0] * vec._values[2];

		_values[2] = _values[0] * vec._values[1] - _values[1] * vec._values[0];

		_values[0] = rTmp[0];
		_values[1] = rTmp[1];
	}
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::dot(const Vector &vec) const {
	ValueTypeT rTmpVal = _values[0] * vec._values[0];

	for(UInt32 i = 1; i < SizeI; i++) {
		rTmpVal += _values[i] * vec._values[i];
	}

	return rTmpVal;
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::operator * (const Vector &vec) const {
	return this->dot(vec);
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT  Vector<ValueTypeT, SizeI>::enclosedAngle(const Vector &vec) const {
	ValueTypeT returnValue;

	if(isZero() || vec.isZero()) {
		returnValue = ValueTypeT(0);
	} else {
		returnValue  = dot(vec);
		returnValue /= (length() * vec.length());

		if((returnValue - ValueTypeT(0.0001)) < -1.) {
			returnValue = tg_math::Pi;
		} else if((returnValue + ValueTypeT(0.0001)) > 1.) {
			returnValue = ValueTypeT(0);
		} else {
			returnValue = acos(returnValue);
		}
	}

	return returnValue;
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::projectTo(const Vector &toVec) {
	ValueTypeT rDot       = this ->dot(toVec);
	ValueTypeT rSquareDot = toVec .dot(toVec);

	if(rSquareDot > ValueTypeT(0.0001)) {
		rDot /= rSquareDot;

		if(abs(rDot) > ValueTypeT(0.0001)) {
			*this  = toVec;
			*this *= rDot;
		} else {
			this->setZero();
			rDot = ValueTypeT(0);
		}
	} else {
		rDot = ValueTypeT(1);
	}

	return rDot;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::reflect(const Vector &Normal) const {
   Vector<ValueTypeT, SizeI> result;

   const ValueTypeT dot_prod(2.0f * this->dot(Normal));
   result = *this - dot_prod * Normal;

   return result;
}

/*-------------------------------------------------------------------------*/
/*                                Math                                     */

template <class ValueTypeT, UInt32 SizeI> inline
Vector <ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::operator -(const Vector &vec) const {
	Vector<ValueTypeT, SizeI> returnValue;

	for(UInt32 i = 0; i < SizeI; i++) {
		returnValue[i] = _values[i] - vec[i];
	}

	return returnValue;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector <ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::operator +(const Vector &vec) const {
	Vector<ValueTypeT, SizeI> returnValue;

	for(UInt32 i = 0; i < SizeI; i++) {
		returnValue[i] = _values[i] + vec[i];
	}

	return returnValue;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector <ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::operator *(const ValueTypeT rVal) const {
	Vector<ValueTypeT, SizeI> returnValue;

	for(UInt32 i = 0; i < SizeI; i++) {
		returnValue[i] = _values[i] * rVal;
	}

	return returnValue;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector <ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::operator /(const ValueTypeT rVal) const {
	Vector<ValueTypeT, SizeI> returnValue;

	for(UInt32 i = 0; i < SizeI; i++) {
		returnValue[i] = _values[i] / rVal;
	}

	return returnValue;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>& Vector<ValueTypeT, SizeI>::operator -=(const Vector &vec) {
	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] = _values[i] - vec[i];
	}

	return *this;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>& Vector<ValueTypeT, SizeI>::operator +=(const Vector &vec) {
	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] = _values[i] + vec[i];
	}

	return *this;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>& Vector<ValueTypeT, SizeI>::operator *=(const ValueTypeT rVal) {
	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] = _values[i] * rVal;
	}

	return *this;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI>& Vector<ValueTypeT, SizeI>::operator /=(const ValueTypeT rVal) {
	for(UInt32 i = 0; i < SizeI; i++) {
		_values[i] = _values[i] / rVal;
	}

	return *this;
}

template <class ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI> Vector<ValueTypeT, SizeI>::operator -() const {
	Vector<ValueTypeT, SizeI> returnValue;

	for(UInt32 i = 0; i < SizeI; i++) {
		returnValue._values[i] = - _values[i];
	}

	return returnValue;
}

/*-------------------------------------------------------------------------*/
/*                             Comparison                                  */

template <class ValueTypeT, UInt32 SizeI> inline
bool Vector<ValueTypeT, SizeI>::operator < (const Vector &other) const {
	bool ret = false;

	for(UInt32 i = 0; i < SizeI; i++) {
		if(_values[i] > other._values[i]) {
			break;
		}
		if(_values[i] < other._values[i]) {
			ret = true;
			break;
		}
	}

	return ret;
}

template <class ValueTypeT, UInt32 SizeI> inline
bool Vector<ValueTypeT, SizeI>::operator ==(const Vector &other) const {
	for(UInt32 i = 0; i < SizeI; i++) {
		if(_values[i] - ValueTypeT(0.0001) > other._values[i] || _values[i] + ValueTypeT(0.0001) < other._values[i]) {
			return false;
		}
	}
	return true;
}

template <class ValueTypeT, UInt32 SizeI> inline
bool Vector<ValueTypeT, SizeI>::operator !=(const Vector &other) const {
	return ! (*this == other);
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT& Vector<ValueTypeT, SizeI>::operator [](int idx) {
	return _values[idx];
}

template <class ValueTypeT, UInt32 SizeI> inline
ValueTypeT Vector<ValueTypeT, SizeI>::operator [](int idx) const {
	return _values[idx];
}

template <class  ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI> operator *(const ValueTypeT val, const Vector<ValueTypeT, SizeI> &vec) {
	Vector<ValueTypeT, SizeI> returnValue;

	for(UInt32 i = 0; i < SizeI; i++)
	{
		returnValue[i] = vec[i] * val;
	}

	return returnValue;
}

template <class  ValueTypeT, UInt32 SizeI> inline
std::ostream& operator <<(std::ostream& out, const Vector<ValueTypeT, SizeI> &vec) {
	out << "Vec" << SizeI << "(";

	for(UInt32 i = 0; i < SizeI; i++)
	{
		out << vec[i];
		if (i + 1 < SizeI) {
			out << ", ";
		} else {
			out << ")";
		}
	}
	return out;
}
