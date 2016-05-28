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
