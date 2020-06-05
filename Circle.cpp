#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

//Note: For some reason if include this before math, M_PI doesn't get seen.  Thought it
// was a namespace issue but that didn't seem to matter.
#include "Circle.h"


// ------------------------------------------------------------------------------
// Naive implementation using sin/cos math functions of 0 .. 2 PI.  Does one point
// per unit of radius.  Logic for that is this should roughly make it equivalent
// to other methods of drawing the circle that increments through each x axis pixel.
// ------------------------------------------------------------------------------
void naiveCircle::calc(int radius)
{
	length = radius * 4;
	result = new Point[length];

	float angle_inc = 2  * M_PI / length;
	float angle = 0;

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

		result[2 * eigths - i ].X = result[i].Y;
		result[2 * eigths - i ].Y = result[i].X;

		result[2 * eigths + i].X = -1 * result[i].Y;
		result[2 * eigths + i].Y = result[i].X;

		result[4 * eigths - i ].X = -1 * result[i].X;
		result[4 * eigths - i ].Y = result[i].Y;

		result[i + 4 * eigths].X = -1 * result[i].X;
		result[i + 4 * eigths].Y = -1 * result[i].Y;

		result[6 * eigths - i ].X = -1 * result[i].Y;
		result[6 * eigths - i ].Y = -1 * result[i].X;

		result[i + 6 * eigths].X = result[i].Y;
		result[i + 6 * eigths].Y = -1 * result[i].X;

		result[length - i ].X = result[i].X;
		result[length - i ].Y = -1 * result[i].Y;

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

	float angle_inc = 2 * M_PI / length;
	float angle = 0;

	for (int i = 0; i < eigths; i++)
	{
		float pow2 = angle * angle;
		float pow3 = pow2 * angle;
		float pow4 = pow2 * pow2;
		float pow5 = pow3 * pow2;
		float pow6 = pow4 * pow2;
		float pow7 = pow5 * pow2;

		result[i].X = (int)((float)radius * (1 - pow2 / 2.0 + pow4 / 24.0 - pow6 / 720.0));
		result[i].Y = (int)((float)radius * (angle - pow3 / 6.0 + pow5 / 120.0 - pow7 / 5040.0));

		result[2 * eigths - i].X = result[i].Y;
		result[2 * eigths - i].Y = result[i].X;

		result[2 * eigths + i].X = -1 * result[i].Y;
		result[2 * eigths + i].Y = result[i].X;

		result[4 * eigths - i].X = -1 * result[i].X;
		result[4 * eigths - i].Y = result[i].Y;

		result[i + 4 * eigths].X = -1 * result[i].X;
		result[i + 4 * eigths].Y = -1 * result[i].Y;

		result[6 * eigths - i].X = -1 * result[i].Y;
		result[6 * eigths - i].Y = -1 * result[i].X;

		result[i + 6 * eigths].X = result[i].Y;
		result[i + 6 * eigths].Y = -1 * result[i].X;

		result[length - i].X = result[i].X;
		result[length - i].Y = -1 * result[i].Y;

		angle = angle + angle_inc;
	}
}

// ------------------------------------------------------------------------------
// Integer approach: Brute force finding integer points by incrementing through
// each X value and then searching for a Y value that makes it exceed the radius.
// 
// Indexing issue where we are getting start and endpoints twice.
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

	//This should take us to 45 degree  mark
	while(result[length-1].X > result[length - 1].Y)
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

	//Mirror
	for (int i = 0; i < length; i++)
	{
		result[2 * length - i - 1].X = result[i].Y;
		result[2 * length - i - 1].Y = result[i].X;

		//std::cout << "2*Length is " << 2 * length - i - 1 << std::endl;

		result[2 * length + i].X = -1 * result[i].Y;
		result[2 * length + i].Y = result[i].X;

		result[4 * length - i - 1].X = -1 * result[i].X;
		result[4 * length - i - 1].Y = result[i].Y;

		result[i + 4 * length].X = -1 * result[i].X;
		result[i + 4 * length].Y = -1 * result[i].Y;

		result[6 * length - i - 1].X = -1 * result[i].Y;
		result[6 * length - i - 1].Y = -1 * result[i].X;

		result[i + 6 * length].X = result[i].Y;
		result[i + 6 * length].Y = -1 * result[i].X;

		result[8 * length - i - 1].X = result[i].X;
		result[8 * length - i - 1].Y = -1 * result[i].Y;
	}

	length = length * 8;
}


// ------------------------------------------------------------------------------
// Integer approach: Stair step.  Start at (radius, 0) and then walk up and check.
// If exceed the radius, check moving in one.  
// 
// Indexing issue where we are getting start and endpoints twice.
// ------------------------------------------------------------------------------
void walkingCircle::calc(int radius)
{
	//TODO: This is worst case size
	result = new Point[40 * (radius + 1)];
	length = 0;

	int radius_sq = radius * radius;

	result[length].X = radius;
	result[length].Y = 0;
	length++;

	//This should take us to 45 degree  mark
	while (result[length - 1].X > result[length - 1].Y)
	{
		//Move directly up first, I think this will be more common since moving up
		//and left would result in a straight line.  Since we are 'more positive' than
		//a straight line, up should be more common than up and left.
		result[length].X = result[length - 1].X;
		result[length].Y = result[length - 1].Y + 1;

		int pos = result[length].X * result[length].X + result[length].Y * result[length].Y;

		if (pos > radius_sq)
		{
			result[length].X = result[length].X - 1;
		}
		length++;
	}

	//Mirror
	for (int i = 0; i < length; i++)
	{
		result[2 * length - i - 1].X = result[i].Y;
		result[2 * length - i - 1].Y = result[i].X;

		//std::cout << "2*Length is " << 2 * length - i - 1 << std::endl;

		result[2 * length + i].X = -1 * result[i].Y;
		result[2 * length + i].Y = result[i].X;

		result[4 * length - i - 1].X = -1 * result[i].X;
		result[4 * length - i - 1].Y = result[i].Y;

		result[i + 4 * length].X = -1 * result[i].X;
		result[i + 4 * length].Y = -1 * result[i].Y;

		result[6 * length - i - 1].X = -1 * result[i].Y;
		result[6 * length - i - 1].Y = -1 * result[i].X;

		result[i + 6 * length].X = result[i].Y;
		result[i + 6 * length].Y = -1 * result[i].X;

		result[8 * length - i - 1].X = result[i].X;
		result[8 * length - i - 1].Y = -1 * result[i].Y;
	}
	length = length * 8;
}