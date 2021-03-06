#ifndef IM_H_INCLUDED
#define IM_H_INCLUDED
#include "little_math.h"

template <typename type, size_t size>
mvector<size_t, size> sample_of_main_diagonal_elements(matrix<type, size, size> &mat, mvector<type, size> &vec)
{
    mvector<size_t , size> numbers;
    for(int i = 0;i < size;i++)
        numbers[i] = i;
    int maxX, maxY;
    for(int i = 0;i < size - 1;i++){
        maxX = i, maxY = i;
        for(int j = i;j < size;j++)
            for(int k = i;k < size;k++)
                if(mat[maxY][maxX] < mat[j][k]){
                    maxY = j; maxX = k;
                }
        swap(vec[i],vec[maxY]);
        swap(numbers[i],numbers[maxX]);
        mat.swap_elements(i,i,maxY,maxX);
    }
    /*type diag = 0,other = 0;
    for(int i = 0;i < size;i++){
        diag += mat[i][i];
        for(int j = 0;j < size;j++)
            if(i != j) other += mat[i][j];
    }

    if(diag < other) throw except::no_diagonal_dominance;
    diag = 1;

    for(int i = 0;i < size;i++)
        diag *= mat[i][i];

    if(diag == 0) throw except::zeros_on_the_diagonal;
    */
    return numbers;
}

template <typename type, size_t size>
mvector<type,size> simple_iteration(matrix<type,size,size> mat, mvector<type,size>
                        vec, double eps, mvector<type,size> initial_approx, std::string options)
{
    mvector<type,size> aprox = initial_approx, result;
    type err;

    mvector<size_t, size> numbers;
    numbers = sample_of_main_diagonal_elements(mat, vec, numbers);

    do{
        for(int i = 0;i < size;i++){
            result[i] = vec[i];
            for(int j = 0;j < size;j++)
                if(i != j)
                    result[i] -= mat[i][j]*aprox[j];
            result[i] /= mat[i][i];
        }

        for(int i = 0;i < size - 1;i++)
            err = std::max(std::abs(result[i] - aprox[i]),std::abs(result[i + 1] - aprox[i + 1]));
        aprox = result;
    }
    while(err > eps);

    for(int i = 0;i < size;i++)
        result[i] = aprox[numbers[i]];

    return result;
}

template <typename type, size_t size>
mvector<type,size> BiCGStab(const matrix<type,size,size>& A,const mvector<type,size>&
                        b,const double& eps, mvector<type,size> x)
{
    mvector<type, size> r, r_, p, r1, x1, s, p1;
    type alpha, beta, w;
    int count = 0;
    r = b - A*x;
    p = r;
    r_ = r;
    do{
        alpha = (r*r_)/((A*p)*r_);
        s = r - (A*p)*alpha;
        w = ((A*s)*s)/((A*s)*(A*s));
        x1 = x + p*alpha + s*w; ///MAGIC IS HERE !!!!
        r1 = b - A*x1;//r1 = s - A*s*w;
        beta = (r1*r_)/(r*r_);//beta = (alpha*(r1*r_))/(w*(r*r_));
        p1 = r1 + p*beta - A*p*w;
        x = x1;
        r = r1;
        p = p1;
    }
    while(r1.magnitude() > eps);
    return x1;
}

template <typename type,size_t size>
matrix<type, size, size> converse_IM(matrix<type, size, size> exemp, double eps)
{
    matrix<type,size,size> ones('E');
    for(int i = 0;i < size;i++)
        ones.set_colum(simple_iteration(exemp,ones.get_colum(i),eps,ones.get_colum(i)),i);
    return ones;
}

#endif
