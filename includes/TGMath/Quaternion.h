#ifndef _Quaternion_H_
#define _Quaternion_H_

#include <iostream>
#include <vector>

#include "TGMath/MathTypes.h"
#include "TGMath/Vector.h"

template <class ValueTypeT>
class QuaternionBase
{
	typedef QuaternionBase<ValueTypeT> Self;

public:

	typedef Vec3f VectorType;

	static const UInt32 _uiSize = 4;

	static const QuaternionBase &identity();

	static QuaternionBase slerp(const QuaternionBase &rot0, const QuaternionBase &rot1, const ValueTypeT t);
	static QuaternionBase nlerp(const QuaternionBase &rot0, const QuaternionBase &rot1, const ValueTypeT t);

	static QuaternionBase inverse(const QuaternionBase &val);
	static QuaternionBase log(const QuaternionBase &val);
	static QuaternionBase exp(const QuaternionBase &val);
	static QuaternionBase conj(const QuaternionBase &val);

	QuaternionBase();
	QuaternionBase(const QuaternionBase &source);
	QuaternionBase(const VectorType &axis, const ValueTypeT angle);
	QuaternionBase(const VectorType &rotateFrom, const VectorType &rotateTo);
	QuaternionBase(const ValueTypeT* values);
	QuaternionBase(const ValueTypeT x, const ValueTypeT y, const ValueTypeT z, const ValueTypeT w);

	~QuaternionBase();

	void setIdentity();

	void setValueAsAxisRad(const ValueTypeT *valsP);
	void setValueAsAxisDeg(const ValueTypeT *valsP);
	void setValueAsQuat (const ValueTypeT *valsP);

	void setValueAsAxisRad(const ValueTypeT x, const ValueTypeT y, const ValueTypeT z, const ValueTypeT w);
	void setValueAsAxisDeg(const ValueTypeT x, const ValueTypeT y, const ValueTypeT z, const ValueTypeT w);
	void setValueAsQuat (const ValueTypeT x, const ValueTypeT y, const ValueTypeT z, const ValueTypeT w);

	void setValue(const Self &quat);

	void setValueAsAxisRad(const VectorType &axis, ValueTypeT angle);
	void setValueAsAxisDeg(const VectorType &axis, ValueTypeT angle);

	void setValue(const VectorType &rotateFrom, const VectorType &rotateTo);

	void setValue(const ValueTypeT alpha, const ValueTypeT beta, const ValueTypeT gamma);


	const ValueTypeT* getValues() const;

	void getValueAsAxisDeg(ValueTypeT &x, ValueTypeT &y, ValueTypeT &z,ValueTypeT &w) const;
	void getValueAsAxisRad(ValueTypeT &x, ValueTypeT &y, ValueTypeT &z, ValueTypeT &w) const;
	void getValueAsQuat(ValueTypeT &x, ValueTypeT &y, ValueTypeT &z, ValueTypeT &w) const;

	void getValueAsAxisRad(VectorType &axis, ValueTypeT &radians) const;
	void getValueAsAxisDeg(VectorType &axis, ValueTypeT &degrees) const;

	void getEulerAngleRad(VectorType &euler) const;
	void getEulerAngleDeg(VectorType &euler) const;

	ValueTypeT x() const;
	ValueTypeT y() const;
	ValueTypeT z() const;
	ValueTypeT w() const;

	ValueTypeT length() const;
	ValueTypeT lengthSquared() const;
	void normalize();
	ValueTypeT dot(const QuaternionBase &rValue) const;

	void invert();
	QuaternionBase inverse() const;

	void multVec(const VectorType &src, VectorType &dst ) const;

	void scaleAngle(ValueTypeT scaleFactor);

	void slerpThis(const QuaternionBase &rot0, const QuaternionBase &rot1, const ValueTypeT t );

	void nlerpThis (const QuaternionBase &rot0, const QuaternionBase &rot1, const ValueTypeT t );

	void mult (const QuaternionBase &other);
	void multLeft (const QuaternionBase &other);

	bool equals (const QuaternionBase &rot, const ValueTypeT tolerance ) const;

	QuaternionBase conj() const;
	QuaternionBase exp() const;
	QuaternionBase log() const;

	void conjThis();
	void expThis();
	void logThis();

	ValueTypeT &operator [](const UInt32 index);
	const ValueTypeT &operator [](const UInt32 index) const;

	void operator *=(const QuaternionBase &other);
	void operator +=(const QuaternionBase &other);

	QuaternionBase operator *(const QuaternionBase &rValue) const;
	QuaternionBase operator +(const QuaternionBase &rValue) const;
	QuaternionBase operator -(const QuaternionBase &rValue) const;
	QuaternionBase operator /(const QuaternionBase &rValue) const;

	QuaternionBase operator *(const ValueTypeT &rightScalor) const;
	QuaternionBase operator /(const ValueTypeT &rightScalor) const;

	QuaternionBase &operator =(const QuaternionBase &source);


	bool operator ==(const QuaternionBase &other) const;
	bool operator !=(const QuaternionBase &other) const;

	static void squad(const std::vector<QuaternionBase> &Q, const std::vector<Real32 > &t, const Real32 &s, QuaternionBase &result);

	static QuaternionBase squad(const std::vector<QuaternionBase> &Q, const std::vector<Real32 > &t, const Real32 &s);

	protected:
	static void slerp(const QuaternionBase &rot0, const QuaternionBase &rot1, QuaternionBase &result, const ValueTypeT t);

	static void nlerp(const QuaternionBase &rot0, const QuaternionBase &rot1, QuaternionBase &result, const ValueTypeT t);

	void mult(const ValueTypeT rVal1[4], const ValueTypeT rVal2[4]);

	private:
	enum ElementIndices
	{
		Q_X = 0,
		Q_Y = 1,
		Q_Z = 2,
		Q_W = 3
	};

	static QuaternionBase _identity;

	ValueTypeT _quat[4];
};

template <class ValueTypeT> inline
QuaternionBase<ValueTypeT> operator *(const ValueTypeT val, const QuaternionBase<ValueTypeT> &quat );

template <class  ValueTypeT> inline
std::ostream& operator <<(std::ostream& out, const QuaternionBase<ValueTypeT> &quat);

typedef QuaternionBase<Real32> Quaternion;

#include <TGMath/Quaternion.inl>
#endif