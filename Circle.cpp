#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

//Note: For some reason if include this before math, M_PI doesn't get seen.  Thought it
// was a namespace issue but that didn't seem to matter.
#include "Circle.h"


// There is one goof with this... the data sets really cover points from [0, 45] degress.
// So when you merge them, you end up with [0, 45] [45, 90] [90, 135] ... when you really
// want [0, 45] (45, 90] (90, 135].  Because of this, we have a few duplicates in the arry.
// TODO: Find a creative way to fix this...
void populate(Point *result, Point *source, int length)
{
	for (int i = 0; i < length ; i++)
	{
		result[2 * length - i - 1] = { source[i].Y, source[i].X };

		result[2 * length + i] = { -1 * source[i].Y, source[i].X };

		result[4 * length - i - 1] = { -1 * source[i].X, source[i].Y };

		result[i + 4 * length] = { -1 * source[i].X, -1 * source[i].Y };

		result[6 * length - i - 1] = { -1 * source[i].Y, -1 * source[i].X };

		result[i + 6 * length] = { source[i].Y, -1 * source[i].X };

		result[8 * length - i - 1] = { source[i].X, -1 * source[i].Y };
	}

}

// ------------------------------------------------------------------------------
// Naive implementation using sin/cos math functions of 0 .. 2 PI.  Does one point
// per unit of radius.  Logic for that is this should roughly make it equivalent
// to other methods of drawing the circle that increments through each x axis pixel.
// ------------------------------------------------------------------------------
void naiveCircle::calc(int radius)
{
	length = radius * 4;
	result = new Point[length];

	double angle_inc = 2  * M_PI / length;
	double angle = 0;

	for (int i = 0; i < length; i++)
	{
		result[i].X = radius * (int)(cos(angle));
		result[i].Y = radius * (int)(sin(angle));
		angle = angle + angle_inc;
	}
}

// ------------------------------------------------------------------------------
// Improving on the naive implementation.  Still  using sin/cos math functions but
// taking advantage of symetry to limit number of calculations.  Still doing one point
// per unit of radius.  See above for logic.
// ------------------------------------------------------------------------------
void naiveEightCircle::calc(int radius)
{
	length = radius * 4;
	result = new Point[length];

	int eigths = length / 8;

	float angle_inc = 2 * M_PI / length;
	float angle = 0;

	for (int i = 0; i <= eigths; i++)
	{
		result[i].X = (int)(radius * cos(angle));
		result[i].Y = (int)(radius * sin(angle));

		result[2 * eigths - i] = { result[i].Y, result[i].X };
		result[2 * eigths + i] = { -1 * result[i].Y, result[i].X };
		result[4 * eigths - i] = { -1 * result[i].X, result[i].Y };
		result[i + 4 * eigths] = { -1 * result[i].X, -1 * result[i].Y };
		result[6 * eigths - i] = { -1 * result[i].Y, -1 * result[i].X };
		result[i + 6 * eigths] = { result[i].Y, -1 * result[i].X };
		result[length - i] = { result[i].X,  -1 * result[i].Y };

		angle = angle + angle_inc;
	}
}


// ------------------------------------------------------------------------------
// Seeing if using Taylor expansions are an faster.  Utilizes symetry of circle
// to reduce calculations required.  Still doing one point per unit of radius.  
// See above for logic.
// ------------------------------------------------------------------------------
void expansionCircle::calc(int radius)
{
	length = radius * 4;
	result = new Point[length];

	int eigths = length / 8;

	double angle_inc = 2 * M_PI / length;
	double angle = 0;

	for (int i = 0; i <= eigths; i++)
	{
		float pow2 = angle * angle;
		float pow3 = pow2 * angle;
		float pow4 = pow2 * pow2;
		float pow5 = pow3 * pow2;
		float pow6 = pow4 * pow2;
		float pow7 = pow5 * pow2;

		result[i].X = (int)((double)radius * (1 - pow2 / 2.0 + pow4 / 24.0 - pow6 / 720.0));
		result[i].Y = (int)((double)radius * (angle - pow3 / 6.0 + pow5 / 120.0 - pow7 / 5040.0));

		/*
		result[2 * eigths - i] = { result[i].Y, result[i].X };
		result[2 * eigths + i] = { -1 * result[i].Y, result[i].X };
		result[4 * eigths - i] = { -1 * result[i].X, result[i].Y };
		result[i + 4 * eigths] = { -1 * result[i].X, -1 * result[i].Y };
		result[6 * eigths - i] = { -1 * result[i].Y, -1 * result[i].X };
		result[i + 6 * eigths] = { result[i].Y, -1 * result[i].X };
		result[length - i] = { result[i].X,  -1*result[i].Y };
		*/

		angle = angle + angle_inc;
	}

	populate(result, result, eigths);
}

