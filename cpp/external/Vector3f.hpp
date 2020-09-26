#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_

#pragma once
#include "cmath"

class  Vector3f
{
protected:

public:
	float X, Y, Z;
	Vector3f()
	{

	}
	Vector3f(float xx, float yy, float zz) : X(xx), Y(yy), Z(zz)
	{

	}

	Vector3f SwitchYZ() const
	{
		return Vector3f(X, Z, Y);
	}

	bool IsValid() const
	{
		return X != 0 && Y != 0 && Z != 0;
	}

	operator float* ()
	{
		return &X;
	}

	float GetX() const
	{
		return X;
	}

	float GetY() const
	{
		return Y;
	}

	float GetZ() const
	{
		return Z;
	}

	float DistanceTo(const Vector3f& v) const
	{
		return sqrt(pow(v.X - X, 2) + pow(v.Z - Z, 2) + pow(v.Y - Y, 2));
	}

	Vector3f Randomize()
	{
		auto xRand = X + (rand() % -50 + 100);
		auto zRand = Z + (rand() % -30 + 70);

		return Vector3f(xRand, Y, zRand);
	}

};

#endif