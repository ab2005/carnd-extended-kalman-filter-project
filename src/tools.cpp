#include <iostream>
#include "tools.h"

namespace tools {

    VectorXd CalculateRMSE(const vector<VectorXd> &estimations,
                           const vector<VectorXd> &ground_truth) {
        VectorXd rmse(4);
        rmse << 0, 0, 0, 0;

        if (estimations.size() == 0 || estimations.size() != ground_truth.size()) {
            cout << "CalculateRMSE () - Error: Invalid input values." << endl;
            return rmse;
        }

        for (int i = 0; i < estimations.size(); ++i) {
            VectorXd res = estimations[i] - ground_truth[i];
            res = res.array() * res.array();
            rmse += res;
        }

        rmse /= estimations.size();
        rmse = rmse.array().sqrt();

        return rmse;
    }

    MatrixXd CalculateJacobian(const VectorXd& x_state) {
        MatrixXd Hj(3,4);
        //recover state parameters
        float px = x_state(0);
        float py = x_state(1);
        float vx = x_state(2);
        float vy = x_state(3);

        //pre-compute a set of terms to avoid repeated calculation
        float c1 = px*px+py*py;
        float c2 = sqrt(c1);
        float c3 = (c1*c2);

        //check division by zero
        if(fabs(c1) < 0.0001){
            cout << "CalculateJacobian () - Error - Division by Zero" << endl;
            cout << x_state << endl;
            return Hj;
        }

        //compute the Jacobian matrix
        Hj << (px/c2),               (py/c2),               0,     0,
        -(py/c1),              (px/c1),               0,     0,
        py*(vx*py - vy*px)/c3, px*(px*vy - py*vx)/c3, px/c2, py/c2;

        return Hj;
    }
}
