#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include "conf_util.h"
#include "str_util.h"
#include "FM_FTRL_machine.h"

using namespace std;
using namespace util;
using namespace alg;

int main(int argc, char* argv[])
{
    if(argc < 2){
        cout << "Usage: bin conf_file"<< endl;
        return -1;
    }

    conf_util fm_conf;
    fm_conf.parse(argv[1]);
    string valid_file = fm_conf.getItem<string>("valid_file");
    uint32_t fea_num = fm_conf.getItem<uint32_t>("fea_num");
    uint32_t fm_dim = fm_conf.getItem<uint32_t>("fm_dim");
    string predict_out = fm_conf.getItem<string>("predict_out");
    string model_file = fm_conf.getItem<string>("model_file");
    
    FM_FTRL_machine fm(fm_dim,fea_num);
    cout << "load model file from disk..." << endl;
    if(fm.load_model(model_file.c_str())){
        cout << "load model file success..." << endl;
    }else{
        cout << "load model file fail..." << endl;
        return -1;
    }

    ifstream infile(valid_file.c_str());
    ofstream outfile(predict_out.c_str());
        
    string line;
    vector<string> vec;
    vector<uint32_t> fea_vec;
    uint32_t cnt = 0;
    while(getline(infile, line)){
        vec.clear();
        util::str_util::trim(line);
        util::str_util::split(line, " ", vec);
        fea_vec.clear();
        for(uint32_t i = 1; i< vec.size();i++){
                //strtoul(line.c_str(), NULL, 0);
            fea_vec.push_back(strtoul(vec[i].c_str(), NULL, 0));
        }
        double pctr = fm.score(fea_vec,0.0);
        outfile << pctr << " " <<  line << endl;
        cnt++;
        if(cnt % 100000 == 0)
            cout << "predict instance " << cnt << std::endl;
    }
    outfile.close();
    return 0;
}
