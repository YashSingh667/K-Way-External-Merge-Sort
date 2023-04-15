#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>



using namespace std;


// Swap function
void swap(string &a, string &b) {
    string temp = a;
    a = b;
    b = temp;
}

int partition(vector<string>& arr, int low, int high) {
    string pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<string>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}






class MinHeap {
public:
    vector<pair<string, int>> heap;

    void push(pair<string, int> element) {
        heap.push_back(element);
        heapify_up(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        heapify_down(0);
    }

    pair<string, int> top() {
        return heap.front();
    }

    bool empty() {
        return heap.empty();
    }

private:
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapify_up(int i) {
        while (i != 0 && heap[parent(i)] > heap[i]) {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    void heapify_down(int i) {
        int minIndex = i;
        int l = left(i);
        int r = right(i);
        if (l < heap.size() && heap[l] < heap[minIndex]) {
            minIndex = l;
        }
        if (r < heap.size() && heap[r] < heap[minIndex]) {
            minIndex = r;
        }
        if (i != minIndex) {
            swap(heap[i], heap[minIndex]);
            heapify_down(minIndex);
        }
    }
};




void k_way_merge(const vector<string> &input_files, const string &output_file) {
    int k = input_files.size();
    vector<ifstream> in_files(k);

    if(input_files.size()==1){
        // ofstream out_file(output_file);
        // in_files[0].open(input_files[0]);
        // string line;
        // while(getline(in_files[0], line)){
        //     out_file << line << endl;
        // }
        // in_files[0].close();
        // out_file.close();

        // cout <<  "this ran " << endl;
        ofstream out_file(output_file);
        in_files[0].open(input_files[0]);
        out_file << in_files[0].rdbuf();
        in_files[0].close();
        out_file.close();
        return;
    }

    for (int i = 0; i < k; ++i) {
        in_files[i].open(input_files[i]);
    }

    ofstream out_file(output_file);
    MinHeap min_heap;
    string line;

    for (int i = 0; i < k; ++i) {
        if (getline(in_files[i], line)) {
            min_heap.push({line, i});
        }
    }

    while (!min_heap.empty()) {
        pair<string, int> min_element = min_heap.top();
        min_heap.pop();
        out_file << min_element.first << endl;

        if (getline(in_files[min_element.second], line)) {
            min_heap.push({line, min_element.second});
        }
    }

    for (int i = 0; i < k; ++i) {
        in_files[i].close();
    }
    out_file.close();
}



int external_merge_sort_withstop ( const char* input , const char * output , const long key_count , const int k = 2 , const int num_merges = 0 ){
    
    vector<vector<string>> input_files;
    vector<vector<string>> output_files;

    ifstream inFile(input);
    
    int succesful_merge = 0;

    vector<string> strings;
    string line;
    int num_lines = 0;
    int stage = 0;
    int num_run = 1;
    int num_lines_threshold;
    if(k == 2){
        num_lines_threshold = 100000;
    }
    else if(k == 8){
        num_lines_threshold = 50000;
    }
    else if(k == 16){
        num_lines_threshold = 25000;
    }
    else {
        num_lines_threshold = 20000;
    }
    int lines_read = 0;

    bool sufficient_key_count = false;

    vector<string> initial_output;
    while (getline(inFile, line) && lines_read < key_count) {
        
        strings.push_back(line);
        lines_read +=1;
        num_lines +=1;


        if(num_lines>=num_lines_threshold){
            sufficient_key_count =true;
            // cout<<num_lines<<endl;
            quickSort(strings, 0, strings.size() - 1);
            string outputFile = "temp."+to_string(stage)+"."+to_string(num_run)+".txt";
            ofstream outFile(outputFile);

            
            for (const string& sortedLine : strings) {
                if(sortedLine == "\n"){
                    // cout << "empty" << endl;
                }

                outFile << sortedLine << '\n';
            }
            num_run +=1;
            num_lines = 0;
            initial_output.push_back(outputFile);
            strings.clear();
        }
        
        
    }

    // outputting remaining lines
    if(strings.size() > 0){

        quickSort(strings, 0, strings.size() - 1);
        string outputFile;
        if(sufficient_key_count){
            outputFile = "temp."+to_string(stage)+"."+to_string(num_run)+".txt";
        }
        else{
            outputFile = output;
        }
        
        ofstream outFile(outputFile);
        initial_output.push_back(outputFile);
            
        for (const string& sortedLine : strings) {
                if(!sortedLine.size()){
                    // cout << "empty" <<endl;
                }
                outFile << sortedLine << endl;
        }
    }
    
    output_files.push_back(initial_output);

    // cout<<"stage 0 size : "<<output_files[0].size()<<endl;
    int k_num = k; // Number of input files
    
    int total_stages = ceil(log(output_files[0].size()) / log(k_num));

    
    stage += 1;
    
    while(stage <= total_stages){
    
        vector <string> current_stage_input = output_files[stage - 1];
        vector<string> current_stage_output;
        double current_stage_input_size = static_cast<double>(current_stage_input.size());
        double double_k_num = static_cast<double>(k_num);
        // cout<<"j ka upper value :"<<int(current_stage_input.size())<<endl;
        // cout<<k_num<<endl;
        // cout<<"value :"<< ceil(current_stage_input_size/double_k_num)<<endl;
        int threshhold = ceil(current_stage_input_size/double_k_num);
        for(int j=0;j<ceil(current_stage_input_size/double_k_num);j++){
            vector<string> inputFileNames;
            for (int i=0;i<k_num;i++){
                int index = k_num*j + i;
                inputFileNames.push_back(current_stage_input[index]);
                if(index == int(current_stage_input.size()) -1){
                    break;
                }
            }
            string outputFileName;
            if( stage == total_stages && j == threshhold-1){
                outputFileName = output;
            }
            else{
                outputFileName = "temp."+to_string(stage)+"."+to_string(j+1)+".txt";
            }
            
            
            if(num_merges==0){
                k_way_merge(inputFileNames, outputFileName);

            }
            else if(succesful_merge < num_merges ){
                k_way_merge(inputFileNames, outputFileName);
            }
            else{
                current_stage_output.push_back(outputFileName);
                inputFileNames.clear();
                return succesful_merge;
            }
            // k_way_merge(inputFileNames, outputFileName);
            if(inputFileNames.size()>1){
                succesful_merge+=1;
            }
            // cout<<"k chala "<<endl;
            current_stage_output.push_back(outputFileName);
            inputFileNames.clear();

        }
        output_files.push_back(current_stage_output);
        stage += 1;

    }


    cout<<"succesful_merges  :  "<<succesful_merge<<endl;
    return succesful_merge; // to be changes according to output format
}

signed main(){
    // ios_base::sync_with_stdio(0);
    // cin.tie(0);
    // cout.tie(0);
    // auto start_time = chrono::high_resolution_clock::now();
    external_merge_sort_withstop("/home/yash_linux/col362/asg3/random.txt","/home/yash_linux/col362/asg3/output.txt",1e9,8,0);

    // auto end_time = chrono::high_resolution_clock::now(); // get the end time
    // auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time); // calculate the duration in milliseconds

    // cout << "Program executed in " << duration.count() << "ms" << endl;

    return 0;
}

