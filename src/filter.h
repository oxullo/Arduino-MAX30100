/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Based on https://www.researchgate.net/publication/3728672_A_simple_real-time_QRS_detection_algorithm
// http://www.atmel.com/images/doc2527.pdf
// http://de.mathworks.com/matlabcentral/fileexchange/45840-complete-pan-tompkins-implementation-ecg-qrs-detector/content/pan_tompkin.m
// http://cnx.org/contents/YR1BUs9_@1/QRS-Detection-Using-Pan-Tompki

#ifndef FILTER_H
#define FILTER_H

// http://www.schwietering.com/jayduino/filtuino/
// Band pass bessel filter order=2 alpha1=0.05 alpha2=0.15
// Fc=100Hz -> Fl=5Hz, Fh=15Hz
class  FilterBeBp2
{
	public:
		FilterBeBp2()
		{
			for(int i=0; i <= 4; i++)
				v[i]=0.0;
		}
	private:
		float v[5];
	public:
		float step(float x) //class II
		{
			v[0] = v[1];
			v[1] = v[2];
			v[2] = v[3];
			v[3] = v[4];
			v[4] = (9.189847175584472949e-2 * x)
				 + (-0.24114073878907088533 * v[0])
				 + (1.15795881596677863179 * v[1])
				 + (-2.41311507478477782485 * v[2])
				 + (2.44900730391579601886 * v[3]);
			return
				 (v[0] + v[4])
				- 2 * v[2];
		}
};

#endif
