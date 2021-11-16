#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

/*defining an array*/
struct arr{
    int size;
    int * elements;
};

/*read array from file*/
arr* read_array(const char * file_name){
    std::ifstream fin;
    fin.open(file_name);

    if(fin){
        int size;

        fin >> size;

        int * array = new int [size];

        for (int i = 0; i < size; i++){
            fin >> array[i];
        }

        arr * a = new arr;
        a->size = size;
        a->elements = array;

        fin.close();

        return a;
    }
    else{
        std::cout << "Connot read file.\nExiting...\n";
        exit(-1);
    }
}

/*utility function to print an array*/
void print_array (const arr * array){
    for(int i =0; i < array->size; i++){
        std::cout << array->elements[i] << "\t";
    }
    std::cout << "\n";
}

void merge(const arr * array, const int left, const int mid, const int right){

    const int sub_array_1 = mid - left + 1;
    const int sub_array_2 = right - mid;

    /*create auxilary arrays*/
    arr * left_array = new arr,
        * right_array = new arr;

    
}

int main(){

    ///*get file name*/
    //char * fileName =(char *) malloc(FILENAME_MAX*sizeof(char));

    //std::cout << "Enter the name of the input file: \n";
    //std::cin >> fileName;
    arr * original_array = read_array("input.txt");

    /*cleanup*/
    delete [] original_array->elements;
    delete(original_array);

    return 0;
}