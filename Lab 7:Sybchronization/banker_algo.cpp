#include <bits/stdc++.h>
#define fr(a,b) for(int i = a; i < b; i++)
#define rep(i,a,b) for(int i = a; i < b; i++)
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL)
using ll = long long;
using namespace std;
 
int main(){
    int process;
    cout<<"No. of processes : ";
    cin>>process;
    int resource_type;
    cout<<"\nNo. of resource type : ";
    cin>>resource_type;
    int available[1][resource_type];
    cout<<"\nAvailablity matrix (Original Availablity - already allocated resources) for resources: ";
    fr(0,resource_type){
        cin>>available[0][i];
    }
    int max_req[process][resource_type];
    cout<<"\n Max matrix: \n";
    fr(0,process){
        rep(j,0,resource_type){
            cin>>max_req[i][j];
        }
    }
    int allocation[process][resource_type];
    cout<<"\n Allocation matrix: \n";
    fr(0,process){
        rep(j,0,resource_type){
            cin>>allocation[i][j];
        }
    }

    int required[process][resource_type];
    fr(0,process){
        rep(j,0,resource_type){
            required[i][j] = max_req[i][j] - allocation[i][j];
        }
    }
    int pro_req;
    cout<<"\nProcess no for which you want to request : ";
    cin>>pro_req;
    vector<int> inp_string(resource_type);
    cout<<"\nrequest string : ";
    string ans;
    
    fr(0,resource_type){
        cin>>inp_string[i];
        if(inp_string[i]>required[pro_req][i]){
            ans = "The req made is more than required for proceess";
        }
    }
    if(ans.size()==0){
        fr(0,resource_type){
            if(available[0][i]>=inp_string[i]){

            }
            else{
                ans ="The req can't be fullfilled now.";
                break;
            }
        }
    }
    if(ans.size()==0){
        ans = "Yes the req can be fullfilled";
    }
    vector<int> safe_seq;
    vector<int> visited(process,false);
    int count = 0;
    int pos = 0;
    while (true){
        fr(0,process){
            if(!visited[i]){
                bool indi = false;
                rep(j,0,resource_type){
                    if(available[0][j]>=required[i][j]){

                    }
                    else{
                        indi = true;
                        break;
                    }
                }
                if(!indi){
                    visited[i] = true;
                    safe_seq.push_back(i);
                    count++;
                    rep(j,0,resource_type){
                        available[0][j] += allocation[i][j];
                    }
                }
            }
        }
        if(count==0){
            break;
        }
        count=0;
    }
    if(safe_seq.size()<process){
        cout<<"\nUNSAFE"<<"\n";
    }
    else{
        cout<<"\nSAFE AND SAFE SEQ IS : ";
        fr(0,process){
            cout<<safe_seq[i]<<" ";
        }
        cout<<"\n";
    }
    cout<<ans;

    
    return 0;
}