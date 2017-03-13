#ifndef _Vector_H_
#define _Vector_H_

#include <vector>
#include <iostream>
#include "TGMath/MathTypes.h"

template <class ValueTypeT, UInt32 SizeI>
class Vector
{
public:
	static const Vector Null;
	
	Vector();
	Vector(const Vector &source);

	Vector(const ValueTypeT rVal1);
	Vector(const ValueTypeT rVal1, const ValueTypeT rVal2);
	Vector(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3);
	Vector(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3, const ValueTypeT rVal4);
      
	~Vector();

	ValueTypeT x() const;
	ValueTypeT y() const;
	ValueTypeT z() const;
	
	void getSeparateValues(ValueTypeT &x) const;
	void getSeparateValues(ValueTypeT &x, ValueTypeT &y) const;
	void getSeparateValues(ValueTypeT &x, ValueTypeT &y, ValueTypeT &z) const;
	void getSeparateValues(ValueTypeT &x, ValueTypeT &y, ValueTypeT &z, ValueTypeT &w) const;

	void setValues(const ValueTypeT rVal1);
	void setValues(const ValueTypeT rVal1, const ValueTypeT rVal2);
	void setValues(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3);
	void setValues(const ValueTypeT rVal1, const ValueTypeT rVal2, const ValueTypeT rVal3, const ValueTypeT rVal4);
	
	bool isZero() const;
	void setZero();
	ValueTypeT length() const;
	ValueTypeT squareLength() const;

	void normalize();

	Vector cross(const Vector &vec) const;
	Vector operator %(const Vector &vec) const;

	void crossThis(const Vector &vec);

	ValueTypeT dot(const Vector &vec) const;
	ValueTypeT operator *(const Vector &vec) const;
	
	ValueTypeT enclosedAngle(const Vector &vec) const;

	ValueTypeT projectTo (const Vector &toVec);

	Vector reflect(const Vector &Normal) const;

	Vector operator -(const Vector &vec) const;
	Vector operator +(const Vector &vec) const;

	Vector operator *(const ValueTypeT val) const;
	Vector operator /(const ValueTypeT val) const;

	Vector& operator -=(const Vector &vec);
	Vector& operator +=(const Vector &vec);

	Vector& operator *=(const ValueTypeT val);
	Vector& operator /=(const ValueTypeT val);

	Vector operator -() const;

	bool operator < (const Vector &other) const;

	bool operator == (const Vector &other) const;
	bool operator != (const Vector &other) const;
	
	ValueTypeT& operator [](int idx);
    ValueTypeT operator [](int idx) const;
protected:
	std::vector<ValueTypeT> _values;
};

template <class  ValueTypeT, UInt32 SizeI> inline
Vector<ValueTypeT, SizeI> operator *(const ValueTypeT val, const Vector<ValueTypeT, SizeI> &vec);

template <class  ValueTypeT, UInt32 SizeI> inline
std::ostream& operator <<(std::ostream& out, const Vector<ValueTypeT, SizeI> &vec);

typedef Vector<Real32, 3> Vec3f;

#include <TGMath\Vector.inl>
#endif