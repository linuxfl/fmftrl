#include <ctime>
#include "FM_FTRL_machine.h"
#include "str_util.h"
#include <sys/timeb.h>  
#include "conf_util.h"

using namespace std;
using namespace util;
using namespace alg;

#define EPSILON 10e-6

void print_help()
{
	std::cout <<"Usage:" << "fmftrl conf_file" << std::endl;
	exit(0);
}

double logloss(double p,double y){
    p = max(min(p, 1. - 1e-15), 1e-15);
    if(y-1 < EPSILON && y-1>EPSILON*(-1)){
        return (-1)*log(p);    
    }else{
        return (-1)*log(1.-p);
    }
}

int main(int argc, char* argv[])
{

	if(argc < 2)
	{
		print_help();
	}
	
    util::conf_util fm_conf;
    fm_conf.parse(argv[1]);
    uint32_t fm_dim  = fm_conf.getItem<uint32_t>("fm_dim");
    double fm_initDev  = fm_conf.getItem<double>("fm_initDev");
    double L1 = fm_conf.getItem<double>("L1");
    double L2 = fm_conf.getItem<double>("L2");
    uint32_t fea_num = fm_conf.getItem<uint32_t>("fea_num");
    double L1_fm = fm_conf.getItem<double>("L1_fm");
    double L2_fm = fm_conf.getItem<double>("L2_fm");
    double alpha = fm_conf.getItem<double>("alpha");
    double beta = fm_conf.getItem<double>("beta");
    double alpha_fm = fm_conf.getItem<double>("alpha_fm");
    double beta_fm = fm_conf.getItem<double>("beta_fm");
    int n_epochs = fm_conf.getItem<int>("n_epochs");
    string train_file = fm_conf.getItem<string>("train_file");
    string model_file = fm_conf.getItem<string>("model_file");
    string ftrl_para_file = fm_conf.getItem<string>("ftrl_para_file");

    string old_model_file;
    string old_ftrl_para;

    bool is_incre = false;
    if(fm_conf.has_item("is_incre")){
        is_incre = fm_conf.getItem<bool>("is_incre");
        if(is_incre)
            std::cout << "increa..." << std::endl;
    }

    if(is_incre){
        if(!fm_conf.has_item("old_model_file")){
            cerr << "has no old model file..." << endl;
            return -1;
        }
        if(!fm_conf.has_item("old_ftrl_para")){
            cerr << " has no old ftrl parameter..." << endl;
            return -1;
        }
        old_ftrl_para = fm_conf.getItem<string>("old_ftrl_para");
        old_model_file = fm_conf.getItem<string>("old_model_file");
        cout << "load model file from disk..." << endl;
    }

    cout << "fm_dim: " << fm_dim << " fm_initDev: " << fm_initDev << " L1_fm: " << 
        L1_fm << " L2_fm: " << L2_fm << " alpha_fm: " << alpha_fm << " beta_fm: " << beta_fm << endl;
    cout << "fea_num: " << fea_num << " L1: " << L1 << " L2: " << L2 << " alpha: " << alpha << " beta: " << beta << endl;

    FM_FTRL_machine learner(fm_dim,fm_initDev,L1,L2,L1_fm,L2_fm,fea_num,alpha,beta,alpha_fm,beta_fm);

    if(is_incre)
    {
        if(learner.load_model(old_model_file.c_str(),old_ftrl_para.c_str())){
            cout << "load old model file success..." << endl;
        }
    }

	std::time_t begin_time;
	time(&begin_time);
    for(int e = 0; e < n_epochs;e++){
        /*if(e==0){
            learner.L1_fm = 0.;
            learner.L2_fm = 0.;
        }else{
            learner.L1_fm = L1_fm;    
            learner.L2_fm = L2_fm;    
        } */ 
        double progressiveLoss = 0.;
        double progressiveCount = 0.;

        int index = 0;
        //y is label x is feature_vec
        string line;
	    ifstream input_stream(train_file.c_str());
        //time_b time3;
        while(getline(input_stream, line))
        {

            time_b time0; 
            //std::time_t time0;
            ftime(&time0);
            //cout<<t1.millitm<<endl;

            vector<string> vec;
            vec.clear();
            util::str_util::trim(line);
            util::str_util::split(line, " ", vec);
            //feature label fea1 fea2
            double y = atof(vec[0].c_str());
            //cout << "get y " << y << endl;
            vector<uint32_t> fea_vec;
            for(size_t i = 1; i < vec.size();i++){
                //strtoul(vec[1].c_str(), NULL, 0);
                //cout << "push feature " << strtoul(vec[i].c_str(),NULL,0) << endl;
                fea_vec.push_back(strtoul(vec[i].c_str(),NULL,0));    
            }
            //std::time_t time1;
            time_b time1;
            ftime(&time1);
            //cout << "begin predict " << endl;
            double p = learner.predict(fea_vec);
            //cout << "after predict " << endl;
            //std::time_t time2;
            //time(&time2);
            time_b time2;
            ftime(&time2);
            double loss = logloss(p,y);
            learner.update(fea_vec,p,y);
            //cout << "after update" << endl;
            //std::time_t time3;
            //time(&time3);
            time_b time3;
            ftime(&time3);

            /*           
            if(index == 0){
                cout << "parse_time:" << get_diff(time1,time0) << " predict_time:" <<  get_diff(time2,time1)
                     << " update_time:" << get_diff(time3,time2)<< endl;
            }else{
                cout << "jump_time:" << get_diff(time3,time0) << " parse_time:" << get_diff(time1,time0) << " predict_time:" <<  get_diff(time2,time1)
                     << " update_time:" << get_diff(time3,time2)<< endl;
            }
            */
            
            
              //  << time2 - time1<< " update_time:" << time3 - time2 << endl;


            progressiveLoss += loss;
            progressiveCount += 1.;
            index += 1;
            if(index%10000 == 0){
                cout << "Epoch " << e << "\tcount: " << index << " Processive Loss: " << progressiveLoss / progressiveCount << endl;    
            }

        }
	//std::time_t end_time;
	//time(&end_time);
       // cout << "Epoch " << i  << " Processive Loss: " << progressiveLoss / progressiveCount <<  " elapsed time: " << difftime(end_time, begin_time) << "seconds" <<  endl;    
    }
    std::time_t end_time;
    time(&end_time);
    learner.save_model(model_file.c_str(),ftrl_para_file.c_str());  

	std::cout << "cost time: " 
		<< difftime(end_time, begin_time) << "seconds." << std::endl;
	//input_stream.close();
	//output_stream.close();
    return 0;
}
