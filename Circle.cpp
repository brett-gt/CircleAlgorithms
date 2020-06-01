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
// Integer approach: Try brute forcing a square area to detect find ide
//
// TODO: DONT THINK THIS ONE WORKS
// ------------------------------------------------------------------------------
void bruteForceCircle::calc(int radius)
{
	//TODO: This is worst case size
	result = new Point[4 * radius];
	length = 0;

	int radius_lower = radius * radius - 1;
	int radius_upper = radius * radius + 1;

	int min_col = 0;

	//Only need increment number of points
	for (int row = radius; row >= 0; row--)
	{
		for (int col = min_col; col <= radius; col++)
		{
			int pos = row * row + col * col;
			if ((pos >= radius_lower) && (pos <= radius_upper))
			{
				result[length].X = row;
				result[length].Y = col;
				min_col = col;
				++length;
				break;
			}
		}
	}

	//Mirror
	for (int i = 0; i < length; i++)
	{
		result[i + length].X = -1 * result[i].Y;
		result[i + length].Y = result[i].X;

		result[i + 2 * length].X = -1 * result[i].X;
		result[i + 2 * length].Y = -1 * result[i].Y;

		result[i + 3 * length].X = result[i].Y;
		result[i + 3 * length].Y = -1 * result[i].X;
	}

	length = length * 4;
}


// ------------------------------------------------------------------------------
// Integer approach: Try brute forcing a square area to detect find ide
//
// TODO: Assumes for now number is divisible by 4.  Could put check in but would
// add a little bit of overhead for testing.
// ------------------------------------------------------------------------------
void strictBruteCircle::calc(int radius)
{
	//TODO: This is worst case size
	result = new Point[4 * (radius+1)];
	length = 0;

	int min_col = 0;

	int radius_sq = radius * radius;

	//Only need increment number of points
	for (int row = radius; row >= 0; row--)
	{
		bool min_found = false;
		for (int col = min_col; col <= radius; col++)
		{
			int pos = row * row + col * col;
			if (pos > radius_sq)
			{
				result[length].X = row;
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
		result[i + length].X = -1 * result[i].Y;
		result[i + length].Y = result[i].X;

		result[i + 2 * length].X = -1 * result[i].X;
		result[i + 2 * length].Y = -1 * result[i].Y;

		result[i + 3 * length].X = result[i].Y;
		result[i + 3 * length].Y = -1 * result[i].X;
	}
	length = length * 4;
}