// ------------------------------------------------------------------------------
// Integer approach: Brute force finding integer points by incrementing through
// each X value and then searching for a Y value that makes it exceed the radius.
// ------------------------------------------------------------------------------
void radiusBoundCircle::calc(int radius)
{
	//TODO: This is worst case size
	result = new Point[4 * (radius+1)];
	result[0].X = radius;
	result[0].Y = 0;
	length = 1;

	int min_col = 0;

	int radius_sq = radius * radius;

	while(result[length-1].X >= result[length - 1].Y) // 1/8th of circle
	{
		result[length].X = result[length - 1].X - 1;
		for (int col = min_col; col <= radius; col++)
		{
			int pos = result[length].X * result[length].X + col * col;
			if (pos > radius_sq)
			{
				result[length].Y = col-1;
				min_col = col-1;
				++length;
				break;
			}
		}
	}
	--length;

	populate(result, result, length);
	length = length * 8;
}



// ------------------------------------------------------------------------------
// Integer approach: Stair step.  Start at (radius, 0) and then walk up and check.
// If exceed the radius, check moving in one.
// ------------------------------------------------------------------------------
void walkingCircleClean::calc(int radius)
{
	std::vector<Point> buffer;

	int radius_sq = radius * radius;

	Point p = { radius, 0 };
	buffer.push_back(p);

	//This should take us to 45 degree  mark
	while (p.X > p.Y)
	{
		//Move directly up first, I think this will be more common since moving up
		//and left would result in a straight line.  Since we are 'more positive' than
		//a straight line, up should be more common than up and left.
		p = { p.X, p.Y + 1 };

		int pos = p.X * p.X + p.Y * p.Y;

		if (pos > radius_sq)
		{
			p.X = p.X - 1;
		}
		buffer.push_back(p);
	}

	//Mirror
	length = buffer.size();
	result = new Point[8 * length];

	for (int i = 0; i < buffer.size(); i++)
	{
		result[i] = buffer[i];
	}
	populate(result, result, length);
	length = length * 8;
}


// ------------------------------------------------------------------------------
// Integer approach: Stair step.  Start at (radius, 0) and then walk up and check.
// If exceed the radius, check moving in one.  
// ------------------------------------------------------------------------------
void walkingCircleEfficient::calc(int radius)
{
	//TODO: This is worst case size
	Point *temp = new Point[40 * (radius + 1)];
	length = 0;

	int radius_sq = radius * radius;

	temp[length] = { radius, 0 };
	length++;

	//This should take us to 45 degree  mark
	while (temp[length - 1].X > temp[length - 1].Y)
	{
		//Move directly up first, I think this will be more common since moving up
		//and left would result in a straight line.  Since we are 'more positive' than
		//a straight line, up should be more common than up and left.
		temp[length] = { temp[length - 1].X, temp[length - 1].Y + 1 };

		int pos = temp[length].X * temp[length].X + temp[length].Y * temp[length].Y;

		if (pos > radius_sq)
		{
			temp[length].X = temp[length].X - 1;
		}
		length++;
	}

	//TODO: Could gain a little efficiency here by memcpy less and then populate
	populate(temp, temp, length);
	length = length * 8;
	result = new Point[length];

	memcpy(result, temp, length * sizeof(Point));
}