#pragma once

#include <string>

// ------------------------------------------------------------------------------
// struct Point
//
// Hold X, Y coordinate pairs.  
//
// Probably not a big reason to have this for this simple program.
// ------------------------------------------------------------------------------
struct Point
{
	int X;
	int Y;
};


// ------------------------------------------------------------------------------
// class baseCircle
//
// Abstract class defining requirements for a circle implementation.
// ------------------------------------------------------------------------------
class baseCircle
{
protected:
	Point *result; //pointer to array holding points in the circle
	int   length;  //length of the result array


public:
	//function prototype for function to calculate points in circle.  This is what 
	//make implementations unique
	virtual void calc(int radius) = 0;     
					
	Point* getResult() { return result; };
	int    getLength() { return length; };

	std::string name;
};


//--------------------------------------------------------------------------------
//  SPECIFIC IMPLEMENTATIONS
//--------------------------------------------------------------------------------
class naiveCircle : public baseCircle
{
public:
	naiveCircle()
	{
		baseCircle::name = "NaiveCircle";
	}

	void calc(int radius);
};

class naiveEightCircle : public baseCircle
{
public:
	naiveEightCircle()
	{
		baseCircle::name = "NaiveEighthCircle";
	}

	void calc(int radius);
};

class expansionCircle : public baseCircle
{
public:
	expansionCircle()
	{
		baseCircle::name = "ExpansionCircle";
	}

	void calc(int radius);
};

class radiusLimitsCircle : public baseCircle
{
public:
	radiusLimitsCircle()
	{
		baseCircle::name = "RadiusLimitsCircle";
	}

	void calc(int radius);
};

class radiusBoundCircle : public baseCircle
{
public:
	radiusBoundCircle()
	{
		baseCircle::name = "RadiusBoundCircle";
	}

	void calc(int radius);
};

class walkingCircleClean : public baseCircle
{
public:
	walkingCircleClean()
	{
		baseCircle::name = "WalkingCircleClean";
	}

	void calc(int radius);
};

class walkingCircleEfficient : public baseCircle
{
public:
	walkingCircleEfficient()
	{
		baseCircle::name = "WalkingCircleEfficient";
	}

	void calc(int radius);
};


