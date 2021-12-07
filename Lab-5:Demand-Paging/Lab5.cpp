#include <bits/stdc++.h>
#define fr(a,b) for(int i = a; i < b; i++)
#define rep(i,a,b) for(int i = a; i < b; i++)
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL)
using ll = long long;
using namespace std;


list<int> free_frames;
unordered_map<int,int> TLB;

void lru(vector<vector<int>>& page_table,vector<int> ref_string,int pg){
    unordered_map<int,int> mp;
    fr(1,page_table.size()){
        if(page_table[i][1]==1){
            bool indi = false;
            for(int j = pg-1;j>=0;j--){
                if(ref_string[j]==i){
                    mp[i] = j;
                    indi = true;
                    break;
                }
            }
            if(!indi){
                mp[i] = INT_MIN;
            }
        }
    }
    int ans;
    int mini = INT_MAX;
    // To find victim frame
    for(auto itr : mp){
        if(itr.second<mini){
            mini = itr.second;
            ans = itr.first;
        }
        if(itr.second==mini){
            ans = min(ans,itr.first);
        }
    }
    // Page replacement
    page_table[ans][1] = 0;
    page_table[ref_string[pg]][1] = 1;
    page_table[ref_string[pg]][0] = page_table[ans][0];
    page_table[ans][0] = -1; 
}

void page_fault_handler(int i,vector<vector<int>>& page_table,vector<int>& ref_string){
    if(!free_frames.empty()){
        page_table[ref_string[i]][0] = free_frames.front();
        page_table[ref_string[i]][1] = 1;
        free_frames.pop_front();
    }
    else{
        lru(page_table,ref_string,i);
    }
}

void update_TLB(vector<int>& ref_string,int s,int i,int f){
    if(TLB.size()==s){
        set<int> st;
        bool indi = false;
        int j;
        for(j = i-1;j>=0;j--){
            st.insert(ref_string[j]);
            if(st.size()==s){
                indi = true;
                break;
            }
        }
        if(indi){
            TLB.erase(ref_string[j]);
        }
        else{
            int k;
            for(k = s;k>=1;k--){
                st.insert(k);
                if(st.size()==s){
                    break;
                }
            }
            TLB.erase(k);
        }
    }
    TLB[ref_string[i]] = f;
    return;
}

int main(){
    int k;
    cout<<"Enter the number of processes : ";
    cin>>k;
    // Virtual address space
    int m;
    cout<<"\nEnter the max number of pages req per process : ";
    cin>>m;
    // Physical address space  m>f
    int f;
    cout<<"\nEnter the number of frames in the main memory : ";
    cin>>f;
    // s<f
    int s;
    cout<<"\nSize of TLB : ";
    cin>>s;
    cout<<"\n";
    srand ( time(NULL) );
    vector<bool> visited(k+1,false);
    int count = 0;
    string a;
    fstream fp;
    fp.open("Result.txt", ios::out | ios::in );
    //At start all the frames are free
    fr(1,f+1){
        free_frames.push_back(i);
    }
    
    while(count<k){
        int curr_process=(rand() % k) + 1;
        while(visited[curr_process]){
            curr_process = (rand() % k) + 1;
        }
        count++;
        visited[curr_process] = true;
        int num_of_pages = (rand() % m) + 1;
        vector<vector<int>> page_table(num_of_pages+1,vector<int> (2));
        fr(1,num_of_pages+1){
            page_table[i][0] = -1;
            page_table[i][1] = 0;
        }

        // Process Dispatched
        a = "Process id "+to_string(curr_process)+" dispatched\n";
        fp<<a;
        cout<<a;

        // initialize TLB and load the pages to main memory
        fr(1,min(s,num_of_pages)+1){
            TLB[i] = free_frames.front();
            page_table[i][0] = free_frames.front();
            page_table[i][1] = 1;
            free_frames.pop_front();
        }
        
        int sz = (rand() % (8*num_of_pages + 1)) + 2*num_of_pages;

        // Generate the reference string
        vector<int> ref_string(sz);
        fr(0,sz){
            ref_string[i] = (rand() % (num_of_pages - 1 + 1)) + 1 ;
        }

        //The process begins

        int page_faults = 0;

        fr(0,sz){
            int pg = ref_string[i];
            // TLB Hit
            if(TLB.find(pg)!=TLB.end()){
                a = "for page reference "+to_string(pg) + ", TLB hit with frame no. "+to_string(TLB[pg])+"\n";
                fp<<a;
                cout<<a;
            }
            // TLB miss
            else{
                //Search in page table
                // Valid bit
                if(page_table[pg][1]==1){
                    a = "for page reference "+to_string(pg) + ", TLB miss -> Page Table valid -> with frame no. "+to_string(page_table[pg][0])+"\n";
                    fp<<a;
                    cout<<a;
                }
                // Invalid bit , Page fault 
                else{
                    page_faults++;
                    page_fault_handler(i,page_table,ref_string);
                    a = "for page reference "+to_string(pg) + ", TLB miss -> Page Table invalid -> page_fault_hnadler invoked -> frame no. "+to_string(page_table[pg][0])+"\n";
                    fp<<a;
                    cout<<a;
                }
                update_TLB(ref_string,s,i,page_table[pg][0]);
                
            }
        }
        // Free all the frames assigned to the pages of this process
        while(!free_frames.empty()){
            free_frames.pop_front();
        }
        fr(1,f+1){
            free_frames.push_back(i);
        }
        TLB.clear();
        // Process end
        a = "Process id "+to_string(curr_process)+" end of execution with page faults : "+to_string(page_faults)+"\n";
        fp<<a;
        cout<<a;
    }
}