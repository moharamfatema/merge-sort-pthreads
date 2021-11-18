#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define FILENAME_MAX 4096

/*defining an array*/
struct arr{
    int size;
    int * elements;
};

/*global vars*/
arr * original_array;

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

void merge(const int begin, const int end){

    int mid = ( begin + end ) / 2;
    int left = begin;
    int right = mid + 1;

    int new_size = end - begin + 1;
    int * new_elements = new int[new_size];
    int new_index = 0;
    
    /*merging*/
    while(left <= mid && right <= end){
        if(original_array->elements[left] > original_array->elements[right])
            new_elements[new_index++] = original_array->elements[right++];
        else
            new_elements[new_index++] = original_array->elements[left++];
    }

    /*merge the remaiining of either array*/
    while(left <= mid)
        new_elements[new_index++] = original_array->elements[left++];

    while(right <= end)
        new_elements[new_index++] = original_array->elements[right++];

    /*copy back to original array*/
    for(int i = 0; i < new_size; i++)
        original_array->elements[i + begin] = new_elements[i];

    /*cleanup*/
    delete[] new_elements;

    
}

/*define merge sort parameters*/
struct Range{
    int begin, end;
};

/*merge-sort as threads*/
void * merge_sort(void * rangeptr){
    Range * range = (Range *) rangeptr;
    
    if( range->begin >= range->end) pthread_exit(0);

    Range left_range, right_range;

    int mid = ( range->begin + range->end ) / 2;
    
    left_range.begin = range->begin;
    left_range.end = mid;

    right_range.begin = mid + 1;
    right_range.end = range->end;

    pthread_t tid1, tid2;

    if( pthread_create(&tid1, nullptr, merge_sort, &left_range)){
        std::cout << "Unable to create thread. Exiting...\n";
        exit(-1);
    }

    if( pthread_create(&tid2, nullptr, merge_sort, &right_range)){
        std::cout << "Unable to create thread. Exiting...\n";
        exit(-1);
    }

    pthread_join(tid1,nullptr);
    pthread_join(tid2,nullptr);

    merge(range->begin, range->end);
    pthread_exit(0);
}


int main(){

    /*get file name*/
    char * fileName =(char *) malloc(FILENAME_MAX*sizeof(char));

    std::cout << "Enter the name of the input file: \n";
    std::cin >> fileName;
    
    original_array = read_array(fileName);

    /*creating the first thread of merge sort*/
    Range range;
    range.begin = 0;
    range.end = original_array->size - 1;

    pthread_t tid;


    if( pthread_create(&tid, nullptr, merge_sort, &range))
    {
        std::cout << "Unable to create thread, Exiting...\n";
        return -1;
    }
    pthread_join(tid, nullptr);

    print_array(original_array);

    /*cleanup*/
    delete [] original_array->elements;
    delete(original_array);

    return 0;
}