#ifndef FM_MACHINE_H
#define FM_MACHINE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <fstream>
#include <random>
#include <sys/timeb.h>

namespace alg{

typedef struct timeb time_b;

class FM_FTRL_machine
{
public:
	FM_FTRL_machine();
    FM_FTRL_machine(uint32_t,uint32_t);
	FM_FTRL_machine(uint32_t fm_dim, double fm_init_Dev,double L1,double L2,
		double L1_fm, double L2_fm,uint32_t fea_num,double alpha,double beta,
		double alpha_fm,double beta_fm);

	~FM_FTRL_machine();

	double predict_raw(std::vector<uint32_t> inx);
	double predict(std::vector<uint32_t> inx);
	int update(std::vector<uint32_t>,double ,double);
	void save_model(const char *model_file,const char *ftrl_para_file);
	bool load_model(const char *model_file,const char *ftrl_para_file);	
    bool load_model(const char *model_file);
    double score(std::vector<uint32_t>,double);

private:
	double *n;
	double *z;
	double *w;

	double **n_fm;
	double **w_fm;
	double **z_fm;
	double **fm_sum;

	uint32_t fm_dim;
	double fm_initDev;
	double L1;
	double L2;
    double L1_fm;
	double L2_fm;

	double alpha;
	double beta;
	uint32_t fea_num;
	double alpha_fm;
	double beta_fm;

	std::default_random_engine generator;
    std::normal_distribution<double>* p_gauss_distribution;
};

}

#endif
